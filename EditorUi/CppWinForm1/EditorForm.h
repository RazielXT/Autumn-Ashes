#pragma once
#include <windows.h>
#include <string>
#include <vcclr.h>
#include <functional>
#include "..\..\Ballz\EditorComm.h"
#include <msclr\marshal_cppstd.h>
#include "SceneList.h"
#include <vector>
#include <cliext/vector>   

using namespace msclr::interop;

extern void SendMsg(UiMessageId id, void* data);

template<typename T>
void CpySendMsg(UiMessageId id, T data)
{
	SendMsg(id, &data);
}

struct SelectionHistoryItem
{
	std::string name;
	std::string group;
};

std::vector<SelectionHistoryItem> selectionHistoryItems;

struct ItemEditableParamInfo
{
	EditableParam param;
	int index;
};
std::map<std::string, ItemEditableParamInfo> currentEditableParams;

ItemEditableParamInfo& getEditableParam(System::String^ name)
{
	auto nameStr = marshal_as<std::string>(name);

	if (currentEditableParams.find(nameStr) == currentEditableParams.end())
		return ItemEditableParamInfo();
	else
		return currentEditableParams[nameStr];
}

System::String^ getItemTypeStr(ItemType type)
{
	if (type == ItemType::Entity)
		return "Entity";
	else if (type == ItemType::Grass)
		return "Grass";
	else if (type == ItemType::Particle)
		return "Particle";
	else
		return "Unknown";
}

