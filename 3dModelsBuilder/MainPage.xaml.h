
#pragma once

#include <string>
#include "MainPage.g.h"
#include "HomePage.xaml.h"
#include "DirectXPage.xaml.h"
#include "SettingsPage.xaml.h"
#include "NoFilePage.xaml.h"


namespace _3dModelsBuilder
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class MainPage sealed
	{
		std::string fileExtension = ".model";
		std::string configFile = "file.cfg";
		wchar_t * defaultFilename = L"new.model";

		//clicks
		void NewButtonClick();
		void FileButtonClick();
		void SaveButtonClick();
		void SaveAsButtonClick();
		void CloseButtonClick();
		void HamburgerButton_Click();
		void ListBox_SelectionChanged();

		//visibility
		void commandBarMakeInvisible();
		void commandBarMakeVisible();

		void CommandInvokedHandler(Windows::UI::Popups::IUICommand^ command);

		void updateCfgFile(const wchar_t * filename);
		void deleteCfgFile();
		void closeFile();
		void saveFile();
		bool isFileSet;
		bool isFileSaved;
		bool isFileNew;
		~MainPage() { deleteCfgFile(); };
	public:
		MainPage();
	};
}