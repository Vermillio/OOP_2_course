﻿//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BINDING_DEBUG_OUTPUT
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif

#include "MainPage.xaml.h"

void ::_3dModelsBuilder::MainPage::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///MainPage.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::_3dModelsBuilder::MainPage::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
    case 1:
        {
            this->myFrame = safe_cast<::Windows::UI::Xaml::Controls::Frame^>(__target);
        }
        break;
    case 2:
        {
            this->CloseFileDialog = safe_cast<::Windows::UI::Xaml::Controls::ContentDialog^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::ContentDialog^>(this->CloseFileDialog))->PrimaryButtonClick += ref new ::Windows::Foundation::TypedEventHandler<::Windows::UI::Xaml::Controls::ContentDialog^, ::Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^>(this, (void (::_3dModelsBuilder::MainPage::*)
                (::Windows::UI::Xaml::Controls::ContentDialog^, ::Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^))&MainPage::CloseButtonClick);
        }
        break;
    case 3:
        {
            this->SaveFileDialog = safe_cast<::Windows::UI::Xaml::Controls::ContentDialog^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::ContentDialog^>(this->SaveFileDialog))->PrimaryButtonClick += ref new ::Windows::Foundation::TypedEventHandler<::Windows::UI::Xaml::Controls::ContentDialog^, ::Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^>(this, (void (::_3dModelsBuilder::MainPage::*)
                (::Windows::UI::Xaml::Controls::ContentDialog^, ::Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^))&MainPage::SaveButtonClick);
        }
        break;
    case 4:
        {
            this->mySplitView = safe_cast<::Windows::UI::Xaml::Controls::SplitView^>(__target);
        }
        break;
    case 5:
        {
            this->menu = safe_cast<::Windows::UI::Xaml::Controls::ListBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::ListBox^>(this->menu))->SelectionChanged += ref new ::Windows::UI::Xaml::Controls::SelectionChangedEventHandler(this, (void (::_3dModelsBuilder::MainPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::SelectionChangedEventArgs^))&MainPage::ListBox_SelectionChanged);
        }
        break;
    case 6:
        {
            this->home = safe_cast<::Windows::UI::Xaml::Controls::ListBoxItem^>(__target);
        }
        break;
    case 7:
        {
            this->editor = safe_cast<::Windows::UI::Xaml::Controls::ListBoxItem^>(__target);
        }
        break;
    case 8:
        {
            this->settings = safe_cast<::Windows::UI::Xaml::Controls::ListBoxItem^>(__target);
        }
        break;
    case 9:
        {
            this->HamburgerButton = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->HamburgerButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::MainPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::HamburgerButton_Click);
        }
        break;
    case 10:
        {
            this->TitleTextBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 11:
        {
            this->newButton = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(this->newButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::MainPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::NewButtonClick);
        }
        break;
    case 12:
        {
            this->fileButton = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(this->fileButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::MainPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::FileButtonClick);
        }
        break;
    case 13:
        {
            this->saveButton = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(this->saveButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::MainPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::SaveButtonClick);
        }
        break;
    case 14:
        {
            this->saveAsButton = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(this->saveAsButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::MainPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::SaveAsButtonClick);
        }
        break;
    case 15:
        {
            this->closeButton = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(this->closeButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::MainPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::CloseButtonClick);
        }
        break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::_3dModelsBuilder::MainPage::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}

#pragma warning(pop)