ItemType getItemTypeFromStr(System::String^ type)
{
	if (type == "Entity")
		return ItemType::Entity;
	else if (type == "Grass")
		return ItemType::Grass;
	else if (type == "Particle")
		return ItemType::Particle;
	else
		return ItemType::Entity;
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
			instance = this;

			addGroupBox->Show();
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

	private: System::Windows::Forms::Button^  sceneListButton;
	private: System::Windows::Forms::ComboBox^  levelsComboBox;
	private: System::Windows::Forms::GroupBox^  paintGroupBox;
	private: System::Windows::Forms::TrackBar^  paintWTrack;
	private: System::Windows::Forms::RadioButton^  paintAdd;
	private: System::Windows::Forms::RadioButton^  paintRemove;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::TrackBar^  paintSizeTrack;
	private: System::Windows::Forms::TextBox^  paintSizeText;
	private: System::Windows::Forms::TextBox^  paintWText;
	private: System::Windows::Forms::Button^  paintFill;



	private: System::Windows::Forms::PictureBox^  levelLoading;
	private: System::Windows::Forms::Panel^  bottomPanel;
	private: System::Windows::Forms::GroupBox^  entityGroupBox;

	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::ComboBox^  animationsComboBox;
	private: System::Windows::Forms::GroupBox^  grassGroupBox;
	private: System::Windows::Forms::CheckBox^  grassPaingPreserveCheckbox;
	private: System::Windows::Forms::NumericUpDown^  grassDensity;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Button^  stopAnimationButton;

	private: System::Windows::Forms::Button^  playAnimationButton;
	private: System::Windows::Forms::NumericUpDown^  grassSizeHMax;

	private: System::Windows::Forms::NumericUpDown^  grassSizeHMin;

	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::NumericUpDown^  grassSizeWMax;

	private: System::Windows::Forms::NumericUpDown^  grassSizeWMin;

	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Button^  reloadShadersbutton;
	private: System::Windows::Forms::Button^  reloadGeometryButton;
	private: System::Windows::Forms::GroupBox^  itemParamsGroupBox;
	private: System::Windows::Forms::Button^  getParamsButton;
	private: System::Windows::Forms::GroupBox^  addEntityGroupBox;

	private: System::Windows::Forms::TextBox^  addEntityPathTextBox;
	private: System::Windows::Forms::Button^  addEntitySearchButton;


	private: System::Windows::Forms::ComboBox^  addItemTypeComboBox;
	private: System::Windows::Forms::Label^  label4;











	private: System::Windows::Forms::ListBox^  lastSelectedList;





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
			this->addEntityGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->addEntityPathTextBox = (gcnew System::Windows::Forms::TextBox());
			this->addEntitySearchButton = (gcnew System::Windows::Forms::Button());
			this->itemParamsGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->getParamsButton = (gcnew System::Windows::Forms::Button());
			this->entityGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->stopAnimationButton = (gcnew System::Windows::Forms::Button());
			this->playAnimationButton = (gcnew System::Windows::Forms::Button());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->animationsComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->grassGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->grassSizeWMax = (gcnew System::Windows::Forms::NumericUpDown());
			this->grassSizeWMin = (gcnew System::Windows::Forms::NumericUpDown());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->grassSizeHMax = (gcnew System::Windows::Forms::NumericUpDown());
			this->grassSizeHMin = (gcnew System::Windows::Forms::NumericUpDown());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->grassPaingPreserveCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->grassDensity = (gcnew System::Windows::Forms::NumericUpDown());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->paintGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->paintFill = (gcnew System::Windows::Forms::Button());
			this->paintSizeText = (gcnew System::Windows::Forms::TextBox());
			this->paintWText = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->paintSizeTrack = (gcnew System::Windows::Forms::TrackBar());
			this->paintAdd = (gcnew System::Windows::Forms::RadioButton());
			this->paintRemove = (gcnew System::Windows::Forms::RadioButton());
			this->paintWTrack = (gcnew System::Windows::Forms::TrackBar());
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
			this->addItemTypeComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
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
			this->reloadShadersbutton = (gcnew System::Windows::Forms::Button());
			this->reloadGeometryButton = (gcnew System::Windows::Forms::Button());
			this->lastSelectedList = (gcnew System::Windows::Forms::ListBox());
			this->sidePanel->SuspendLayout();
			this->addEntityGroupBox->SuspendLayout();
			this->itemParamsGroupBox->SuspendLayout();
			this->entityGroupBox->SuspendLayout();
			this->grassGroupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassSizeWMax))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassSizeWMin))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassSizeHMax))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassSizeHMin))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassDensity))->BeginInit();
			this->paintGroupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->paintSizeTrack))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->paintWTrack))->BeginInit();
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
			this->bottomPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// sidePanel
			// 
			this->sidePanel->AutoScroll = true;
			this->sidePanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->sidePanel->Controls->Add(this->addEntityGroupBox);
			this->sidePanel->Controls->Add(this->itemParamsGroupBox);
			this->sidePanel->Controls->Add(this->entityGroupBox);
			this->sidePanel->Controls->Add(this->grassGroupBox);
			this->sidePanel->Controls->Add(this->paintGroupBox);
			this->sidePanel->Controls->Add(this->placementGroupBox);
			this->sidePanel->Controls->Add(this->utilsGroupBox);
			this->sidePanel->Controls->Add(this->selectionGroupBox);
			this->sidePanel->Controls->Add(this->sceneGroupBox);
			this->sidePanel->Controls->Add(this->addGroupBox);
			this->sidePanel->Controls->Add(this->toolsGroupBox);
			this->sidePanel->Dock = System::Windows::Forms::DockStyle::Left;
			this->sidePanel->Location = System::Drawing::Point(0, 0);
			this->sidePanel->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->sidePanel->Name = L"sidePanel";
			this->sidePanel->Size = System::Drawing::Size(399, 780);
			this->sidePanel->TabIndex = 16;
			// 
			// addEntityGroupBox
			// 
			this->addEntityGroupBox->AutoSize = true;
			this->addEntityGroupBox->Controls->Add(this->addEntityPathTextBox);
			this->addEntityGroupBox->Controls->Add(this->addEntitySearchButton);
			this->addEntityGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->addEntityGroupBox->Location = System::Drawing::Point(0, 1045);
			this->addEntityGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->addEntityGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->addEntityGroupBox->Name = L"addEntityGroupBox";
			this->addEntityGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->addEntityGroupBox->Size = System::Drawing::Size(374, 92);
			this->addEntityGroupBox->TabIndex = 30;
			this->addEntityGroupBox->TabStop = false;
			this->addEntityGroupBox->Text = L"Add entity";
			this->addEntityGroupBox->Visible = false;
			// 
			// addEntityPathTextBox
			// 
			this->addEntityPathTextBox->Location = System::Drawing::Point(28, 21);
			this->addEntityPathTextBox->Name = L"addEntityPathTextBox";
			this->addEntityPathTextBox->Size = System::Drawing::Size(329, 22);
			this->addEntityPathTextBox->TabIndex = 7;
			this->addEntityPathTextBox->TextChanged += gcnew System::EventHandler(this, &EditorForm::addEntityPath_TextChanged);
			// 
			// addEntitySearchButton
			// 
			this->addEntitySearchButton->Location = System::Drawing::Point(28, 49);
			this->addEntitySearchButton->Name = L"addEntitySearchButton";
			this->addEntitySearchButton->Size = System::Drawing::Size(75, 23);
			this->addEntitySearchButton->TabIndex = 4;
			this->addEntitySearchButton->Text = L"Search";
			this->addEntitySearchButton->UseVisualStyleBackColor = true;
			this->addEntitySearchButton->Click += gcnew System::EventHandler(this, &EditorForm::addEntityPathSearch_Click);
			// 
			// itemParamsGroupBox
			// 
			this->itemParamsGroupBox->AutoSize = true;
			this->itemParamsGroupBox->Controls->Add(this->getParamsButton);
			this->itemParamsGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->itemParamsGroupBox->Location = System::Drawing::Point(0, 984);
			this->itemParamsGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->itemParamsGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->itemParamsGroupBox->Name = L"itemParamsGroupBox";
			this->itemParamsGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->itemParamsGroupBox->Size = System::Drawing::Size(374, 61);
			this->itemParamsGroupBox->TabIndex = 29;
			this->itemParamsGroupBox->TabStop = false;
			this->itemParamsGroupBox->Text = L"Params";
			// 
			// getParamsButton
			// 
			this->getParamsButton->Location = System::Drawing::Point(142, 19);
			this->getParamsButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->getParamsButton->Name = L"getParamsButton";
			this->getParamsButton->Size = System::Drawing::Size(75, 23);
			this->getParamsButton->TabIndex = 14;
			this->getParamsButton->Text = L"Get";
			this->getParamsButton->UseVisualStyleBackColor = true;
			this->getParamsButton->Click += gcnew System::EventHandler(this, &EditorForm::getParamsButton_Click);
			// 
			// entityGroupBox
			// 
			this->entityGroupBox->AutoSize = true;
			this->entityGroupBox->Controls->Add(this->stopAnimationButton);
			this->entityGroupBox->Controls->Add(this->playAnimationButton);
			this->entityGroupBox->Controls->Add(this->label10);
			this->entityGroupBox->Controls->Add(this->animationsComboBox);
			this->entityGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->entityGroupBox->Location = System::Drawing::Point(0, 914);
			this->entityGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->entityGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->entityGroupBox->Name = L"entityGroupBox";
			this->entityGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->entityGroupBox->Size = System::Drawing::Size(374, 70);
			this->entityGroupBox->TabIndex = 28;
			this->entityGroupBox->TabStop = false;
			this->entityGroupBox->Text = L"Entity";
			this->entityGroupBox->Visible = false;
			// 
			// stopAnimationButton
			// 
			this->stopAnimationButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"stopAnimationButton.BackgroundImage")));
			this->stopAnimationButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->stopAnimationButton->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->stopAnimationButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Khaki;
			this->stopAnimationButton->FlatAppearance->MouseOverBackColor = System::Drawing::SystemColors::ControlDark;
			this->stopAnimationButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->stopAnimationButton->Location = System::Drawing::Point(316, 21);
			this->stopAnimationButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->stopAnimationButton->Name = L"stopAnimationButton";
			this->stopAnimationButton->Size = System::Drawing::Size(30, 30);
			this->stopAnimationButton->TabIndex = 26;
			this->stopAnimationButton->UseVisualStyleBackColor = true;
			this->stopAnimationButton->Click += gcnew System::EventHandler(this, &EditorForm::stopAnimationButton_Click);
			// 
			// playAnimationButton
			// 
			this->playAnimationButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"playAnimationButton.BackgroundImage")));
			this->playAnimationButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->playAnimationButton->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->playAnimationButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Khaki;
			this->playAnimationButton->FlatAppearance->MouseOverBackColor = System::Drawing::SystemColors::ControlDark;
			this->playAnimationButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->playAnimationButton->Location = System::Drawing::Point(280, 21);
			this->playAnimationButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->playAnimationButton->Name = L"playAnimationButton";
			this->playAnimationButton->Size = System::Drawing::Size(30, 30);
			this->playAnimationButton->TabIndex = 25;
			this->playAnimationButton->UseVisualStyleBackColor = true;
			this->playAnimationButton->Click += gcnew System::EventHandler(this, &EditorForm::playAnimationButton_Click);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(25, 28);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(77, 17);
			this->label10->TabIndex = 3;
			this->label10->Text = L"Animations";
			// 
			// animationsComboBox
			// 
			this->animationsComboBox->FormattingEnabled = true;
			this->animationsComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Placement" });
			this->animationsComboBox->Location = System::Drawing::Point(102, 25);
			this->animationsComboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->animationsComboBox->Name = L"animationsComboBox";
			this->animationsComboBox->Size = System::Drawing::Size(170, 24);
			this->animationsComboBox->TabIndex = 2;
			// 
			// grassGroupBox
			// 
			this->grassGroupBox->AutoSize = true;
			this->grassGroupBox->Controls->Add(this->grassSizeWMax);
			this->grassGroupBox->Controls->Add(this->grassSizeWMin);
			this->grassGroupBox->Controls->Add(this->label14);
			this->grassGroupBox->Controls->Add(this->grassSizeHMax);
			this->grassGroupBox->Controls->Add(this->grassSizeHMin);
			this->grassGroupBox->Controls->Add(this->label13);
			this->grassGroupBox->Controls->Add(this->grassPaingPreserveCheckbox);
			this->grassGroupBox->Controls->Add(this->grassDensity);
			this->grassGroupBox->Controls->Add(this->label15);
			this->grassGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->grassGroupBox->Location = System::Drawing::Point(0, 765);
			this->grassGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->grassGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->grassGroupBox->Name = L"grassGroupBox";
			this->grassGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->grassGroupBox->Size = System::Drawing::Size(374, 149);
			this->grassGroupBox->TabIndex = 27;
			this->grassGroupBox->TabStop = false;
			this->grassGroupBox->Text = L"Grass";
			this->grassGroupBox->Visible = false;
			// 
			// grassSizeWMax
			// 
			this->grassSizeWMax->DecimalPlaces = 2;
			this->grassSizeWMax->Location = System::Drawing::Point(200, 76);
			this->grassSizeWMax->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->grassSizeWMax->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 20, 0, 0, 0 });
			this->grassSizeWMax->Name = L"grassSizeWMax";
			this->grassSizeWMax->Size = System::Drawing::Size(91, 22);
			this->grassSizeWMax->TabIndex = 39;
			this->grassSizeWMax->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->grassSizeWMax->ValueChanged += gcnew System::EventHandler(this, &EditorForm::grassSizeWMax_ValueChanged);
			// 
			// grassSizeWMin
			// 
			this->grassSizeWMin->DecimalPlaces = 2;
			this->grassSizeWMin->Location = System::Drawing::Point(103, 76);
			this->grassSizeWMin->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->grassSizeWMin->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 20, 0, 0, 0 });
			this->grassSizeWMin->Name = L"grassSizeWMin";
			this->grassSizeWMin->Size = System::Drawing::Size(91, 22);
			this->grassSizeWMin->TabIndex = 38;
			this->grassSizeWMin->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->grassSizeWMin->ValueChanged += gcnew System::EventHandler(this, &EditorForm::grassSizeWMin_ValueChanged);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(11, 78);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(85, 17);
			this->label14->TabIndex = 37;
			this->label14->Text = L"Width range";
			// 
			// grassSizeHMax
			// 
			this->grassSizeHMax->DecimalPlaces = 2;
			this->grassSizeHMax->Location = System::Drawing::Point(200, 46);
			this->grassSizeHMax->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->grassSizeHMax->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 20, 0, 0, 0 });
			this->grassSizeHMax->Name = L"grassSizeHMax";
			this->grassSizeHMax->Size = System::Drawing::Size(91, 22);
			this->grassSizeHMax->TabIndex = 36;
			this->grassSizeHMax->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->grassSizeHMax->ValueChanged += gcnew System::EventHandler(this, &EditorForm::grassSizeHMax_ValueChanged);
			// 
			// grassSizeHMin
			// 
			this->grassSizeHMin->DecimalPlaces = 2;
			this->grassSizeHMin->Location = System::Drawing::Point(103, 46);
			this->grassSizeHMin->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->grassSizeHMin->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 20, 0, 0, 0 });
			this->grassSizeHMin->Name = L"grassSizeHMin";
			this->grassSizeHMin->Size = System::Drawing::Size(91, 22);
			this->grassSizeHMin->TabIndex = 35;
			this->grassSizeHMin->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->grassSizeHMin->ValueChanged += gcnew System::EventHandler(this, &EditorForm::grassSizeHMin_ValueChanged);
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(6, 51);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(90, 17);
			this->label13->TabIndex = 34;
			this->label13->Text = L"Height range";
			// 
			// grassPaingPreserveCheckbox
			// 
			this->grassPaingPreserveCheckbox->AutoSize = true;
			this->grassPaingPreserveCheckbox->Location = System::Drawing::Point(101, 107);
			this->grassPaingPreserveCheckbox->Margin = System::Windows::Forms::Padding(4);
			this->grassPaingPreserveCheckbox->Name = L"grassPaingPreserveCheckbox";
			this->grassPaingPreserveCheckbox->Size = System::Drawing::Size(159, 21);
			this->grassPaingPreserveCheckbox->TabIndex = 33;
			this->grassPaingPreserveCheckbox->Text = L"Preserve paint mask";
			this->grassPaingPreserveCheckbox->UseVisualStyleBackColor = true;
			this->grassPaingPreserveCheckbox->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::grassPaingPreserveCheckbox_CheckedChanged);
			// 
			// grassDensity
			// 
			this->grassDensity->DecimalPlaces = 2;
			this->grassDensity->Location = System::Drawing::Point(103, 18);
			this->grassDensity->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->grassDensity->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 20, 0, 0, 0 });
			this->grassDensity->Name = L"grassDensity";
			this->grassDensity->Size = System::Drawing::Size(91, 22);
			this->grassDensity->TabIndex = 3;
			this->grassDensity->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->grassDensity->ValueChanged += gcnew System::EventHandler(this, &EditorForm::grassDensity_ValueChanged);
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(41, 21);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(55, 17);
			this->label15->TabIndex = 1;
			this->label15->Text = L"Density";
			// 
			// paintGroupBox
			// 
			this->paintGroupBox->AutoSize = true;
			this->paintGroupBox->Controls->Add(this->paintFill);
			this->paintGroupBox->Controls->Add(this->paintSizeText);
			this->paintGroupBox->Controls->Add(this->paintWText);
			this->paintGroupBox->Controls->Add(this->label12);
			this->paintGroupBox->Controls->Add(this->label11);
			this->paintGroupBox->Controls->Add(this->paintSizeTrack);
			this->paintGroupBox->Controls->Add(this->paintAdd);
			this->paintGroupBox->Controls->Add(this->paintRemove);
			this->paintGroupBox->Controls->Add(this->paintWTrack);
			this->paintGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->paintGroupBox->Location = System::Drawing::Point(0, 566);
			this->paintGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->paintGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->paintGroupBox->Name = L"paintGroupBox";
			this->paintGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->paintGroupBox->Size = System::Drawing::Size(374, 199);
			this->paintGroupBox->TabIndex = 26;
			this->paintGroupBox->TabStop = false;
			this->paintGroupBox->Text = L"Paint";
			this->paintGroupBox->Visible = false;
			// 
			// paintFill
			// 
			this->paintFill->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"paintFill.BackgroundImage")));
			this->paintFill->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->paintFill->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->paintFill->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Khaki;
			this->paintFill->FlatAppearance->MouseOverBackColor = System::Drawing::SystemColors::ControlDark;
			this->paintFill->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->paintFill->Location = System::Drawing::Point(237, 19);
			this->paintFill->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->paintFill->Name = L"paintFill";
			this->paintFill->Size = System::Drawing::Size(53, 54);
			this->paintFill->TabIndex = 31;
			this->paintFill->UseVisualStyleBackColor = true;
			this->paintFill->Click += gcnew System::EventHandler(this, &EditorForm::paintFill_Click);
			// 
			// paintSizeText
			// 
			this->paintSizeText->Location = System::Drawing::Point(102, 121);
			this->paintSizeText->Margin = System::Windows::Forms::Padding(4);
			this->paintSizeText->Name = L"paintSizeText";
			this->paintSizeText->ReadOnly = true;
			this->paintSizeText->Size = System::Drawing::Size(53, 22);
			this->paintSizeText->TabIndex = 30;
			this->paintSizeText->Text = L"5";
			// 
			// paintWText
			// 
			this->paintWText->Location = System::Drawing::Point(104, 90);
			this->paintWText->Margin = System::Windows::Forms::Padding(4);
			this->paintWText->Name = L"paintWText";
			this->paintWText->ReadOnly = true;
			this->paintWText->Size = System::Drawing::Size(53, 22);
			this->paintWText->TabIndex = 29;
			this->paintWText->Text = L"50";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(37, 125);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(35, 17);
			this->label12->TabIndex = 28;
			this->label12->Text = L"Size";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(37, 94);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(52, 17);
			this->label11->TabIndex = 27;
			this->label11->Text = L"Weight";
			// 
			// paintSizeTrack
			// 
			this->paintSizeTrack->LargeChange = 10;
			this->paintSizeTrack->Location = System::Drawing::Point(184, 122);
			this->paintSizeTrack->Margin = System::Windows::Forms::Padding(4);
			this->paintSizeTrack->Maximum = 50;
			this->paintSizeTrack->Minimum = 1;
			this->paintSizeTrack->Name = L"paintSizeTrack";
			this->paintSizeTrack->Size = System::Drawing::Size(139, 56);
			this->paintSizeTrack->TabIndex = 26;
			this->paintSizeTrack->TickStyle = System::Windows::Forms::TickStyle::None;
			this->paintSizeTrack->Value = 5;
			this->paintSizeTrack->Scroll += gcnew System::EventHandler(this, &EditorForm::paintSizeTrack_Scroll);
			// 
			// paintAdd
			// 
			this->paintAdd->Appearance = System::Windows::Forms::Appearance::Button;
			this->paintAdd->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"paintAdd.BackgroundImage")));
			this->paintAdd->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->paintAdd->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->paintAdd->FlatAppearance->CheckedBackColor = System::Drawing::Color::Khaki;
			this->paintAdd->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->paintAdd->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->paintAdd->Location = System::Drawing::Point(102, 19);
			this->paintAdd->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->paintAdd->Name = L"paintAdd";
			this->paintAdd->Size = System::Drawing::Size(53, 54);
			this->paintAdd->TabIndex = 23;
			this->paintAdd->TabStop = true;
			this->paintAdd->UseVisualStyleBackColor = true;
			this->paintAdd->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::paintAdd_CheckedChanged);
			// 
			// paintRemove
			// 
			this->paintRemove->Appearance = System::Windows::Forms::Appearance::Button;
			this->paintRemove->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"paintRemove.BackgroundImage")));
			this->paintRemove->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->paintRemove->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlLight;
			this->paintRemove->FlatAppearance->CheckedBackColor = System::Drawing::Color::Khaki;
			this->paintRemove->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->paintRemove->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->paintRemove->Location = System::Drawing::Point(170, 19);
			this->paintRemove->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->paintRemove->Name = L"paintRemove";
			this->paintRemove->Size = System::Drawing::Size(53, 54);
			this->paintRemove->TabIndex = 24;
			this->paintRemove->TabStop = true;
			this->paintRemove->UseVisualStyleBackColor = true;
			this->paintRemove->CheckedChanged += gcnew System::EventHandler(this, &EditorForm::paintRemove_CheckedChanged);
			// 
			// paintWTrack
			// 
			this->paintWTrack->Location = System::Drawing::Point(184, 91);
			this->paintWTrack->Margin = System::Windows::Forms::Padding(4);
			this->paintWTrack->Maximum = 100;
			this->paintWTrack->Name = L"paintWTrack";
			this->paintWTrack->Size = System::Drawing::Size(139, 56);
			this->paintWTrack->TabIndex = 6;
			this->paintWTrack->TickStyle = System::Windows::Forms::TickStyle::None;
			this->paintWTrack->Value = 50;
			this->paintWTrack->Scroll += gcnew System::EventHandler(this, &EditorForm::paintWTrack_Scroll);
			// 
			// placementGroupBox
			// 
			this->placementGroupBox->AutoSize = true;
			this->placementGroupBox->Controls->Add(this->rayButton);
			this->placementGroupBox->Controls->Add(this->placementOffsetNum);
			this->placementGroupBox->Controls->Add(this->label8);
			this->placementGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->placementGroupBox->Location = System::Drawing::Point(0, 506);
			this->placementGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->placementGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->placementGroupBox->Name = L"placementGroupBox";
			this->placementGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->placementGroupBox->Size = System::Drawing::Size(374, 60);
			this->placementGroupBox->TabIndex = 25;
			this->placementGroupBox->TabStop = false;
			this->placementGroupBox->Text = L"Placement";
			this->placementGroupBox->Visible = false;
			// 
			// rayButton
			// 
			this->rayButton->Location = System::Drawing::Point(235, 18);
			this->rayButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->rayButton->Name = L"rayButton";
			this->rayButton->Size = System::Drawing::Size(75, 23);
			this->rayButton->TabIndex = 4;
			this->rayButton->Text = L"Ray";
			this->rayButton->UseVisualStyleBackColor = true;
			this->rayButton->Click += gcnew System::EventHandler(this, &EditorForm::rayButton_Click);
			// 
			// placementOffsetNum
			// 
			this->placementOffsetNum->DecimalPlaces = 2;
			this->placementOffsetNum->Location = System::Drawing::Point(103, 18);
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
			this->utilsGroupBox->Location = System::Drawing::Point(0, 445);
			this->utilsGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->utilsGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->utilsGroupBox->Name = L"utilsGroupBox";
			this->utilsGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->utilsGroupBox->Size = System::Drawing::Size(374, 61);
			this->utilsGroupBox->TabIndex = 22;
			this->utilsGroupBox->TabStop = false;
			this->utilsGroupBox->Text = L"Utilities";
			this->utilsGroupBox->Visible = false;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(45, 25);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(40, 17);
			this->label9->TabIndex = 1;
			this->label9->Text = L"Type";
			// 
			// utilsComboBox
			// 
			this->utilsComboBox->FormattingEnabled = true;
			this->utilsComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Placement" });
			this->utilsComboBox->Location = System::Drawing::Point(101, 18);
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
			this->selectionGroupBox->Location = System::Drawing::Point(0, 253);
			this->selectionGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->selectionGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->selectionGroupBox->Name = L"selectionGroupBox";
			this->selectionGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->selectionGroupBox->Size = System::Drawing::Size(374, 192);
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
			this->selectionNameButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->selectionNameButton->Name = L"selectionNameButton";
			this->selectionNameButton->Size = System::Drawing::Size(75, 23);
			this->selectionNameButton->TabIndex = 12;
			this->selectionNameButton->UseVisualStyleBackColor = true;
			this->selectionNameButton->Click += gcnew System::EventHandler(this, &EditorForm::selectionNameButton_Click);
			// 
			// selectionListBox
			// 
			this->selectionListBox->BackColor = System::Drawing::Color::Gainsboro;
			this->selectionListBox->FormattingEnabled = true;
			this->selectionListBox->ItemHeight = 16;
			this->selectionListBox->Location = System::Drawing::Point(101, 105);
			this->selectionListBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->selectionListBox->Name = L"selectionListBox";
			this->selectionListBox->Size = System::Drawing::Size(232, 68);
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
			this->label2->Location = System::Drawing::Point(29, 52);
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
			this->sceneGroupBox->Location = System::Drawing::Point(0, 157);
			this->sceneGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->sceneGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->sceneGroupBox->Name = L"sceneGroupBox";
			this->sceneGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->sceneGroupBox->Size = System::Drawing::Size(374, 96);
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
			this->skyboxComboBox->Location = System::Drawing::Point(101, 18);
			this->skyboxComboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->skyboxComboBox->Name = L"skyboxComboBox";
			this->skyboxComboBox->Size = System::Drawing::Size(231, 24);
			this->skyboxComboBox->TabIndex = 2;
			this->skyboxComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::skyboxComboBox_SelectedIndexChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(52, 62);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(35, 17);
			this->label6->TabIndex = 2;
			this->label6->Text = L"LUT";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(35, 22);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(53, 17);
			this->label5->TabIndex = 1;
			this->label5->Text = L"Skybox";
			// 
			// addGroupBox
			// 
			this->addGroupBox->AutoSize = true;
			this->addGroupBox->Controls->Add(this->addItemTypeComboBox);
			this->addGroupBox->Controls->Add(this->label4);
			this->addGroupBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->addGroupBox->Location = System::Drawing::Point(0, 96);
			this->addGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->addGroupBox->MinimumSize = System::Drawing::Size(0, 20);
			this->addGroupBox->Name = L"addGroupBox";
			this->addGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->addGroupBox->Size = System::Drawing::Size(374, 61);
			this->addGroupBox->TabIndex = 20;
			this->addGroupBox->TabStop = false;
			this->addGroupBox->Text = L"Add";
			this->addGroupBox->Visible = false;
			// 
			// addItemTypeComboBox
			// 
			this->addItemTypeComboBox->FormattingEnabled = true;
			this->addItemTypeComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Entity" });
			this->addItemTypeComboBox->Location = System::Drawing::Point(101, 18);
			this->addItemTypeComboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->addItemTypeComboBox->Name = L"addItemTypeComboBox";
			this->addItemTypeComboBox->Size = System::Drawing::Size(231, 24);
			this->addItemTypeComboBox->TabIndex = 9;
			this->addItemTypeComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::addItemTypeComboBox_SelectedIndexChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(47, 21);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(40, 17);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Type";
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
			this->toolsGroupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->toolsGroupBox->Name = L"toolsGroupBox";
			this->toolsGroupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->toolsGroupBox->Size = System::Drawing::Size(374, 96);
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
			this->utilsCheckBox->Location = System::Drawing::Point(235, 21);
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
			this->sceneCheckBox->Location = System::Drawing::Point(93, 21);
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
			this->addItemCheckBox->Location = System::Drawing::Point(164, 21);
			this->addItemCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->addItemCheckBox->Name = L"addItemCheckBox";
			this->addItemCheckBox->Size = System::Drawing::Size(53, 54);
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
			this->rotateObjButton->Location = System::Drawing::Point(359, 23);
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
			this->scaleObjButton->Location = System::Drawing::Point(288, 23);
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
			this->moveObjButton->Location = System::Drawing::Point(217, 23);
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
			this->selectObjButton->Location = System::Drawing::Point(149, 23);
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
			this->topPanel->Controls->Add(this->levelsComboBox);
			this->topPanel->Controls->Add(this->sceneListButton);
			this->topPanel->Controls->Add(this->rotateObjButton);
			this->topPanel->Controls->Add(this->scaleObjButton);
			this->topPanel->Controls->Add(this->selectObjButton);
			this->topPanel->Controls->Add(this->moveObjButton);
			this->topPanel->Dock = System::Windows::Forms::DockStyle::Top;
			this->topPanel->Location = System::Drawing::Point(399, 0);
			this->topPanel->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->topPanel->Name = L"topPanel";
			this->topPanel->Size = System::Drawing::Size(965, 87);
			this->topPanel->TabIndex = 18;
			// 
			// levelsComboBox
			// 
			this->levelsComboBox->FormattingEnabled = true;
			this->levelsComboBox->Location = System::Drawing::Point(776, 23);
			this->levelsComboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->levelsComboBox->Name = L"levelsComboBox";
			this->levelsComboBox->Size = System::Drawing::Size(177, 24);
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
			this->sceneListButton->Location = System::Drawing::Point(21, 23);
			this->sceneListButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->sceneListButton->Name = L"sceneListButton";
			this->sceneListButton->Size = System::Drawing::Size(53, 54);
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
			this->renderPanel->Location = System::Drawing::Point(399, 87);
			this->renderPanel->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->renderPanel->Name = L"renderPanel";
			this->renderPanel->Size = System::Drawing::Size(965, 612);
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
			this->levelLoading->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->levelLoading->Name = L"levelLoading";
			this->levelLoading->Size = System::Drawing::Size(965, 612);
			this->levelLoading->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->levelLoading->TabIndex = 2;
			this->levelLoading->TabStop = false;
			this->levelLoading->UseWaitCursor = true;
			// 
			// bottomPanel
			// 
			this->bottomPanel->Controls->Add(this->reloadShadersbutton);
			this->bottomPanel->Controls->Add(this->reloadGeometryButton);
			this->bottomPanel->Controls->Add(this->lastSelectedList);
			this->bottomPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->bottomPanel->Location = System::Drawing::Point(399, 699);
			this->bottomPanel->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->bottomPanel->Name = L"bottomPanel";
			this->bottomPanel->Size = System::Drawing::Size(965, 81);
			this->bottomPanel->TabIndex = 21;
			// 
			// reloadShadersbutton
			// 
			this->reloadShadersbutton->Location = System::Drawing::Point(6, 5);
			this->reloadShadersbutton->Name = L"reloadShadersbutton";
			this->reloadShadersbutton->Size = System::Drawing::Size(136, 26);
			this->reloadShadersbutton->TabIndex = 2;
			this->reloadShadersbutton->Text = L"Reload shaders";
			this->reloadShadersbutton->UseVisualStyleBackColor = true;
			this->reloadShadersbutton->Click += gcnew System::EventHandler(this, &EditorForm::reloadShadersbutton_Click);
			// 
			// reloadGeometryButton
			// 
			this->reloadGeometryButton->Location = System::Drawing::Point(6, 33);
			this->reloadGeometryButton->Name = L"reloadGeometryButton";
			this->reloadGeometryButton->Size = System::Drawing::Size(136, 26);
			this->reloadGeometryButton->TabIndex = 1;
			this->reloadGeometryButton->Text = L"Reload geometry";
			this->reloadGeometryButton->UseVisualStyleBackColor = true;
			this->reloadGeometryButton->Click += gcnew System::EventHandler(this, &EditorForm::reloadGeometryButton_Click);
			// 
			// lastSelectedList
			// 
			this->lastSelectedList->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->lastSelectedList->BackColor = System::Drawing::Color::Gainsboro;
			this->lastSelectedList->FormattingEnabled = true;
			this->lastSelectedList->ItemHeight = 16;
			this->lastSelectedList->Location = System::Drawing::Point(715, 0);
			this->lastSelectedList->Name = L"lastSelectedList";
			this->lastSelectedList->Size = System::Drawing::Size(250, 84);
			this->lastSelectedList->TabIndex = 0;
			this->lastSelectedList->SelectedIndexChanged += gcnew System::EventHandler(this, &EditorForm::lastSelectedList_SelectedIndexChanged);
			// 
			// EditorForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->AutoSize = true;
			this->BackColor = System::Drawing::SystemColors::ControlDark;
			this->ClientSize = System::Drawing::Size(1364, 780);
			this->Controls->Add(this->renderPanel);
			this->Controls->Add(this->bottomPanel);
			this->Controls->Add(this->topPanel);
			this->Controls->Add(this->sidePanel);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"EditorForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Editor";
			this->Closing += gcnew System::ComponentModel::CancelEventHandler(this, &EditorForm::EditorForm_FormClosing);
			this->sidePanel->ResumeLayout(false);
			this->sidePanel->PerformLayout();
			this->addEntityGroupBox->ResumeLayout(false);
			this->addEntityGroupBox->PerformLayout();
			this->itemParamsGroupBox->ResumeLayout(false);
			this->entityGroupBox->ResumeLayout(false);
			this->entityGroupBox->PerformLayout();
			this->grassGroupBox->ResumeLayout(false);
			this->grassGroupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassSizeWMax))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassSizeWMin))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassSizeHMax))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassSizeHMin))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grassDensity))->EndInit();
			this->paintGroupBox->ResumeLayout(false);
			this->paintGroupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->paintSizeTrack))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->paintWTrack))->EndInit();
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
			this->bottomPanel->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

