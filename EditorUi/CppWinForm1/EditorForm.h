#pragma once
#include <windows.h>
#include <string>
#include <vcclr.h>
#include <functional>
#include "..\..\Ballz\EditorComm.h"
#include <msclr\marshal_cppstd.h>

using namespace msclr::interop;

extern void SendMsg(UiMessageId id, void* data);

template<typename T>
void CpySendMsg(UiMessageId id, T data)
{
	SendMsg(id, &data);
}

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

		EditorForm(void)
		{
			InitializeComponent();

			labels = gcnew array<System::Windows::Forms::Label^>(10);
			numerics = gcnew array<System::Windows::Forms::NumericUpDown^>(10);

			instance = this;
		}

		HWND getRenderWindowHandle()
		{
			auto intHwnd = renderPanel->Handle;
			return static_cast<HWND>(intHwnd.ToPointer());;
		}

		HWND getTopWindowHandle()
		{
			auto intHwnd = this->Handle;
			return static_cast<HWND>(intHwnd.ToPointer());;
		}
	private: System::Windows::Forms::Panel^  topPanel;

	public:

	private: System::Windows::Forms::Panel^  renderPanel;
	private: System::Windows::Forms::GroupBox^  placementGroupBox;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::NumericUpDown^  placementOffsetNum;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::GroupBox^  utilsGroupBox;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::ComboBox^  utilsComboBox;
	private: System::Windows::Forms::PictureBox^  levelLoading;

	private: System::Windows::Forms::Panel^  bottomPanel;


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
	private: array<System::Windows::Forms::Label^>^  labels;
	private: array<System::Windows::Forms::NumericUpDown^>^  numerics;
	private: System::Windows::Forms::Panel^  sidePanel;
	private: System::Windows::Forms::GroupBox^  selectionGroupBox;
	private: System::Windows::Forms::Button^  selectionNameButton;
	private: System::Windows::Forms::ListBox^  selectionListBox;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  entScaleZ;
	private: System::Windows::Forms::NumericUpDown^  entScaleY;
	private: System::Windows::Forms::NumericUpDown^  entScaleX;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  entPosZ;
	private: System::Windows::Forms::NumericUpDown^  entPosY;
	private: System::Windows::Forms::NumericUpDown^  entPosX;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  sceneGroupBox;
	private: System::Windows::Forms::ComboBox^  lutComboBox;
	private: System::Windows::Forms::ComboBox^  skyboxComboBox;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::GroupBox^  addGroupBox;
	private: System::Windows::Forms::TextBox^  addItemPrefixTextBox;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ComboBox^  addItemTypeComboBox;
	private: System::Windows::Forms::GroupBox^  listGroupBox;
	private: System::Windows::Forms::TreeView^  sceneListTree;

	private: System::Windows::Forms::RadioButton^  rotateObjButton;
	private: System::Windows::Forms::RadioButton^  scaleObjButton;
	private: System::Windows::Forms::RadioButton^  moveObjButton;
	private: System::Windows::Forms::RadioButton^  selectObjButton;
	private: System::Windows::Forms::GroupBox^  toolsGroupBox;
	private: System::Windows::Forms::CheckBox^  utilsCheckBox;
	private: System::Windows::Forms::CheckBox^  sceneCheckBox;
	private: System::Windows::Forms::CheckBox^  addItemCheckBox;

	private: System::Windows::Forms::CheckBox^  listCheckBox;

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
			this->sidePanel = (gcnew System::Windows::Forms::Panel());
			this->placementGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->placementOffsetNum = (gcnew System::Windows::Forms::NumericUpDown());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->utilsGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->utilsComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->selectionGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->selectionNameButton = (gcnew System::Windows::Forms::Button());
			this->selectionListBox = (gcnew System::Windows::Forms::ListBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->entScaleZ = (gcnew System::Windows::Forms::NumericUpDown());
			this->entScaleY = (gcnew System::Windows::Forms::NumericUpDown());
			this->entScaleX = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->entPosZ = (gcnew System::Windows::Forms::NumericUpDown());
			this->entPosY = (gcnew System::Windows::Forms::NumericUpDown());
			this->entPosX = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->sceneGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->lutComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->skyboxComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->addGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->addItemPrefixTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->addItemTypeComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->listGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->sceneListTree = (gcnew System::Windows::Forms::TreeView());
			this->toolsGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->utilsCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->sceneCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->addItemCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->listCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->rotateObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->scaleObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->moveObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->selectObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->topPanel = (gcnew System::Windows::Forms::Panel());
			this->renderPanel = (gcnew System::Windows::Forms::Panel());
			this->bottomPanel = (gcnew System::Windows::Forms::Panel());
			this->levelLoading = (gcnew System::Windows::Forms::PictureBox());
			this->sidePanel->SuspendLayout();
			this->placementGroupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->placementOffsetNum))->BeginInit();
			this->utilsGroupBox->SuspendLayout();
			this->selectionGroupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleZ))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleX))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosZ))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosX))->BeginInit();
			this->sceneGroupBox->SuspendLayout();
			this->addGroupBox->SuspendLayout();
			this->listGroupBox->SuspendLayout();
			this->toolsGroupBox->SuspendLayout();
			this->topPanel->SuspendLayout();
			this->renderPanel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->levelLoading))->BeginInit();
			this->SuspendLayout();
			// 
			// sidePanel
			// 
			this->sidePanel->AutoScroll = true;
			this->sidePanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->sidePanel->Controls->Add(this->placementGroupBox);
			this->sidePanel->Controls->Add(this->utilsGroupBox);
			this->sidePanel->Controls->Add(this->selectionGroupBox);
			this->sidePanel->Controls->Add(this->sceneGroupBox);
			this->sidePanel->Controls->Add(this->addGroupBox);
			this->sidePanel->Controls->Add(this->listGroupBox);
			this->sidePanel->Controls->Add(this->toolsGroupBox);
			this->sidePanel->Dock = System::Windows::Forms::DockStyle::Left;
			this->sidePanel->Location = System::Drawing::Point(0, 0);
			this->sidePanel->Name = L"sidePanel";
			this->sidePanel->Size = System::Drawing::Size(398, 780);
			this->sidePanel->TabIndex = 16;
			// 
			// placementGroupBox
			// 
			this->placementGroupBox->AutoSize = true;
			this->placementGroupBox->Controls->Add(this->button1);
			this->placementGroupBox->Controls->Add(this->placementOffsetNum);
			this->placementGroupBox->Controls->Add(this->label8);
			this->placementGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->placementGroupBox->Location = System::Drawing::Point(0, 653);
			this->placementGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->placementGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->placementGroupBox->Name = L"placementGroupBox";
			this->placementGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->placementGroupBox->Size = System::Drawing::Size(394, 62);
			this->placementGroupBox->TabIndex = 25;
			this->placementGroupBox->TabStop = false;
			this->placementGroupBox->Text = L"Placement";
			this->placementGroupBox->Visible = false;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(234, 19);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 4;
			this->button1->Text = L"Ray";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &EditorForm::button1_Click);
			// 
			// placementOffsetNum
			// 
			this->placementOffsetNum->DecimalPlaces = 2;
			this->placementOffsetNum->Location = System::Drawing::Point(103, 19);
			this->placementOffsetNum->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->placementOffsetNum->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->placementOffsetNum->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->placementOffsetNum->Name = L"placementOffsetNum";
			this->placementOffsetNum->Size = System::Drawing::Size(91, 22);
			this->placementOffsetNum->TabIndex = 3;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(41, 21);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(46, 17);
			this->label8->TabIndex = 1;
			this->label8->Text = L"Offset";
			// 
			// utilsGroupBox
			// 
			this->utilsGroupBox->AutoSize = true;
			this->utilsGroupBox->Controls->Add(this->label9);
			this->utilsGroupBox->Controls->Add(this->utilsComboBox);
			this->utilsGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->utilsGroupBox->Location = System::Drawing::Point(0, 591);
			this->utilsGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->utilsGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->utilsGroupBox->Name = L"utilsGroupBox";
			this->utilsGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->utilsGroupBox->Size = System::Drawing::Size(394, 62);
			this->utilsGroupBox->TabIndex = 22;
			this->utilsGroupBox->TabStop = false;
			this->utilsGroupBox->Text = L"Utilities";
			this->utilsGroupBox->Visible = false;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(45, 24);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(40, 17);
			this->label9->TabIndex = 1;
			this->label9->Text = L"Type";
			// 
			// utilsComboBox
			// 
			this->utilsComboBox->FormattingEnabled = true;
			this->utilsComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Placement" });
			this->utilsComboBox->Location = System::Drawing::Point(101, 19);
			this->utilsComboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->utilsComboBox->Name = L"utilsComboBox";
			this->utilsComboBox->Size = System::Drawing::Size(231, 24);
			this->utilsComboBox->TabIndex = 0;
			this->utilsComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::utilsComboBox_SelectedIndexChanged);
			// 
			// selectionGroupBox
			// 
			this->selectionGroupBox->AutoSize = true;
			this->selectionGroupBox->Controls->Add(this->selectionNameButton);
			this->selectionGroupBox->Controls->Add(this->selectionListBox);
			this->selectionGroupBox->Controls->Add(this->label3);
			this->selectionGroupBox->Controls->Add(this->entScaleZ);
			this->selectionGroupBox->Controls->Add(this->entScaleY);
			this->selectionGroupBox->Controls->Add(this->entScaleX);
			this->selectionGroupBox->Controls->Add(this->label2);
			this->selectionGroupBox->Controls->Add(this->entPosZ);
			this->selectionGroupBox->Controls->Add(this->entPosY);
			this->selectionGroupBox->Controls->Add(this->entPosX);
			this->selectionGroupBox->Controls->Add(this->label1);
			this->selectionGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->selectionGroupBox->Location = System::Drawing::Point(0, 414);
			this->selectionGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->selectionGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->selectionGroupBox->Name = L"selectionGroupBox";
			this->selectionGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->selectionGroupBox->Size = System::Drawing::Size(394, 177);
			this->selectionGroupBox->TabIndex = 19;
			this->selectionGroupBox->TabStop = false;
			this->selectionGroupBox->Text = L"Selection";
			this->selectionGroupBox->Visible = false;
			// 
			// selectionNameButton
			// 
			this->selectionNameButton->AutoSize = true;
			this->selectionNameButton->FlatAppearance->BorderSize = 0;
			this->selectionNameButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Gray;
			this->selectionNameButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->selectionNameButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->selectionNameButton->Location = System::Drawing::Point(101, 20);
			this->selectionNameButton->Name = L"selectionNameButton";
			this->selectionNameButton->Size = System::Drawing::Size(75, 23);
			this->selectionNameButton->TabIndex = 12;
			this->selectionNameButton->UseVisualStyleBackColor = true;
			this->selectionNameButton->Click += gcnew System::EventHandler(this, &EditorForm::selectionNameButton_Click);
			// 
			// selectionListBox
			// 
			this->selectionListBox->FormattingEnabled = true;
			this->selectionListBox->ItemHeight = 16;
			this->selectionListBox->Location = System::Drawing::Point(102, 105);
			this->selectionListBox->Name = L"selectionListBox";
			this->selectionListBox->Size = System::Drawing::Size(232, 52);
			this->selectionListBox->TabIndex = 11;
			this->selectionListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::selectionListBox_SelectedIndexChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(44, 81);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(43, 17);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Scale";
			this->label3->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// entScaleZ
			// 
			this->entScaleZ->DecimalPlaces = 2;
			this->entScaleZ->Location = System::Drawing::Point(295, 78);
			this->entScaleZ->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->entScaleZ->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->entScaleZ->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleZ->Name = L"entScaleZ";
			this->entScaleZ->Size = System::Drawing::Size(91, 22);
			this->entScaleZ->TabIndex = 8;
			this->entScaleZ->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleZ->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entScaleZ_ValueChanged);
			// 
			// entScaleY
			// 
			this->entScaleY->DecimalPlaces = 2;
			this->entScaleY->Location = System::Drawing::Point(199, 78);
			this->entScaleY->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->entScaleY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->entScaleY->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleY->Name = L"entScaleY";
			this->entScaleY->Size = System::Drawing::Size(91, 22);
			this->entScaleY->TabIndex = 7;
			this->entScaleY->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleY->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entScaleY_ValueChanged);
			// 
			// entScaleX
			// 
			this->entScaleX->DecimalPlaces = 2;
			this->entScaleX->Location = System::Drawing::Point(103, 78);
			this->entScaleX->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->entScaleX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->entScaleX->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleX->Name = L"entScaleX";
			this->entScaleX->Size = System::Drawing::Size(91, 22);
			this->entScaleX->TabIndex = 6;
			this->entScaleX->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->entScaleX->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entScaleX_ValueChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(30, 52);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(58, 17);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Position";
			this->label2->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// entPosZ
			// 
			this->entPosZ->DecimalPlaces = 2;
			this->entPosZ->Location = System::Drawing::Point(295, 50);
			this->entPosZ->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->entPosZ->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->entPosZ->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->entPosZ->Name = L"entPosZ";
			this->entPosZ->Size = System::Drawing::Size(91, 22);
			this->entPosZ->TabIndex = 4;
			this->entPosZ->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entPosZ_ValueChanged);
			// 
			// entPosY
			// 
			this->entPosY->DecimalPlaces = 2;
			this->entPosY->Location = System::Drawing::Point(199, 50);
			this->entPosY->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->entPosY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->entPosY->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->entPosY->Name = L"entPosY";
			this->entPosY->Size = System::Drawing::Size(91, 22);
			this->entPosY->TabIndex = 3;
			this->entPosY->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entPosY_ValueChanged);
			// 
			// entPosX
			// 
			this->entPosX->DecimalPlaces = 2;
			this->entPosX->Location = System::Drawing::Point(103, 50);
			this->entPosX->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->entPosX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->entPosX->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->entPosX->Name = L"entPosX";
			this->entPosX->Size = System::Drawing::Size(91, 22);
			this->entPosX->TabIndex = 2;
			this->entPosX->ValueChanged += gcnew System::EventHandler(this, &EditorForm::entPosX_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(44, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(45, 17);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Name";
			// 
			// sceneGroupBox
			// 
			this->sceneGroupBox->AutoSize = true;
			this->sceneGroupBox->Controls->Add(this->lutComboBox);
			this->sceneGroupBox->Controls->Add(this->skyboxComboBox);
			this->sceneGroupBox->Controls->Add(this->label6);
			this->sceneGroupBox->Controls->Add(this->label5);
			this->sceneGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->sceneGroupBox->Location = System::Drawing::Point(0, 319);
			this->sceneGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->sceneGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->sceneGroupBox->Name = L"sceneGroupBox";
			this->sceneGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->sceneGroupBox->Size = System::Drawing::Size(394, 95);
			this->sceneGroupBox->TabIndex = 21;
			this->sceneGroupBox->TabStop = false;
			this->sceneGroupBox->Text = L"Scene";
			this->sceneGroupBox->Visible = false;
			// 
			// lutComboBox
			// 
			this->lutComboBox->FormattingEnabled = true;
			this->lutComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Tree" });
			this->lutComboBox->Location = System::Drawing::Point(101, 52);
			this->lutComboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->lutComboBox->Name = L"lutComboBox";
			this->lutComboBox->Size = System::Drawing::Size(231, 24);
			this->lutComboBox->TabIndex = 3;
			this->lutComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::lutComboBox_SelectedIndexChanged);
			// 
			// skyboxComboBox
			// 
			this->skyboxComboBox->FormattingEnabled = true;
			this->skyboxComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Tree" });
			this->skyboxComboBox->Location = System::Drawing::Point(101, 19);
			this->skyboxComboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->skyboxComboBox->Name = L"skyboxComboBox";
			this->skyboxComboBox->Size = System::Drawing::Size(231, 24);
			this->skyboxComboBox->TabIndex = 2;
			this->skyboxComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::skyboxComboBox_SelectedIndexChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(52, 61);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(35, 17);
			this->label6->TabIndex = 2;
			this->label6->Text = L"LUT";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(34, 22);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(53, 17);
			this->label5->TabIndex = 1;
			this->label5->Text = L"Skybox";
			// 
			// addGroupBox
			// 
			this->addGroupBox->AutoSize = true;
			this->addGroupBox->Controls->Add(this->addItemPrefixTextBox);
			this->addGroupBox->Controls->Add(this->label7);
			this->addGroupBox->Controls->Add(this->label4);
			this->addGroupBox->Controls->Add(this->addItemTypeComboBox);
			this->addGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->addGroupBox->Location = System::Drawing::Point(0, 219);
			this->addGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->addGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->addGroupBox->Name = L"addGroupBox";
			this->addGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->addGroupBox->Size = System::Drawing::Size(394, 100);
			this->addGroupBox->TabIndex = 20;
			this->addGroupBox->TabStop = false;
			this->addGroupBox->Text = L"Add";
			this->addGroupBox->Visible = false;
			// 
			// addItemPrefixTextBox
			// 
			this->addItemPrefixTextBox->Location = System::Drawing::Point(101, 57);
			this->addItemPrefixTextBox->Margin = System::Windows::Forms::Padding(4);
			this->addItemPrefixTextBox->Name = L"addItemPrefixTextBox";
			this->addItemPrefixTextBox->Size = System::Drawing::Size(231, 22);
			this->addItemPrefixTextBox->TabIndex = 3;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(46, 60);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(43, 17);
			this->label7->TabIndex = 2;
			this->label7->Text = L"Prefix";
			this->label7->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(4, 22);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(85, 17);
			this->label4->TabIndex = 1;
			this->label4->Text = L"Object Type";
			// 
			// addItemTypeComboBox
			// 
			this->addItemTypeComboBox->FormattingEnabled = true;
			this->addItemTypeComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Tree" });
			this->addItemTypeComboBox->Location = System::Drawing::Point(101, 19);
			this->addItemTypeComboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->addItemTypeComboBox->Name = L"addItemTypeComboBox";
			this->addItemTypeComboBox->Size = System::Drawing::Size(231, 24);
			this->addItemTypeComboBox->TabIndex = 0;
			// 
			// listGroupBox
			// 
			this->listGroupBox->Controls->Add(this->sceneListTree);
			this->listGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->listGroupBox->Location = System::Drawing::Point(0, 96);
			this->listGroupBox->Margin = System::Windows::Forms::Padding(4);
			this->listGroupBox->Name = L"listGroupBox";
			this->listGroupBox->Padding = System::Windows::Forms::Padding(4);
			this->listGroupBox->Size = System::Drawing::Size(394, 123);
			this->listGroupBox->TabIndex = 18;
			this->listGroupBox->TabStop = false;
			this->listGroupBox->Text = L"List";
			this->listGroupBox->Visible = false;
			// 
			// sceneListTree
			// 
			this->sceneListTree->BackColor = System::Drawing::Color::Silver;
			this->sceneListTree->Dock = System::Windows::Forms::DockStyle::Fill;
			this->sceneListTree->Location = System::Drawing::Point(4, 19);
			this->sceneListTree->Margin = System::Windows::Forms::Padding(4);
			this->sceneListTree->Name = L"sceneListTree";
			this->sceneListTree->Size = System::Drawing::Size(386, 100);
			this->sceneListTree->TabIndex = 1;
			this->sceneListTree->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &EditorForm::sceneListTree_AfterSelect);
			// 
			// toolsGroupBox
			// 
			this->toolsGroupBox->AutoSize = true;
			this->toolsGroupBox->BackColor = System::Drawing::SystemColors::ControlDark;
			this->toolsGroupBox->Controls->Add(this->utilsCheckBox);
			this->toolsGroupBox->Controls->Add(this->sceneCheckBox);
			this->toolsGroupBox->Controls->Add(this->addItemCheckBox);
			this->toolsGroupBox->Controls->Add(this->listCheckBox);
			this->toolsGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->toolsGroupBox->Location = System::Drawing::Point(0, 0);
			this->toolsGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->toolsGroupBox->Name = L"toolsGroupBox";
			this->toolsGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->toolsGroupBox->Size = System::Drawing::Size(394, 96);
			this->toolsGroupBox->TabIndex = 16;
			this->toolsGroupBox->TabStop = false;
			this->toolsGroupBox->Text = L"Tools";
			// 
			// utilsCheckBox
			// 
			this->utilsCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->utilsCheckBox->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"utilsCheckBox.BackgroundImage")));
			this->utilsCheckBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->utilsCheckBox->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->utilsCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::Color::DodgerBlue;
			this->utilsCheckBox->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->utilsCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->utilsCheckBox->Location = System::Drawing::Point(279, 21);
			this->utilsCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->utilsCheckBox->Name = L"utilsCheckBox";
			this->utilsCheckBox->Size = System::Drawing::Size(53, 54);
			this->utilsCheckBox->TabIndex = 32;
			this->utilsCheckBox->UseVisualStyleBackColor = true;
			this->utilsCheckBox->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::utilsCheckBox_CheckedChanged);
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
			this->sceneCheckBox->Location = System::Drawing::Point(138, 21);
			this->sceneCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->sceneCheckBox->Name = L"sceneCheckBox";
			this->sceneCheckBox->Size = System::Drawing::Size(53, 54);
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
			this->addItemCheckBox->Location = System::Drawing::Point(209, 21);
			this->addItemCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->addItemCheckBox->Name = L"addItemCheckBox";
			this->addItemCheckBox->Size = System::Drawing::Size(53, 54);
			this->addItemCheckBox->TabIndex = 30;
			this->addItemCheckBox->UseVisualStyleBackColor = true;
			this->addItemCheckBox->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::addItemCheckBox_CheckedChanged);
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
			this->listCheckBox->Location = System::Drawing::Point(66, 21);
			this->listCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->listCheckBox->Name = L"listCheckBox";
			this->listCheckBox->Size = System::Drawing::Size(53, 54);
			this->listCheckBox->TabIndex = 28;
			this->listCheckBox->UseVisualStyleBackColor = true;
			this->listCheckBox->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::listCheckBox_CheckedChanged);
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
			this->rotateObjButton->Location = System::Drawing::Point(547, 23);
			this->rotateObjButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->rotateObjButton->Name = L"rotateObjButton";
			this->rotateObjButton->Size = System::Drawing::Size(53, 54);
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
			this->scaleObjButton->Location = System::Drawing::Point(476, 23);
			this->scaleObjButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->scaleObjButton->Name = L"scaleObjButton";
			this->scaleObjButton->Size = System::Drawing::Size(53, 54);
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
			this->moveObjButton->Location = System::Drawing::Point(405, 23);
			this->moveObjButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->moveObjButton->Name = L"moveObjButton";
			this->moveObjButton->Size = System::Drawing::Size(53, 54);
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
			this->selectObjButton->Location = System::Drawing::Point(337, 23);
			this->selectObjButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->selectObjButton->Name = L"selectObjButton";
			this->selectObjButton->Size = System::Drawing::Size(53, 54);
			this->selectObjButton->TabIndex = 20;
			this->selectObjButton->TabStop = true;
			this->selectObjButton->UseVisualStyleBackColor = true;
			this->selectObjButton->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::selectObjButton_CheckedChanged);
			// 
			// topPanel
			// 
			this->topPanel->Controls->Add(this->rotateObjButton);
			this->topPanel->Controls->Add(this->scaleObjButton);
			this->topPanel->Controls->Add(this->selectObjButton);
			this->topPanel->Controls->Add(this->moveObjButton);
			this->topPanel->Dock = System::Windows::Forms::DockStyle::Top;
			this->topPanel->Location = System::Drawing::Point(398, 0);
			this->topPanel->Name = L"topPanel";
			this->topPanel->Size = System::Drawing::Size(966, 87);
			this->topPanel->TabIndex = 18;
			// 
			// renderPanel
			// 
			this->renderPanel->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->renderPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->renderPanel->Controls->Add(this->levelLoading);
			this->renderPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->renderPanel->Location = System::Drawing::Point(398, 87);
			this->renderPanel->Name = L"renderPanel";
			this->renderPanel->Size = System::Drawing::Size(966, 693);
			this->renderPanel->TabIndex = 19;
			// 
			// bottomPanel
			// 
			this->bottomPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->bottomPanel->Location = System::Drawing::Point(398, 699);
			this->bottomPanel->Name = L"bottomPanel";
			this->bottomPanel->Size = System::Drawing::Size(966, 81);
			this->bottomPanel->TabIndex = 20;
			// 
			// levelLoading
			// 
			this->levelLoading->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->levelLoading->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->levelLoading->Dock = System::Windows::Forms::DockStyle::Fill;
			this->levelLoading->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"levelLoading.Image")));
			this->levelLoading->InitialImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"levelLoading.InitialImage")));
			this->levelLoading->Location = System::Drawing::Point(0, 0);
			this->levelLoading->Name = L"levelLoading";
			this->levelLoading->Size = System::Drawing::Size(966, 693);
			this->levelLoading->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->levelLoading->TabIndex = 0;
			this->levelLoading->TabStop = false;
			// 
			// EditorForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->AutoSize = true;
			this->BackColor = System::Drawing::SystemColors::ControlDark;
			this->ClientSize = System::Drawing::Size(1364, 780);
			this->Controls->Add(this->bottomPanel);
			this->Controls->Add(this->renderPanel);
			this->Controls->Add(this->topPanel);
			this->Controls->Add(this->sidePanel);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"EditorForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Editor";
			this->sidePanel->ResumeLayout(false);
			this->sidePanel->PerformLayout();
			this->placementGroupBox->ResumeLayout(false);
			this->placementGroupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->placementOffsetNum))->EndInit();
			this->utilsGroupBox->ResumeLayout(false);
			this->utilsGroupBox->PerformLayout();
			this->selectionGroupBox->ResumeLayout(false);
			this->selectionGroupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleZ))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entScaleX))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosZ))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->entPosX))->EndInit();
			this->sceneGroupBox->ResumeLayout(false);
			this->sceneGroupBox->PerformLayout();
			this->addGroupBox->ResumeLayout(false);
			this->addGroupBox->PerformLayout();
			this->listGroupBox->ResumeLayout(false);
			this->toolsGroupBox->ResumeLayout(false);
			this->topPanel->ResumeLayout(false);
			this->renderPanel->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->levelLoading))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

