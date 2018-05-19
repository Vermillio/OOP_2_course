//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"
#include <fstream>
#include <DirectXMath.h>

using namespace _3dModelsBuilder;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;


double M_PI = 3.14159265358979323846;

DirectXPage::DirectXPage():
	m_windowVisible(true),
	m_coreInput(nullptr)
{
	InitializeComponent();

	// Register event handlers for page lifecycle.
	CoreWindow^ window = Window::Current->CoreWindow;

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &DirectXPage::OnVisibilityChanged);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDisplayContentsInvalidated);

	swapChainPanel->CompositionScaleChanged += 
		ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &DirectXPage::OnCompositionScaleChanged);

	swapChainPanel->SizeChanged +=
		ref new SizeChangedEventHandler(this, &DirectXPage::OnSwapChainPanelSizeChanged);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	m_deviceResources = std::make_shared<DX::DeviceResources>();
	m_deviceResources->SetSwapChainPanel(swapChainPanel);

	// Register our SwapChainPanel to get independent input pointer events
	auto workItemHandler = ref new WorkItemHandler([this] (IAsyncAction ^)
	{
		// The CoreIndependentInputSource will raise pointer events for the specified device types on whichever thread it's created on.
		m_coreInput = swapChainPanel->CreateCoreIndependentInputSource(
			Windows::UI::Core::CoreInputDeviceTypes::Mouse |
			Windows::UI::Core::CoreInputDeviceTypes::Touch |
			Windows::UI::Core::CoreInputDeviceTypes::Pen
			);

		// Register for pointer events, which will be raised on the background thread.
		m_coreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerPressed);
		m_coreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerMoved);
		m_coreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerReleased);

		// Begin processing input messages as they're delivered.
		m_coreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
	});

	// Run task on a dedicated high priority background thread.
	m_inputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	m_main = std::unique_ptr<_3dModelsBuilderMain>(new _3dModelsBuilderMain(m_deviceResources));

	if (!readConfigFile()) {

	}
	m_main->StartRenderLoop();
}

DirectXPage::~DirectXPage()
{
	// Stop rendering and processing events on destruction.
	m_main->StopRenderLoop();
	m_coreInput->Dispatcher->StopProcessEvents();
}


// Saves the current state of the app for suspend and terminate events.
void DirectXPage::SaveInternalState(IPropertySet^ state)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->Trim();

	// Stop rendering when the app is suspended.
	m_main->StopRenderLoop();

	// Put code to save app state here.
}

// Loads the current state of the app for resume events.
void DirectXPage::LoadInternalState(IPropertySet^ state)
{
	// Put code to load app state here.

	// Start rendering when the app is resumed.
	m_main->StartRenderLoop();
}

bool _3dModelsBuilder::DirectXPage::readConfigFile()
{
	std::ifstream f(configFile);
	std::string currentFilename;
	if (!f.eof())
		f >> currentFilename;
	f.close();

	std::wstring ws(currentFilename.begin(), currentFilename.end());
	FileName->Text = ref new String(ws.c_str());
	return parseModelFile(currentFilename);
}

bool _3dModelsBuilder::DirectXPage::parseModelFile(std::string filename)
{
	return m_main->parseModelFile(filename);
}

// Window event handlers.

void DirectXPage::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
	if (m_windowVisible)
	{
		m_main->StartRenderLoop();
	}
	else
	{
		m_main->StopRenderLoop();
	}
}

// DisplayInformation event handlers.

void DirectXPage::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->ValidateDevice();
}

// Called when the app bar button is clicked.
void DirectXPage::AppBarButton_Click(Object^ sender, RoutedEventArgs^ e)
{
	// Use the app bar if it is appropriate for your app. Design the app bar, 
	// then fill in event handlers (like this one).
}

void DirectXPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e)
{
	m_main->StartTracking();
	if (e->CurrentPoint->Properties->IsLeftButtonPressed) {
		prevPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
		RayCasting(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	}
}

void DirectXPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e)
{
	// Update the pointer tracking code.
	if (m_main->IsTracking())
	{
		m_main->StartPointerMove();
		float2 curPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
		auto k=e->CurrentPoint->Properties;
		m_main->TrackingUpdate(prevPointerPos, curPointerPos, e->CurrentPoint->Properties);
		prevPointerPos = curPointerPos;
	}
}

void DirectXPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e)
{
	if (m_main->IsTracking()) {
		m_main->StopTracking();
	// Stop tracking pointer movement when the pointer is released.
		if (m_main->IsPointerMove())
			m_main->StopPointerMove();
	}
}

void DirectXPage::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetLogicalSize(e->NewSize);
	m_main->CreateWindowSizeDependentResources();
}


void _3dModelsBuilder::DirectXPage::XMoveSlider_ValueChanged(Platform::Object ^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^ e)
{
//	float2 curPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	if (slidersTracking)
		m_main->moveX(XMoveSlider->Value);
//	prevPointerPos = curPointerPos;
}

