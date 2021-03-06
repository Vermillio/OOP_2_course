﻿#include "pch.h"
#include "Sample3DSceneRenderer.h"
#include "..\Common\DirectXHelper.h"
#include <DirectXCollision.h>
#include <fstream>

using namespace _3dModelsBuilder;
using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_degreesPerSecond(0.2),
	m_indexCount(0),
	m_tracking(false),
	m_pointerMove(false),
	m_deviceResources(deviceResources),
	isVertexBufferSet(false),
	isIndexBufferSet(false),
	isZYprojRendering(false),
    isZXprojRendering(false),
    isXYprojRendering(false),
	drawingMode(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	intersectionsRendering(true)
{
	CreateWindowSizeDependentResources();
	CreateDeviceDependentResources();
	initWorldModel();
}

float2 _3dModelsBuilder::Sample3DSceneRenderer::normalizeCoordinates(float2 mousePoint) {
	Size screenSize = m_deviceResources->GetLogicalSize();
	float x_ = (2.0f * mousePoint.x) / screenSize.Width - 1.0f;
	float y_ = 1.0f - (2.0f * mousePoint.y) / screenSize.Height;
	return float2(x_, y_);
}

bool _3dModelsBuilder::Sample3DSceneRenderer::parseModelFile(std::string filename)
{
	std::string extension = ".model";
	if (filename.size() <= extension.size())
		return false;
	if (filename.substr(filename.size() - extension.size(), extension.size()) != ".model")
		return false;
	std::ifstream f(filename);
	if (!f.good())
		return false;
	std::string c;
	while (!f.eof()) {
		f >> c;
	}
	return true;
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float width = outputSize.Width;
	float height = outputSize.Height;

	//viewports.resize(4);

	//viewports[0].Width = width;
	//viewports[0].Height = height;
	//viewports[0].MinDepth = 0.0f;
	//viewports[0].MaxDepth = 1.0f;
	//viewports[0].TopLeftX = 0;
	//viewports[0].TopLeftY = 0;

//	viewports[1].Width = width / ;
//	viewports[1].Height = height;
//	viewports[1].MinDepth = 0.0f;
//	viewports[1].MaxDepth = 1.0f;
//	viewports[1].TopLeftX = ;
//	viewports[1].TopLeftY = 0;



	float aspectRatio = width / height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
		fovAngleY *= 2.0f;

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
	);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));

	cameraPos = float4(0.0f, 0.7f, 1.5f, 0.0f);

	m_constantBufferData.lightPos = { -1.0f, 1.0f, -1.0f };
	m_constantBufferData.viewPos = { 0.0f, 0.7f, 1.5f };
	m_constantBufferData.lightColor = { 1.0f, 1.0f, 1.0f };


	XMMATRIX worldM = XMMatrixIdentity();
	XMStoreFloat4x4(&world, worldM);


	worldModel.setConstantBuffer(m_constantBufferData);
	for (size_t i = 0; i < models.size(); ++i)
		models[i].setConstantBuffer(m_constantBufferData);
}

//// Called once per frame, rotates the cube and calculates the model and view matrices.
//void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
//{
//	if (!m_tracking)
//	{
//		// Convert degrees to radians, then convert seconds to rotation angle
//		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
//		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
//		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));
//
//	}
//}


void Sample3DSceneRenderer::StartTracking()
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float2 prevPos, float2 curPos, bool LeftButtonPressed, bool RightButtonPressed, int MouseWheelDelta)
{
	if (m_tracking)
	{
		if (MouseWheelDelta != 0) {
			float power = pow(1, (1 + (double)MouseWheelDelta*0.005));
			for (size_t i = 0; i < models.size(); ++i) {
				if (models[i].isSelected())
					models[i].scale(power);
				//			XMMATRIX worldM = XMLoadFloat4x4(&world) * XMMatrixScaling(power, power, power);
				//			XMStoreFloat4x4(&world, worldM);
			}
		}
		float2 prevPosNormalized = normalizeCoordinates(prevPos);
		float2 curPosNormalized = normalizeCoordinates(curPos);

		if (LeftButtonPressed) {
			for (size_t i = 0; i < models.size(); ++i) {
				if (models[i].isSelected())
					models[i].applyAction(prevPosNormalized, curPosNormalized);
			}
		}
		else if (RightButtonPressed) {
//			rotate(float3(, , 0));

			XMMATRIX worldM;
			if (abs(curPosNormalized.y - prevPosNormalized.y) > abs(curPosNormalized.x - prevPosNormalized.x))
				worldM = XMMatrixMultiply(XMMatrixTranspose(XMMatrixRotationX(prevPosNormalized.y - curPosNormalized.y)), XMLoadFloat4x4(&world));
			else
				worldM = XMMatrixMultiply(XMMatrixTranspose(XMMatrixRotationY(curPosNormalized.x - prevPosNormalized.x)), XMLoadFloat4x4(&world));
			XMStoreFloat4x4(&world, worldM);
		}
		if (intersectionsRendering)
			recalcIntersections();
	}

}

void Sample3DSceneRenderer::StopTracking()
{
	m_tracking = false;
}

// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
		return;

	//always on top enable
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_deviceResources->GetD3DDeviceContext()->RSSetState(m_pRasterState.Get());

	render(models, m_deviceResources, context, m_constantBuffer, m_vertexShader, m_pixelShader, m_inputLayout, drawingMode, world);
	
	for (size_t i = 0; i < models.size(); ++i)
		if (models[i].isSelected()) {
			models[i].renderProjections(m_deviceResources, context, m_constantBuffer, m_vertexShader, m_flatPixelShader, m_inputLayout, D3D_PRIMITIVE_TOPOLOGY_LINELIST, world);
		}

	ComPtr<ID3D11DepthStencilState> curState;
	UINT curStateArg;
	context->OMGetDepthStencilState(&curState, &curStateArg);
	context->OMSetDepthStencilState(depthDisabledState.Get(), 0);

	if (intersectionsRendering)
		render(intersections, m_deviceResources, context, m_constantBuffer, m_vertexShader, m_flatPixelShader, m_inputLayout, D3D_PRIMITIVE_TOPOLOGY_LINELIST, world);

	worldModel.renderAxes(m_deviceResources, context, m_constantBuffer, m_vertexShader, m_flatPixelShader, m_inputLayout, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, world);
	
	for (size_t i = 0; i < models.size(); ++i)
		if (models[i].isSelected()) {
			models[i].renderAxes(m_deviceResources, context, m_constantBuffer, m_vertexShader, m_flatPixelShader, m_inputLayout, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, world);
		}

	////always on top disable
	context->OMSetDepthStencilState(curState.Get(), curStateArg);
}

void _3dModelsBuilder::Sample3DSceneRenderer::render( vector<Model>& models, shared_ptr<DX::DeviceResources>& m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer>& m_constantBuffer, ComPtr<ID3D11VertexShader>& m_vertexShader, ComPtr<ID3D11PixelShader>& m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout, D3D_PRIMITIVE_TOPOLOGY drawingMode, const float4x4 & world)
{
	if (models.size()!=0)
		for (size_t i = 0; i < models.size(); ++i) {
			models[i].render(m_deviceResources, context, m_constantBuffer, m_vertexShader, m_pixelShader, m_inputLayout, drawingMode, world);
		}
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");
	auto loadPS2Task = DX::ReadDataAsync(L"FlatPixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
			)
		);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				&fileData[0],
				fileData.size(),
				&m_inputLayout
			)
		);
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_pixelShader
			)
		);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		//constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		//constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		//constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//constantBufferDesc.MiscFlags = 0;
		//constantBufferDesc.StructureByteStride = 0;

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
			)
		);
	});

	auto createPS2Task = loadPS2Task.then([this](const vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_flatPixelShader
			)
		);
	});

	auto createDepthStates = createPS2Task.then([this]() {
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

		depthStencilDesc.DepthEnable = FALSE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = FALSE;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//// Stencil operations if pixel is back-facing
		//depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		//depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		m_deviceResources->GetD3DDevice()->CreateDepthStencilState(&depthStencilDesc, &depthDisabledState);

	});

	// Once the cube is loaded, the object is ready to be rendered.
	createDepthStates.then([this]() {
		CD3D11_RASTERIZER_DESC rasterDesc(D3D11_FILL_SOLID, D3D11_CULL_NONE,
			FALSE /* FrontCounterClockwise */,
			D3D11_DEFAULT_DEPTH_BIAS,
			D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
			D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
			TRUE /* DepthClipEnable */,
			FALSE /* ScissorEnable */,
			TRUE /* MultisampleEnable */,
			FALSE /* AntialiasedLineEnable */);
		m_deviceResources->GetD3DDevice()->CreateRasterizerState(&rasterDesc, &m_pRasterState);
		m_loadingComplete = true;
	});


}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}

Cube::Cube(UINT title, const float3 &startPoint, float sideLen)
{
	_title = title;
	create(startPoint, sideLen);
}

void Cube::create(const float3 &startPoint, float sideLen)
{
	float3 delta = float3(sideLen, sideLen, sideLen);
	float Ax = startPoint.x - delta.x / 2.0f;
	float Bx = startPoint.x + delta.x / 2.0f;
	float Ay = startPoint.y - delta.y / 2.0f;
	float By = startPoint.y + delta.y / 2.0f;
	float Az = startPoint.z - delta.z / 2.0f;
	float Bz = startPoint.z + delta.z / 2.0f;
	
	
	// position color
	vertices = {
		{float3(Ax, Ay, Bz),  float3(0,0,0), float3(0, 0, 1)},
		{float3(Bx, Ay, Bz),  float3(0,0,0), float3(0, 0, 1)},
		{float3(Ax, By, Bz),  float3(0,0,0), float3(0, 0, 1)},
		{float3(Bx, By, Bz),  float3(0,0,0), float3(0, 0, 1)},
		
		{float3(Ax, By, Bz),  float3(0,0,0), float3(0, 1, 0)},
		{float3(Bx, By, Bz),  float3(0,0,0), float3(0, 1, 0)},
		{float3(Ax, By, Az),  float3(0,0,0), float3(0, 1, 0)},
		{float3(Bx, By, Az),  float3(0,0,0), float3(0, 1, 0)},
		
		{float3(Ax, By, Az),  float3(0,0,0), float3(0, 0, -1)},
		{float3(Bx, By, Az),  float3(0,0,0), float3(0, 0, -1)},
		{float3(Ax, Ay, Az),  float3(0,0,0), float3(0, 0, -1)},
		{float3(Bx, Ay, Az),  float3(0,0,0), float3(0, 0, -1)},
		
		{float3(Ax, Ay, Az),  float3(0,0,0), float3(0, -1, 0)},
		{float3(Bx, Ay, Az),  float3(0,0,0), float3(0, -1, 0)},
		{float3(Ax, Ay, Bz),  float3(0,0,0), float3(0, -1, 0)},
		{float3(Bx, Ay, Bz),  float3(0,0,0), float3(0, -1, 0)},
			
		{float3(Bx, Ay, Bz),  float3(0,0,0), float3(1, 0, 0)},
		{float3(Bx, Ay, Az),  float3(0,0,0), float3(1, 0, 0)},
		{float3(Bx, By, Bz),  float3(0,0,0), float3(1, 0, 0)},
		{float3(Bx, By, Az),  float3(0,0,0), float3(1, 0, 0)},
		
		{float3(Ax, Ay, Az),  float3(0,0,0), float3(-1, 0, 0)},
		{float3(Ax, Ay, Bz),  float3(0,0,0), float3(-1, 0, 0)},
		{float3(Ax, By, Az),  float3(0,0,0), float3(-1, 0, 0)},
		{float3(Ax, By, Bz),  float3(0,0,0), float3(-1, 0, 0)},
	};
	indices = {
				1,0,  2,
				2, 3, 1,

				4, 6, 7,
				7, 5, 4,
				
				9, 8, 11,
				11, 8, 10,
				
				13, 12, 14,
				14,15,13,
				
				18,19,16,
				16, 19, 17,
				
				21,20,22,
				22, 23,21,};

	position = startPoint;
	scaleCoeff = 1;
}

