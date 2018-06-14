//
// SettingsPage.xaml.cpp
// Implementation of the SettingsPage class
//

#include "pch.h"
#include "SettingsPage.xaml.h"
#include <fstream>

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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

SettingsPage::SettingsPage()
{
	InitializeComponent();
}


void _3dModelsBuilder::SettingsPage::themeToggleSwitch_IsEnabledChanged(Platform::Object^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e)
{
	std::vector<std::string> tmp;
	std::string s;
	std::ifstream fin(settingsFile, std::ios::in);
	if (!fin.eof())
		fin >> s;
	while (!fin.eof()) {
		fin >> s;
		tmp.push_back(s);
	}
	fin.close();
	std::ofstream fout(settingsFile, std::ios::trunc | std::ios::out);
	if (themeToggleSwitch->IsEnabled)
		fout << "light";
	else
		fout << "dark";
	for (size_t i = 0; i < tmp.size(); ++i)
		fout << tmp[i];
}
