#pragma once

#include <string>
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
	/// Summary for SceneList
	/// </summary>
	public ref class SceneList : public System::Windows::Forms::Form
	{
	public:
		SceneList(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		System::Windows::Forms::TreeView^ getTree()
		{
			return sceneListTree;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SceneList()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  listGroupBox;
	protected:
	private: System::Windows::Forms::TreeView^  sceneListTree;

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
			this->listGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->sceneListTree = (gcnew System::Windows::Forms::TreeView());
			this->listGroupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// listGroupBox
			// 
			this->listGroupBox->Controls->Add(this->sceneListTree);
			this->listGroupBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listGroupBox->Location = System::Drawing::Point(0, 0);
			this->listGroupBox->Margin = System::Windows::Forms::Padding(4);
			this->listGroupBox->Name = L"listGroupBox";
			this->listGroupBox->Padding = System::Windows::Forms::Padding(4);
			this->listGroupBox->Size = System::Drawing::Size(428, 368);
			this->listGroupBox->TabIndex = 19;
			this->listGroupBox->TabStop = false;
			this->listGroupBox->Text = L"List";
			// 
			// sceneListTree
			// 
			this->sceneListTree->BackColor = System::Drawing::Color::Silver;
			this->sceneListTree->Dock = System::Windows::Forms::DockStyle::Fill;
			this->sceneListTree->Location = System::Drawing::Point(4, 19);
			this->sceneListTree->Margin = System::Windows::Forms::Padding(4);
			this->sceneListTree->Name = L"sceneListTree";
			this->sceneListTree->Size = System::Drawing::Size(420, 345);
			this->sceneListTree->TabIndex = 1;
			this->sceneListTree->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &SceneList::sceneListTree_AfterSelect);
			// 
			// SceneList
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlDark;
			this->ClientSize = System::Drawing::Size(428, 368);
			this->Controls->Add(this->listGroupBox);
			this->Name = L"SceneList";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"SceneList";
			this->TopMost = true;
			this->listGroupBox->ResumeLayout(false);
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
		case UiMessageId::SelectWorldItem:
		{
			SelectWorldItemData change;

			if (param->subtype == "Tree")
			{
				System::Windows::Forms::TreeNode^ node = sceneListTree->SelectedNode;
				change.groupName = marshal_as<std::string>(node->Parent->Text);
				change.item.name = marshal_as<std::wstring>(node->Text);
			}

			SendMsg(UiMessageId::SelectWorldItem, &change);
			break;
		}
		default:
			SendMsg(param->id, nullptr);
			break;
		}
	}
	private: System::Void SendAsyncMsg(UiMessageId id, const char* subtype)
	{
		System::Threading::Thread^ newThread = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &SceneList::SendAsyncMsgThread));
		auto param = gcnew AsyncParam();
		param->id = id;
		param->subtype = gcnew System::String(subtype);

		newThread->Start(param);
	}
	private: System::Void SendAsyncMsg(UiMessageId id)
	{
		SendAsyncMsg(id, "");
	}

	private: System::Void sceneListTree_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
		if (!e->Node->IsSelected || e->Node->Level == 0)
			return;

		SendAsyncMsg(UiMessageId::SelectWorldItem, "Tree");
	}
	};
}