Tetrahedron::Tetrahedron(UINT title, const float3 &startPoint, float sideLen)
{
	_title = title;
	create(startPoint, sideLen);
}

void Tetrahedron::create(const float3 &startPoint, float sideLen)
{
	float3 A = float3(startPoint.x, startPoint.y + sideLen / (float)sqrt(3), startPoint.z);
	float3 B = float3(startPoint.x + sideLen / 2.0f, startPoint.y - (float)sqrt(3)*sideLen / 6.0f, startPoint.z - (float)(float)sqrt(3)*sideLen / 6.0f);
	float3 C = float3(startPoint.x - sideLen / 2.0f, startPoint.y - (float)sqrt(3)*sideLen / 6.0f, startPoint.z - (float)(float)sqrt(3)*sideLen / 6.0f);
	float3 D = float3(startPoint.x, startPoint.y - (float)sqrt(3)*sideLen / 6, startPoint.z + sideLen / (float)sqrt(3));
	//position color
	vertices = {
	{ C, float3(0.0f, 0.0f, 0.0f), float3() },
	{ B, float3(0.0f, 0.0f, 1.0f), float3() },
	{ A, float3(0.0f, 1.0f, 0.0f), float3() },

	{ D, float3(0.0f, 0.0f, 1.0f), float3() },
	{ C, float3(0.0f, 1.0f, 0.0f), float3() },
	{ B, float3(0.0f, 1.0f, 1.0f), float3() },

	{ D, float3(0.0f, 0.0f, 0.0f), float3() },
	{ C, float3(0.0f, 1.0f, 0.0f), float3() },
	{ A, float3(0.0f, 1.0f, 1.0f), float3() },

	{ D, float3(0.0f, 0.0f, 0.0f), float3() },
	{ B, float3(0.0f, 0.0f, 1.0f), float3() },
	{ A, float3(0.0f, 1.0f, 1.0f), float3() },
	};
	//triangles
	indices = {
		0,1,2,
		5,4,3,
		7,8,6,
		11,10,9,
	};

	position = startPoint;
	scaleCoeff = 1;
	calcNormals();
}

void _3dModelsBuilder::Sample3DSceneRenderer::addCube(UINT title)
{
	models.push_back(Cube(title, float3(0, 0, 0), 0.5));
	models[models.size() - 1].setConstantBuffer(m_constantBufferData);
	models[models.size() - 1].createAxes();
}

void _3dModelsBuilder::Sample3DSceneRenderer::addCube(UINT title, float3 startPoint, float sideLen, float3 rotation, float3 color)
{
	Cube cube(title, startPoint, sideLen);
	cube.rotate(rotation);
	cube.setColor(color);
	models.push_back(cube);
	models[models.size() - 1].setConstantBuffer(m_constantBufferData);
	models[models.size() - 1].createAxes();
}

void _3dModelsBuilder::Sample3DSceneRenderer::addTetrahedron(UINT title)
{
	models.push_back(Tetrahedron(title, float3(0, 0, 0), 0.8f));
	models[models.size() - 1].setConstantBuffer(m_constantBufferData);
	models[models.size() - 1].createAxes();

}

void _3dModelsBuilder::Sample3DSceneRenderer::addTetrahedron(UINT title, float3 startPoint, float sideLen, float3 rotation, float3 color)
{
	Tetrahedron tetrahedron(title, startPoint, sideLen);
	tetrahedron.rotateAbs(rotation);
	tetrahedron.setColor(color);
	models.push_back(tetrahedron);
	models[models.size() - 1].setConstantBuffer(m_constantBufferData);
	models[models.size() - 1].createAxes();
}

void _3dModelsBuilder::Model::updateProjXY()
{
	vector<Vertex> v_transformed=vertices;
	for (size_t i = 0; i < vertices.size(); ++i) {
		v_transformed[i].pos= transformFloat3(v_transformed[i].pos, m_constantBufferData.model);
	}

	vector<Vertex> v_proj;

	for (size_t i = 0; i < v_transformed.size(); ++i) {
		v_proj.push_back(v_transformed[i].projXY());
	}
	v_proj.insert(v_proj.end(), v_transformed.begin(), v_transformed.end());

	vector<unsigned short> i_proj = indices;
	for (size_t i = 0; i < v_proj.size()/2; ++i) {
		i_proj.push_back(i);
		i_proj.push_back(i + v_proj.size()/2);
	}
	
	if (!projXY) {

		projXY = make_shared<Model>(v_proj, i_proj);
		projXY->setConstantBuffer(m_constantBufferData);
	}
	else projXY->updateMesh(v_proj, i_proj);
	projXY->setColor(float3(1, 1, 1));
}

