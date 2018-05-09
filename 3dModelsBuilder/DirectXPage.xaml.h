//
// DirectXPage.xaml.h
// Declaration of the DirectXPage class.
//

#pragma once

#include "DirectXPage.g.h"

#include "Common\DeviceResources.h"
#include "_3dModelsBuilderMain.h"

namespace _3dModelsBuilder
{
	using float3 = DirectX::XMFLOAT3;
	using float2 = DirectX::XMFLOAT2;
	using float4 = DirectX::XMFLOAT4;
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;


	/// <summary>
	/// A page that hosts a DirectX SwapChainPanel.
	/// </summary>
	public ref class DirectXPage sealed
	{
	public:
		DirectXPage();
		virtual ~DirectXPage();

		//clicks

		void OnXMoveSliderClick();
		//void OnYMoveSliderClick();
		//void OnZMoveSliderClick();
		//void OnXRotateSliderClick();
		//void OnYRotateSliderClick();
		//void OnZRotateSliderClick();
		//void OnScaleSliderClick();
		//void OnRedSliderClick();
		//void OnGreenSliderClick();
		//void OnBlueSliderClick();


		void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

	private:

		float2 prevPointerPos;


		std::string defaultCubeName="cube";
		std::string defaultTetrahedronName = "tetr";

		std::vector<std::string> modelTitles;

		//add shapes
		void addCube(const std::string title, float3 startPos, float sideLen, float3 rotation, float3 color);
		void addTetrahedron(const std::string title, float3 startPos, float sideLen, float3 rotation, float3 color);

		bool readConfigFile();
		bool parseModelFile(std::string filename);
		std::string configFile = "file.cfg";

		int action = 0;

		// XAML low-level rendering event handler.
		void OnRendering(Platform::Object^ sender, Platform::Object^ args);

		// Window event handlers.
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

		// Other event handlers.
		void AppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
		void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

		// Track our independent input on a background worker thread.
		Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
		Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;

		// Independent input handling functions.
		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);

		// Resources used to render the DirectX content in the XAML page background.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<_3dModelsBuilderMain> m_main; 
		bool m_windowVisible;
		void XMoveSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void AddModelButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OkButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		//show/hide

		void showModelOptions();
		void hideModelOptions();

		void RayCasting(float x, float y);

		void CancelButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void swapChainPanel_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e);
		void MenuFlyoutItem_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuFlyoutItem_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuFlyoutItem_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}

