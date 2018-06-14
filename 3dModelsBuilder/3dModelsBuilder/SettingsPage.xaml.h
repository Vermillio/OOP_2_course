//
// SettingsPage.xaml.h
// Declaration of the SettingsPage class
//

#pragma once

#include <string>
#include <vector>
#include "SettingsPage.g.h"

namespace _3dModelsBuilder
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SettingsPage sealed
	{
		std::string settingsFile = "settings.cfg";
		void ThemeToggleSwitchClick();
	public:
		SettingsPage();
	private:
		void themeToggleSwitch_IsEnabledChanged(Platform::Object^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
	};
}
