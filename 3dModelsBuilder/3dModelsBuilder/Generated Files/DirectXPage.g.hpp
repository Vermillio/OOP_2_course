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

#include "DirectXPage.xaml.h"

void ::_3dModelsBuilder::DirectXPage::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///DirectXPage.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::_3dModelsBuilder::DirectXPage::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
    case 1:
        {
            this->swapChainPanel = safe_cast<::Windows::UI::Xaml::Controls::SwapChainPanel^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::SwapChainPanel^>(this->swapChainPanel))->RightTapped += ref new ::Windows::UI::Xaml::Input::RightTappedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^))&DirectXPage::swapChainPanel_RightTapped);
            (safe_cast<::Windows::UI::Xaml::Controls::SwapChainPanel^>(this->swapChainPanel))->ManipulationStarted += ref new ::Windows::UI::Xaml::Input::ManipulationStartedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Input::ManipulationStartedRoutedEventArgs^))&DirectXPage::swapChainPanel_ManipulationStarted);
            (safe_cast<::Windows::UI::Xaml::Controls::SwapChainPanel^>(this->swapChainPanel))->ManipulationCompleted += ref new ::Windows::UI::Xaml::Input::ManipulationCompletedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Input::ManipulationCompletedRoutedEventArgs^))&DirectXPage::swapChainPanel_ManipulationCompleted);
            (safe_cast<::Windows::UI::Xaml::Controls::SwapChainPanel^>(this->swapChainPanel))->ManipulationDelta += ref new ::Windows::UI::Xaml::Input::ManipulationDeltaEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^))&DirectXPage::swapChainPanel_ManipulationDelta);
        }
        break;
    case 2:
        {
            this->FileName = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 3:
        {
            this->FileTree = safe_cast<::Windows::UI::Xaml::Controls::ListView^>(__target);
        }
        break;
    case 4:
        {
            this->AddModelButton = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->AddModelButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::AddModelButton_Click);
        }
        break;
    case 5:
        {
            this->RemoveModelButton = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->RemoveModelButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::RemoveModelButton_Click);
        }
        break;
    case 6:
        {
            this->ResetModelButton = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->ResetModelButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::ResetModelButton_Click);
        }
        break;
    case 7:
        {
            this->ResetCameraButton = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->ResetCameraButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::ResetCameraButton_Click);
        }
        break;
    case 8:
        {
            this->DrawingTypeSwitch = safe_cast<::Windows::UI::Xaml::Controls::ToggleSwitch^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::ToggleSwitch^>(this->DrawingTypeSwitch))->Toggled += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::DrawingTypeSwitch_Toggled);
        }
        break;
    case 9:
        {
            this->AddModelOptions = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
        }
        break;
    case 10:
        {
            this->Options = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
        }
        break;
    case 11:
        {
            this->BlueSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->BlueSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&DirectXPage::BlueSlider_ValueChanged);
        }
        break;
    case 12:
        {
            this->BlueText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 13:
        {
            this->GreenSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->GreenSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&DirectXPage::GreenSlider_ValueChanged);
        }
        break;
    case 14:
        {
            this->GreenText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 15:
        {
            this->RedSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->RedSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&DirectXPage::RedSlider_ValueChanged);
        }
        break;
    case 16:
        {
            this->RedText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 17:
        {
            this->ScaleSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->ScaleSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&DirectXPage::ScaleSlider_ValueChanged);
        }
        break;
    case 18:
        {
            this->ScaleText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 19:
        {
            this->ZRotateSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->ZRotateSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&DirectXPage::ZRotateSlider_ValueChanged);
        }
        break;
    case 20:
        {
            this->ZRotateText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 21:
        {
            this->YRotateSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->YRotateSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&DirectXPage::YRotateSlider_ValueChanged);
        }
        break;
    case 22:
        {
            this->YRotateText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 23:
        {
            this->XRotateSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->XRotateSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&DirectXPage::XRotateSlider_ValueChanged);
        }
        break;
    case 24:
        {
            this->XRotateText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 25:
        {
            this->ZMoveSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->ZMoveSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&DirectXPage::ZMoveSlider_ValueChanged);
        }
        break;
    case 26:
        {
            this->ZMoveText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 27:
        {
            this->YMoveSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->YMoveSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&DirectXPage::YMoveSlider_ValueChanged);
        }
        break;
    case 28:
        {
            this->YMoveText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 29:
        {
            this->XMoveSlider = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Slider^>(this->XMoveSlider))->ValueChanged += ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&DirectXPage::XMoveSlider_ValueChanged);
        }
        break;
    case 30:
        {
            this->XMoveText = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
        }
        break;
    case 31:
        {
            this->ModelTypeSwitch = safe_cast<::Windows::UI::Xaml::Controls::ToggleSwitch^>(__target);
        }
        break;
    case 32:
        {
            this->OkButton = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->OkButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::OkButton_Click);
        }
        break;
    case 33:
        {
            this->CancelButton = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->CancelButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::CancelButton_Click);
        }
        break;
    case 34:
        {
            this->CancelButtonIcon = safe_cast<::Windows::UI::Xaml::Controls::FontIcon^>(__target);
        }
        break;
    case 35:
        {
            this->OkButtonIcon = safe_cast<::Windows::UI::Xaml::Controls::FontIcon^>(__target);
        }
        break;
    case 36:
        {
            this->bottomAppBar = safe_cast<::Windows::UI::Xaml::Controls::AppBar^>(__target);
        }
        break;
    case 37:
        {
            this->ProjButton = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
        }
        break;
    case 38:
        {
            this->IntersecButton = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(this->IntersecButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::IntersecButton_Click);
        }
        break;
    case 39:
        {
            this->IntersecIcon = safe_cast<::Windows::UI::Xaml::Controls::FontIcon^>(__target);
        }
        break;
    case 40:
        {
            this->ItemX = safe_cast<::Windows::UI::Xaml::Controls::MenuFlyoutItem^>(__target);
        }
        break;
    case 41:
        {
            this->ItemY = safe_cast<::Windows::UI::Xaml::Controls::MenuFlyoutItem^>(__target);
        }
        break;
    case 42:
        {
            this->ItemZ = safe_cast<::Windows::UI::Xaml::Controls::MenuFlyoutItem^>(__target);
        }
        break;
    case 43:
        {
            ::Windows::UI::Xaml::Controls::CheckBox^ element43 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(element43))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::CheckboxXY_Click);
        }
        break;
    case 44:
        {
            ::Windows::UI::Xaml::Controls::CheckBox^ element44 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(element44))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::CheckboxZX_Click);
        }
        break;
    case 45:
        {
            ::Windows::UI::Xaml::Controls::CheckBox^ element45 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(element45))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::_3dModelsBuilder::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::CheckboxZY_Click);
        }
        break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::_3dModelsBuilder::DirectXPage::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}

#pragma warning(pop)


