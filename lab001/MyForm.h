#pragma once

namespace lab001 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::ComboBox^  animationDropDown;
	protected: 

	protected: 
	private: System::Windows::Forms::Label^  label1;
	public: System::Windows::Forms::CheckBox^  cbThirdPersonCamera;
	public: System::Windows::Forms::TrackBar^  tbCubeY;
	public: System::Windows::Forms::TrackBar^  tbCubeX;
	public: System::Windows::Forms::TrackBar^  tbCubeZ;
	private: System::Windows::Forms::Label^  lblCubeX;
	private: System::Windows::Forms::Label^  lblCubeY;
	private: System::Windows::Forms::Label^  lblCubeZ;
	public: 

	public: 



	public: 



	public: 


	private: 



	protected:





	private:
	protected:










	public: 

	public:
	private:


	public:
	private:
	public:





	private: 




	public: 
	private: 

	public: 

	private: 

	public: 
	public: 
	private: 


	public: 
	protected: 

	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->animationDropDown = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->cbThirdPersonCamera = (gcnew System::Windows::Forms::CheckBox());
			this->tbCubeY = (gcnew System::Windows::Forms::TrackBar());
			this->tbCubeX = (gcnew System::Windows::Forms::TrackBar());
			this->tbCubeZ = (gcnew System::Windows::Forms::TrackBar());
			this->lblCubeX = (gcnew System::Windows::Forms::Label());
			this->lblCubeY = (gcnew System::Windows::Forms::Label());
			this->lblCubeZ = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbCubeY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbCubeX))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbCubeZ))->BeginInit();
			this->SuspendLayout();
			// 
			// animationDropDown
			// 
			this->animationDropDown->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->animationDropDown->FormattingEnabled = true;
			this->animationDropDown->Items->AddRange(gcnew cli::array< System::Object^  >(16) {L"Idle", L"Idle with gun", L"Idle with gun aiming", 
				L"Idle with gun firing", L"Kneel firing", L"Kneel idle", L"Jump", L"Kick ball", L"Chicken Dance", L"Cat", L"Salsa", L"Run", L"Run with gun", 
				L"T-Pose", L"Walk", L"Walk with gun"});
			this->animationDropDown->Location = System::Drawing::Point(74, 20);
			this->animationDropDown->Name = L"animationDropDown";
			this->animationDropDown->Size = System::Drawing::Size(197, 21);
			this->animationDropDown->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(56, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Animation:";
			// 
			// cbThirdPersonCamera
			// 
			this->cbThirdPersonCamera->AutoSize = true;
			this->cbThirdPersonCamera->Location = System::Drawing::Point(12, 47);
			this->cbThirdPersonCamera->Name = L"cbThirdPersonCamera";
			this->cbThirdPersonCamera->Size = System::Drawing::Size(150, 17);
			this->cbThirdPersonCamera->TabIndex = 2;
			this->cbThirdPersonCamera->Text = L"Enable 3rd person camera";
			this->cbThirdPersonCamera->UseVisualStyleBackColor = true;
			// 
			// tbCubeY
			// 
			this->tbCubeY->Location = System::Drawing::Point(39, 139);
			this->tbCubeY->Maximum = 100;
			this->tbCubeY->Minimum = -100;
			this->tbCubeY->Name = L"tbCubeY";
			this->tbCubeY->Size = System::Drawing::Size(232, 45);
			this->tbCubeY->TabIndex = 3;
			this->tbCubeY->Value = 10;
			this->tbCubeY->Scroll += gcnew System::EventHandler(this, &MyForm::tbCubeY_Scroll);
			// 
			// tbCubeX
			// 
			this->tbCubeX->Location = System::Drawing::Point(39, 88);
			this->tbCubeX->Maximum = 100;
			this->tbCubeX->Minimum = -100;
			this->tbCubeX->Name = L"tbCubeX";
			this->tbCubeX->Size = System::Drawing::Size(232, 45);
			this->tbCubeX->TabIndex = 4;
			this->tbCubeX->Value = 20;
			this->tbCubeX->Scroll += gcnew System::EventHandler(this, &MyForm::tbCubeX_Scroll);
			// 
			// tbCubeZ
			// 
			this->tbCubeZ->Location = System::Drawing::Point(39, 190);
			this->tbCubeZ->Maximum = 100;
			this->tbCubeZ->Minimum = -100;
			this->tbCubeZ->Name = L"tbCubeZ";
			this->tbCubeZ->Size = System::Drawing::Size(232, 45);
			this->tbCubeZ->TabIndex = 5;
			this->tbCubeZ->Scroll += gcnew System::EventHandler(this, &MyForm::tbCubeZ_Scroll);
			// 
			// lblCubeX
			// 
			this->lblCubeX->AutoSize = true;
			this->lblCubeX->Location = System::Drawing::Point(2, 92);
			this->lblCubeX->Name = L"lblCubeX";
			this->lblCubeX->Size = System::Drawing::Size(42, 13);
			this->lblCubeX->TabIndex = 6;
			this->lblCubeX->Text = L"Cube X";
			// 
			// lblCubeY
			// 
			this->lblCubeY->AutoSize = true;
			this->lblCubeY->Location = System::Drawing::Point(2, 144);
			this->lblCubeY->Name = L"lblCubeY";
			this->lblCubeY->Size = System::Drawing::Size(42, 13);
			this->lblCubeY->TabIndex = 7;
			this->lblCubeY->Text = L"Cube Y";
			// 
			// lblCubeZ
			// 
			this->lblCubeZ->AutoSize = true;
			this->lblCubeZ->Location = System::Drawing::Point(2, 194);
			this->lblCubeZ->Name = L"lblCubeZ";
			this->lblCubeZ->Size = System::Drawing::Size(42, 13);
			this->lblCubeZ->TabIndex = 8;
			this->lblCubeZ->Text = L"Cube Z";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 322);
			this->Controls->Add(this->lblCubeZ);
			this->Controls->Add(this->lblCubeY);
			this->Controls->Add(this->lblCubeX);
			this->Controls->Add(this->tbCubeZ);
			this->Controls->Add(this->tbCubeX);
			this->Controls->Add(this->tbCubeY);
			this->Controls->Add(this->cbThirdPersonCamera);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->animationDropDown);
			this->Name = L"MyForm";
			this->Text = L"Assignment #4 (Final)";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbCubeY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbCubeX))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbCubeZ))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 this->animationDropDown->SelectedIndex = 0;
		 }
private: System::Void tbCubeX_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 lblCubeX->Text = "x: " + tbCubeX->Value / 10.0f;
		 }
private: System::Void tbCubeY_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 lblCubeY->Text = "y: " + tbCubeY->Value / 10.0f;
		 }
private: System::Void tbCubeZ_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 lblCubeZ->Text = "z: " + tbCubeZ->Value / 10.0f;
		 }
};
}
