﻿#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------


namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class SwapChainPanel;
                ref class TextBlock;
                ref class ListView;
                ref class Button;
                ref class StackPanel;
                ref class Slider;
                ref class ToggleSwitch;
                ref class AppBar;
            }
        }
    }
}

namespace _3dModelsBuilder
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class DirectXPage : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector,
        public ::Windows::UI::Xaml::Markup::IComponentConnector2
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
        virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
    
        private: ::Windows::UI::Xaml::Controls::SwapChainPanel^ swapChainPanel;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ FileName;
        private: ::Windows::UI::Xaml::Controls::ListView^ FileTree;
        private: ::Windows::UI::Xaml::Controls::Button^ AddModelButton;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ AddModelOptions;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ Options;
        private: ::Windows::UI::Xaml::Controls::Slider^ BlueSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ BlueText;
        private: ::Windows::UI::Xaml::Controls::Slider^ GreenSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ GreenText;
        private: ::Windows::UI::Xaml::Controls::Slider^ RedSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ RedText;
        private: ::Windows::UI::Xaml::Controls::Slider^ ScaleSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ ScaleText;
        private: ::Windows::UI::Xaml::Controls::Slider^ ZRotateSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ ZRotateText;
        private: ::Windows::UI::Xaml::Controls::Slider^ YRotateSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ YRotateText;
        private: ::Windows::UI::Xaml::Controls::Slider^ XRotateSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ XRotateText;
        private: ::Windows::UI::Xaml::Controls::Slider^ ZMoveSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ ZMoveText;
        private: ::Windows::UI::Xaml::Controls::Slider^ YMoveSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ YMoveText;
        private: ::Windows::UI::Xaml::Controls::Slider^ XMoveSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ XMoveText;
        private: ::Windows::UI::Xaml::Controls::ToggleSwitch^ ModelTypeSwitch;
        private: ::Windows::UI::Xaml::Controls::Button^ OkButton;
        private: ::Windows::UI::Xaml::Controls::Button^ CancelButton;
        private: ::Windows::UI::Xaml::Controls::AppBar^ bottomAppBar;
    };
}
