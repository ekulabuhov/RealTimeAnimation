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
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 322);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->animationDropDown);
			this->Name = L"MyForm";
			this->Text = L"Assignment #4 (Final)";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 this->animationDropDown->SelectedIndex = 0;
		 }
};
}