void _3dModelsBuilder::Model::updateProjZX()
{
	vector<Vertex> v_transformed = vertices;
	for (size_t i = 0; i < vertices.size(); ++i) {
		v_transformed[i].pos = transformFloat3(v_transformed[i].pos, m_constantBufferData.model);
	}

	vector<Vertex> v_proj;

	for (size_t i = 0; i < v_transformed.size(); ++i) {
		v_proj.push_back(v_transformed[i].projZX());
	}
	v_proj.insert(v_proj.end(), v_transformed.begin(), v_transformed.end());

	vector<unsigned short> i_proj = indices;
	for (size_t i = 0; i < v_proj.size() / 2; ++i) {
		i_proj.push_back(i);
		i_proj.push_back(i + v_proj.size() / 2);
	}

	if (!projZX) {

		projZX = make_shared<Model>(v_proj, i_proj);
		projZX->setConstantBuffer(m_constantBufferData);
	}
	else projZX->updateMesh(v_proj, i_proj);
	projZX->setColor(float3(1, 1, 1));
}

void _3dModelsBuilder::Model::updateProjZY()
{
	vector<Vertex> v_transformed = vertices;
	for (size_t i = 0; i < vertices.size(); ++i) {
		v_transformed[i].pos = transformFloat3(v_transformed[i].pos, m_constantBufferData.model);
	}

	vector<Vertex> v_proj;

	for (size_t i = 0; i < v_transformed.size(); ++i) {
		v_proj.push_back(v_transformed[i].projZY());
	}
	v_proj.insert(v_proj.end(), v_transformed.begin(), v_transformed.end());

	vector<unsigned short> i_proj = indices;
	for (size_t i = 0; i < v_proj.size() / 2; ++i) {
		i_proj.push_back(i);
		i_proj.push_back(i + v_proj.size() / 2);
	}

	if (!projZY) {

		projZY = make_shared<Model>(v_proj, i_proj);
		projZY->setConstantBuffer(m_constantBufferData);
	}
	else projZY->updateMesh(v_proj, i_proj);
	projZY->setColor(float3(1, 1, 1));
}

void _3dModelsBuilder::Model::calcNormals()
{
	for (size_t i = 0; i < indices.size(); i+=3) {
		float3 n = normal(vertices[indices[i]].pos, vertices[indices[i + 1]].pos, vertices[indices[i + 2]].pos);
		vertices[indices[i]].normal = vertices[indices[i+1]].normal = vertices[indices[i+2]].normal = n;
	}
}

float3 _3dModelsBuilder::Model::normal(float3 v1, float3 v2, float3 v3)
{
	XMVECTOR v1_ = XMLoadFloat3(&v1);
	XMVECTOR v2_ = XMLoadFloat3(&v2);
	XMVECTOR v3_ = XMLoadFloat3(&v3);
	XMVECTOR n = XMVector3Normalize(XMVector3Cross(XMVectorSubtract(v3_, v1_), XMVectorSubtract(v2_, v1_)));
	float3 res;
	XMStoreFloat3(&res, n);
	return res;
}

void _3dModelsBuilder::Model::init()
{
	_isSelected = false;
	isVertexBufferSet = false;
	isIndexBufferSet = false;
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixIdentity());
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixIdentity());
	XMStoreFloat4x4(&m_constantBufferData.projection, XMMatrixIdentity());
	rotation = float3(0, 0, 0);
	position = float3(0, 0, 0);
	scaleCoeff = 1;
	isProjXYrendering = false;
	isProjZYrendering = false;
	isProjZXrendering = false;
}

_3dModelsBuilder::Model::Model()
{
	init();
}

_3dModelsBuilder::Model::Model(const vector<float3>& vertices, const vector<unsigned short>& indices)
{
	init();
	this->vertices.resize(vertices.size());
	for (size_t i = 0; i < vertices.size(); ++i)
		this->vertices[i] = { vertices[i], float3(0,0,0), float3() };
	this->indices = indices;
}

_3dModelsBuilder::Model::Model(const vector<Vertex>& vertices, const vector<unsigned short>& indices) {
	init();
	this->vertices = vertices;
	this->indices = indices;
}

void _3dModelsBuilder::Model::updateMesh(vector<Vertex>& _vertices, vector<unsigned short>& _indices)
{
	vertices = _vertices;
	indices = _indices;
}