private: ref class AsyncParam
{
public: UiMessageId id;
public: System::String^ subtype;
public: int subId;
public: System::Windows::Forms::Control^ source;
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
			change.item.name = marshal_as<std::string>(node->Text);
		}
		else if (param->subtype == "History")
		{
			int idx = lastSelectedList->Items->Count - 1 - lastSelectedList->SelectedIndex;
			auto& i = selectionHistoryItems[idx];
			change.groupName = i.group;
			change.item.name = i.name;
		}
		else
			change.item.name = marshal_as<std::string>(selectionListBox->SelectedItem->ToString());

		SendMsg(UiMessageId::SelectWorldItem, &change);
		break;
	}
	case UiMessageId::AddItemMode:
	{
		AddItemModeInfo info;

		if (param->subId == (int)ItemType::Entity)
		{
			info.type = ItemType::Entity;
			info.path = marshal_as<std::string>(addEntityPathTextBox->Text);
		}

		SendMsg(UiMessageId::AddItemMode, &info);
		break;
	}		
	case UiMessageId::SelectionInfoChanged:
	{
		SelectionInfoChange change;
		Ogre::Vector3 vparam;
		Ogre::Vector4 v4param;
		EditableParam eParam;
		float fparam;
		std::string sparam;

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
		if (change.change == SelectionInfoChange::Id::GrassSize)
		{
			v4param.x = System::Decimal::ToSingle(grassSizeHMin->Value);
			v4param.y = System::Decimal::ToSingle(grassSizeHMax->Value);
			v4param.z = System::Decimal::ToSingle(grassSizeWMin->Value);
			v4param.w = System::Decimal::ToSingle(grassSizeWMax->Value);
			change.data = &v4param;
		}
		if (change.change == SelectionInfoChange::Id::GrassDensity)
		{
			fparam = System::Decimal::ToSingle(grassDensity->Value);
			change.data = &fparam;
		}
		if (change.change == SelectionInfoChange::Id::PaintFill)
		{
			fparam = System::Decimal::ToSingle(paintWTrack->Value)*0.01f;
			change.data = &fparam;
		}
		if (change.change == SelectionInfoChange::Id::PaintAdd || change.change == SelectionInfoChange::Id::PaintRemove)
		{
			vparam = { System::Decimal::ToSingle(paintWTrack->Value)*0.01f, System::Decimal::ToSingle(paintSizeTrack->Value) , 0};
			change.data = &vparam;
		}
		if (change.change == SelectionInfoChange::Id::PaintSizeChange)
		{
			fparam = System::Decimal::ToSingle(paintSizeTrack->Value);
			change.data = &fparam;
		}
		if (change.change == SelectionInfoChange::Id::PaintWChange)
		{
			fparam = System::Decimal::ToSingle(paintWTrack->Value)*0.01f;
			change.data = &fparam;
		}
		if (change.change == SelectionInfoChange::Id::GrassPaintPreserve)
		{
			fparam = grassPaingPreserveCheckbox->Checked ? 1.0f : 0.0f;
			change.data = &fparam;
		}
		if (change.change == SelectionInfoChange::Id::ActiveAnimation)
		{
			if(param->subtype != "")
				sparam = marshal_as<std::string>(animationsComboBox->Text);
			change.data = &sparam;
		}
		if (change.change == SelectionInfoChange::Id::ParamChanged)
		{
			auto& info = getEditableParam(param->subtype);
			
			if (info.param.name.empty() || !param->source)
				break;

			if (info.param.type == EditableParam::Type::Floats)
			{
				auto item = (System::Windows::Forms::NumericUpDown^) param->source;
				info.param.floats.buffer[info.index] = System::Decimal::ToSingle(item->Value);
				eParam = info.param;
				eParam.floats.size = info.index;
				change.data = &eParam;
			}
			else
				break;
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
		auto lvlName = marshal_as<std::string>(levelsComboBox->SelectedText);
		SendMsg(UiMessageId::LoadLevel, &lvlName);
		renderPanel->Focus();
		break;
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
private: System::Void SendAsyncMsg(UiMessageId id, int subid, System::Windows::Forms::Control^ source)
{
	System::Threading::Thread^ newThread = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &EditorForm::SendAsyncMsgThread));
	auto param = gcnew AsyncParam();
	param->id = id;
	param->subtype = source->Name;
	param->source = source;
	param->subId = subid;

	newThread->Start(param);
}	 
private: System::Void SendAsyncMsg(UiMessageId id, int subid, System::String^ data)
{
	System::Threading::Thread^ newThread = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &EditorForm::SendAsyncMsgThread));
	auto param = gcnew AsyncParam();
	param->id = id;
	param->subtype = data;
	param->subId = subid;

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
		addEntityGroupBox->Hide();
		return;
	}

	addGroupBox->Show();
	addItemTypeComboBox->Text = addItemTypeComboBox->Text;
}

