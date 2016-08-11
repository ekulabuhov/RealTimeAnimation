#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

GLint TextureFromFile(const char* path, string directory);

class Model 
{
public:
    /*  Functions   */
    // Constructor, expects a filepath to a 3D model.
    Model(GLchar* path)
    {
		m_NumBones = 0;
		_animationIndex = 0;
		scene = NULL;

        this->loadModel(path);
    }

    // Draws the model, and thus all its meshes
	void Draw(Shader shader)
    {
		shader.setUniformMatrix4fv("model", this->_modelMatrix);
        for(GLuint i = 0; i < this->meshes.size(); i++)
            this->meshes[i].Draw(shader);
    }

	void Draw(Shader shader, vector<glm::mat4>& Transforms)
    {
		for (GLuint i = 0 ; i < Transforms.size() ; i++) {
			char Name[128];
			memset(Name, 0, sizeof(Name));
			_snprintf_s(Name, sizeof(Name), "gBones[%d]", i);
            shader.setUniformMatrix4fv(Name, Transforms[i]);
        }
		this->Draw(shader);
    }

	void Rotate(glm::vec3 rotation)
	{
		this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	} 

	void Scale(glm::vec3 scale)
    {
		this->_modelMatrix = glm::scale(this->_modelMatrix, scale);
		// EK: for now I'm assuming that the model is scaled evenly in all dimensions
		this->_scaler = scale.x;
		this->_scale = scale;
    }

	void Translate(glm::vec3 translate)
    {
		this->_modelMatrix = glm::translate(this->_modelMatrix, translate);
    }

	void SetPosition(glm::vec3 position) 
	{
		if (position.x == position.x) {
			this->_modelMatrix[3] = glm::vec4(position, 1);
		} else {
			throw;
		}
	}

	void SetRotationFromQuaternion(glm::quat quat) 
	{
		auto position = this->GetPosition();

		// Rebuild the matrix - equivalent of threejs matrix.compose
		this->_modelMatrix = glm::mat4_cast(quat);
		this->Scale(this->_scale);
		this->SetPosition(position);

		this->_quaternion = quat;
	}

	// There's an x, y, z and positional vectors in modelMatrix:
	// xx  yx  zx  px
	// xy  yy  zy  py
	// xz  yz  zz  pz
	// xw  yw  zw  pw
	// You can see it by looking at identity matrix:
	// 1   0   0   0
	// 0   1   0   0
	// 0   0   1   0
	// 0   0   0   1
	glm::vec3 GetPosition()
	{
		auto positionVector = this->_modelMatrix[3];
		return glm::vec3(positionVector.x, positionVector.y, positionVector.z);
	}

	glm::vec3 GetForwardVector() 
	{
		auto zVector = this->_modelMatrix[2];
		return glm::vec3(zVector.x, zVector.y, zVector.z) / this->_scaler;
	}

	glm::vec3 GetUpVector()
	{
		auto upMatrix = this->_modelMatrix[1];
		return glm::vec3(upMatrix.x, upMatrix.y, upMatrix.z) / this->_scaler;
	}

	float GetYRotation() {
		glm::vec3 zVector(0.0f, 0.0f, 1.0f);
		float crossProduct = glm::cross(GetForwardVector(), zVector).y;
		float playerRotation = acos(glm::dot(GetForwardVector(), zVector)) * 180 / M_PI;
		if (crossProduct < 0)
			playerRotation = 360 - playerRotation;

		return playerRotation;
	}

	void SetModelMatrix(glm::mat4 m) {
		this->_modelMatrix = m;
	}

	void SetAnimationIndex(int index) {
		this->_animationIndex = index;
	}

	void BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms, map<string,glm::quat> KinematicTransforms) {
		glm::mat4 Identity(1.0f);

		float TicksPerSecond = scene->mAnimations[_animationIndex]->mTicksPerSecond != 0 ? scene->mAnimations[_animationIndex]->mTicksPerSecond : 25.0f;
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, scene->mAnimations[_animationIndex]->mDuration);

		ReadNodeHeirarchy(AnimationTime, scene->mRootNode, Identity, _animationIndex, KinematicTransforms);

		Transforms.resize(m_NumBones);

		for (GLuint i = 0 ; i < m_NumBones ; i++) {
			Transforms[i] = m_BoneInfo[i].FinalTransformation;
		}
	}

	map<string,GLuint> m_BoneMapping;	// maps a bone name to its index
	vector<BoneInfo> m_BoneInfo;		// contains final offset and transformations of the bone
	glm::mat4 _modelMatrix;
    