void _3dModelsBuilder::Model::updateBuffers(const shared_ptr<DX::DeviceResources>& deviceResources)
{
	if (vertices.size() == 0 || indices.size() == 0)
		return;
	auto context = deviceResources->GetD3DDeviceContext();
	if (isVertexBufferSet)
	{
		if (vertices.size() == 0)
			return;
		D3D11_MAPPED_SUBRESOURCE resource;
		context->Map(m_vertexBuffers.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &vertices[0], sizeof(Vertex)*vertices.size());
		context->Unmap(m_vertexBuffers.Get(), 0);
	}
	else {
		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = &vertices[0];
		resourceData.SysMemPitch = vertices.size();
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(Vertex)*vertices.size(), D3D11_BIND_VERTEX_BUFFER);
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;

		deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&resourceData,
			&m_vertexBuffers
		);

		isVertexBufferSet = true;
	}

	if (isIndexBufferSet) {
		int m_indexCount = sizeof(indices);
		if (m_indexCount == 0) {
			int k = m_indexCount;
		}
		D3D11_MAPPED_SUBRESOURCE resource;
		context->Map(m_indexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &indices[0], sizeof(unsigned int) * sizeof(indices));
		context->Unmap(m_indexBuffer.Get(), 0);
	}
	else {
		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		if (indices.size() == 0) {
			return;
		}
		indexBufferData.pSysMem = &indices[0];
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(indices) * sizeof(unsigned int), D3D11_BIND_INDEX_BUFFER);
		indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		indexBufferDesc.MiscFlags = 0;
		DX::ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&m_indexBuffer
			)
		);
		isIndexBufferSet = true;
	}

			// Each vertex is one instance of the VertexPositionColor struct.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		auto p = m_vertexBuffers.Get();
		context->IASetVertexBuffers(
			0,
			1,
			&p,
			&stride,
			&offset
		);

		context->IASetIndexBuffer(
			m_indexBuffer.Get(),
			DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
			0
		);
}

void _3dModelsBuilder::Model::setColor(float3 color)
{
	for (size_t i = 0; i < vertices.size(); ++i) {
		if (color.x!=-1)
			vertices[i].color.x = color.x;
		if (color.y != -1)
			vertices[i].color.y = color.y;
		if (color.z != -1)
			vertices[i].color.z = color.z;
	}
}

void _3dModelsBuilder::Model::setIdentity()
{
	XMMATRIX identity= XMMatrixIdentity();
	XMStoreFloat4x4(&m_constantBufferData.model, identity);
	position = rotation=float3(0, 0, 0);
	scaleCoeff = 1;
	if (axes.size() != 0) {
		axes[0].setIdentity();
		axes[1].setIdentity();
		axes[2].setIdentity();
		axes[1].rotateAbs(float3(0, 0, -XM_PI / 2));
		axes[2].rotateAbs(float3(XM_PI / 2, 0, 0));
	}
}

void _3dModelsBuilder::Model::rotateAbs(float3 rotation) {
	rotate(rotation - this->rotation);
}

void _3dModelsBuilder::Model::moveAbs(float3 position)
{
	move(position-this->position);
}

void _3dModelsBuilder::Model::scaleAbs(float k)
{
	scale(k / scaleCoeff);
}

void _3dModelsBuilder::Model::rotate(float3 rotation){

	float3 tmpPos = position;
	moveAbs(float3(0,0,0));
	XMMATRIX current = XMLoadFloat4x4(&m_constantBufferData.model);
	current = //XMMatrixRotationX(rotation.x) *
				XMMatrixTranspose(XMMatrixRotationX(rotation.x)) * 
				//XMMatrixRotationY(rotation.y)*
				XMMatrixTranspose(XMMatrixRotationY(rotation.y))*
				XMMatrixTranspose(XMMatrixRotationZ(rotation.z)) * 
				current;
	
	XMStoreFloat4x4(&m_constantBufferData.model, current);
	
	if (axes.size()!=0)
		for (size_t i = 0; i < axes.size(); ++i) {
			axes[i].rotate(rotation);
		}
	this->rotation = this->rotation + rotation;
	move(tmpPos);

	if (isProjXYrendering)
		updateProjXY();
	if (isProjZYrendering)
		updateProjZY();
	if (isProjZXrendering)
		updateProjZX();
}

void _3dModelsBuilder::Model::move(float3 position)
{
	if (this->position.x + position.x > 2)
		position.x = 2 - this->position.x;
	else if (this->position.x + position.x < -2)
		position.x = -2 - this->position.x;

	if (this->position.y + position.y > 2)
		position.y = 2 - this->position.y;
	else if (this->position.y + position.y < -2)
		position.y = -2 - this->position.y;

	if (this->position.z + position.z > 2)
		position.z = 2 - this->position.z;
	else if (this->position.z + position.z < -2)
		position.z = -2 - this->position.z;



	XMMATRIX current = XMLoadFloat4x4(&m_constantBufferData.model);
	
	current = XMMatrixMultiply(
		XMMatrixTranspose(XMMatrixTranslation(position.x, position.y, position.z)), current);

	XMStoreFloat4x4(&m_constantBufferData.model, current);
	if (axes.size() != 0) {
		axes[0].move(position);
		axes[1].move(position);
		axes[2].move(position);
	}
//	axes[0].scale((this->position.x + position.x + axes[0].len) / (this->position.x + axes[0].len));
//	axes[1].scale((this->position.y + position.y + axes[1].len) / (this->position.y + axes[1].len));
//	axes[2].scale((this->position.z + position.z + axes[2].len) / (this->position.z + axes[2].len));
	this->position = this->position + position;
	if (isProjXYrendering)
		updateProjXY();
	if (isProjZYrendering)
		updateProjZY();
	if (isProjZXrendering)
		updateProjZX();
}

void _3dModelsBuilder::Model::scale(float k)
{
	if (k == 0)
		return;
	float3 tmpPos = position;
	moveAbs(float3(0, 0, 0));
	XMMATRIX current = XMLoadFloat4x4(&m_constantBufferData.model);

	current = XMMatrixMultiply(
		XMMatrixScaling(k, k, k), current);

	XMStoreFloat4x4(&m_constantBufferData.model, current);
	this->scaleCoeff *= k;
	move(tmpPos);
	if (isProjXYrendering)
		updateProjXY();
	if (isProjZYrendering)
		updateProjZY();
	if (isProjZXrendering)
		updateProjZX();
}