private: void deselectSelectionEditButtons()
{
	selectObjButton->Checked = false;
	moveObjButton->Checked = false;
	scaleObjButton->Checked = false;
	rotateObjButton->Checked = false;
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
	paintGroupBox->Hide();
	entityGroupBox->Hide();
	itemParamsGroupBox->Hide();
}
	
public: System::Void setLoading(bool enabled)
{
	levelLoading->Visible = enabled;

	if (enabled)
		clearSceneInfo();
}

public: System::Void clearSceneInfo()
{
	hideItemInfo();

	selectionHistoryItems.clear();
	lastSelectedList->SelectedIndex = -1;
	lastSelectedList->Items->Clear();
}

public: System::Void setProperties(EditorProperties* info)
{
	levelsComboBox->Items->Clear();

	for (auto& l : info->levels)
	{
		levelsComboBox->Items->Add(gcnew System::String(l.data()));
	}
}

private: void addParamGroupLabel(const char* text, int height)
{
	auto label = (gcnew System::Windows::Forms::Label());

	label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	label->Location = System::Drawing::Point(25, height);
	label->Text = gcnew System::String(text);
	label->Name = label->Text + L"ParamsLabel";
	label->Size = System::Drawing::Size(330, 17);
	label->TabIndex = 12;
	label->TextAlign = System::Drawing::ContentAlignment::TopCenter;

	itemParamsGroupBox->Controls->Add(label);
}

