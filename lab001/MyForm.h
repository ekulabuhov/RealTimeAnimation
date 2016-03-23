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
	public: System::Windows::Forms::Label^  lblAngleX;
	public: System::Windows::Forms::TrackBar^  posXTrackBar;
	public: System::Windows::Forms::TrackBar^  posYTrackBar;
	protected:



	public: System::Windows::Forms::Label^  lblAngleY;
	public: System::Windows::Forms::TrackBar^  posZTrackBar;
	private:
	protected:







	public: System::Windows::Forms::Label^  lblAngleZ;
	public: System::Windows::Forms::CheckBox^  cbAnimate;

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
			this->lblAngleX = (gcnew System::Windows::Forms::Label());
			this->posXTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->posYTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->lblAngleY = (gcnew System::Windows::Forms::Label());
			this->posZTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->lblAngleZ = (gcnew System::Windows::Forms::Label());
			this->cbAnimate = (gcnew System::Windows::Forms::CheckBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->posXTrackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->posYTrackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->posZTrackBar))->BeginInit();
			this->SuspendLayout();
			// 
			// lblAngleX
			// 
			this->lblAngleX->AutoSize = true;
			this->lblAngleX->Location = System::Drawing::Point(9, 7);
			this->lblAngleX->Name = L"lblAngleX";
			this->lblAngleX->Size = System::Drawing::Size(31, 13);
			this->lblAngleX->TabIndex = 3;
			this->lblAngleX->Text = L"posX";
			// 
			// posXTrackBar
			// 
			this->posXTrackBar->Location = System::Drawing::Point(12, 23);
			this->posXTrackBar->Maximum = 200;
			this->posXTrackBar->Name = L"posXTrackBar";
			this->posXTrackBar->Size = System::Drawing::Size(260, 45);
			this->posXTrackBar->TabIndex = 4;
			this->posXTrackBar->Value = 100;
			// 
			// posYTrackBar
			// 
			this->posYTrackBar->Location = System::Drawing::Point(12, 73);
			this->posYTrackBar->Maximum = 200;
			this->posYTrackBar->Name = L"posYTrackBar";
			this->posYTrackBar->Size = System::Drawing::Size(260, 45);
			this->posYTrackBar->TabIndex = 6;
			this->posYTrackBar->Value = 100;
			// 
			// lblAngleY
			// 
			this->lblAngleY->AutoSize = true;
			this->lblAngleY->Location = System::Drawing::Point(9, 57);
			this->lblAngleY->Name = L"lblAngleY";
			this->lblAngleY->Size = System::Drawing::Size(31, 13);
			this->lblAngleY->TabIndex = 5;
			this->lblAngleY->Text = L"posY";
			// 
			// posZTrackBar
			// 
			this->posZTrackBar->Location = System::Drawing::Point(13, 128);
			this->posZTrackBar->Maximum = 200;
			this->posZTrackBar->Name = L"posZTrackBar";
			this->posZTrackBar->Size = System::Drawing::Size(260, 45);
			this->posZTrackBar->TabIndex = 9;
			this->posZTrackBar->Value = 100;
			// 
			// lblAngleZ
			// 
			this->lblAngleZ->AutoSize = true;
			this->lblAngleZ->Location = System::Drawing::Point(10, 112);
			this->lblAngleZ->Name = L"lblAngleZ";
			this->lblAngleZ->Size = System::Drawing::Size(31, 13);
			this->lblAngleZ->TabIndex = 8;
			this->lblAngleZ->Text = L"posZ";
			// 
			// cbAnimate
			// 
			this->cbAnimate->AutoSize = true;
			this->cbAnimate->Location = System::Drawing::Point(12, 179);
			this->cbAnimate->Name = L"cbAnimate";
			this->cbAnimate->Size = System::Drawing::Size(64, 17);
			this->cbAnimate->TabIndex = 10;
			this->cbAnimate->Text = L"Animate";
			this->cbAnimate->UseVisualStyleBackColor = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 322);
			this->Controls->Add(this->cbAnimate);
			this->Controls->Add(this->posZTrackBar);
			this->Controls->Add(this->lblAngleZ);
			this->Controls->Add(this->posYTrackBar);
			this->Controls->Add(this->lblAngleY);
			this->Controls->Add(this->posXTrackBar);
			this->Controls->Add(this->lblAngleX);
			this->Name = L"MyForm";
			this->Text = L"Assignment #3 (IK)";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->posXTrackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->posYTrackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->posZTrackBar))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
};
}