private: ref class AsyncParam
{
public: UiMessageId id;
public: System::String^ subtype;
};
private: System::Void SendAsyncMsgThread(System::Object^ input)
{
	AsyncParam^ param = (AsyncParam^)input;

	switch (param->id)
	{
	case UiMessageId::GetWorldItems:
		break;
	case UiMessageId::SelectWorldItem:
	{
		SelectWorldItemData change;

		if (param->subtype == "Tree")
		{
			System::Windows::Forms::TreeNode^ node = sceneListTree->SelectedNode;
			change.groupName = marshal_as<std::string>(node->Parent->Text);
			change.item.name = marshal_as<std::wstring>(node->Text);
		}
		else
			change.item.name = marshal_as<std::wstring>(selectionListBox->SelectedItem->ToString());

		SendMsg(UiMessageId::SelectWorldItem, &change);
		break;
	}
	case UiMessageId::AddItemMode:
	{
		SendMsg(UiMessageId::AddItemMode, nullptr);
		break;
	}		
	case UiMessageId::SelectionInfoChanged:
	{
		SelectionInfoChange change;
		Ogre::Vector3 scale = { System::Decimal::ToSingle(entScaleX->Value), System::Decimal::ToSingle(entScaleY->Value) , System::Decimal::ToSingle(entScaleZ->Value) };
		Ogre::Vector3 pos = { System::Decimal::ToSingle(entPosX->Value), System::Decimal::ToSingle(entPosY->Value) , System::Decimal::ToSingle(entPosZ->Value) };

		if (param->subtype == "Scale")
		{
			change.change = SelectionInfoChange::SelectionChange::Scale;
			change.data = &scale;
		}
		if (param->subtype == "Pos")
		{
			change.change = SelectionInfoChange::SelectionChange::Pos;
			change.data = &pos;
		}

		SendMsg(UiMessageId::SelectionInfoChanged, &change);
		break;
	}		
	case UiMessageId::SceneSettingsChanged:
	{
		SceneSettingsChange change;
		std::string name;

		if (param->subtype == "Sky")
		{
			change.change = SceneSettingsChange::SceneChange::Skybox;
			name = marshal_as<std::string>(skyboxComboBox->SelectedText);
			change.data = &name;
		}	
		if (param->subtype == "Lut")
		{
			change.change = SceneSettingsChange::SceneChange::Lut;
			name = marshal_as<std::string>(lutComboBox->SelectedText);
			change.data = &name;
		}

		SendMsg(UiMessageId::SceneSettingsChanged, &change);
		break;
	}		
	case UiMessageId::PlacementRayUtil:
	{
		float offset = System::Decimal::ToSingle(placementOffsetNum->Value);
		SendMsg(UiMessageId::PlacementRayUtil, &offset);
		break;
	}
	default:
		SendMsg(param->id, nullptr);
		break;
	}
}
private: System::Void SendAsyncMsg(UiMessageId id, const char* subtype)
{
	System::Threading::Thread^ newThread = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &EditorForm::SendAsyncMsgThread));
	auto param = gcnew AsyncParam();
	param->id = id;
	param->subtype = gcnew System::String(subtype);

	newThread->Start(param);
}
private: System::Void SendAsyncMsg(UiMessageId id)
{
	SendAsyncMsg(id, "");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
private: System::Void sceneCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (!((System::Windows::Forms::CheckBox^)sender)->Checked)
	{
		sceneGroupBox->Hide();
		return;
	}


	GetSceneSettingsData data;
	SendMsg(UiMessageId::GetSceneSettings, &data);

	reportChange = false;

	skyboxComboBox->Items->Clear();
	if (!data.skyboxOptions.empty())
	{
		for (auto skybox : data.skyboxOptions)
		{
			skyboxComboBox->Items->Add(gcnew System::String(skybox.data()));
		}
		skyboxComboBox->SelectedIndex = data.currentSkyboxId;
	}

	lutComboBox->Items->Clear();
	if (!data.lutOptions.empty())
	{
		for (auto lut : data.lutOptions)
		{
			lutComboBox->Items->Add(gcnew System::String(lut.data()));
		}
		lutComboBox->SelectedIndex = data.currentLutId;
	}

	reportChange = true;

	sceneGroupBox->Show();
	//SendAsyncMsg(UiMessageId::GetSceneSettings);
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

	SendAsyncMsg(UiMessageId::AddItemMode);
}