private: void addEditableParam(EditableParam& param, int height)
{
	if (param.type != EditableParam::Type::Floats)
		return;

	auto label = (gcnew System::Windows::Forms::Label());
	label->AutoSize = true;
	label->Location = System::Drawing::Point(25, height);
	label->Text = gcnew System::String(param.name.data());
	label->Name = label->Text + L"ParamLabel";
	label->Size = System::Drawing::Size(77, 17);
	label->TabIndex = 3;
	
	itemParamsGroupBox->Controls->Add(label);

	height += 18;

	reportSelectionChange = false;

	for (int i = 0; i < param.floats.size; i++)
	{
		System::Windows::Forms::NumericUpDown^ numeric;

		numeric = (gcnew System::Windows::Forms::NumericUpDown());
		numeric->DecimalPlaces = 3;
		numeric->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
		numeric->Maximum = System::Decimal(10000);
		numeric->Minimum = System::Decimal(-10000);
		numeric->Size = System::Drawing::Size(75, 22);
		numeric->TabIndex = 11;
		numeric->ValueChanged += gcnew System::EventHandler(this, &EditorForm::editParamValueChanged);

		numeric->Location = System::Drawing::Point(32 + 86 * i, height);
		numeric->Name = gcnew System::String(param.name.data()) + L"Numeric" + System::Int32(i).ToString();
		numeric->Increment = System::Decimal(param.floats.step);
		numeric->Value = System::Decimal(param.floats.buffer[i]);

		itemParamsGroupBox->Controls->Add(numeric);

		currentEditableParams[marshal_as<std::string>(numeric->Name)] = { param,i };
	}	

	reportSelectionChange = true;
}

