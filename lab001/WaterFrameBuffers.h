//
// Created by Eugene Kulabuhov on 22/08/2016.
//

#ifndef REALTIMEANIMATION_WATERFRAMEBUFFERS_H
#define REALTIMEANIMATION_WATERFRAMEBUFFERS_H

#include <GL/glew.h>

class WaterFrameBuffers {
private:
    int REFLECTION_WIDTH = 320;
    int REFLECTION_HEIGHT = 180;

    int REFRACTION_WIDTH = 1280;
    int REFRACTION_HEIGHT = 720;

    GLuint SCR_WIDTH = 800, SCR_HEIGHT = 800;

    GLuint reflectionFrameBuffer;
    GLuint reflectionTexture;
    GLuint reflectionDepthBuffer;

    GLuint refractionFrameBuffer;
    GLuint refractionTexture;
    GLuint refractionDepthTexture;
    GLuint createFrameBuffer();
    GLuint createTextureAttachment( int width, int height);
    GLuint createDepthTextureAttachment(int width, int height);
    GLuint createDepthBufferAttachment(int width, int height);
    void initialiseReflectionFrameBuffer();
    void initialiseRefractionFrameBuffer();
    void bindFrameBuffer(int frameBuffer, int width, int height);

public:
    WaterFrameBuffers();
    void cleanUp();
    void bindReflectionFrameBuffer();
    void bindRefractionFrameBuffer();
    void unbindCurrentFrameBuffer();
    GLuint getReflectionTexture();
    GLuint getRefractionTexture();
    GLuint getRefractionDepthTexture();
};


#endif //REALTIMEANIMATION_WATERFRAMEBUFFERS_H
