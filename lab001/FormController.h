//#pragma once
//
//#include "windows.h"
//#include "MyForm.h"
//#include <GL\glew.h>
//
//using namespace lab001;
//
//public ref class FormController
//{
//public:
//	static MyForm^ myForm = gcnew MyForm();
//	GLfloat refractiveIndex;
//	GLfloat mixRatio;
//	GLfloat textureRatio;
//	bool rotate, normalMap;
//	void EntryPoint()
//	{
//		myForm->refractiveTrackBar->Value = 15;
//		Application::EnableVisualStyles();
//		Application::Run(myForm);
//	}
//
//	void Update(int rotX, int rotY, int rotZ)
//	{
//		this->refractiveIndex = myForm->refractiveTrackBar->Value / 10.0f;
//		this->mixRatio = myForm->ratioTrackBar->Value / 100.0f;
//		this->textureRatio = myForm->textureTrackBar->Value / 100.0f;
//		this->rotate = myForm->rotateCB->Checked;
//		this->normalMap = myForm->normalMapCB->Checked;
//	}
//};