#pragma once
#include <windows.h>
#include <string>
#include <vcclr.h>
#include <functional>
#include "..\..\Ballz\EditorComm.h"
#include <msclr\marshal_cppstd.h>
#include "SceneList.h"

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

		SceneList^ sceneListForm;

	private: System::Windows::Forms::Panel^  topPanel;

	public:

	private: System::Windows::Forms::Panel^  renderPanel;
	private: System::Windows::Forms::GroupBox^  placementGroupBox;
	private: System::Windows::Forms::Button^  rayButton;

	private: System::Windows::Forms::NumericUpDown^  placementOffsetNum;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::GroupBox^  utilsGroupBox;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::ComboBox^  utilsComboBox;
	private: System::Windows::Forms::PictureBox^  levelLoading;
	private: System::Windows::Forms::Button^  sceneListButton;
	private: System::Windows::Forms::ComboBox^  levelsComboBox;
	private: System::Windows::Forms::GroupBox^  grassGroupBox;

	private: System::Windows::Forms::TrackBar^  grassPaintWTrack;
	private: System::Windows::Forms::NumericUpDown^  grassDensity;



	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::RadioButton^  grassPaintAdd;
	private: System::Windows::Forms::RadioButton^  grassPaintRemove;



	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::TrackBar^  grassPaintSizeTrack;
	private: System::Windows::Forms::TextBox^  grassPaintSizeText;
	private: System::Windows::Forms::TextBox^  grassPaintWText;
	private: System::Windows::Forms::Button^  grassPaintFill;
	private: System::Windows::Forms::CheckBox^  grassPaingPreserveCheckbox;





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



	private: System::Windows::Forms::RadioButton^  rotateObjButton;
	private: System::Windows::Forms::RadioButton^  scaleObjButton;
	private: System::Windows::Forms::RadioButton^  moveObjButton;
	private: System::Windows::Forms::RadioButton^  selectObjButton;
	private: System::Windows::Forms::GroupBox^  toolsGroupBox;
	private: System::Windows::Forms::CheckBox^  utilsCheckBox;
	private: System::Windows::Forms::CheckBox^  sceneCheckBox;
	private: System::Windows::Forms::CheckBox^  addItemCheckBox;



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
			this->grassGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->grassPaintFill = (gcnew System::Windows::Forms::Button());
			this->grassPaintSizeText = (gcnew System::Windows::Forms::TextBox());
			this->grassPaintWText = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->grassPaintSizeTrack = (gcnew System::Windows::Forms::TrackBar());
			this->grassPaintAdd = (gcnew System::Windows::Forms::RadioButton());
			this->grassPaintRemove = (gcnew System::Windows::Forms::RadioButton());
			this->grassPaintWTrack = (gcnew System::Windows::Forms::TrackBar());
			this->grassDensity = (gcnew System::Windows::Forms::NumericUpDown());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->placementGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->rayButton = (gcnew System::Windows::Forms::Button());
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
			this->toolsGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->utilsCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->sceneCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->addItemCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->rotateObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->scaleObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->moveObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->selectObjButton = (gcnew System::Windows::Forms::RadioButton());
			this->topPanel = (gcnew System::Windows::Forms::Panel());
			this->levelsComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->sceneListButton = (gcnew System::Windows::Forms::Button());
			this->renderPanel = (gcnew System::Windows::Forms::Panel());
			this->levelLoading = (gcnew System::Windows::Forms::PictureBox());
			this->bottomPanel = (gcnew System::Windows::Forms::Panel());
			this->grassPaingPreserveCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->sidePanel->SuspendLayout();
			this->grassGroupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassPaintSizeTrack))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassPaintWTrack))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassDensity))->BeginInit();
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
			this->sidePanel->Controls->Add(this->grassGroupBox);
			this->sidePanel->Controls->Add(this->placementGroupBox);
			this->sidePanel->Controls->Add(this->utilsGroupBox);
			this->sidePanel->Controls->Add(this->selectionGroupBox);
			this->sidePanel->Controls->Add(this->sceneGroupBox);
			this->sidePanel->Controls->Add(this->addGroupBox);
			this->sidePanel->Controls->Add(this->toolsGroupBox);
			this->sidePanel->Dock = System::Windows::Forms::DockStyle::Left;
			this->sidePanel->Location = System::Drawing::Point(0, 0);
			this->sidePanel->Margin = System::Windows::Forms::Padding(2);
			this->sidePanel->Name = L"sidePanel";
			this->sidePanel->Size = System::Drawing::Size(300, 634);
			this->sidePanel->TabIndex = 16;
			// 
			// grassGroupBox
			// 
			this->grassGroupBox->AutoSize = true;
			this->grassGroupBox->Controls->Add(this->grassPaingPreserveCheckbox);
			this->grassGroupBox->Controls->Add(this->grassPaintFill);
			this->grassGroupBox->Controls->Add(this->grassPaintSizeText);
			this->grassGroupBox->Controls->Add(this->grassPaintWText);
			this->grassGroupBox->Controls->Add(this->label12);
			this->grassGroupBox->Controls->Add(this->label11);
			this->grassGroupBox->Controls->Add(this->grassPaintSizeTrack);
			this->grassGroupBox->Controls->Add(this->grassPaintAdd);
			this->grassGroupBox->Controls->Add(this->grassPaintRemove);
			this->grassGroupBox->Controls->Add(this->grassPaintWTrack);
			this->grassGroupBox->Controls->Add(this->grassDensity);
			this->grassGroupBox->Controls->Add(this->label10);
			this->grassGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->grassGroupBox->Location = System::Drawing::Point(0, 506);
			this->grassGroupBox->Margin = System::Windows::Forms::Padding(2);
			this->grassGroupBox->MinimumSize = System::Drawing::Size(0, 16);
			this->grassGroupBox->Name = L"grassGroupBox";
			this->grassGroupBox->Padding = System::Windows::Forms::Padding(2);
			this->grassGroupBox->Size = System::Drawing::Size(279, 198);
			this->grassGroupBox->TabIndex = 26;
			this->grassGroupBox->TabStop = false;
			this->grassGroupBox->Text = L"Grass";
			this->grassGroupBox->Visible = false;
			// 
			// grassPaintFill
			// 
			this->grassPaintFill->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"grassPaintFill.BackgroundImage")));
			this->grassPaintFill->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->grassPaintFill->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->grassPaintFill->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Khaki;
			this->grassPaintFill->FlatAppearance->MouseOverBackColor = System::Drawing::SystemColors::ControlDark;
			this->grassPaintFill->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->grassPaintFill->Location = System::Drawing::Point(177, 51);
			this->grassPaintFill->Margin = System::Windows::Forms::Padding(2);
			this->grassPaintFill->Name = L"grassPaintFill";
			this->grassPaintFill->Size = System::Drawing::Size(40, 44);
			this->grassPaintFill->TabIndex = 31;
			this->grassPaintFill->UseVisualStyleBackColor = true;
			this->grassPaintFill->Click += gcnew System::EventHandler(this, &EditorForm::grassPaintFill_Click);
			// 
			// grassPaintSizeText
			// 
			this->grassPaintSizeText->Location = System::Drawing::Point(76, 134);
			this->grassPaintSizeText->Name = L"grassPaintSizeText";
			this->grassPaintSizeText->ReadOnly = true;
			this->grassPaintSizeText->Size = System::Drawing::Size(41, 20);
			this->grassPaintSizeText->TabIndex = 30;
			this->grassPaintSizeText->Text = L"5";
			// 
			// grassPaintWText
			// 
			this->grassPaintWText->Location = System::Drawing::Point(77, 109);
			this->grassPaintWText->Name = L"grassPaintWText";
			this->grassPaintWText->ReadOnly = true;
			this->grassPaintWText->Size = System::Drawing::Size(41, 20);
			this->grassPaintWText->TabIndex = 29;
			this->grassPaintWText->Text = L"50";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(27, 137);
			this->label12->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(27, 13);
			this->label12->TabIndex = 28;
			this->label12->Text = L"Size";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(27, 112);
			this->label11->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(41, 13);
			this->label11->TabIndex = 27;
			this->label11->Text = L"Weight";
			// 
			// grassPaintSizeTrack
			// 
			this->grassPaintSizeTrack->LargeChange = 10;
			this->grassPaintSizeTrack->Location = System::Drawing::Point(137, 135);
			this->grassPaintSizeTrack->Maximum = 50;
			this->grassPaintSizeTrack->Minimum = 1;
			this->grassPaintSizeTrack->Name = L"grassPaintSizeTrack";
			this->grassPaintSizeTrack->Size = System::Drawing::Size(104, 45);
			this->grassPaintSizeTrack->TabIndex = 26;
			this->grassPaintSizeTrack->TickStyle = System::Windows::Forms::TickStyle::None;
			this->grassPaintSizeTrack->Value = 5;
			this->grassPaintSizeTrack->Scroll += gcnew System::EventHandler(this, &EditorForm::grassPaintSizeTrack_Scroll);
			// 
			// grassPaintAdd
			// 
			this->grassPaintAdd->Appearance = System::Windows::Forms::Appearance::Button;
			this->grassPaintAdd->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"grassPaintAdd.BackgroundImage")));
			this->grassPaintAdd->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->grassPaintAdd->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->grassPaintAdd->FlatAppearance->CheckedBackColor = System::Drawing::Color::Khaki;
			this->grassPaintAdd->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->grassPaintAdd->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->grassPaintAdd->Location = System::Drawing::Point(76, 51);
			this->grassPaintAdd->Margin = System::Windows::Forms::Padding(2);
			this->grassPaintAdd->Name = L"grassPaintAdd";
			this->grassPaintAdd->Size = System::Drawing::Size(40, 44);
			this->grassPaintAdd->TabIndex = 23;
			this->grassPaintAdd->TabStop = true;
			this->grassPaintAdd->UseVisualStyleBackColor = true;
			this->grassPaintAdd->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::grassPaintAdd_CheckedChanged);
			// 
			// grassPaintRemove
			// 
			this->grassPaintRemove->Appearance = System::Windows::Forms::Appearance::Button;
			this->grassPaintRemove->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"grassPaintRemove.BackgroundImage")));
			this->grassPaintRemove->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->grassPaintRemove->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->grassPaintRemove->FlatAppearance->CheckedBackColor = System::Drawing::Color::Khaki;
			this->grassPaintRemove->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->grassPaintRemove->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->grassPaintRemove->Location = System::Drawing::Point(127, 51);
			this->grassPaintRemove->Margin = System::Windows::Forms::Padding(2);
			this->grassPaintRemove->Name = L"grassPaintRemove";
			this->grassPaintRemove->Size = System::Drawing::Size(40, 44);
			this->grassPaintRemove->TabIndex = 24;
			this->grassPaintRemove->TabStop = true;
			this->grassPaintRemove->UseVisualStyleBackColor = true;
			this->grassPaintRemove->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::grassPaintRemove_CheckedChanged);
			// 
			// grassPaintWTrack
			// 
			this->grassPaintWTrack->Location = System::Drawing::Point(137, 110);
			this->grassPaintWTrack->Maximum = 100;
			this->grassPaintWTrack->Name = L"grassPaintWTrack";
			this->grassPaintWTrack->Size = System::Drawing::Size(104, 45);
			this->grassPaintWTrack->TabIndex = 6;
			this->grassPaintWTrack->TickStyle = System::Windows::Forms::TickStyle::None;
			this->grassPaintWTrack->Value = 50;
			this->grassPaintWTrack->Scroll += gcnew System::EventHandler(this, &EditorForm::grassPaintWTrack_Scroll);
			// 
			// grassDensity
			// 
			this->grassDensity->DecimalPlaces = 2;
			this->grassDensity->Location = System::Drawing::Point(77, 15);
			this->grassDensity->Margin = System::Windows::Forms::Padding(2);
			this->grassDensity->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 20, 0, 0, 0 });
			this->grassDensity->Name = L"grassDensity";
			this->grassDensity->Size = System::Drawing::Size(68, 20);
			this->grassDensity->TabIndex = 3;
			this->grassDensity->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->grassDensity->ValueChanged += gcnew System::EventHandler(this, &EditorForm::grassDensity_ValueChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(31, 17);
			this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(42, 13);
			this->label10->TabIndex = 1;
			this->label10->Text = L"Density";
			// 
			// placementGroupBox
			// 
			this->placementGroupBox->AutoSize = true;
			this->placementGroupBox->Controls->Add(this->rayButton);
			this->placementGroupBox->Controls->Add(this->placementOffsetNum);
			this->placementGroupBox->Controls->Add(this->label8);
			this->placementGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->placementGroupBox->Location = System::Drawing::Point(0, 454);
			this->placementGroupBox->Margin = System::Windows::Forms::Padding(2);
			this->placementGroupBox->MinimumSize = System::Drawing::Size(0, 16);
			this->placementGroupBox->Name = L"placementGroupBox";
			this->placementGroupBox->Padding = System::Windows::Forms::Padding(2);
			this->placementGroupBox->Size = System::Drawing::Size(279, 52);
			this->placementGroupBox->TabIndex = 25;
			this->placementGroupBox->TabStop = false;
			this->placementGroupBox->Text = L"Placement";
			this->placementGroupBox->Visible = false;
			// 
			// rayButton
			// 
			this->rayButton->Location = System::Drawing::Point(176, 15);
			this->rayButton->Margin = System::Windows::Forms::Padding(2);
			this->rayButton->Name = L"rayButton";
			this->rayButton->Size = System::Drawing::Size(56, 19);
			this->rayButton->TabIndex = 4;
			this->rayButton->Text = L"Ray";
			this->rayButton->UseVisualStyleBackColor = true;
			this->rayButton->Click += gcnew System::EventHandler(this, &EditorForm::rayButton_Click);
			// 
			// placementOffsetNum
			// 
			this->placementOffsetNum->DecimalPlaces = 2;
			this->placementOffsetNum->Location = System::Drawing::Point(77, 15);
			this->placementOffsetNum->Margin = System::Windows::Forms::Padding(2);
			this->placementOffsetNum->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->placementOffsetNum->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->placementOffsetNum->Name = L"placementOffsetNum";
			this->placementOffsetNum->Size = System::Drawing::Size(68, 20);
			this->placementOffsetNum->TabIndex = 3;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(31, 17);
			this->label8->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(35, 13);
			this->label8->TabIndex = 1;
			this->label8->Text = L"Offset";
			// 
			// utilsGroupBox
			// 
			this->utilsGroupBox->AutoSize = true;
			this->utilsGroupBox->Controls->Add(this->label9);
			this->utilsGroupBox->Controls->Add(this->utilsComboBox);
			this->utilsGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->utilsGroupBox->Location = System::Drawing::Point(0, 401);
			this->utilsGroupBox->Margin = System::Windows::Forms::Padding(2);
			this->utilsGroupBox->MinimumSize = System::Drawing::Size(0, 16);
			this->utilsGroupBox->Name = L"utilsGroupBox";
			this->utilsGroupBox->Padding = System::Windows::Forms::Padding(2);
			this->utilsGroupBox->Size = System::Drawing::Size(279, 53);
			this->utilsGroupBox->TabIndex = 22;
			this->utilsGroupBox->TabStop = false;
			this->utilsGroupBox->Text = L"Utilities";
			this->utilsGroupBox->Visible = false;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(34, 20);
			this->label9->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(31, 13);
			this->label9->TabIndex = 1;
			this->label9->Text = L"Type";
			// 
			// utilsComboBox
			// 
			this->utilsComboBox->FormattingEnabled = true;
			this->utilsComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Placement" });
			this->utilsComboBox->Location = System::Drawing::Point(76, 15);
			this->utilsComboBox->Margin = System::Windows::Forms::Padding(2);
			this->utilsComboBox->Name = L"utilsComboBox";
			this->utilsComboBox->Size = System::Drawing::Size(174, 21);
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
			this->selectionGroupBox->Location = System::Drawing::Point(0, 243);
			this->selectionGroupBox->Margin = System::Windows::Forms::Padding(2);
			this->selectionGroupBox->MinimumSize = System::Drawing::Size(0, 16);
			this->selectionGroupBox->Name = L"selectionGroupBox";
			this->selectionGroupBox->Padding = System::Windows::Forms::Padding(2);
			this->selectionGroupBox->Size = System::Drawing::Size(279, 158);
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
			this->selectionNameButton->Location = System::Drawing::Point(76, 16);
			this->selectionNameButton->Margin = System::Windows::Forms::Padding(2);
			this->selectionNameButton->Name = L"selectionNameButton";
			this->selectionNameButton->Size = System::Drawing::Size(56, 19);
			this->selectionNameButton->TabIndex = 12;
			this->selectionNameButton->UseVisualStyleBackColor = true;
			this->selectionNameButton->Click += gcnew System::EventHandler(this, &EditorForm::selectionNameButton_Click);
			// 
			// selectionListBox
			// 
			this->selectionListBox->FormattingEnabled = true;
			this->selectionListBox->Location = System::Drawing::Point(76, 85);
			this->selectionListBox->Margin = System::Windows::Forms::Padding(2);
			this->selectionListBox->Name = L"selectionListBox";
			this->selectionListBox->Size = System::Drawing::Size(175, 56);
			this->selectionListBox->TabIndex = 11;
			this->selectionListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::selectionListBox_SelectedIndexChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(33, 66);
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
			this->entScaleZ->Location = System::Drawing::Point(221, 63);
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
			this->entScaleY->Location = System::Drawing::Point(149, 63);
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
			this->entScaleX->Location = System::Drawing::Point(77, 63);
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
			this->label2->Location = System::Drawing::Point(22, 42);
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
			this->entPosZ->Location = System::Drawing::Point(221, 41);
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
			this->entPosY->Location = System::Drawing::Point(149, 41);
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
			this->entPosX->Location = System::Drawing::Point(77, 41);
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
			this->label1->Location = System::Drawing::Point(33, 19);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
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
			this->sceneGroupBox->Location = System::Drawing::Point(0, 163);
			this->sceneGroupBox->Margin = System::Windows::Forms::Padding(2);
			this->sceneGroupBox->MinimumSize = System::Drawing::Size(0, 16);
			this->sceneGroupBox->Name = L"sceneGroupBox";
			this->sceneGroupBox->Padding = System::Windows::Forms::Padding(2);
			this->sceneGroupBox->Size = System::Drawing::Size(279, 80);
			this->sceneGroupBox->TabIndex = 21;
			this->sceneGroupBox->TabStop = false;
			this->sceneGroupBox->Text = L"Scene";
			this->sceneGroupBox->Visible = false;
			// 
			// lutComboBox
			// 
			this->lutComboBox->FormattingEnabled = true;
			this->lutComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Tree" });
			this->lutComboBox->Location = System::Drawing::Point(76, 42);
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
			this->skyboxComboBox->Location = System::Drawing::Point(76, 15);
			this->skyboxComboBox->Margin = System::Windows::Forms::Padding(2);
			this->skyboxComboBox->Name = L"skyboxComboBox";
			this->skyboxComboBox->Size = System::Drawing::Size(174, 21);
			this->skyboxComboBox->TabIndex = 2;
			this->skyboxComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::skyboxComboBox_SelectedIndexChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(39, 50);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(28, 13);
			this->label6->TabIndex = 2;
			this->label6->Text = L"LUT";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(26, 18);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(42, 13);
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
			this->addGroupBox->Location = System::Drawing::Point(0, 79);
			this->addGroupBox->Margin = System::Windows::Forms::Padding(2);
			this->addGroupBox->MinimumSize = System::Drawing::Size(0, 16);
			this->addGroupBox->Name = L"addGroupBox";
			this->addGroupBox->Padding = System::Windows::Forms::Padding(2);
			this->addGroupBox->Size = System::Drawing::Size(279, 84);
			this->addGroupBox->TabIndex = 20;
			this->addGroupBox->TabStop = false;
			this->addGroupBox->Text = L"Add";
			this->addGroupBox->Visible = false;
			// 
			// addItemPrefixTextBox
			// 
			this->addItemPrefixTextBox->Location = System::Drawing::Point(76, 46);
			this->addItemPrefixTextBox->Name = L"addItemPrefixTextBox";
			this->addItemPrefixTextBox->Size = System::Drawing::Size(174, 20);
			this->addItemPrefixTextBox->TabIndex = 3;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(34, 49);
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
			this->label4->Location = System::Drawing::Point(3, 18);
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
			this->addItemTypeComboBox->Location = System::Drawing::Point(76, 15);
			this->addItemTypeComboBox->Margin = System::Windows::Forms::Padding(2);
			this->addItemTypeComboBox->Name = L"addItemTypeComboBox";
			this->addItemTypeComboBox->Size = System::Drawing::Size(174, 21);
			this->addItemTypeComboBox->TabIndex = 0;
			// 
			// toolsGroupBox
			// 
			this->toolsGroupBox->AutoSize = true;
			this->toolsGroupBox->BackColor = System::Drawing::SystemColors::ControlDark;
			this->toolsGroupBox->Controls->Add(this->utilsCheckBox);
			this->toolsGroupBox->Controls->Add(this->sceneCheckBox);
			this->toolsGroupBox->Controls->Add(this->addItemCheckBox);
			this->toolsGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->toolsGroupBox->Location = System::Drawing::Point(0, 0);
			this->toolsGroupBox->Margin = System::Windows::Forms::Padding(2);
			this->toolsGroupBox->Name = L"toolsGroupBox";
			this->toolsGroupBox->Padding = System::Windows::Forms::Padding(2);
			this->toolsGroupBox->Size = System::Drawing::Size(279, 79);
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
			this->utilsCheckBox->Location = System::Drawing::Point(176, 17);
			this->utilsCheckBox->Name = L"utilsCheckBox";
			this->utilsCheckBox->Size = System::Drawing::Size(40, 44);
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
			this->sceneCheckBox->Location = System::Drawing::Point(70, 17);
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
			this->addItemCheckBox->Location = System::Drawing::Point(123, 17);
			this->addItemCheckBox->Name = L"addItemCheckBox";
			this->addItemCheckBox->Size = System::Drawing::Size(40, 44);
			this->addItemCheckBox->TabIndex = 30;
			this->addItemCheckBox->UseVisualStyleBackColor = true;
			this->addItemCheckBox->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::addItemCheckBox_CheckedChanged);
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
			this->rotateObjButton->Location = System::Drawing::Point(269, 19);
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
			this->scaleObjButton->Location = System::Drawing::Point(216, 19);
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
			this->moveObjButton->Location = System::Drawing::Point(163, 19);
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
			this->selectObjButton->Location = System::Drawing::Point(112, 19);
			this->selectObjButton->Margin = System::Windows::Forms::Padding(2);
			this->selectObjButton->Name = L"selectObjButton";
			this->selectObjButton->Size = System::Drawing::Size(40, 44);
			this->selectObjButton->TabIndex = 20;
			this->selectObjButton->TabStop = true;
			this->selectObjButton->UseVisualStyleBackColor = true;
			this->selectObjButton->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::selectObjButton_CheckedChanged);
			// 
			// topPanel
			// 
			this->topPanel->Controls->Add(this->levelsComboBox);
			this->topPanel->Controls->Add(this->sceneListButton);
			this->topPanel->Controls->Add(this->rotateObjButton);
			this->topPanel->Controls->Add(this->scaleObjButton);
			this->topPanel->Controls->Add(this->selectObjButton);
			this->topPanel->Controls->Add(this->moveObjButton);
			this->topPanel->Dock = System::Windows::Forms::DockStyle::Top;
			this->topPanel->Location = System::Drawing::Point(300, 0);
			this->topPanel->Margin = System::Windows::Forms::Padding(2);
			this->topPanel->Name = L"topPanel";
			this->topPanel->Size = System::Drawing::Size(723, 71);
			this->topPanel->TabIndex = 18;
			// 
			// levelsComboBox
			// 
			this->levelsComboBox->FormattingEnabled = true;
			this->levelsComboBox->Location = System::Drawing::Point(582, 19);
			this->levelsComboBox->Margin = System::Windows::Forms::Padding(2);
			this->levelsComboBox->Name = L"levelsComboBox";
			this->levelsComboBox->Size = System::Drawing::Size(134, 21);
			this->levelsComboBox->TabIndex = 25;
			this->levelsComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::levelsComboBox_SelectedIndexChanged);
			// 
			// sceneListButton
			// 
			this->sceneListButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"sceneListButton.BackgroundImage")));
			this->sceneListButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->sceneListButton->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->sceneListButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Khaki;
			this->sceneListButton->FlatAppearance->MouseOverBackColor = System::Drawing::SystemColors::ControlDark;
			this->sceneListButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->sceneListButton->Location = System::Drawing::Point(16, 19);
			this->sceneListButton->Margin = System::Windows::Forms::Padding(2);
			this->sceneListButton->Name = L"sceneListButton";
			this->sceneListButton->Size = System::Drawing::Size(40, 44);
			this->sceneListButton->TabIndex = 24;
			this->sceneListButton->UseVisualStyleBackColor = true;
			this->sceneListButton->Click += gcnew System::EventHandler(this, &EditorForm::sceneListButton_Click);
			// 
			// renderPanel
			// 
			this->renderPanel->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->renderPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->renderPanel->Controls->Add(this->levelLoading);
			this->renderPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->renderPanel->Location = System::Drawing::Point(300, 71);
			this->renderPanel->Margin = System::Windows::Forms::Padding(2);
			this->renderPanel->Name = L"renderPanel";
			this->renderPanel->Size = System::Drawing::Size(723, 563);
			this->renderPanel->TabIndex = 19;
			// 
			// levelLoading
			// 
			this->levelLoading->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->levelLoading->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->levelLoading->Dock = System::Windows::Forms::DockStyle::Fill;
			this->levelLoading->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"levelLoading.Image")));
			this->levelLoading->InitialImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"levelLoading.InitialImage")));
			this->levelLoading->Location = System::Drawing::Point(0, 0);
			this->levelLoading->Margin = System::Windows::Forms::Padding(2);
			this->levelLoading->Name = L"levelLoading";
			this->levelLoading->Size = System::Drawing::Size(723, 563);
			this->levelLoading->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->levelLoading->TabIndex = 0;
			this->levelLoading->TabStop = false;
			this->levelLoading->UseWaitCursor = true;
			// 
			// bottomPanel
			// 
			this->bottomPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->bottomPanel->Location = System::Drawing::Point(300, 568);
			this->bottomPanel->Margin = System::Windows::Forms::Padding(2);
			this->bottomPanel->Name = L"bottomPanel";
			this->bottomPanel->Size = System::Drawing::Size(723, 66);
			this->bottomPanel->TabIndex = 20;
			// 
			// grassPaingPreserveCheckbox
			// 
			this->grassPaingPreserveCheckbox->AutoSize = true;
			this->grassPaingPreserveCheckbox->Location = System::Drawing::Point(164, 16);
			this->grassPaingPreserveCheckbox->Name = L"grassPaingPreserveCheckbox";
			this->grassPaingPreserveCheckbox->Size = System::Drawing::Size(96, 17);
			this->grassPaingPreserveCheckbox->TabIndex = 32;
			this->grassPaingPreserveCheckbox->Text = L"Preserve mask";
			this->grassPaingPreserveCheckbox->UseVisualStyleBackColor = true;
			this->grassPaingPreserveCheckbox->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::grassPaingPreserveCheckbox_CheckedChanged);
			// 
			// EditorForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->AutoSize = true;
			this->BackColor = System::Drawing::SystemColors::ControlDark;
			this->ClientSize = System::Drawing::Size(1023, 634);
			this->Controls->Add(this->bottomPanel);
			this->Controls->Add(this->renderPanel);
			this->Controls->Add(this->topPanel);
			this->Controls->Add(this->sidePanel);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"EditorForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Editor";
			this->Closing += gcnew System::ComponentModel::CancelEventHandler(this, &EditorForm::EditorForm_FormClosing);
			this->sidePanel->ResumeLayout(false);
			this->sidePanel->PerformLayout();
			this->grassGroupBox->ResumeLayout(false);
			this->grassGroupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassPaintSizeTrack))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassPaintWTrack))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassDensity))->EndInit();
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
public: int subId;
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
			System::Windows::Forms::TreeNode^ node = sceneListForm->getTree()->SelectedNode;
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
		Ogre::Vector3 vparam;
		float fparam;

		change.change = (SelectionInfoChange::Id)param->subId;

		if (change.change == SelectionInfoChange::Id::Scale)
		{
			vparam = { System::Decimal::ToSingle(entScaleX->Value), System::Decimal::ToSingle(entScaleY->Value) , System::Decimal::ToSingle(entScaleZ->Value) };
			change.data = &vparam;
		}
		if (change.change == SelectionInfoChange::Id::Pos)
		{
			vparam = { System::Decimal::ToSingle(entPosX->Value), System::Decimal::ToSingle(entPosY->Value) , System::Decimal::ToSingle(entPosZ->Value) };
			change.data = &vparam;
		}
		if (change.change == SelectionInfoChange::Id::GrassDensity)
		{
			fparam = System::Decimal::ToSingle(grassDensity->Value);
			change.data = &fparam;
		}
		if (change.change == SelectionInfoChange::Id::GrassPaintFill)
		{
			fparam = System::Decimal::ToSingle(grassPaintWTrack->Value)*0.01f;
			change.data = &fparam;
		}
		if (change.change == SelectionInfoChange::Id::GrassPaintAdd || change.change == SelectionInfoChange::Id::GrassPaintRemove)
		{
			vparam = { System::Decimal::ToSingle(grassPaintWTrack->Value)*0.01f, System::Decimal::ToSingle(grassPaintSizeTrack->Value) , 0};
			change.data = &vparam;
		}
		if (change.change == SelectionInfoChange::Id::GrassPaintSizeChange)
		{
			fparam = System::Decimal::ToSingle(grassPaintSizeTrack->Value);
			change.data = &fparam;
		}
		if (change.change == SelectionInfoChange::Id::GrassPaintWChange)
		{
			fparam = System::Decimal::ToSingle(grassPaintWTrack->Value)*0.01f;
			change.data = &fparam;
		}
		if (change.change == SelectionInfoChange::Id::GrassPaintPreserve)
		{
			fparam = grassPaingPreserveCheckbox->Checked ? 1.0f : 0.0f;
			change.data = &fparam;
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
	case UiMessageId::LoadLevel:
	{
		auto lvlName = marshal_as<std::string>(levelsComboBox->SelectedItem->ToString());
		SendMsg(UiMessageId::LoadLevel, &lvlName);
	}
	default:
		SendMsg(param->id, nullptr);
		break;
	}
}
private: System::Void SendAsyncMsg(UiMessageId id, int subid)
{
	System::Threading::Thread^ newThread = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &EditorForm::SendAsyncMsgThread));
	auto param = gcnew AsyncParam();
	param->id = id;
	param->subtype = gcnew System::String("");
	param->subId = subid;

	newThread->Start(param);
}
private: System::Void SendAsyncMsg(UiMessageId id, const char* subtype)
{
	System::Threading::Thread^ newThread = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &EditorForm::SendAsyncMsgThread));
	auto param = gcnew AsyncParam();
	param->id = id;
	param->subtype = gcnew System::String(subtype);
	param->subId = 0;

	newThread->Start(param);
}
private: System::Void SendAsyncMsg(UiMessageId id)
{
	SendAsyncMsg(id, "");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private: System::Void sceneCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (!((System::Windows::Forms::CheckBox^)sender)->Checked)
	{
		sceneGroupBox->Hide();
		return;
	}


	GetSceneSettingsData data;
	SendMsg(UiMessageId::GetSceneSettings, &data);

	reportSelectionChange = false;

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

	reportSelectionChange = true;

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

private: bool reportSelectionChange = true;

public: System::Void hideItemInfo()
{
	selectionNameButton->Text = "";
	selectionGroupBox->Hide();
	grassGroupBox->Hide();
}
	
public: System::Void setLoading(bool enabled)
{
	levelLoading->Visible = enabled;
}

public: System::Void setProperties(EditorProperties* info)
{
	levelsComboBox->Items->Clear();

	for (auto& l : info->levels)
	{
		levelsComboBox->Items->Add(gcnew System::String(l.data()));
	}
}

public: System::Void showItemInfo(SelectionInfo* info)
{
	//selectionGroupBox->BringToFront();

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

	reportSelectionChange = false;
	bool showGrassInfo = false;
	if (info->subtype == SelectionInfo::Grass && info->names.empty())
	{
		showGrassInfo = true;
		grassPaintAdd->Checked = false;
		grassPaintRemove->Checked = false;

		auto subinfo = (GrassSelectionInfo*)info->subtypeData;

		grassDensity->Value = System::Decimal(subinfo->density);
		grassPaingPreserveCheckbox->Checked = subinfo->preserveMask;

		grassGroupBox->BringToFront();
	}

	entPosX->Value = System::Decimal(info->pos.x);
	entPosY->Value = System::Decimal(info->pos.y);
	entPosZ->Value = System::Decimal(info->pos.z);

	entScaleX->Value = System::Decimal(info->scale.x);
	entScaleY->Value = System::Decimal(info->scale.y);
	entScaleZ->Value = System::Decimal(info->scale.z);

	reportSelectionChange = true;

	grassGroupBox->Visible = showGrassInfo;
	selectionGroupBox->Show();
}

private: System::Void skyboxComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SceneSettingsChanged, "Sky");
}
private: System::Void lutComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SceneSettingsChanged, "Lut");
}

