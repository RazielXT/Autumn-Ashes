#pragma once
#include <windows.h>
#include <string>
#include <vcclr.h>
#include <functional>
#include "..\..\Ballz\EditorComm.h"
#include <msclr\marshal_cppstd.h>

using namespace msclr::interop;

extern void SendMsg(UiMessageId id, void* data);

namespace CppWinForm1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class EditorForm : public System::Windows::Forms::Form
	{
	public:

		static EditorForm^ instance;	

		void UserControl1_Paint(System::Object^ sender, PaintEventArgs^ e)
		{
			auto box = (System::Windows::Forms::GroupBox^) sender;
			ControlPaint::DrawBorder(e->Graphics, box->ClientRectangle, Color::Black, ButtonBorderStyle::Solid);
		}

		EditorForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			labels = gcnew array<System::Windows::Forms::Label^>(10);
			numerics = gcnew array<System::Windows::Forms::NumericUpDown^>(10);

			instance = this;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EditorForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::GroupBox^  toolsGroupBox;






	private: array<System::Windows::Forms::Label^>^  labels;
	private: array<System::Windows::Forms::NumericUpDown^>^  numerics;


	private: System::Windows::Forms::GroupBox^  selectGroupBox;


	private: System::Windows::Forms::GroupBox^  listGroupBox;
	private: System::Windows::Forms::TreeView^  sceneListTree;



	private: System::Windows::Forms::GroupBox^  entityGroupBox;



	private: System::Windows::Forms::GroupBox^  addGroupBox;
	private: System::Windows::Forms::RadioButton^  selectObjButton;
	private: System::Windows::Forms::RadioButton^  rotateObjButton;





	private: System::Windows::Forms::RadioButton^  scaleObjButton;

	private: System::Windows::Forms::RadioButton^  moveObjButton;












	private: System::Windows::Forms::GroupBox^  sceneGroupBox;
	private: System::Windows::Forms::Label^  label1;




	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  entScaleZ;

	private: System::Windows::Forms::NumericUpDown^  entScaleY;

	private: System::Windows::Forms::NumericUpDown^  entScaleX;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  entPosZ;

	private: System::Windows::Forms::NumericUpDown^  entPosY;

	private: System::Windows::Forms::NumericUpDown^  entPosX;

private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::ComboBox^  addItemTypeComboBox;

private: System::Windows::Forms::CheckBox^  sceneCheckBox;

private: System::Windows::Forms::CheckBox^  addItemCheckBox;

private: System::Windows::Forms::CheckBox^  selectCheckBox;

private: System::Windows::Forms::CheckBox^  listCheckBox;
private: System::Windows::Forms::Label^  entNameLabel;
private: System::Windows::Forms::ComboBox^  lutComboBox;

private: System::Windows::Forms::ComboBox^  skyboxComboBox;

private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::TextBox^  addItemPrefixTextBox;



private: System::Windows::Forms::Label^  label7;









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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(EditorForm::typeid));
			this->toolsGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->sceneCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->addItemCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->selectCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->listCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->selectGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->rotateObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->scaleObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->moveObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->selectObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->listGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->sceneListTree = (gcnew System::Windows::Forms::TreeView());
			this->entityGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->entNameLabel = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->entScaleZ = (gcnew System::Windows::Forms::NumericUpDown());
			this->entScaleY = (gcnew System::Windows::Forms::NumericUpDown());
			this->entScaleX = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->entPosZ = (gcnew System::Windows::Forms::NumericUpDown());
			this->entPosY = (gcnew System::Windows::Forms::NumericUpDown());
			this->entPosX = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->addGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->addItemPrefixTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->addItemTypeComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->sceneGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->lutComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->skyboxComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->toolsGroupBox->SuspendLayout();
			this->selectGroupBox->SuspendLayout();
			this->listGroupBox->SuspendLayout();
			this->entityGroupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleZ))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleX))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosZ))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosX))->BeginInit();
			this->addGroupBox->SuspendLayout();
			this->sceneGroupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// toolsGroupBox
			// 
			this->toolsGroupBox->AutoSize = true;
			this->toolsGroupBox->BackColor = System::Drawing::SystemColors::ControlDark;
			this->toolsGroupBox->Controls->Add(this->sceneCheckBox);
			this->toolsGroupBox->Controls->Add(this->addItemCheckBox);
			this->toolsGroupBox->Controls->Add(this->selectCheckBox);
			this->toolsGroupBox->Controls->Add(this->listCheckBox);
			this->toolsGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->toolsGroupBox->Location = System::Drawing::Point(0, 0);
			this->toolsGroupBox->Margin = System::Windows::Forms::Padding(2);
			this->toolsGroupBox->Name = L"toolsGroupBox";
			this->toolsGroupBox->Padding = System::Windows::Forms::Padding(2);
			this->toolsGroupBox->Size = System::Drawing::Size(362, 84);
			this->toolsGroupBox->TabIndex = 5;
			this->toolsGroupBox->TabStop = false;
			this->toolsGroupBox->Text = L"Tools";
			// 
			// sceneCheckBox
			// 
			this->sceneCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->sceneCheckBox->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"sceneCheckBox.BackgroundImage")));
			this->sceneCheckBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->sceneCheckBox->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->sceneCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::Color::DodgerBlue;
			this->sceneCheckBox->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->sceneCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->sceneCheckBox->Location = System::Drawing::Point(180, 22);
			this->sceneCheckBox->Name = L"sceneCheckBox";
			this->sceneCheckBox->Size = System::Drawing::Size(40, 44);
			this->sceneCheckBox->TabIndex = 31;
			this->sceneCheckBox->UseVisualStyleBackColor = true;
			this->sceneCheckBox->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::sceneCheckBox_CheckedChanged);
			// 
			// addItemCheckBox
			// 
			this->addItemCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->addItemCheckBox->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"addItemCheckBox.BackgroundImage")));
			this->addItemCheckBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->addItemCheckBox->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->addItemCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::Color::DodgerBlue;
			this->addItemCheckBox->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->addItemCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->addItemCheckBox->Location = System::Drawing::Point(233, 22);
			this->addItemCheckBox->Name = L"addItemCheckBox";
			this->addItemCheckBox->Size = System::Drawing::Size(40, 44);
			this->addItemCheckBox->TabIndex = 30;
			this->addItemCheckBox->UseVisualStyleBackColor = true;
			this->addItemCheckBox->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::addItemCheckBox_CheckedChanged);
			// 
			// selectCheckBox
			// 
			this->selectCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->selectCheckBox->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"selectCheckBox.BackgroundImage")));
			this->selectCheckBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->selectCheckBox->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->selectCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::Color::DodgerBlue;
			this->selectCheckBox->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->selectCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->selectCheckBox->Location = System::Drawing::Point(127, 22);
			this->selectCheckBox->Name = L"selectCheckBox";
			this->selectCheckBox->Size = System::Drawing::Size(40, 44);
			this->selectCheckBox->TabIndex = 29;
			this->selectCheckBox->UseVisualStyleBackColor = true;
			this->selectCheckBox->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::selectCheckBox_CheckedChanged);
			// 
			// listCheckBox
			// 
			this->listCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->listCheckBox->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"listCheckBox.BackgroundImage")));
			this->listCheckBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->listCheckBox->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->listCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::Color::DodgerBlue;
			this->listCheckBox->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->listCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->listCheckBox->Location = System::Drawing::Point(76, 22);
			this->listCheckBox->Name = L"listCheckBox";
			this->listCheckBox->Size = System::Drawing::Size(40, 44);
			this->listCheckBox->TabIndex = 28;
			this->listCheckBox->UseVisualStyleBackColor = true;
			this->listCheckBox->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::listCheckBox_CheckedChanged);
			// 
			// selectGroupBox
			// 
			this->selectGroupBox->AutoSize = true;
			this->selectGroupBox->Controls->Add(this->rotateObjButton);
			this->selectGroupBox->Controls->Add(this->scaleObjButton);
			this->selectGroupBox->Controls->Add(this->moveObjButton);
			this->selectGroupBox->Controls->Add(this->selectObjButton);
			this->selectGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->selectGroupBox->Location = System::Drawing::Point(0, 84);
			this->selectGroupBox->Name = L"selectGroupBox";
			this->selectGroupBox->Size = System::Drawing::Size(362, 90);
			this->selectGroupBox->TabIndex = 11;
			this->selectGroupBox->TabStop = false;
			this->selectGroupBox->Text = L"Select";
			this->selectGroupBox->Visible = false;
			// 
			// rotateObjButton
			// 
			this->rotateObjButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->rotateObjButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"rotateObjButton.BackgroundImage")));
			this->rotateObjButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->rotateObjButton->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->rotateObjButton->FlatAppearance->CheckedBackColor = System::Drawing::Color::Khaki;
			this->rotateObjButton->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->rotateObjButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->rotateObjButton->Location = System::Drawing::Point(233, 28);
			this->rotateObjButton->Margin = System::Windows::Forms::Padding(2);
			this->rotateObjButton->Name = L"rotateObjButton";
			this->rotateObjButton->Size = System::Drawing::Size(40, 44);
			this->rotateObjButton->TabIndex = 23;
			this->rotateObjButton->TabStop = true;
			this->rotateObjButton->UseVisualStyleBackColor = true;
			this->rotateObjButton->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::rotateObjButton_CheckedChanged);
			// 
			// scaleObjButton
			// 
			this->scaleObjButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->scaleObjButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"scaleObjButton.BackgroundImage")));
			this->scaleObjButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->scaleObjButton->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->scaleObjButton->FlatAppearance->CheckedBackColor = System::Drawing::Color::Khaki;
			this->scaleObjButton->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->scaleObjButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->scaleObjButton->Location = System::Drawing::Point(180, 28);
			this->scaleObjButton->Margin = System::Windows::Forms::Padding(2);
			this->scaleObjButton->Name = L"scaleObjButton";
			this->scaleObjButton->Size = System::Drawing::Size(40, 44);
			this->scaleObjButton->TabIndex = 22;
			this->scaleObjButton->TabStop = true;
			this->scaleObjButton->UseVisualStyleBackColor = true;
			this->scaleObjButton->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::scaleObjButton_CheckedChanged);
			// 
			// moveObjButton
			// 
			this->moveObjButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->moveObjButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"moveObjButton.BackgroundImage")));
			this->moveObjButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->moveObjButton->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->moveObjButton->FlatAppearance->CheckedBackColor = System::Drawing::Color::Khaki;
			this->moveObjButton->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->moveObjButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->moveObjButton->Location = System::Drawing::Point(127, 28);
			this->moveObjButton->Margin = System::Windows::Forms::Padding(2);
			this->moveObjButton->Name = L"moveObjButton";
			this->moveObjButton->Size = System::Drawing::Size(40, 44);
			this->moveObjButton->TabIndex = 21;
			this->moveObjButton->TabStop = true;
			this->moveObjButton->UseVisualStyleBackColor = true;
			this->moveObjButton->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::moveObjButton_CheckedChanged);
			// 
			// selectObjButton
			// 
			this->selectObjButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->selectObjButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"selectObjButton.BackgroundImage")));
			this->selectObjButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->selectObjButton->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->selectObjButton->FlatAppearance->CheckedBackColor = System::Drawing::Color::Khaki;
			this->selectObjButton->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->selectObjButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->selectObjButton->Location = System::Drawing::Point(76, 28);
			this->selectObjButton->Margin = System::Windows::Forms::Padding(2);
			this->selectObjButton->Name = L"selectObjButton";
			this->selectObjButton->Size = System::Drawing::Size(40, 44);
			this->selectObjButton->TabIndex = 20;
			this->selectObjButton->TabStop = true;
			this->selectObjButton->UseVisualStyleBackColor = true;
			this->selectObjButton->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::selectObjButton_CheckedChanged);
			// 
			// listGroupBox
			// 
			this->listGroupBox->Controls->Add(this->sceneListTree);
			this->listGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->listGroupBox->Location = System::Drawing::Point(0, 174);
			this->listGroupBox->Name = L"listGroupBox";
			this->listGroupBox->Size = System::Drawing::Size(362, 100);
			this->listGroupBox->TabIndex = 12;
			this->listGroupBox->TabStop = false;
			this->listGroupBox->Text = L"List";
			this->listGroupBox->Visible = false;
			// 
			// sceneListTree
			// 
			this->sceneListTree->BackColor = System::Drawing::Color::Silver;
			this->sceneListTree->Dock = System::Windows::Forms::DockStyle::Fill;
			this->sceneListTree->Location = System::Drawing::Point(3, 16);
			this->sceneListTree->Name = L"sceneListTree";
			this->sceneListTree->Size = System::Drawing::Size(356, 81);
			this->sceneListTree->TabIndex = 1;
			// 
			// entityGroupBox
			// 
			this->entityGroupBox->AutoSize = true;
			this->entityGroupBox->Controls->Add(this->entNameLabel);
			this->entityGroupBox->Controls->Add(this->label3);
			this->entityGroupBox->Controls->Add(this->entScaleZ);
			this->entityGroupBox->Controls->Add(this->entScaleY);
			this->entityGroupBox->Controls->Add(this->entScaleX);
			this->entityGroupBox->Controls->Add(this->label2);
			this->entityGroupBox->Controls->Add(this->entPosZ);
			this->entityGroupBox->Controls->Add(this->entPosY);
			this->entityGroupBox->Controls->Add(this->entPosX);
			this->entityGroupBox->Controls->Add(this->label1);
			this->entityGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->entityGroupBox->Location = System::Drawing::Point(0, 447);
			this->entityGroupBox->Margin = System::Windows::Forms::Padding(2);
			this->entityGroupBox->MinimumSize = System::Drawing::Size(0, 16);
			this->entityGroupBox->Name = L"entityGroupBox";
			this->entityGroupBox->Padding = System::Windows::Forms::Padding(2);
			this->entityGroupBox->Size = System::Drawing::Size(362, 99);
			this->entityGroupBox->TabIndex = 13;
			this->entityGroupBox->TabStop = false;
			this->entityGroupBox->Text = L"Entity";
			this->entityGroupBox->Visible = false;
			// 
			// entNameLabel
			// 
			this->entNameLabel->AutoSize = true;
			this->entNameLabel->Location = System::Drawing::Point(109, 17);
			this->entNameLabel->Name = L"entNameLabel";
			this->entNameLabel->Size = System::Drawing::Size(33, 13);
			this->entNameLabel->TabIndex = 10;
			this->entNameLabel->Text = L"name";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(50, 64);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(34, 13);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Scale";
			this->label3->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// entScaleZ
			// 
			this->entScaleZ->DecimalPlaces = 2;
			this->entScaleZ->Location = System::Drawing::Point(256, 62);
			this->entScaleZ->Margin = System::Windows::Forms::Padding(2);
			this->entScaleZ->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->entScaleZ->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleZ->Name = L"entScaleZ";
			this->entScaleZ->Size = System::Drawing::Size(68, 20);
			this->entScaleZ->TabIndex = 8;
			this->entScaleZ->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleZ->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entScaleZ_ValueChanged);
			// 
			// entScaleY
			// 
			this->entScaleY->DecimalPlaces = 2;
			this->entScaleY->Location = System::Drawing::Point(184, 62);
			this->entScaleY->Margin = System::Windows::Forms::Padding(2);
			this->entScaleY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->entScaleY->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleY->Name = L"entScaleY";
			this->entScaleY->Size = System::Drawing::Size(68, 20);
			this->entScaleY->TabIndex = 7;
			this->entScaleY->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleY->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entScaleY_ValueChanged);
			// 
			// entScaleX
			// 
			this->entScaleX->DecimalPlaces = 2;
			this->entScaleX->Location = System::Drawing::Point(112, 62);
			this->entScaleX->Margin = System::Windows::Forms::Padding(2);
			this->entScaleX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->entScaleX->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleX->Name = L"entScaleX";
			this->entScaleX->Size = System::Drawing::Size(68, 20);
			this->entScaleX->TabIndex = 6;
			this->entScaleX->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleX->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entScaleX_ValueChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(40, 41);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(44, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Position";
			this->label2->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// entPosZ
			// 
			this->entPosZ->DecimalPlaces = 2;
			this->entPosZ->Location = System::Drawing::Point(256, 39);
			this->entPosZ->Margin = System::Windows::Forms::Padding(2);
			this->entPosZ->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->entPosZ->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->entPosZ->Name = L"entPosZ";
			this->entPosZ->Size = System::Drawing::Size(68, 20);
			this->entPosZ->TabIndex = 4;
			this->entPosZ->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entPosZ_ValueChanged);
			// 
			// entPosY
			// 
			this->entPosY->DecimalPlaces = 2;
			this->entPosY->Location = System::Drawing::Point(184, 39);
			this->entPosY->Margin = System::Windows::Forms::Padding(2);
			this->entPosY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->entPosY->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->entPosY->Name = L"entPosY";
			this->entPosY->Size = System::Drawing::Size(68, 20);
			this->entPosY->TabIndex = 3;
			this->entPosY->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entPosY_ValueChanged);
			// 
			// entPosX
			// 
			this->entPosX->DecimalPlaces = 2;
			this->entPosX->Location = System::Drawing::Point(112, 39);
			this->entPosX->Margin = System::Windows::Forms::Padding(2);
			this->entPosX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->entPosX->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->entPosX->Name = L"entPosX";
			this->entPosX->Size = System::Drawing::Size(68, 20);
			this->entPosX->TabIndex = 2;
			this->entPosX->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entPosX_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(50, 17);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Name";
			// 
			// addGroupBox
			// 
			this->addGroupBox->AutoSize = true;
			this->addGroupBox->Controls->Add(this->addItemPrefixTextBox);
			this->addGroupBox->Controls->Add(this->label7);
			this->addGroupBox->Controls->Add(this->label4);
			this->addGroupBox->Controls->Add(this->addItemTypeComboBox);
			this->addGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->addGroupBox->Location = System::Drawing::Point(0, 274);
			this->addGroupBox->Margin = System::Windows::Forms::Padding(2);
			this->addGroupBox->MinimumSize = System::Drawing::Size(0, 16);
			this->addGroupBox->Name = L"addGroupBox";
			this->addGroupBox->Padding = System::Windows::Forms::Padding(2);
			this->addGroupBox->Size = System::Drawing::Size(362, 86);
			this->addGroupBox->TabIndex = 14;
			this->addGroupBox->TabStop = false;
			this->addGroupBox->Text = L"Add";
			this->addGroupBox->Visible = false;
			// 
			// addItemPrefixTextBox
			// 
			this->addItemPrefixTextBox->Location = System::Drawing::Point(112, 48);
			this->addItemPrefixTextBox->Name = L"addItemPrefixTextBox";
			this->addItemPrefixTextBox->Size = System::Drawing::Size(174, 20);
			this->addItemPrefixTextBox->TabIndex = 3;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(52, 51);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(33, 13);
			this->label7->TabIndex = 2;
			this->label7->Text = L"Prefix";
			this->label7->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(20, 20);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(65, 13);
			this->label4->TabIndex = 1;
			this->label4->Text = L"Object Type";
			// 
			// addItemTypeComboBox
			// 
			this->addItemTypeComboBox->FormattingEnabled = true;
			this->addItemTypeComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Tree" });
			this->addItemTypeComboBox->Location = System::Drawing::Point(112, 17);
			this->addItemTypeComboBox->Margin = System::Windows::Forms::Padding(2);
			this->addItemTypeComboBox->Name = L"addItemTypeComboBox";
			this->addItemTypeComboBox->Size = System::Drawing::Size(174, 21);
			this->addItemTypeComboBox->TabIndex = 0;
			this->addItemTypeComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::addItemComboBox_SelectedIndexChanged);
			// 
			// sceneGroupBox
			// 
			this->sceneGroupBox->AutoSize = true;
			this->sceneGroupBox->Controls->Add(this->lutComboBox);
			this->sceneGroupBox->Controls->Add(this->skyboxComboBox);
			this->sceneGroupBox->Controls->Add(this->label6);
			this->sceneGroupBox->Controls->Add(this->label5);
			this->sceneGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->sceneGroupBox->Location = System::Drawing::Point(0, 360);
			this->sceneGroupBox->Margin = System::Windows::Forms::Padding(2);
			this->sceneGroupBox->MinimumSize = System::Drawing::Size(0, 16);
			this->sceneGroupBox->Name = L"sceneGroupBox";
			this->sceneGroupBox->Padding = System::Windows::Forms::Padding(2);
			this->sceneGroupBox->Size = System::Drawing::Size(362, 87);
			this->sceneGroupBox->TabIndex = 15;
			this->sceneGroupBox->TabStop = false;
			this->sceneGroupBox->Text = L"Scene";
			this->sceneGroupBox->Visible = false;
			// 
			// lutComboBox
			// 
			this->lutComboBox->FormattingEnabled = true;
			this->lutComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Tree" });
			this->lutComboBox->Location = System::Drawing::Point(112, 49);
			this->lutComboBox->Margin = System::Windows::Forms::Padding(2);
			this->lutComboBox->Name = L"lutComboBox";
			this->lutComboBox->Size = System::Drawing::Size(174, 21);
			this->lutComboBox->TabIndex = 3;
			this->lutComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::lutComboBox_SelectedIndexChanged);
			// 
			// skyboxComboBox
			// 
			this->skyboxComboBox->FormattingEnabled = true;
			this->skyboxComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Tree" });
			this->skyboxComboBox->Location = System::Drawing::Point(112, 22);
			this->skyboxComboBox->Margin = System::Windows::Forms::Padding(2);
			this->skyboxComboBox->Name = L"skyboxComboBox";
			this->skyboxComboBox->Size = System::Drawing::Size(174, 21);
			this->skyboxComboBox->TabIndex = 2;
			this->skyboxComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::skyboxComboBox_SelectedIndexChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(56, 57);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(28, 13);
			this->label6->TabIndex = 2;
			this->label6->Text = L"LUT";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(43, 25);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(42, 13);
			this->label5->TabIndex = 1;
			this->label5->Text = L"Skybox";
			// 
			// EditorForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->AutoSize = true;
			this->BackColor = System::Drawing::SystemColors::ControlDark;
			this->ClientSize = System::Drawing::Size(362, 634);
			this->Controls->Add(this->entityGroupBox);
			this->Controls->Add(this->sceneGroupBox);
			this->Controls->Add(this->addGroupBox);
			this->Controls->Add(this->listGroupBox);
			this->Controls->Add(this->selectGroupBox);
			this->Controls->Add(this->toolsGroupBox);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"EditorForm";
			this->Text = L"Editor";
			this->toolsGroupBox->ResumeLayout(false);
			this->selectGroupBox->ResumeLayout(false);
			this->listGroupBox->ResumeLayout(false);
			this->entityGroupBox->ResumeLayout(false);
			this->entityGroupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleZ))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleX))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosZ))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosX))->EndInit();
			this->addGroupBox->ResumeLayout(false);
			this->addGroupBox->PerformLayout();
			this->sceneGroupBox->ResumeLayout(false);
			this->sceneGroupBox->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void listCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (!((System::Windows::Forms::CheckBox^)sender)->Checked)
	{
		listGroupBox->Hide();
		return;
	}

	GetWorldItemsData data;
	SendMsg(UiMessageId::GetWorldItems, &data);

	sceneListTree->Nodes->Clear();

	for (auto group : data.groups)
	{
		auto id = sceneListTree->Nodes->Add(gcnew TreeNode(gcnew System::String(group.name.data())));

		for (auto item : group.items)
		{
			sceneListTree->Nodes[id]->Nodes->Add(gcnew TreeNode(gcnew System::String(item.name.data())));
		}
	}

	listGroupBox->Show();
}
private: System::Void selectCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (!((System::Windows::Forms::CheckBox^)sender)->Checked)
	{
		selectGroupBox->Hide();
		return;
	}

	selectGroupBox->Show();
}
private: System::Void sceneCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (!((System::Windows::Forms::CheckBox^)sender)->Checked)
	{
		sceneGroupBox->Hide();
		return;
	}

	GetSceneSettingsData data;
	SendMsg(UiMessageId::GetSceneSettings, &data);

	skyboxComboBox->Items->Clear();
	for (auto skybox : data.skyboxOptions)
	{
		skyboxComboBox->Items->Add(gcnew System::String(skybox.data()));
	}
	skyboxComboBox->SelectedIndex = data.currentSkyboxId;

	lutComboBox->Items->Clear();
	for (auto lut : data.lutOptions)
	{
		lutComboBox->Items->Add(gcnew System::String(lut.data()));
	}
	lutComboBox->SelectedIndex = data.currentLutId;

	sceneGroupBox->Show();
}
private: System::Void addItemCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (!((System::Windows::Forms::CheckBox^)sender)->Checked)
	{
		addGroupBox->Hide();
		return;
	}

	addGroupBox->Show();

	AddItemModeInfo info;
	info.itemType = marshal_as<std::string>(addItemTypeComboBox->Text);
	info.prefix = marshal_as<std::string>(addItemPrefixTextBox->Text);

	SendMsg(UiMessageId::AddItemMode, nullptr);
}