void _3dModelsBuilder::Model::setConstantBuffer(const ModelViewProjectionConstantBuffer & buff)
{
	XMFLOAT4X4 model = m_constantBufferData.model;
	m_constantBufferData = buff;
	m_constantBufferData.model = model;
	if (axes.size() != 0)
		for (size_t i = 0; i < axes.size(); ++i)
			axes[i].setConstantBuffer(buff);
}

bool _3dModelsBuilder::Model::checkRayCollision(float x, float y, float screenWidth, float screenHeight, float4x4 world)
{
	float3 orig = float3(x, y, 0);
	float3 dest = float3(x, y, 1);
	XMVECTOR orig_unproj = XMVector3Unproject(XMLoadFloat3(&orig),
		0,
		0,
		screenWidth,
		screenHeight,
		0,
		1,
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData.projection)),
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData.view)),
		XMMatrixTranspose(XMLoadFloat4x4(&world)*XMLoadFloat4x4(&m_constantBufferData.model))
	);

	XMVECTOR dest_unproj = XMVector3Unproject(XMLoadFloat3(&dest),
		0,
		0,
		screenWidth,
		screenHeight,
		0,
		1,
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData.projection)),
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData.view)),
		XMMatrixTranspose(XMLoadFloat4x4(&world)*XMLoadFloat4x4(&m_constantBufferData.model))
	);

	XMVECTOR direction = XMVectorSubtract(dest_unproj, orig_unproj);
	direction=XMVector3Normalize(direction);

	for (size_t i = 0; i < indices.size(); i += 3) {
		float4 v0_= float4(vertices[indices[i]].pos.x, vertices[indices[i]].pos.y, vertices[indices[i]].pos.z, 0.0f);
		float4 v1_ = float4(vertices[indices[i+1]].pos.x, vertices[indices[i+1]].pos.y, vertices[indices[i+1]].pos.z, 0.0f);
		float4 v2_ = float4(vertices[indices[i+2]].pos.x, vertices[indices[i+2]].pos.y, vertices[indices[i+2]].pos.z, 0.0f);

		XMVECTOR v0 = XMLoadFloat4(&v0_);
		XMVECTOR v1 = XMLoadFloat4(&v1_);
		XMVECTOR v2 = XMLoadFloat4(&v2_);
		
		float dist;
		if (DirectX::TriangleTests::Intersects(orig_unproj, direction, v0, v1,v2, dist) ||
			DirectX::TriangleTests::Intersects(orig_unproj, direction, v2, v1, v0, dist))
			return true;
	}
	return false;
}

void _3dModelsBuilder::Model::createAxes()
{
	axes.resize(3);
	axes[0] = Axis(float3(0,0,0), 0.5f);
	axes[1] = Axis(float3(0,0,0), 0.5f);
	axes[2] = Axis(float3(0,0,0), 0.5f);
	axes[1].rotateAbs(float3(0, 0, -XM_PI / 2));
	axes[2].rotateAbs(float3(XM_PI / 2, 0, 0));

	axes[0].move(position);
	axes[1].move(position);
	axes[2].move(position);

	axes[0].setColor(float3(1, 0, 0));
	axes[1].setColor(float3(0, 1, 0));
	axes[2].setColor(float3(0, 0, 1));
	axes[0].setConstantBuffer(m_constantBufferData);
	axes[1].setConstantBuffer(m_constantBufferData);
	axes[2].setConstantBuffer(m_constantBufferData);
}

void _3dModelsBuilder::Model::render(shared_ptr<DX::DeviceResources> &m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer>& m_constantBuffer, ComPtr<ID3D11VertexShader>& m_vertexShader, ComPtr<ID3D11PixelShader>& m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout, D3D11_PRIMITIVE_TOPOLOGY drawingMode, const float4x4 &world)
{
	if (vertices.size() == 0 || indices.size() == 0)
		return;

	updateBuffers(m_deviceResources);
	auto newBuffer = m_constantBufferData;
	XMMATRIX model_m = XMLoadFloat4x4(&newBuffer.model);
	XMMATRIX world_m = XMLoadFloat4x4(&world);
	XMStoreFloat4x4(&newBuffer.model, world_m * model_m);

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource1(
		m_constantBuffer.Get(),
		0,
		NULL,
		&newBuffer,
		0,
		0,
		0
	);

//	if (!isSelected())
//		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	else
	context->IASetPrimitiveTopology(drawingMode);
	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
	);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers1(
		0,
		1,
		m_constantBuffer.GetAddressOf(),
		nullptr,
		nullptr
	);


	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
	);

	// Draw the objects.
	context->DrawIndexed(
		indices.size(),
		0,
		0
	);
}

void _3dModelsBuilder::Model::renderAxes(shared_ptr<DX::DeviceResources>& m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer>& m_constantBuffer, ComPtr<ID3D11VertexShader>& m_vertexShader, ComPtr<ID3D11PixelShader>& m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout, D3D_PRIMITIVE_TOPOLOGY drawingMode, const float4x4 &world)
{
//	if (isSelected()
	for (size_t i = 0; i < axes.size(); ++i)
		axes[i].render(m_deviceResources, context, m_constantBuffer, m_vertexShader, m_pixelShader, m_inputLayout, drawingMode, world);	
//	if (axes[0].m_constantBufferData.projection._11!=)
}