private:
    /*  Model Data  */
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

	GLuint m_NumBones;
	glm::mat4 m_GlobalInverseTransform;

	const aiScene* scene;
	Assimp::Importer importer;
	int _animationIndex;
	float _scaler;
	glm::quat _quaternion;
	glm::vec3 _scale;

    /*  Functions   */
    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string path)
    {
        // Read file via ASSIMP
        scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
        // Check for errors
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }

		_aiMatrixToGlm(scene->mRootNode->mTransformation, m_GlobalInverseTransform);
		m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);

        // Retrieve the directory path of the filepath
        this->directory = path.substr(0, path.find_last_of('/'));

        // Process ASSIMP's root node recursively
        this->processNode(scene->mRootNode, scene);
    }

    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene)
    {
        // Process each mesh located at the current node
        for(GLuint i = 0; i < node->mNumMeshes; i++)
        {
            // The node object only contains indices to index the actual objects in the scene. 
            // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
            this->meshes.push_back(this->processMesh(mesh, scene));			
        }
        // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(GLuint i = 0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene);
        }

    }

	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
	{
		for (GLuint i = 0 ; i < pAnimation->mNumChannels ; i++) {
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        
			if (string(pNodeAnim->mNodeName.data) == NodeName) {
				return pNodeAnim;
			}
		}
    
		return NULL;
	}

	GLuint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumScalingKeys > 0);
    
		for (GLuint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
				return i;
			}
		}
    
		assert(0);

		return 0;
	}

	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumScalingKeys == 1) {
			Out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}

		GLuint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
		GLuint NextScalingIndex = (ScalingIndex + 1);
		assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	GLuint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumRotationKeys > 0);

		for (GLuint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
				return i;
			}
		}
    
		assert(0);

		return 0;
	}

	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1) {
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}
    
		GLuint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		GLuint NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;    
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}

	GLuint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{    
		for (GLuint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
				return i;
			}
		}
    
		assert(0);

		return 0;
	}

	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumPositionKeys == 1) {
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}
            
		GLuint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
		GLuint NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	// Recursive function
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform, int animationIndex, map<string,glm::quat> KinematicTransforms) {
		string NodeName(pNode->mName.data);

		const aiAnimation* pAnimation = scene->mAnimations[animationIndex];

		glm::mat4 NodeTransformation;
		_aiMatrixToGlm(pNode->mTransformation, NodeTransformation);

		const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);
			
		if (pNodeAnim) {
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			glm::mat4 ScallingM = glm::scale(glm::vec3(Scaling.x, Scaling.y, Scaling.z));

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);        
			glm::mat4 RotationM;
			_aiMatrixToGlm(RotationQ.GetMatrix(), RotationM);

			//if (NodeName == "Fbx01_R_UpperArm") {
			//	glm::vec3 EulerAngles(0, 0, glm::radians(90.0f));
			//	glm::quat MyQuaternion(EulerAngles);
			//	RotationM = glm::mat4_cast(MyQuaternion);
			//}

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
			glm::mat4 TranslationM = glm::translate(glm::vec3(Translation.x, Translation.y, Translation.z));

			// EK: IK can override animations
			if (KinematicTransforms.find(NodeName) != KinematicTransforms.end()) {
				glm::quat MyQuaternion = KinematicTransforms[NodeName];
				RotationM = glm::mat4_cast(MyQuaternion);
			}

			NodeTransformation = TranslationM * RotationM * ScallingM;
		}	

		glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
			GLuint BoneIndex = m_BoneMapping[NodeName];
			glm::mat4 result = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
			m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
			m_BoneInfo[BoneIndex].GlobalTransformation = GlobalTransformation;
		}

		for (GLuint i = 0; i < pNode->mNumChildren; i++) {
			ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation, animationIndex, KinematicTransforms);
		}
	}

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // Data to fill
        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;
		vector<VertexBoneData> bones;

		GLuint NumVertices = 0;

		// Count the number of vertices and indices
		for (GLuint i = 0 ; i < scene->mNumMeshes ; i++) {        
			NumVertices += scene->mMeshes[i]->mNumVertices;
		}

		// Reserve space in the vectors for the vertex attributes and indices
		bones.resize(NumVertices);

        // Walk through each of the mesh's vertices
        for(GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // Positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // Normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // Texture Coordinates
            if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            vertices.push_back(vertex);
        }

		
		// Load bones
		for(GLuint i = 0; i < mesh->mNumBones; i++) 
		{
			GLuint BoneIndex = 0;
			string BoneName(mesh->mBones[i]->mName.data);

			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
				// Allocate an index for a new bone
				BoneIndex = m_NumBones;
				m_NumBones++;

				BoneInfo bi;
				m_BoneInfo.push_back(bi);
				//m_BoneInfo[BoneIndex].BoneOffset = mesh->mBones[i]->mOffsetMatrix;
				_aiMatrixToGlm(mesh->mBones[i]->mOffsetMatrix, m_BoneInfo[BoneIndex].BoneOffset);
				//m_BoneInfo[BoneIndex].BoneOffset = glm::transpose(m_BoneInfo[BoneIndex].BoneOffset);
				m_BoneMapping[BoneName] = BoneIndex;
			}
			else 
			{
				BoneIndex = m_BoneMapping[BoneName];
			}

			for (GLuint j = 0; j < mesh->mBones[i]->mNumWeights; j++) 
			{
				//GLuint VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
				GLuint VertexID = 0 + mesh->mBones[i]->mWeights[j].mVertexId;
				float Weight  = mesh->mBones[i]->mWeights[j].mWeight;                   
				bones[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}


        // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(GLuint i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // Retrieve all indices of the face and store them in the indices vector
            for(GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // Process materials
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
            // Same applies to other texture as the following list summarizes:
            // Diffuse: texture_diffuseN
            // Specular: texture_specularN
            // Normal: texture_normalN

            // 1. Diffuse maps
            vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. Specular maps
            vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
        
        // Return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures, bones);
    }

	void _aiMatrixToGlm(const aiMatrix4x4 &aiMat, glm::mat4 &glmMat)
	{
        //inverse from row-major to column-major
		aiMatrix4x4 temp = aiMat;
		temp.Transpose();

		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				glmMat[i][j] = temp[i][j];
	}

	void _aiMatrixToGlm(const aiMatrix3x3 &aiMat, glm::mat4 &glmMat)
	{
		//inverse from row-major to column-major
		aiMatrix3x3 temp = aiMat;
		temp.Transpose();

		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				glmMat[i][j] = temp[i][j];
	}

    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            GLboolean skip = false;
            for(GLuint j = 0; j < textures_loaded.size(); j++)
            {
                if(textures_loaded[j].path == str)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(!skip)
            {   // If texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str;
                textures.push_back(texture);
                this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};




GLint TextureFromFile(const char* path, string directory)
{
     //Generate texture ID and load texture data 
    string filename = string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height,channels;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);	

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}