private: System::Void selectObjButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if(entNameLabel->Text != "")
		entityGroupBox->Show();

	SendMsg(UiMessageId::SelectMode, nullptr);
}
private: System::Void moveObjButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	SendMsg(UiMessageId::MoveMode, nullptr);
}
private: System::Void scaleObjButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	SendMsg(UiMessageId::ScaleMode, nullptr);
}
private: System::Void rotateObjButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	SendMsg(UiMessageId::RotateMode, nullptr);
}

private: System::Void addItemComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
}

private: bool reportEntityChange = true;

public: System::Void showItemInfo(EntityInfo* info)
{
	if (info->name.empty())
	{
		entNameLabel->Text = "";
		entityGroupBox->Hide();
		return;
	}

	entityGroupBox->BringToFront();
	entityGroupBox->Show();

	entNameLabel->Text = gcnew System::String(info->name.data());

	reportEntityChange = false;

	entPosX->Value = System::Decimal(info->pos.x);
	entPosY->Value = System::Decimal(info->pos.y);
	entPosZ->Value = System::Decimal(info->pos.z);

	entScaleX->Value = System::Decimal(info->scale.x);
	entScaleY->Value = System::Decimal(info->scale.y);
	entScaleZ->Value = System::Decimal(info->scale.z);

	reportEntityChange = true;
}