void _3dModelsBuilder::DirectXPage::YMoveSlider_ValueChanged(Platform::Object ^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^ e)
{
//	float2 curPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	if (slidersTracking)
		m_main->moveY(YMoveSlider->Value);
//	prevPointerPos = curPointerPos;
}

void _3dModelsBuilder::DirectXPage::ZMoveSlider_ValueChanged(Platform::Object ^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^ e)
{
//	float2 curPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	if (slidersTracking)
		m_main->moveZ(ZMoveSlider->Value);
//	prevPointerPos = curPointerPos;
}
void _3dModelsBuilder::DirectXPage::XRotateSlider_ValueChanged(Platform::Object ^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^ e)
{
//	float2 curPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	if (slidersTracking)
		m_main->rotateX(XRotateSlider->Value/180*DirectX::XM_PI);
//	prevPointerPos = curPointerPos;
}
void _3dModelsBuilder::DirectXPage::YRotateSlider_ValueChanged(Platform::Object ^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^ e)
{
//	float2 curPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	if (slidersTracking)
		m_main->rotateY(YRotateSlider->Value/180*DirectX::XM_PI);
//	prevPointerPos = curPointerPos;
}
void _3dModelsBuilder::DirectXPage::ZRotateSlider_ValueChanged(Platform::Object ^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^ e)
{
//	float2 curPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	if (slidersTracking)
		m_main->rotateZ(ZRotateSlider->Value/180*DirectX::XM_PI);
//	prevPointerPos = curPointerPos;
}
void _3dModelsBuilder::DirectXPage::ScaleSlider_ValueChanged(Platform::Object ^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^ e)
{
//	float2 curPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	if (slidersTracking)
		m_main->scale(ScaleSlider->Value);
//	prevPointerPos = curPointerPos;
}
void _3dModelsBuilder::DirectXPage::RedSlider_ValueChanged(Platform::Object ^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^ e)
{
//	float2 curPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	if (slidersTracking)
		m_main->setRed(RedSlider->Value/RedSlider->Maximum);
//	prevPointerPos = curPointerPos;
}
void _3dModelsBuilder::DirectXPage::GreenSlider_ValueChanged(Platform::Object ^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^ e)
{
//	float2 curPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	if (slidersTracking)
		m_main->setGreen(GreenSlider->Value / GreenSlider->Maximum);
//	prevPointerPos = curPointerPos;
}
void _3dModelsBuilder::DirectXPage::BlueSlider_ValueChanged(Platform::Object ^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^ e)
{
//	float2 curPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	if (slidersTracking)
		m_main->setBlue(BlueSlider->Value / BlueSlider->Maximum);
//	prevPointerPos = curPointerPos;
}

void _3dModelsBuilder::DirectXPage::AddModelButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	showModelOptions();
	hideRemoveButton();
	hideResetButton();
	slidersTracking = false;

	//show model configuration dialog
}

void _3dModelsBuilder::DirectXPage::OkButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	float3 startPoint;
	startPoint.x=float(XMoveSlider->Value);
	startPoint.y = float(YMoveSlider->Value);
	startPoint.z = float(ZMoveSlider->Value);

	float sideLen = float(ScaleSlider->Value);
	float3 rotation;
	rotation.x = DirectX::XMConvertToRadians(XRotateSlider->Value);
	rotation.y = DirectX::XMConvertToRadians(YRotateSlider->Value);
	rotation.z = DirectX::XMConvertToRadians(ZRotateSlider->Value);

	float3 color;
	color.x = RedSlider->Value/RedSlider->Maximum;
	color.y = GreenSlider->Value / RedSlider->Maximum;
	color.z = BlueSlider->Value / RedSlider->Maximum;

	if (ModelTypeSwitch->IsOn)
		addTetrahedron(defaultTetrahedronName+std::to_string(modelTitles.size()), startPoint, sideLen, rotation, color);
	else 
		addCube(defaultTetrahedronName + std::to_string(modelTitles.size()), startPoint, sideLen, rotation, color);
	
	hideModelOptions();
}

void _3dModelsBuilder::DirectXPage::showModelOptions()
{
	AddModelButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	AddModelOptions->Visibility = Windows::UI::Xaml::Visibility::Visible;
	Options->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void _3dModelsBuilder::DirectXPage::hideModelOptions()
{
	AddModelButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
	AddModelOptions->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	Options->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void _3dModelsBuilder::DirectXPage::hideRemoveButton()
{
	RemoveModelButton->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
	{
		RemoveModelButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}));
}

void _3dModelsBuilder::DirectXPage::showRemoveButton()
{
	RemoveModelButton->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
	{
		RemoveModelButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}));
}

void _3dModelsBuilder::DirectXPage::hideResetButton()
{
	ResetModelButton->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
	{
		ResetModelButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}));
}

