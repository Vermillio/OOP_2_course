﻿#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace _3dModelsBuilder
{
	using float3 = DirectX::XMFLOAT3;
	using float2 = DirectX::XMFLOAT2;
	using float4 = DirectX::XMFLOAT4;

	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;


	class _3dModelsBuilderMain : public DX::IDeviceNotify
	{
	public:
		_3dModelsBuilderMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~_3dModelsBuilderMain();
		void CreateWindowSizeDependentResources();
		void StartTracking() { m_sceneRenderer->StartTracking(); }
		void StartPointerMove() { m_sceneRenderer->StartPointerMove(); }
		void StopPointerMove() { m_sceneRenderer->StopPointerMove(); }
		bool IsPointerMove() { return m_sceneRenderer->IsPointerMove(); }
		void TrackingUpdate(float2 prevPos, float2 curPos) { curMousePos = curPos; prevMousePos = prevPos; }
		void StopTracking() { m_sceneRenderer->StopTracking(); }
		bool IsTracking() { return m_sceneRenderer->IsTracking(); }
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

		//add shapes
		void addCube(UINT title, float3 startPos, float sideLen, float3 rotation, float3 color);
		void addTetrahedron(UINT title, float3 startPos, float sideLen, float3 rotation, float3 color);

		//ray cast mouse position
		std::vector<UINT> rayCasting(float x, float y);

		//parse
		bool parseModelFile(std::string filename);
	private:
		void ProcessInput();
		void Update();
		bool Render();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		// Track current input pointer position.
		float2 prevMousePos;
		float2 curMousePos;

	};
}