private: void initEditableParamsControls(std::vector<EditableParams>& params)
{
	int height = 45;

	itemParamsGroupBox->SuspendLayout();
	for (auto& g : params)
	{
		addParamGroupLabel(g.groupName.data(), height);
		height += 17;

		for (auto& p : g.params)
		{
			addEditableParam(p, height);
			height += 42;
		}
	}

	getParamsButton->Text = "Save";
	itemParamsGroupBox->ResumeLayout();
}

private: void resetEditableParams()
{
	itemParamsGroupBox->Controls->Clear();
	itemParamsGroupBox->Controls->Add(getParamsButton);
	getParamsButton->Text = "Get";

	currentEditableParams.clear();
}

public: System::Void changeItemInfo(SelectionInfoChange* info)
{
	if (info->change == SelectionInfoChange::Id::SendParams)
	{
		auto resp = (EditableParamsResponse*)info->data;

		initEditableParamsControls(resp->params);
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

	if (info->names.empty())
	{
		addToSelectionHistory(selectionNameButton->Text, getItemTypeStr(info->type));
	}

	reportSelectionChange = false;
	bool showGrassGroup = false;
	bool showEntityGroup = false;
	bool showParamsGroup = info->hasParams;

	resetEditableParams();
	
	if (info->typeData)
	{
		if (info->type == ItemType::Grass)
		{
			auto subinfo = (GrassSelectionInfo*)info->typeData;

			grassDensity->Value = System::Decimal(subinfo->density);
			grassSizeHMin->Value = System::Decimal(subinfo->minHSize);
			grassSizeHMax->Value = System::Decimal(subinfo->maxHSize);
			grassSizeWMin->Value = System::Decimal(subinfo->minWSize);
			grassSizeWMax->Value = System::Decimal(subinfo->maxWSize);
			grassPaingPreserveCheckbox->Checked = subinfo->preserveMask;

			grassGroupBox->BringToFront();
			showGrassGroup = true;
		}

		if (info->type == ItemType::Entity)
		{
			auto subinfo = (EntitySelectionInfo*)info->typeData;

			animationsComboBox->Items->Clear();
			animationsComboBox->Text = "";
			for (auto& a : subinfo->animNames)
			{
				animationsComboBox->Items->Add(gcnew System::String(a.data()));
			}

			if (!subinfo->animNames.empty())
				animationsComboBox->SelectedIndex = 0;

			entityGroupBox->BringToFront();
			showEntityGroup = true;

			/*EditableParamsResponse resp;
			//if (data->change == SelectionInfoChange::Id::GetParams)
			{
				auto& paramsData = resp.params;

				EditableParams params;
				params.groupName = "Hello";

				for (size_t i = 0; i < 15; i++)
				{
					EditableParam param;
					param.size = 2;
					param.buffer[0] = 5;
					param.buffer[1] = 10;
					param.group = params.groupName;
					param.step = 0.1;
					param.name = "World" + std::to_string(i);
					params.params.push_back(param);
				}

				paramsData.push_back(params);
			}

			initEditableParamsControls(resp.params);*/
		}
	}

	bool showPaintInfo = info->usePaint;
	if (showPaintInfo)
	{
		paintAdd->Checked = false;
		paintRemove->Checked = false;
	}

	entPosX->Value = System::Decimal(info->pos.x);
	entPosY->Value = System::Decimal(info->pos.y);
	entPosZ->Value = System::Decimal(info->pos.z);

	entScaleX->Value = System::Decimal(info->scale.x);
	entScaleY->Value = System::Decimal(info->scale.y);
	entScaleZ->Value = System::Decimal(info->scale.z);

	reportSelectionChange = true;

	paintGroupBox->Visible = showPaintInfo;
	grassGroupBox->Visible = showGrassGroup;
	entityGroupBox->Visible = showEntityGroup;
	itemParamsGroupBox->Visible = showParamsGroup;
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
		auto id = sceneListTree->Nodes->Add(gcnew TreeNode(getItemTypeStr(group.type)));

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

private: System::Void paintWTrack_Scroll(System::Object^  sender, System::EventArgs^  e) {
	paintWText->Text = paintWTrack->Value.ToString();

	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::PaintWChange);
}
private: System::Void paintSizeTrack_Scroll(System::Object^  sender, System::EventArgs^  e) {
	paintSizeText->Text = paintSizeTrack->Value.ToString();

	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::PaintSizeChange);
}
private: System::Void paintFill_Click(System::Object^  sender, System::EventArgs^  e) {
	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::PaintFill);
}
private: System::Void grassDensity_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::GrassDensity);
}
private: System::Void grassSizeHMin_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange)
		return;

SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::GrassSize);
}
private: System::Void grassSizeHMax_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::GrassSize);
}
private: System::Void grassSizeWMin_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void grassSizeWMax_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void paintAdd_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange || paintAdd->Checked == false)
		return;

	deselectSelectionEditButtons();

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::PaintAdd);
}
private: System::Void paintRemove_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange || paintRemove->Checked == false)
		return;

	deselectSelectionEditButtons();

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::PaintRemove);
}
private: System::Void grassPaingPreserveCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (!reportSelectionChange)
		return;

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::GrassPaintPreserve);
}

private: void addToSelectionHistory(System::String^ name, System::String^ group)
{
	SelectionHistoryItem item;
	item.name = marshal_as<std::string>(name);
	item.group = marshal_as<std::string>(group);

	for (auto it = selectionHistoryItems.begin(); it != selectionHistoryItems.end();)
	{
		if (it->name == item.name)
			it = selectionHistoryItems.erase(it);
		else
			it++;
	}

	selectionHistoryItems.push_back(item);

	while (selectionHistoryItems.size() > 6)
		selectionHistoryItems.erase(selectionHistoryItems.begin());

	lastSelectedList->Items->Clear();
	lastSelectedList->SelectedIndex = -1;

	for (auto it = selectionHistoryItems.rbegin(); it != selectionHistoryItems.rend(); it++)
	{
		auto txt = it->name + " \t(" + it->group + ")";
		lastSelectedList->Items->Add(gcnew System::String(txt.data()));
	}
}

