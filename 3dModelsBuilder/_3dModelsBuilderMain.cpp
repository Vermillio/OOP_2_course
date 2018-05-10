#include "pch.h"
#include "_3dModelsBuilderMain.h"
#include "Common\DirectXHelper.h"

using namespace _3dModelsBuilder;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
_3dModelsBuilderMain::_3dModelsBuilderMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));

	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

_3dModelsBuilderMain::~_3dModelsBuilderMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void _3dModelsBuilderMain::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

void _3dModelsBuilderMain::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
	{
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			Update();
			if (Render())
			{
				m_deviceResources->Present();
			}
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void _3dModelsBuilderMain::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

// Updates the application state once per frame.
void _3dModelsBuilderMain::Update() 
{
	ProcessInput();

	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		//m_sceneRenderer->Update(m_timer);
		m_fpsTextRenderer->Update(m_timer);
	});
}

bool _3dModelsBuilder::_3dModelsBuilderMain::parseModelFile(std::string filename)
{
	return m_sceneRenderer->parseModelFile(filename);
}

bool _3dModelsBuilder::_3dModelsBuilderMain::changeStateOrthoProjectionX()
{
	return m_sceneRenderer->changeStateOrthoProjectionX();
}
bool _3dModelsBuilder::_3dModelsBuilderMain::changeStateOrthoProjectionY()
{
	return m_sceneRenderer->changeStateOrthoProjectionY();
}
bool _3dModelsBuilder::_3dModelsBuilderMain::changeStateOrthoProjectionZ()
{
	return m_sceneRenderer->changeStateOrthoProjectionZ();
}

void _3dModelsBuilder::_3dModelsBuilderMain::moveX(float val) {
	m_sceneRenderer->sliderMoveX(val);
};
void _3dModelsBuilder::_3dModelsBuilderMain::moveY(float val) {
	m_sceneRenderer->sliderMoveY(val);
};
void _3dModelsBuilder::_3dModelsBuilderMain::moveZ(float val) {
	m_sceneRenderer->sliderMoveZ(val);
};
void _3dModelsBuilder::_3dModelsBuilderMain::rotateX(float val) {
	m_sceneRenderer->sliderRotateX(val);
};
void _3dModelsBuilder::_3dModelsBuilderMain::rotateY(float val) {
	m_sceneRenderer->sliderRotateY(val);
};
void _3dModelsBuilder::_3dModelsBuilderMain::rotateZ(float val) {
	m_sceneRenderer->sliderRotateZ(val);
};
void _3dModelsBuilder::_3dModelsBuilderMain::scale(float val) {
	m_sceneRenderer->sliderScale(val);
};
void _3dModelsBuilder::_3dModelsBuilderMain::setRed(float val) {
	m_sceneRenderer->sliderRed(val);
};
void _3dModelsBuilder::_3dModelsBuilderMain::setGreen(float val) {
	m_sceneRenderer->sliderGreen(val);
};
void _3dModelsBuilder::_3dModelsBuilderMain::setBlue(float val) {
	m_sceneRenderer->sliderBlue(val);
}
std::vector<UINT> _3dModelsBuilder::_3dModelsBuilderMain::RemoveSelected()
{
	return m_sceneRenderer->RemoveSelected();
}
void _3dModelsBuilder::_3dModelsBuilderMain::ResetSelected()
{
	m_sceneRenderer->ResetSelected();
}
void _3dModelsBuilder::_3dModelsBuilderMain::setWireframeMode()
{
	m_sceneRenderer->setWireframeMode();
}
void _3dModelsBuilder::_3dModelsBuilderMain::setSolidMode()
{
	m_sceneRenderer->setSolidMode();
}
;


// Process all input from the user before updating game state
void _3dModelsBuilderMain::ProcessInput()
{
	// TODO: Add per frame input handling here.
	m_sceneRenderer->TrackingUpdate(prevMousePos, curMousePos);
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool _3dModelsBuilderMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	m_sceneRenderer->Render();
	m_fpsTextRenderer->Render();

	return true;
}

std::vector<UINT> _3dModelsBuilder::_3dModelsBuilderMain::rayCasting(float x, float y)
{
	return m_sceneRenderer->rayCasting(x, y);
}

void _3dModelsBuilder::_3dModelsBuilderMain::addCube(UINT title, float3 startPos, float sideLen, float3 rotation, float3 color)
{
	m_sceneRenderer->addCube(title, startPos, sideLen, rotation, color);
}

void _3dModelsBuilder::_3dModelsBuilderMain::addTetrahedron(UINT title, float3 startPos, float sideLen, float3 rotation, float3 color)
{
	m_sceneRenderer->addTetrahedron(title, startPos, sideLen, rotation, color);
}

// Notifies renderers that device resources need to be released.
void _3dModelsBuilderMain::OnDeviceLost()
{
	m_sceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void _3dModelsBuilderMain::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