void _3dModelsBuilder::Model::renderProjections(shared_ptr<DX::DeviceResources>& m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer>& m_constantBuffer, ComPtr<ID3D11VertexShader>& m_vertexShader, ComPtr<ID3D11PixelShader>& m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout, D3D11_PRIMITIVE_TOPOLOGY drawingMode, const float4x4 & world)
{
	if (isProjXYrendering)
		projXY->render(m_deviceResources, context, m_constantBuffer, m_vertexShader, m_pixelShader, m_inputLayout, drawingMode, world);

	if (isProjZXrendering)
		projZX->render(m_deviceResources, context, m_constantBuffer, m_vertexShader, m_pixelShader, m_inputLayout, drawingMode, world);

	if (isProjZYrendering)
		projZY->render(m_deviceResources, context, m_constantBuffer, m_vertexShader, m_pixelShader, m_inputLayout, drawingMode, world);
}

bool _3dModelsBuilder::Model::checkAxesCollision(float x, float y, float screenWidth, float screenHeight, float4x4 world)
{
	if (axes.size() == 0) {
		selectionAction = 0;
		return false;
	}
	if (axes[0].checkRayCollision(x, y, screenWidth, screenHeight, world)) {
		selectionAction = 2;
		return true;
	}
	else if (axes[1].checkRayCollision(x, y, screenWidth, screenHeight, world)) {
		selectionAction = 1;
		return true;
	}
	else if (axes[2].checkRayCollision(x, y, screenWidth, screenHeight, world)) {
		selectionAction = 3;
		return true;
	}
	else selectionAction = 0;
	return false;
}

void _3dModelsBuilder::Model::applyAction(float2 prevMP, float2 curMP)
{
	if (selectionAction == 0) {
		if (abs(prevMP.y - curMP.y) > abs(curMP.x - prevMP.x))
			rotate(float3(prevMP.y-curMP.y, 0, 0));
		else 	rotate(float3(0, curMP.x - prevMP.x, 0));
	}
	else if (selectionAction == 1) {
		move(float3(curMP.x - prevMP.x, 0,0));
	}
	else if (selectionAction == 2) {
		move(float3(0,curMP.y - prevMP.y, 0));
	}
	else if (selectionAction == 3) {
		move(float3(0, 0, -(curMP.y - prevMP.y)));
	}
}

void _3dModelsBuilder::Model::intersect(const Model & model, Model & intersection)
{
	vector<float3> IPoints;
	vector<float3> curIPoints;
	vector<unsigned short> indices;


	for (size_t i = 0; i < this->indices.size(); i += 3) {
		for (size_t j = 0; j < model.indices.size(); j += 3) {

			XMVECTOR v[] = { XMVector3Transform(XMLoadFloat3(&this->vertices[this->indices[i]].pos),
								XMMatrixTranspose(XMLoadFloat4x4(&this->m_constantBufferData.model))),
							XMVector3Transform(XMLoadFloat3(&this->vertices[this->indices[i + 1]].pos),
								XMMatrixTranspose(XMLoadFloat4x4(&this->m_constantBufferData.model))),
							XMVector3Transform(XMLoadFloat3(&this->vertices[this->indices[i + 2]].pos),
								XMMatrixTranspose(XMLoadFloat4x4(&this->m_constantBufferData.model))),
							XMVector3Transform(XMLoadFloat3(&model.vertices[model.indices[j]].pos),
								XMMatrixTranspose(XMLoadFloat4x4(&model.m_constantBufferData.model))),
							XMVector3Transform(XMLoadFloat3(&model.vertices[model.indices[j + 1]].pos),
								XMMatrixTranspose(XMLoadFloat4x4(&model.m_constantBufferData.model))),
							XMVector3Transform(XMLoadFloat3(&model.vertices[model.indices[j + 2]].pos),
								XMMatrixTranspose(XMLoadFloat4x4(&model.m_constantBufferData.model)))
			};
			float3 p[6];

			for (size_t i = 0; i < 6; ++i)
				XMStoreFloat3(&p[i], v[i]);

			curIPoints = intersectTriangles(
				p[0], p[1],
				p[2], p[3],
				p[4], p[5]);

			if (curIPoints.size() != 0) {
				for (int k = 0; k < curIPoints.size(); ++k) {
					for (int l = k + 1; l < curIPoints.size(); ++l) {
						indices.push_back(IPoints.size() + k);
						indices.push_back(IPoints.size() + l);
					}
				}
				IPoints.insert(IPoints.end(), curIPoints.begin(), curIPoints.end());
			}
		}
	}

	intersection=Model(IPoints, indices);
}
vector<UINT> _3dModelsBuilder::Sample3DSceneRenderer::rayCasting(float x, float y)
{
	vector<UINT> collidingModels;
	Size screenSize = m_deviceResources->GetLogicalSize();
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected()) {
			if (models[i].checkAxesCollision(x, y, screenSize.Width, screenSize.Height, world) || 
				models[i].checkRayCollision(x, y, screenSize.Width, screenSize.Height, world))
				collidingModels.push_back(models[i].title());
			else models[i].deselect();
		}
		else if (models[i].checkRayCollision(x, y, screenSize.Width, screenSize.Height, world)) {
			models[i].select();
			collidingModels.push_back(models[i].title());
		}
		else models[i].deselect();
	}
	return collidingModels;
}
bool _3dModelsBuilder::Sample3DSceneRenderer::switchRenderProjZY()
{
	isZYprojRendering = !isZYprojRendering;
	if (isZYprojRendering) {
		for (size_t i = 0; i < models.size(); ++i)
			models[i].startRenderProjZY();
		return true;
	}
	else {
		for (size_t i = 0; i < models.size(); ++i)
			models[i].stopRenderProjZY();
		return false;
	}
}
bool _3dModelsBuilder::Sample3DSceneRenderer::switchRenderProjZX()
{
	isZXprojRendering = !isZXprojRendering;
	if (isZXprojRendering) {
		for (size_t i = 0; i < models.size(); ++i)
			models[i].startRenderProjZX();
		return true;
	}
	else {
		for (size_t i = 0; i < models.size(); ++i)
			models[i].stopRenderProjZX();
		return false;
	}
}
bool _3dModelsBuilder::Sample3DSceneRenderer::switchRenderProjXY()
{
	isXYprojRendering = !isXYprojRendering;
	if (isXYprojRendering) {
		for (size_t i = 0; i < models.size(); ++i)
			models[i].startRenderProjXY();
		return true;
	}
	else {
		for (size_t i = 0; i < models.size(); ++i)
			models[i].stopRenderProjXY();
		return false;
	}
}