void _3dModelsBuilder::DirectXPage::showResetButton()
{
	ResetModelButton->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
	{
		ResetModelButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}));
}

void _3dModelsBuilder::DirectXPage::hideResetCameraButton()
{
	ResetCameraButton->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
	{
		ResetCameraButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}));
}

void _3dModelsBuilder::DirectXPage::showResetCameraButton()
{
	ResetCameraButton->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
	{
		ResetCameraButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}));
}


void _3dModelsBuilder::DirectXPage::showSliders()
{
	Options->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
	{
		Options->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}));
}

void _3dModelsBuilder::DirectXPage::hideSliders()
{
	Options->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
	{
		Options->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}));
}

void _3dModelsBuilder::DirectXPage::resetSliders()
{
	XMoveSlider->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
			 {
		XMoveSlider->Value = 0;
		}));
	YMoveSlider->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
			 {
		YMoveSlider->Value = 0;
		}));
	ZMoveSlider->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
			 {
		ZMoveSlider->Value = 0;
		}));
	XRotateSlider->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
			 {
		XRotateSlider->Value = 0;
		}));
	YRotateSlider->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
			 {
		YRotateSlider->Value = 0;
		}));
	ZRotateSlider->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
			 {
		ZRotateSlider->Value = 0;
		}));
	ScaleSlider->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this]
			 {
		ScaleSlider->Value = 1;
		}));
}

void _3dModelsBuilder::DirectXPage::RayCasting(float x, float y)
{
	std::vector<UINT> titlesSelected = m_main->rayCasting(x, y);
	if (titlesSelected.size() != 0) {
		showSliders();
		showResetButton();
		showRemoveButton();
		slidersTracking = true;
	}
	else {
		hideSliders();
		hideResetButton();
		hideRemoveButton();
		slidersTracking = false;
	}
}

void _3dModelsBuilder::DirectXPage::addCube(const std::string title, float3 startPos, float sideLen, float3 rotation, float3 color)
{
	modelTitles.push_back(title);
	m_main->addCube(modelTitles.size()-1, startPos, sideLen, rotation, color);
	//redrawModelsList();
}

void _3dModelsBuilder::DirectXPage::addTetrahedron(const std::string title, float3 startPos, float sideLen, float3 rotation, float3 color)
{
	modelTitles.push_back(title);
	m_main->addTetrahedron(modelTitles.size()-1, startPos, sideLen, rotation, color);
	//redrawModelsList();
}

void _3dModelsBuilder::DirectXPage::CancelButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	hideModelOptions();
	slidersTracking = true;
}

void _3dModelsBuilder::DirectXPage::swapChainPanel_RightTapped(Platform::Object ^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs ^ e)
{
	swapChainPanel->ContextFlyout->ShowAttachedFlyout((SwapChainPanel^)sender);
		//ShowAttachedFlyout(sender);
}


void _3dModelsBuilder::DirectXPage::RemoveModelButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	std::vector<UINT> titlesRemoved=m_main->RemoveSelected();
	std::sort(titlesRemoved.rbegin(), titlesRemoved.rend());
	for (size_t i = 0; i < titlesRemoved.size(); ++i) {
		modelTitles.erase(modelTitles.begin() + titlesRemoved[i]);
	}
	hideSliders();
}


void _3dModelsBuilder::DirectXPage::ResetModelButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_main->ResetSelected();
	resetSliders();
}


void _3dModelsBuilder::DirectXPage::DrawingTypeSwitch_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (DrawingTypeSwitch->IsOn) {
		m_main->setWireframeMode();
	}
	else {
		m_main->setSolidMode();
	}
}


void _3dModelsBuilder::DirectXPage::swapChainPanel_ManipulationStarted(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationStartedRoutedEventArgs^ e)
{
	m_main->StartTracking();
//	prevPointerPos = float2(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);


}


void _3dModelsBuilder::DirectXPage::swapChainPanel_ManipulationCompleted(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationCompletedRoutedEventArgs^ e)
{

}


void _3dModelsBuilder::DirectXPage::swapChainPanel_ManipulationDelta(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e)
{

}


void _3dModelsBuilder::DirectXPage::ResetCameraButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_main->ResetWorldModel();
}

void _3dModelsBuilder::DirectXPage::CheckboxZY_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_main->switchRenderProjZY();
}


void _3dModelsBuilder::DirectXPage::CheckboxZX_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_main->switchRenderProjZX();
}


void _3dModelsBuilder::DirectXPage::CheckboxXY_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_main->switchRenderProjXY();
}

void _3dModelsBuilder::DirectXPage::IntersecButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (intersectionsRendering) {
		intersectionsRendering = false;
		m_main->StopIntersectionsRendering();
		IntersecIcon->Glyph = "K";
	}
	else {
		intersectionsRendering = true;
		m_main->StartIntersectionsRendering();
		IntersecIcon->Glyph = "B";
	}
}
