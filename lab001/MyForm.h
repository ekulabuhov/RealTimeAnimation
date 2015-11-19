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


	private: System::Windows::Forms::Label^  lblRefractiveIndex;
	public: System::Windows::Forms::TrackBar^  refractiveTrackBar;
	public: System::Windows::Forms::TrackBar^  ratioTrackBar;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	public: System::Windows::Forms::TrackBar^  textureTrackBar;
	private: 
	private: System::Windows::Forms::Label^  label4;
	public: System::Windows::Forms::CheckBox^  rotateCB;
	public: System::Windows::Forms::CheckBox^  normalMapCB;

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
			this->lblRefractiveIndex = (gcnew System::Windows::Forms::Label());
			this->refractiveTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->ratioTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textureTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->rotateCB = (gcnew System::Windows::Forms::CheckBox());
			this->normalMapCB = (gcnew System::Windows::Forms::CheckBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->refractiveTrackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ratioTrackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->textureTrackBar))->BeginInit();
			this->SuspendLayout();
			// 
			// lblRefractiveIndex
			// 
			this->lblRefractiveIndex->AutoSize = true;
			this->lblRefractiveIndex->Location = System::Drawing::Point(12, 7);
			this->lblRefractiveIndex->Name = L"lblRefractiveIndex";
			this->lblRefractiveIndex->Size = System::Drawing::Size(105, 13);
			this->lblRefractiveIndex->TabIndex = 3;
			this->lblRefractiveIndex->Text = L"Refractive index: 1.5";
			// 
			// refractiveTrackBar
			// 
			this->refractiveTrackBar->Location = System::Drawing::Point(12, 23);
			this->refractiveTrackBar->Maximum = 50;
			this->refractiveTrackBar->Name = L"refractiveTrackBar";
			this->refractiveTrackBar->Size = System::Drawing::Size(260, 45);
			this->refractiveTrackBar->TabIndex = 4;
			this->refractiveTrackBar->Value = 15;
			this->refractiveTrackBar->Scroll += gcnew System::EventHandler(this, &MyForm::trackBar1_Scroll);
			// 
			// ratioTrackBar
			// 
			this->ratioTrackBar->Location = System::Drawing::Point(12, 73);
			this->ratioTrackBar->Maximum = 100;
			this->ratioTrackBar->Name = L"ratioTrackBar";
			this->ratioTrackBar->Size = System::Drawing::Size(260, 45);
			this->ratioTrackBar->TabIndex = 6;
			this->ratioTrackBar->Value = 50;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(9, 57);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Reflect";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(231, 57);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(42, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Refract";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(198, 112);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(74, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Transmittance";
			// 
			// textureTrackBar
			// 
			this->textureTrackBar->Location = System::Drawing::Point(13, 128);
			this->textureTrackBar->Maximum = 100;
			this->textureTrackBar->Name = L"textureTrackBar";
			this->textureTrackBar->Size = System::Drawing::Size(260, 45);
			this->textureTrackBar->TabIndex = 9;
			this->textureTrackBar->Value = 20;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(10, 112);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(43, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Texture";
			// 
			// rotateCB
			// 
			this->rotateCB->AutoSize = true;
			this->rotateCB->Location = System::Drawing::Point(15, 168);
			this->rotateCB->Name = L"rotateCB";
			this->rotateCB->Size = System::Drawing::Size(58, 17);
			this->rotateCB->TabIndex = 11;
			this->rotateCB->Text = L"Rotate";
			this->rotateCB->UseVisualStyleBackColor = true;
			// 
			// normalMapCB
			// 
			this->normalMapCB->AutoSize = true;
			this->normalMapCB->Location = System::Drawing::Point(80, 168);
			this->normalMapCB->Name = L"normalMapCB";
			this->normalMapCB->Size = System::Drawing::Size(83, 17);
			this->normalMapCB->TabIndex = 12;
			this->normalMapCB->Text = L"Normal Map";
			this->normalMapCB->UseVisualStyleBackColor = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 322);
			this->Controls->Add(this->normalMapCB);
			this->Controls->Add(this->rotateCB);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textureTrackBar);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->ratioTrackBar);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->refractiveTrackBar);
			this->Controls->Add(this->lblRefractiveIndex);
			this->Name = L"MyForm";
			this->Text = L"Assignment #2";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->refractiveTrackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ratioTrackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->textureTrackBar))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
				 lblRefractiveIndex->Text = "Refractive index: " + (refractiveTrackBar->Value / 10.0f).ToString();
			 }
};
}
