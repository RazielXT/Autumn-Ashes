#pragma once
#include <windows.h>
#include <string>
#include <vcclr.h>
#include <functional>
#include "..\..\Ballz\EditorComm.h"

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
	private: System::Windows::Forms::TreeView^  objectsTree;

	private: System::Windows::Forms::GroupBox^  entityGroupBox;



	private: System::Windows::Forms::GroupBox^  addGroupBox;
	private: System::Windows::Forms::RadioButton^  selectObjButton;
	private: System::Windows::Forms::RadioButton^  rotateObjButton;





	private: System::Windows::Forms::RadioButton^  scaleObjButton;

	private: System::Windows::Forms::RadioButton^  moveObjButton;


	private: System::Windows::Forms::RadioButton^  sceneToolButton;

	private: System::Windows::Forms::RadioButton^  addToolButton;

	private: System::Windows::Forms::RadioButton^  selectToolButton;
	private: System::Windows::Forms::RadioButton^  listToolButton;




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
	private: System::Windows::Forms::TextBox^  entNameBox;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::ComboBox^  comboBox1;






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
			this->sceneToolButton = (gcnew System::Windows::Forms::RadioButton());
			this->addToolButton = (gcnew System::Windows::Forms::RadioButton());
			this->selectToolButton = (gcnew System::Windows::Forms::RadioButton());
			this->listToolButton = (gcnew System::Windows::Forms::RadioButton());
			this->selectGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->rotateObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->scaleObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->moveObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->selectObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->listGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->objectsTree = (gcnew System::Windows::Forms::TreeView());
			this->entityGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->addGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->sceneGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->entNameBox = (gcnew System::Windows::Forms::TextBox());
			this->entPosX = (gcnew System::Windows::Forms::NumericUpDown());
			this->entPosY = (gcnew System::Windows::Forms::NumericUpDown());
			this->entPosZ = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->entScaleZ = (gcnew System::Windows::Forms::NumericUpDown());
			this->entScaleY = (gcnew System::Windows::Forms::NumericUpDown());
			this->entScaleX = (gcnew System::Windows::Forms::NumericUpDown());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->toolsGroupBox->SuspendLayout();
			this->selectGroupBox->SuspendLayout();
			this->listGroupBox->SuspendLayout();
			this->entityGroupBox->SuspendLayout();
			this->addGroupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosX))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosZ))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleZ))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleX))->BeginInit();
			this->SuspendLayout();
			// 
			// toolsGroupBox
			// 
			this->toolsGroupBox->AutoSize = true;
			this->toolsGroupBox->BackColor = System::Drawing::SystemColors::ControlDark;
			this->toolsGroupBox->Controls->Add(this->sceneToolButton);
			this->toolsGroupBox->Controls->Add(this->addToolButton);
			this->toolsGroupBox->Controls->Add(this->selectToolButton);
			this->toolsGroupBox->Controls->Add(this->listToolButton);
			this->toolsGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->toolsGroupBox->Location = System::Drawing::Point(0, 0);
			this->toolsGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->toolsGroupBox->Name = L"toolsGroupBox";
			this->toolsGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->toolsGroupBox->Size = System::Drawing::Size(483, 101);
			this->toolsGroupBox->TabIndex = 5;
			this->toolsGroupBox->TabStop = false;
			this->toolsGroupBox->Text = L"Tools";
			// 
			// sceneToolButton
			// 
			this->sceneToolButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->sceneToolButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"sceneToolButton.BackgroundImage")));
			this->sceneToolButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->sceneToolButton->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->sceneToolButton->FlatAppearance->CheckedBackColor = System::Drawing::Color::DodgerBlue;
			this->sceneToolButton->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->sceneToolButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->sceneToolButton->Location = System::Drawing::Point(319, 27);
			this->sceneToolButton->Name = L"sceneToolButton";
			this->sceneToolButton->Size = System::Drawing::Size(54, 54);
			this->sceneToolButton->TabIndex = 27;
			this->sceneToolButton->TabStop = true;
			this->sceneToolButton->UseVisualStyleBackColor = true;
			this->sceneToolButton->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::sceneToolButton_CheckedChanged);
			// 
			// addToolButton
			// 
			this->addToolButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->addToolButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"addToolButton.BackgroundImage")));
			this->addToolButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->addToolButton->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->addToolButton->FlatAppearance->CheckedBackColor = System::Drawing::Color::DodgerBlue;
			this->addToolButton->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->addToolButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->addToolButton->Location = System::Drawing::Point(248, 27);
			this->addToolButton->Name = L"addToolButton";
			this->addToolButton->Size = System::Drawing::Size(54, 54);
			this->addToolButton->TabIndex = 26;
			this->addToolButton->TabStop = true;
			this->addToolButton->UseVisualStyleBackColor = true;
			this->addToolButton->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::addToolButton_CheckedChanged);
			// 
			// selectToolButton
			// 
			this->selectToolButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->selectToolButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"selectToolButton.BackgroundImage")));
			this->selectToolButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->selectToolButton->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->selectToolButton->FlatAppearance->CheckedBackColor = System::Drawing::Color::DodgerBlue;
			this->selectToolButton->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->selectToolButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->selectToolButton->Location = System::Drawing::Point(177, 27);
			this->selectToolButton->Name = L"selectToolButton";
			this->selectToolButton->Size = System::Drawing::Size(54, 54);
			this->selectToolButton->TabIndex = 25;
			this->selectToolButton->TabStop = true;
			this->selectToolButton->UseVisualStyleBackColor = true;
			this->selectToolButton->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::selectToolButton_CheckedChanged);
			// 
			// listToolButton
			// 
			this->listToolButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->listToolButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"listToolButton.BackgroundImage")));
			this->listToolButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->listToolButton->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->listToolButton->FlatAppearance->CheckedBackColor = System::Drawing::Color::DodgerBlue;
			this->listToolButton->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->listToolButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->listToolButton->Location = System::Drawing::Point(109, 27);
			this->listToolButton->Name = L"listToolButton";
			this->listToolButton->Size = System::Drawing::Size(54, 54);
			this->listToolButton->TabIndex = 24;
			this->listToolButton->TabStop = true;
			this->listToolButton->UseVisualStyleBackColor = true;
			this->listToolButton->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::listToolButton_CheckedChanged);
			// 
			// selectGroupBox
			// 
			this->selectGroupBox->AutoSize = true;
			this->selectGroupBox->Controls->Add(this->rotateObjButton);
			this->selectGroupBox->Controls->Add(this->scaleObjButton);
			this->selectGroupBox->Controls->Add(this->moveObjButton);
			this->selectGroupBox->Controls->Add(this->selectObjButton);
			this->selectGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->selectGroupBox->Location = System::Drawing::Point(0, 101);
			this->selectGroupBox->Margin = System::Windows::Forms::Padding(4);
			this->selectGroupBox->Name = L"selectGroupBox";
			this->selectGroupBox->Padding = System::Windows::Forms::Padding(4);
			this->selectGroupBox->Size = System::Drawing::Size(483, 110);
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
			this->rotateObjButton->Location = System::Drawing::Point(311, 34);
			this->rotateObjButton->Name = L"rotateObjButton";
			this->rotateObjButton->Size = System::Drawing::Size(54, 54);
			this->rotateObjButton->TabIndex = 23;
			this->rotateObjButton->TabStop = true;
			this->rotateObjButton->UseVisualStyleBackColor = true;
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
			this->scaleObjButton->Location = System::Drawing::Point(240, 34);
			this->scaleObjButton->Name = L"scaleObjButton";
			this->scaleObjButton->Size = System::Drawing::Size(54, 54);
			this->scaleObjButton->TabIndex = 22;
			this->scaleObjButton->TabStop = true;
			this->scaleObjButton->UseVisualStyleBackColor = true;
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
			this->moveObjButton->Location = System::Drawing::Point(169, 34);
			this->moveObjButton->Name = L"moveObjButton";
			this->moveObjButton->Size = System::Drawing::Size(54, 54);
			this->moveObjButton->TabIndex = 21;
			this->moveObjButton->TabStop = true;
			this->moveObjButton->UseVisualStyleBackColor = true;
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
			this->selectObjButton->Location = System::Drawing::Point(101, 34);
			this->selectObjButton->Name = L"selectObjButton";
			this->selectObjButton->Size = System::Drawing::Size(54, 54);
			this->selectObjButton->TabIndex = 20;
			this->selectObjButton->TabStop = true;
			this->selectObjButton->UseVisualStyleBackColor = true;
			this->selectObjButton->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::selectObjButton_CheckedChanged);
			// 
			// listGroupBox
			// 
			this->listGroupBox->Controls->Add(this->objectsTree);
			this->listGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->listGroupBox->Location = System::Drawing::Point(0, 211);
			this->listGroupBox->Margin = System::Windows::Forms::Padding(4);
			this->listGroupBox->Name = L"listGroupBox";
			this->listGroupBox->Padding = System::Windows::Forms::Padding(4);
			this->listGroupBox->Size = System::Drawing::Size(483, 123);
			this->listGroupBox->TabIndex = 12;
			this->listGroupBox->TabStop = false;
			this->listGroupBox->Text = L"List";
			this->listGroupBox->Visible = false;
			// 
			// objectsTree
			// 
			this->objectsTree->BackColor = System::Drawing::Color::Silver;
			this->objectsTree->Dock = System::Windows::Forms::DockStyle::Fill;
			this->objectsTree->Location = System::Drawing::Point(4, 19);
			this->objectsTree->Margin = System::Windows::Forms::Padding(4);
			this->objectsTree->Name = L"objectsTree";
			this->objectsTree->Size = System::Drawing::Size(475, 100);
			this->objectsTree->TabIndex = 1;
			// 
			// entityGroupBox
			// 
			this->entityGroupBox->AutoSize = true;
			this->entityGroupBox->Controls->Add(this->label3);
			this->entityGroupBox->Controls->Add(this->entScaleZ);
			this->entityGroupBox->Controls->Add(this->entScaleY);
			this->entityGroupBox->Controls->Add(this->entScaleX);
			this->entityGroupBox->Controls->Add(this->label2);
			this->entityGroupBox->Controls->Add(this->entPosZ);
			this->entityGroupBox->Controls->Add(this->entPosY);
			this->entityGroupBox->Controls->Add(this->entPosX);
			this->entityGroupBox->Controls->Add(this->entNameBox);
			this->entityGroupBox->Controls->Add(this->label1);
			this->entityGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->entityGroupBox->Location = System::Drawing::Point(0, 580);
			this->entityGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->entityGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->entityGroupBox->Name = L"entityGroupBox";
			this->entityGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->entityGroupBox->Size = System::Drawing::Size(483, 119);
			this->entityGroupBox->TabIndex = 13;
			this->entityGroupBox->TabStop = false;
			this->entityGroupBox->Text = L"Entity";
			this->entityGroupBox->Visible = false;
			// 
			// addGroupBox
			// 
			this->addGroupBox->Controls->Add(this->label4);
			this->addGroupBox->Controls->Add(this->comboBox1);
			this->addGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->addGroupBox->Location = System::Drawing::Point(0, 334);
			this->addGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->addGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->addGroupBox->Name = L"addGroupBox";
			this->addGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->addGroupBox->Size = System::Drawing::Size(483, 123);
			this->addGroupBox->TabIndex = 14;
			this->addGroupBox->TabStop = false;
			this->addGroupBox->Text = L"Add";
			this->addGroupBox->Visible = false;
			// 
			// sceneGroupBox
			// 
			this->sceneGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->sceneGroupBox->Location = System::Drawing::Point(0, 457);
			this->sceneGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->sceneGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->sceneGroupBox->Name = L"sceneGroupBox";
			this->sceneGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->sceneGroupBox->Size = System::Drawing::Size(483, 123);
			this->sceneGroupBox->TabIndex = 15;
			this->sceneGroupBox->TabStop = false;
			this->sceneGroupBox->Text = L"Scene";
			this->sceneGroupBox->Visible = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(82, 21);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(45, 17);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Name";
			// 
			// entNameBox
			// 
			this->entNameBox->Location = System::Drawing::Point(177, 21);
			this->entNameBox->Name = L"entNameBox";
			this->entNameBox->Size = System::Drawing::Size(222, 22);
			this->entNameBox->TabIndex = 1;
			// 
			// entPosX
			// 
			this->entPosX->Location = System::Drawing::Point(177, 49);
			this->entPosX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->entPosX->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->entPosX->Name = L"entPosX";
			this->entPosX->Size = System::Drawing::Size(70, 22);
			this->entPosX->TabIndex = 2;
			// 
			// entPosY
			// 
			this->entPosY->Location = System::Drawing::Point(253, 49);
			this->entPosY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->entPosY->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->entPosY->Name = L"entPosY";
			this->entPosY->Size = System::Drawing::Size(70, 22);
			this->entPosY->TabIndex = 3;
			// 
			// entPosZ
			// 
			this->entPosZ->Location = System::Drawing::Point(329, 49);
			this->entPosZ->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->entPosZ->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->entPosZ->Name = L"entPosZ";
			this->entPosZ->Size = System::Drawing::Size(70, 22);
			this->entPosZ->TabIndex = 4;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(69, 51);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(58, 17);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Position";
			this->label2->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(82, 79);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(43, 17);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Scale";
			this->label3->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// entScaleZ
			// 
			this->entScaleZ->Location = System::Drawing::Point(329, 77);
			this->entScaleZ->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->entScaleZ->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleZ->Name = L"entScaleZ";
			this->entScaleZ->Size = System::Drawing::Size(70, 22);
			this->entScaleZ->TabIndex = 8;
			this->entScaleZ->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			// 
			// entScaleY
			// 
			this->entScaleY->Location = System::Drawing::Point(253, 77);
			this->entScaleY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->entScaleY->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleY->Name = L"entScaleY";
			this->entScaleY->Size = System::Drawing::Size(70, 22);
			this->entScaleY->TabIndex = 7;
			this->entScaleY->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			// 
			// entScaleX
			// 
			this->entScaleX->Location = System::Drawing::Point(177, 77);
			this->entScaleX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->entScaleX->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleX->Name = L"entScaleX";
			this->entScaleX->Size = System::Drawing::Size(70, 22);
			this->entScaleX->TabIndex = 6;
			this->entScaleX->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Tree" });
			this->comboBox1->Location = System::Drawing::Point(177, 21);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(222, 24);
			this->comboBox1->TabIndex = 0;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::comboBox1_SelectedIndexChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(42, 24);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(85, 17);
			this->label4->TabIndex = 1;
			this->label4->Text = L"Object Type";
			// 
			// EditorForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->AutoSize = true;
			this->BackColor = System::Drawing::SystemColors::ControlDark;
			this->ClientSize = System::Drawing::Size(483, 780);
			this->Controls->Add(this->entityGroupBox);
			this->Controls->Add(this->sceneGroupBox);
			this->Controls->Add(this->addGroupBox);
			this->Controls->Add(this->listGroupBox);
			this->Controls->Add(this->selectGroupBox);
			this->Controls->Add(this->toolsGroupBox);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"EditorForm";
			this->Text = L"Editor";
			this->toolsGroupBox->ResumeLayout(false);
			this->selectGroupBox->ResumeLayout(false);
			this->listGroupBox->ResumeLayout(false);
			this->entityGroupBox->ResumeLayout(false);
			this->entityGroupBox->PerformLayout();
			this->addGroupBox->ResumeLayout(false);
			this->addGroupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosX))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosZ))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleZ))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleX))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

	}

