#pragma once
#include <fstream>

namespace CppWinForm1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for LogsList
	/// </summary>
	public ref class LogsList : public System::Windows::Forms::Form
	{
	public:
		LogsList(void)
		{
			InitializeComponent();
			
			std::ifstream logFile("Ogre.log");

			if (logFile.good())
			{
				std::string line;
				while (std::getline(logFile, line))
				{
					if (line.find("CG ERROR") != std::string::npos)
					{
						std::string line2;
						std::getline(logFile, line2);
						line += "\n\n" + line2;

						auto cols = gcnew cli::array< System::String^  >(3) { "CG", "", gcnew String(line.data()) };
						dataGridView1->Rows->Add(cols);
						
						for (int i = 0; i < dataGridView1->Rows[dataGridView1->Rows->Count - 1]->Cells->Count; i++)
							dataGridView1->Rows[dataGridView1->Rows->Count - 1]->Cells[i]->Style->BackColor = System::Drawing::Color::LightSalmon;
					}
					else if (line.find("EXCEPTION") != std::string::npos)
					{
						std::string line2;
						std::getline(logFile, line2);
						line += "\n" + line2;

						std::string place;
						if (line2.find(".material") != std::string::npos)
						{
							auto endid = line2.find(".material");

							size_t startid = line2.rfind(" ", endid);

							if (startid == std::string::npos)
								startid = 0;

							place = line2.substr(startid, endid - startid + 2);
						}
						else if (line2.find(".program") != std::string::npos)
						{
							auto endid = line2.find(".program");

							size_t startid = line2.rfind(" ", endid);

							if (startid == std::string::npos)
								startid = 0;

							place = line2.substr(startid, endid - startid + 2);
						}

						auto cols = gcnew cli::array< System::String^  >(3) { "EXC", gcnew String(place.data()), gcnew String(line.data()) };
						dataGridView1->Rows->Add(cols);
					}
				}

				dataGridView1->Sort(dataGridView1->Columns[0], System::ComponentModel::ListSortDirection::Ascending);
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~LogsList()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Type;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Place;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Text;









	protected:



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
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Type = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Place = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Text = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AutoSizeRowsMode = System::Windows::Forms::DataGridViewAutoSizeRowsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->ColumnHeadersVisible = false;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {
				this->Type, this->Place,
					this->Text
			});
			this->dataGridView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dataGridView1->Location = System::Drawing::Point(0, 0);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 24;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(938, 554);
			this->dataGridView1->TabIndex = 0;
			// 
			// Type
			// 
			this->Type->HeaderText = L"Type";
			this->Type->Name = L"Type";
			this->Type->ReadOnly = true;
			this->Type->Width = 50;
			// 
			// Place
			// 
			this->Place->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->Place->HeaderText = L"Place";
			this->Place->Name = L"Place";
			this->Place->ReadOnly = true;
			this->Place->Width = 5;
			// 
			// Text
			// 
			this->Text->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->Text->DefaultCellStyle = dataGridViewCellStyle1;
			this->Text->HeaderText = L"Text";
			this->Text->Name = L"Text";
			this->Text->ReadOnly = true;
			// 
			// LogsList
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(938, 554);
			this->Controls->Add(this->dataGridView1);
			this->Name = L"LogsList";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