private: System::Void lastSelectedList_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

	if (lastSelectedList->SelectedIndex != -1)
	{
		SendAsyncMsg(UiMessageId::SelectWorldItem, "History");
	}
}

private: System::Void playAnimationButton_Click(System::Object^  sender, System::EventArgs^  e) {

	if (animationsComboBox->Text != "")
	{
		SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::ActiveAnimation, animationsComboBox->Text);
	}
}

private: System::Void stopAnimationButton_Click(System::Object^  sender, System::EventArgs^  e) {

	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::ActiveAnimation, "");
}
private: System::Void reloadShadersbutton_Click(System::Object^  sender, System::EventArgs^  e) {
	SendAsyncMsg(UiMessageId::ReloadShaders);
}
private: System::Void reloadGeometryButton_Click(System::Object^  sender, System::EventArgs^  e) {
	SendAsyncMsg(UiMessageId::ReloadGeometry);
}

private: System::Void editParamValueChanged(System::Object^  sender, System::EventArgs^  e) {

	if (!reportSelectionChange)
		return;

	auto ctrl = (System::Windows::Forms::Control^) sender;
	SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::ParamChanged, ctrl);
}

private: System::Void getParamsButton_Click(System::Object^  sender, System::EventArgs^  e) {

	if (getParamsButton->Text == "Get")
	{
		SelectionInfoChange change;
		EditableParamsResponse resp;
		change.change = SelectionInfoChange::Id::GetParams;
		change.data = &resp;
		SendMsg(UiMessageId::SelectionInfoChanged, &change);
		
		initEditableParamsControls(resp.params);
	}
	else if (getParamsButton->Text == "Save")
	{
		SendAsyncMsg(UiMessageId::SelectionInfoChanged, (int)SelectionInfoChange::Id::SaveParams);
	}
}

delegate void SelectFileDelegate(System::String^);

void SetAddItemObj(System::String^ fileName)
{
	addEntityPathTextBox->Text = fileName;
}

void SelectFile()
{
	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
	openFileDialog1->Filter = "Mesh files|*.mesh";

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		SelectFileDelegate^ d = gcnew SelectFileDelegate(this, &EditorForm::SetAddItemObj);
		Invoke(d, openFileDialog1->FileName);
	}
}

private: System::Void addEntityPathSearch_Click(System::Object^  sender, System::EventArgs^  e) {

	System::Threading::Thread^ newThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &EditorForm::SelectFile));
	newThread->SetApartmentState(System::Threading::ApartmentState::STA);
	newThread->Start();
}
private: System::Void addEntityPath_TextChanged(System::Object^  sender, System::EventArgs^  e) {

	SendAsyncMsg(UiMessageId::AddItemMode, (int)ItemType::Entity);
}

private: System::Void addItemTypeComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

	addEntityGroupBox->Hide();

	if (addItemTypeComboBox->Text == "Entity")
		addEntityGroupBox->Show();
}
};
}