private: System::Void skyboxComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void lutComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
}

private: void entPosChanged()
{
	if (!reportEntityChange)
		return;

	Ogre::Vector3 v3 = {System::Decimal::ToSingle(entPosX->Value), System::Decimal::ToSingle(entPosY->Value) , System::Decimal::ToSingle(entPosZ->Value) };

	EntityInfoChange change;
	change.change = EntityInfoChange::EntityChange::Pos;
	change.data = &v3;

	SendMsg(UiMessageId::EntityInfoChanged, &change);
}
private: System::Void entPosX_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	entPosChanged();
}
private: System::Void entPosY_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	entPosChanged();
}
private: System::Void entPosZ_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	entPosChanged();
}

private: void entScaleChanged()
{
	if (!reportEntityChange)
		return;

	Ogre::Vector3 v3 = { System::Decimal::ToSingle(entScaleX->Value), System::Decimal::ToSingle(entScaleY->Value) , System::Decimal::ToSingle(entScaleZ->Value) };

	EntityInfoChange change;
	change.change = EntityInfoChange::EntityChange::Scale;
	change.data = &v3;

	SendMsg(UiMessageId::EntityInfoChanged, &change);
}
private: System::Void entScaleX_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	entScaleChanged();
}
private: System::Void entScaleY_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	entScaleChanged();
}
private: System::Void entScaleZ_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	entScaleChanged();
}
};
}