private: void entPosChanged()
{
	if (!reportSelectionChange)
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
	if (!reportSelectionChange)
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

private: System::Void selectionNameButton_Click(System::Object^  sender, System::EventArgs^  e) {
	SendAsyncMsg(UiMessageId::LookAtSelection);
}
private: System::Void utilsCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	hideUtilsGroups();

	if (utilsCheckBox->Checked)
		utilsComboBox->SelectedIndex = -1;

	utilsGroupBox->Visible = utilsCheckBox->Checked;
}

private: System::Void hideUtilsGroups()
{
	placementGroupBox->Visible = false;
}

private: System::Void utilsComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	
	hideUtilsGroups();

	if (utilsComboBox->SelectedIndex == -1)
		return;

	auto item = utilsComboBox->SelectedItem->ToString();

	if (item == "Placement")
	{
		placementGroupBox->BringToFront();
		placementGroupBox->Visible = true;
	}
}
private: System::Void rayButton_Click(System::Object^  sender, System::EventArgs^  e) {
	SendAsyncMsg(UiMessageId::PlacementRayUtil);
}

private: System::Void sceneListButton_Click(System::Object^  sender, System::EventArgs^  e) {

	if (!sceneListForm || sceneListForm->Visible == false)
		sceneListForm = gcnew SceneList();

	auto sceneListTree = sceneListForm->getTree();

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

	sceneListForm->Visible = true;
}
private: System::Void levelsComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	if (levelsComboBox->SelectedIndex == -1)
		return;

	SendAsyncMsg(UiMessageId::LoadLevel);
}
private: System::Void EditorForm_FormClosing(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
	SendMsg(UiMessageId::CloseEditor, nullptr);
}

private: System::Void grassPaintWTrack_Scroll(System::Object^  sender, System::EventArgs^  e) {
	grassPaintWText->Text = grassPaintWTrack->Value.ToString();

	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::GrassPaintWChange);
}
private: System::Void grassPaintSizeTrack_Scroll(System::Object^  sender, System::EventArgs^  e) {
	grassPaintSizeText->Text = grassPaintSizeTrack->Value.ToString();

	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::GrassPaintSizeChange);
}
private: System::Void grassPaintFill_Click(System::Object^  sender, System::EventArgs^  e) {
	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::GrassPaintFill);
}
private: System::Void grassDensity_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::GrassDensity);
}
private: System::Void grassPaintAdd_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::GrassPaintAdd);
}
private: System::Void grassPaintRemove_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::GrassPaintRemove);
}
private: System::Void grassPaingPreserveCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::GrassPaintPreserve);
}
};
}