_3dModelsBuilder::Axis::Axis(float3 pos, float length)
{
	init();
	position = pos;
	len = length;
	vertices.resize(9);
	vertices[0].pos = pos;
	vertices[6].pos = pos;
	vertices[7].pos = pos;
	vertices[8].pos = pos;

	vertices[6].pos.x -= 0.005f;
	vertices[7].pos.x += 0.005f;
	vertices[8].pos.z -= 0.005f;
	vertices[0].pos.z += 0.005f;


	vertices[1].pos = pos;
	vertices[1].pos.y = pos.y + length;
	vertices[2].pos = pos;
	vertices[2].pos.y = pos.y + length;
	vertices[3].pos = vertices[2].pos;
	vertices[4].pos = vertices[2].pos;
	vertices[5].pos = vertices[2].pos;
	
	vertices[2].pos.y -= 0.06f;
	vertices[3].pos.y -= 0.06f;
	vertices[4].pos.y -= 0.06f;
	vertices[5].pos.y -= 0.06f;

	vertices[2].pos.x -= 0.02f;
	vertices[3].pos.x += 0.02f;
	vertices[4].pos.z -= 0.02f;
	vertices[5].pos.z += 0.02f;
	indices = {
		1, 2, 4,
		1, 5, 2,
		1, 3, 5,
		1, 4, 3,
		2, 3, 5,
		3, 4, 2,

		1, 8, 6,
		1, 6, 0,
		1, 0, 7,
		1, 7, 8,
		6, 7, 0,
		7, 8, 6,

	};
	calcNormals();
};

void _3dModelsBuilder::Sample3DSceneRenderer::sliderMoveX(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected()) {
			models[i].moveAbs(float3(val, models[i].position.y, models[i].position.z));
		}
	}
	if (intersectionsRendering)
		recalcIntersections();
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderMoveY(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected()) {
			models[i].moveAbs(float3(models[i].position.x, val, models[i].position.z));
		}
	}
	if (intersectionsRendering)
		recalcIntersections();
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderMoveZ(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected()) {
			models[i].moveAbs(float3(models[i].position.x, models[i].position.y, val));
		}
	}
	if (intersectionsRendering)
		recalcIntersections();
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderRotateX(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].rotateAbs(float3(val, models[i].rotation.y, models[i].rotation.z));
	}
	if (intersectionsRendering)
		recalcIntersections();
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderRotateY(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].rotateAbs(float3(models[i].rotation.x, val, models[i].rotation.z));
	}
	if (intersectionsRendering)
		recalcIntersections();
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderRotateZ(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].rotateAbs(float3(models[i].rotation.x, models[i].rotation.y, val));
	}
	if (intersectionsRendering)
		recalcIntersections();
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderScale(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].scaleAbs(val);
	}
	if (intersectionsRendering)
		recalcIntersections();
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderRed(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].setColor(float3(val, -1, -1));
	}
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderGreen(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].setColor(float3(-1, val, -1));
	}
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderBlue(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].setColor(float3(-1, -1, val));
	}
}
vector<UINT> _3dModelsBuilder::Sample3DSceneRenderer::RemoveSelected()
{
	vector<UINT> titlesRemoved;
	for (int i = models.size()-1; i >= 0; --i) {
		if (models[i].isSelected()) {
			titlesRemoved.push_back(models[i].title());
			if (models.size() == 1) {
				models.clear();
				break;
			}
			models.erase(models.begin() + i);
		}
	}
	return titlesRemoved;
}
void _3dModelsBuilder::Sample3DSceneRenderer::ResetSelected()
{
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].setIdentity();
	}
}
void _3dModelsBuilder::Sample3DSceneRenderer::ResetWorldModel()
{
	XMStoreFloat4x4(&world, XMMatrixIdentity());
}
void _3dModelsBuilder::Sample3DSceneRenderer::setWireframeMode()
{
	drawingMode = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}
void _3dModelsBuilder::Sample3DSceneRenderer::setSolidMode()
{
	drawingMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void _3dModelsBuilder::Sample3DSceneRenderer::initWorldModel()
{
	worldModel.setConstantBuffer(m_constantBufferData);
	worldModel.createAxes();
	int k = 0;
}

void _3dModelsBuilder::Sample3DSceneRenderer::recalcIntersections()
{
	intersections.clear();
	for (size_t i = 0; i < models.size(); ++i)
		for (size_t j = i + 1; j < models.size(); ++j) {
			Model intersection;
			models[i].intersect(models[j], intersection);
			if (intersection.vertices.size() == 0 || intersection.indices.size() == 0)
				continue;
			auto buffer = models[j].getConstantBuffer();
			intersection.setConstantBuffer(buffer);
			intersection.setColor(float3(1, 1, 1));
			intersections.push_back(intersection);
		}
}