private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	System::Windows::Forms::NumericUpDown^ numeric = (System::Windows::Forms::NumericUpDown^)sender;

	pin_ptr<const wchar_t> ppchar = PtrToStringChars(numeric->Name);

	//::MessageBox(0, std::to_wstring(System::Decimal::ToSingle(numeric->Value)).data(), ppchar, MB_OK);
}

private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {

	/*pin_ptr<const wchar_t> ppchar = PtrToStringChars(button3->Text);
	std::wstring wstr = ppchar;

	pin_ptr<const wchar_t> ppchar2 = PtrToStringChars(button1->Text);
	std::wstring wstr2 = ppchar2;

	auto i = treeView1->Nodes->Add(gcnew TreeNode(button3->Text));
	treeView1->Nodes[i]->Nodes->Add(gcnew TreeNode(button1->Text));*/
}



private: void hideGroups()
{
	listGroupBox->Hide();
	addGroupBox->Hide();
	selectGroupBox->Hide();
	sceneGroupBox->Hide();
}

private: System::Void listToolButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	hideGroups();
	listGroupBox->Show();
}
private: System::Void selectToolButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	hideGroups();
	selectGroupBox->Show();
}
private: System::Void addToolButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	hideGroups();
	addGroupBox->Show();

	SendMsg(UiMessageId::AddItemMode, nullptr);
}
private: System::Void sceneToolButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	hideGroups();
	sceneGroupBox->Show();
}
private: System::Void selectObjButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

//	if(((System::Windows::Forms::RadioButton^)sender)->i
	entityGroupBox->BringToFront();
	entityGroupBox->Show();

	SendMsg(UiMessageId::SelectMode, nullptr);
}

public: System::Void showItemInfo(EntityInfo* info)
{
	entityGroupBox->BringToFront();
	entityGroupBox->Show();

	entNameBox->Text = gcnew System::String(info->name.data());

	entPosX->Value = System::Decimal(info->pos.x);
	entPosY->Value = System::Decimal(info->pos.y);
	entPosZ->Value = System::Decimal(info->pos.z);

	entScaleX->Value = System::Decimal(info->scale.x);
	entScaleY->Value = System::Decimal(info->scale.y);
	entScaleZ->Value = System::Decimal(info->scale.z);

}

private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
}
};
}
