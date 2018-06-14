//
// MainPage.xaml.cpp
// Implementation of the MainPage class
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <ppltasks.h>
#include <fstream>
//#include <msclr\marshal.h>

using namespace _3dModelsBuilder;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage::Pickers;
using namespace Windows::UI::Popups;
//using namespace System::IO::FileStream;
//using namespace msclr::interop;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
	myFrame->Navigate(HomePage::typeid);
}

void _3dModelsBuilder::MainPage::HamburgerButton_Click()
{
	mySplitView->IsPaneOpen = !mySplitView->IsPaneOpen;
}

void _3dModelsBuilder::MainPage::ListBox_SelectionChanged()
{
	switch (menu->SelectedIndex) {
	case 0:
		//
		TitleTextBlock->Text = "Home";
		myFrame->Navigate(HomePage::typeid);
		commandBarMakeInvisible();
		break;
	case 1:
		//
		TitleTextBlock->Text = "Editor";
		if (isFileSet || isFileNew) {
			myFrame->Navigate(DirectXPage::typeid);
		}
		else
			myFrame->Navigate(NoFilePage::typeid);
		commandBarMakeVisible();
		break;
	case 2:
		//
		TitleTextBlock->Text = "Settings";
		myFrame->Navigate(SettingsPage::typeid);
		commandBarMakeInvisible();
		break;
	default:
		break;
	}
}

void _3dModelsBuilder::MainPage::commandBarMakeVisible() {
	newButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
	fileButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
	saveButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
	saveAsButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
	closeButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void _3dModelsBuilder::MainPage::CommandInvokedHandler(Windows::UI::Popups::IUICommand^ command)
{
	if (command->Label == "Yes") {
		SaveButtonClick();
		closeFile();
	}
	else if (command->Label == "No") {
		closeFile();
	}
}

void _3dModelsBuilder::MainPage::updateCfgFile(const wchar_t * filename)
{
	std::ofstream f(configFile, std::ios::out | std::ios::trunc);
	f << filename;
	f.close();
}

void _3dModelsBuilder::MainPage::deleteCfgFile()
{
	remove(configFile.c_str());
}

void _3dModelsBuilder::MainPage::closeFile()
{

}

void _3dModelsBuilder::MainPage::saveFile()
{
	std::ifstream f(configFile);
	std::string currentFilename;
	if (f.eof())
		return;
	f >> currentFilename;
}

void _3dModelsBuilder::MainPage::commandBarMakeInvisible()
{
	newButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	fileButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	saveButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	saveAsButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	closeButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}


void _3dModelsBuilder::MainPage::NewButtonClick()
{
	CloseButtonClick();
	isFileNew = true;
	updateCfgFile(defaultFilename);
	myFrame->Navigate(DirectXPage::typeid);
}

void _3dModelsBuilder::MainPage::FileButtonClick()
{
	CloseButtonClick();
	FileOpenPicker^ fileOpenPicker = ref new FileOpenPicker();
	fileOpenPicker->ViewMode = PickerViewMode::Thumbnail;
	fileOpenPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;
	fileOpenPicker->CommitButtonText = "Load";
	std::wstring wc(fileExtension.begin(), fileExtension.end());
	String^ ext= ref new String(wc.c_str());
	Array<String^>^ extensions=ref new Array<String^>(&ext, 1);
	fileOpenPicker->FileTypeFilter->ReplaceAll(extensions);
	concurrency::create_task(fileOpenPicker->PickSingleFileAsync()).then([this](Windows::Storage::StorageFile^ file)
	{
		if (file)
		{
			updateCfgFile(file->Path->Data());
			isFileSet = true;
		}
	});
	myFrame->Navigate(DirectXPage::typeid);
}

void _3dModelsBuilder::MainPage::SaveButtonClick()
{
	if (isFileNew) {
		SaveAsButtonClick();
		return;
	}
	saveFile();
	isFileSaved = true;
}

void _3dModelsBuilder::MainPage::SaveAsButtonClick()
{
	if (!isFileSet)
		return;

	FileOpenPicker^ fileOpenPicker = ref new FileOpenPicker();
	fileOpenPicker->ViewMode = PickerViewMode::Thumbnail;
	fileOpenPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;
	fileOpenPicker->CommitButtonText = "Load";
	fileOpenPicker->FileTypeFilter->Append(".dds");
	concurrency::create_task(fileOpenPicker->PickSingleFileAsync()).then([this](Windows::Storage::StorageFile^ file)
	{
		if (file)
		{
			//save(file);
			isFileSaved = true;
		}
	});
}

void _3dModelsBuilder::MainPage::CloseButtonClick()
{
	if (!isFileSet)
		return;
	if (!isFileSaved) {
		MessageDialog^ msg = ref new MessageDialog("File has changed. Save?");
		msg->Title = "Warning";

		// Add commands and set their callbacks.
		UICommand^ continueCommand = ref new UICommand("Yes", ref new UICommandInvokedHandler(this, &MainPage::CommandInvokedHandler));
		UICommand^ upgradeCommand = ref new UICommand("No", ref new UICommandInvokedHandler(this, &MainPage::CommandInvokedHandler));
		UICommand^ cancelCommand = ref new UICommand("Cancel", ref new UICommandInvokedHandler(this, &MainPage::CommandInvokedHandler));
		return;
	}
	//close document
	isFileSet = false;
}