private: System::Void selectObjButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (!selectObjButton->Checked)
		return;

	if(selectionNameButton->Text != "")
		selectionGroupBox->Show();

	SendAsyncMsg(UiMessageId::SelectMode);
}
private: System::Void moveObjButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (!moveObjButton->Checked)
		return;

	SendAsyncMsg(UiMessageId::MoveMode);
}
private: System::Void scaleObjButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (!scaleObjButton->Checked)
		return;

	SendAsyncMsg(UiMessageId::ScaleMode);
}
private: System::Void rotateObjButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (!rotateObjButton->Checked)
		return;

	SendAsyncMsg(UiMessageId::RotateMode);
}

private: System::Void addItemComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
}

private: bool reportChange = true;

public: System::Void hideItemInfo()
{
	selectionNameButton->Text = "";
	selectionGroupBox->Hide();
}
	
public: System::Void setLoading(bool enabled)
{
	levelLoading->Visible = enabled;
}

public: System::Void showItemInfo(SelectionInfo* info)
{
	selectionGroupBox->BringToFront();
	selectionGroupBox->Show();

	if (!info->names.empty())
	{
		selectionListBox->Show();
		selectionListBox->Items->Clear();

		for (auto i : info->names)
		{
			selectionListBox->Items->Add(gcnew System::String(i.data()));
		}
	}
	else
		selectionListBox->Hide();

	selectionNameButton->Text = gcnew System::String(info->name.data());

	reportChange = false;

	entPosX->Value = System::Decimal(info->pos.x);
	entPosY->Value = System::Decimal(info->pos.y);
	entPosZ->Value = System::Decimal(info->pos.z);

	entScaleX->Value = System::Decimal(info->scale.x);
	entScaleY->Value = System::Decimal(info->scale.y);
	entScaleZ->Value = System::Decimal(info->scale.z);

	reportChange = true;
}

private: System::Void skyboxComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportChange)
		return;

	SendAsyncMsg(UiMessageId::SceneSettingsChanged, "Sky");
}
private: System::Void lutComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportChange)
		return;

	SendAsyncMsg(UiMessageId::SceneSettingsChanged, "Lut");
}

private: void entPosChanged()
{
	if (!reportChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, "Pos");
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
	if (!reportChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, "Scale");
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

private: System::Void selectionListBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {	
	SendAsyncMsg(UiMessageId::SelectWorldItem, "List");
}

private: System::Void sceneListTree_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {	
	if (!e->Node->IsSelected || e->Node->Level == 0)
		return;

	SendAsyncMsg(UiMessageId::SelectWorldItem, "Tree");
}

private: System::Void selectionNameButton_Click(System::Object^  sender, System::EventArgs^  e) {
	SendAsyncMsg(UiMessageId::LookAtSelection);
}
private: System::Void utilsCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	hideUtilsGroups();

	utilsGroupBox->Visible = utilsCheckBox->Checked;
}

private: System::Void hideUtilsGroups()
{
	placementGroupBox->Visible = false;
}

private: System::Void utilsComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	auto item = utilsComboBox->SelectedItem->ToString();
	hideUtilsGroups();

	if (item == "Placement")
	{
		placementGroupBox->Visible = true;
	}
}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
	SendAsyncMsg(UiMessageId::PlacementRayUtil);
}
};
}
