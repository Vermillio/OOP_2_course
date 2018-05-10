#include "pch.h"
#include "Sample3DSceneRenderer.h"
#include "..\Common\DirectXHelper.h"
#include <DirectXCollision.h>
#include <fstream>

using namespace _3dModelsBuilder;
using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_degreesPerSecond(0.2),
	m_indexCount(0),
	m_tracking(false),
	m_pointerMove(false),
	m_deviceResources(deviceResources),
	isVertexBufferSet(false),
	isIndexBufferSet(false),
	isXprojRendering(false),
    isYprojRendering(false),
    isZprojRendering(false),
	drawingMode(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	CreateWindowSizeDependentResources();
	CreateDeviceDependentResources();
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
	float aspectRatio = outputSize.Width / outputSize.Height;
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

	m_constantBufferData.lightPos = { -1.0f, 1.0f, -1.0f };
	m_constantBufferData.viewPos = { 0.0f, 0.7f, 1.5f };
	m_constantBufferData.lightColor = { 1.0f, 1.0f, 1.0f };
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
void Sample3DSceneRenderer::TrackingUpdate(float2 prevPos, float2 curPos)
{
	if (m_tracking)
	{
		float2 prevPosNormalized = normalizeCoordinates(prevPos);
		float2 curPosNormalized = normalizeCoordinates(curPos);
		for (size_t i = 0; i < models.size(); ++i) {
			if (models[i].isSelected()) {
				models[i].applyAction(prevPosNormalized, curPosNormalized);
			}
		}
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
	//ID3D11DepthStencilState *curState;
	//UINT curStateArg;
	//context->OMGetDepthStencilState(&curState, &curStateArg);
	//
	//context->OMSetDepthStencilState(depthDisabledState.Get(), 0);

	renderProjections();

	for (size_t i = 0; i < models.size(); ++i)
		models[i].renderAxes(m_deviceResources, context, m_constantBuffer, m_vertexShader, m_pixelShader, m_inputLayout, drawingMode);

	////always on top disable
	//context->OMSetDepthStencilState(curState, curStateArg);

	for (size_t i = 0; i < models.size(); ++i)
		models[i].render(m_deviceResources, context, m_constantBuffer, m_vertexShader, m_pixelShader, m_inputLayout, drawingMode);
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
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
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
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
		int in = 0;
	});
	auto createDepthStates = createPSTask.then([this]() {
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

		depthStencilDesc.DepthEnable = TRUE;
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
				1,  0, 2,
				1,  2, 3,
				5,  4, 6,
				5,  6, 7,
				9,  8, 10,
				9, 10,11,
				13,12, 14,
				13,14, 15,
				17,16, 18,
				17,18, 19,
				21,20, 22,
				21,22, 23 };

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
	float3 A = float3(startPoint.x, startPoint.y + sideLen / sqrt(3), startPoint.z);
	float3 B = float3(startPoint.x + sideLen / 2, startPoint.y - sqrt(3)*sideLen / 6, startPoint.z - sqrt(3)*sideLen / 6);
	float3 C = float3(startPoint.x - sideLen / 2, startPoint.y - sqrt(3)*sideLen / 6, startPoint.z - sqrt(3)*sideLen / 6);
	float3 D = float3(startPoint.x, startPoint.y - sqrt(3)*sideLen / 6, startPoint.z + sideLen / sqrt(3));
	//position color
	vertices = {
	{ A, float3(0.0f, 0.0f, 0.0f), float3() },
	{ B, float3(0.0f, 0.0f, 1.0f), float3() },
	{ C, float3(0.0f, 1.0f, 0.0f), float3() },

	{ B, float3(0.0f, 0.0f, 1.0f), float3() },
	{ C, float3(0.0f, 1.0f, 0.0f), float3() },
	{ D, float3(0.0f, 1.0f, 1.0f), float3() },

	{ A, float3(0.0f, 0.0f, 0.0f), float3() },
	{ C, float3(0.0f, 1.0f, 0.0f), float3() },
	{ D, float3(0.0f, 1.0f, 1.0f), float3() },

	{ A, float3(0.0f, 0.0f, 0.0f), float3() },
	{ B, float3(0.0f, 0.0f, 1.0f), float3() },
	{ D, float3(0.0f, 1.0f, 1.0f), float3() },
	};
	//triangles
	indices = {
		2,1,0,
		3,4,5,
		8,7,6,
		9,10, 11,
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
	models.push_back(Tetrahedron(title, float3(0, 0, 0), 0.8));
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
	XMVECTOR n = XMVector3Normalize(XMVector3Cross(XMVectorSubtract(v2_, v1_), XMVectorSubtract(v3_, v1_)));
	float3 res;
	XMStoreFloat3(&res, n);
	return res;
}

_3dModelsBuilder::Model::Model()
{
	_isSelected = false;
	isVertexBufferSet = false;
	isIndexBufferSet = false;
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixIdentity());
	rotation = float3(0, 0, 0);
	position = float3(0, 0, 0);
	scaleCoeff = 0;
}

void _3dModelsBuilder::Model::updateBuffers(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	auto context = deviceResources->GetD3DDeviceContext();
	if (isVertexBufferSet)
	{
		// This is where it copies the new vertices to the buffer.
		// but it's causing flickering in the entire screen...
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
		D3D11_MAPPED_SUBRESOURCE resource;
		context->Map(m_indexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &indices[0], sizeof(unsigned int) * sizeof(indices));
		context->Unmap(m_indexBuffer.Get(), 0);
	}
	else {
		int m_indexCount = sizeof(indices);
		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
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
		axes[2].rotateAbs(float3(-XM_PI / 2, 0, 0));
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
	current = XMMatrixMultiply(
		XMMatrixMultiply(
		XMMatrixMultiply(XMMatrixTranspose(XMMatrixRotationX(rotation.x)),
			XMMatrixRotationY(rotation.y)),
		XMMatrixTranspose(XMMatrixRotationZ(rotation.z))), current);
	XMStoreFloat4x4(&m_constantBufferData.model, current);
	
	for (size_t i = 0; i < axes.size(); ++i) {
		axes[i].rotate(rotation);
	}
	this->rotation = this->rotation + rotation;
	move(tmpPos);
}

void _3dModelsBuilder::Model::move(float3 position)
{
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
}

void _3dModelsBuilder::Model::scale(float k)
{
	float3 tmpPos = position;
	moveAbs(float3(0, 0, 0));
	XMMATRIX current = XMLoadFloat4x4(&m_constantBufferData.model);

	current = XMMatrixMultiply(
		XMMatrixScaling(position.x, position.y, position.z), current);

	XMStoreFloat4x4(&m_constantBufferData.model, current);
	this->scaleCoeff *= k;
	for (size_t i = 0; i < axes.size(); ++i)
		axes[i].scale(k);
	move(tmpPos);
}

void _3dModelsBuilder::Model::setConstantBuffer(const ModelViewProjectionConstantBuffer & buff)
{
	XMFLOAT4X4 model = m_constantBufferData.model;
	m_constantBufferData = buff;
	m_constantBufferData.model = model;
}

bool _3dModelsBuilder::Model::checkRayCollision(const float3 &rayOrigin, const float3 &rayDirection)
{
//	float3 orig_ = float3(rayOrigin.x, rayOrigin.y, rayOrigin.z);
//	float3 dir_ = float3(rayDirection.x, rayOrigin.y, rayOrigin.z);
	XMVECTOR orig = XMLoadFloat3(&rayOrigin);
	XMVECTOR dir = XMLoadFloat3(&rayDirection);
//	XMMATRIX model = XMLoadFloat4x4(&m_constantBufferData.model);
	XMMATRIX inverseviewproj = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_constantBufferData.view) * XMLoadFloat4x4(&m_constantBufferData.projection));

	orig = XMVector4Transform(orig, inverseviewproj);
	dir = XMVector4Transform(orig, inverseviewproj);
	dir = XMVector3Normalize(dir);

	for (size_t i = 0; i < indices.size(); i += 3) {
		float3 v0_= float3(vertices[indices[i]].pos.x, vertices[indices[i]].pos.y, vertices[indices[i]].pos.z);
		float3 v1_ = float3(vertices[indices[i+1]].pos.x, vertices[indices[i+1]].pos.y, vertices[indices[i+1]].pos.z);
		float3 v2_ = float3(vertices[indices[i+2]].pos.x, vertices[indices[i+2]].pos.y, vertices[indices[i+2]].pos.z);

		XMVECTOR v0 = XMLoadFloat3(&v0_);
		XMVECTOR v1 = XMLoadFloat3(&v1_);
		XMVECTOR v2 = XMLoadFloat3(&v2_);
		
		float dist;
		if (DirectX::TriangleTests::Intersects(orig, dir, v0, v1,v2, dist) ||
			DirectX::TriangleTests::Intersects(orig, dir, v2, v1, v0, dist))
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
	axes[2].rotateAbs(float3(-XM_PI / 2, 0, 0));

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

void _3dModelsBuilder::Model::render(std::shared_ptr<DX::DeviceResources> &m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer>& m_constantBuffer, ComPtr<ID3D11VertexShader>& m_vertexShader, ComPtr<ID3D11PixelShader>& m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout, D3D11_PRIMITIVE_TOPOLOGY drawingMode)
{
	updateBuffers(m_deviceResources);

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource1(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
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

void _3dModelsBuilder::Model::renderAxes(std::shared_ptr<DX::DeviceResources>& m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer>& m_constantBuffer, ComPtr<ID3D11VertexShader>& m_vertexShader, ComPtr<ID3D11PixelShader>& m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout, D3D_PRIMITIVE_TOPOLOGY drawingMode)
{
	if (isSelected())
		for (size_t i = 0; i < axes.size(); ++i)
			axes[i].render(m_deviceResources, context, m_constantBuffer, m_vertexShader, m_pixelShader, m_inputLayout, drawingMode);	
}

void _3dModelsBuilder::Model::checkAxesCollision(float3 rayOrigin, float3 rayDirection)
{
	if (axes.size() == 0) {
		selectionAction = 0;
	}
	if (axes[0].checkRayCollision(rayOrigin, rayDirection)) {
		selectionAction = 1;
	}
	else if (axes[1].checkRayCollision(rayOrigin, rayDirection)) {
		selectionAction = 2;
	}
	else if (axes[2].checkRayCollision(rayOrigin, rayDirection)) {
		selectionAction = 3;
	}
}

void _3dModelsBuilder::Model::applyAction(float2 prevMP, float2 curMP)
{
	if (selectionAction == 0) {
		rotate(float3(curMP.y-prevMP.y, curMP.x - prevMP.x, 0));
	}
	else if (selectionAction == 1) {
		move(float3(curMP.x - prevMP.x, position.y,position.z));
	}
	else if (selectionAction == 2) {
		move(float3(position.x,curMP.y - prevMP.y, position.z));
	}
	else if (selectionAction == 3) {
		move(float3(position.x, position.y, -(curMP.y - prevMP.y)));
	}
}

std::vector<UINT> _3dModelsBuilder::Sample3DSceneRenderer::rayCasting(float x, float y)
{
	std::vector<UINT> collidingModels;
	float2 point = normalizeCoordinates(float2(x, y));
	float3 rayOrigin = float3(point.x, point.y, 10.0f);
	float3 rayDirection = float3(0, 0, -1.0f);

	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected()) {
			models[i].checkAxesCollision(rayOrigin, rayDirection);
			collidingModels.push_back(models[i].title());
		}
		if (models[i].checkRayCollision(rayOrigin, rayDirection)) {
			models[i].select();
			collidingModels.push_back(models[i].title());
		}
		else models[i].deselect();
	}

	return collidingModels;
}

bool _3dModelsBuilder::Sample3DSceneRenderer::changeStateOrthoProjectionX()
{
	isXprojRendering = !isXprojRendering;
	if (isXprojRendering)
		return true;
	else return false;
}
bool _3dModelsBuilder::Sample3DSceneRenderer::changeStateOrthoProjectionY()
{
	isYprojRendering = !isYprojRendering;
	if (isYprojRendering)
		return true;
	else return false;
}
bool _3dModelsBuilder::Sample3DSceneRenderer::changeStateOrthoProjectionZ()
{
	isZprojRendering = !isZprojRendering;
	if (isZprojRendering)
		return true;
	else return false;
}

void _3dModelsBuilder::Sample3DSceneRenderer::renderProjections()
{
	if (isXprojRendering)
	{
	}	//
	if (isYprojRendering)
	{
	}	//
	if (isZprojRendering)
	{
	}

}

_3dModelsBuilder::Axis::Axis(float3 pos, float length)
{
	position = pos;
	len = length;
	vertices.resize(9);
	vertices[0].pos = pos;
	vertices[6].pos = pos;
	vertices[7].pos = pos;
	vertices[8].pos = pos;

	vertices[6].pos.x -= 0.005;
	vertices[7].pos.x += 0.005;
	vertices[8].pos.z -= 0.005;
	vertices[0].pos.z += 0.005;


	vertices[1].pos = pos;
	vertices[1].pos.y = pos.y + length;
	vertices[2].pos = pos;
	vertices[2].pos.y = pos.y + length;
	vertices[3].pos = vertices[2].pos;
	vertices[4].pos = vertices[2].pos;
	vertices[5].pos = vertices[2].pos;
	
	vertices[2].pos.y -= 0.06;
	vertices[3].pos.y -= 0.06;
	vertices[4].pos.y -= 0.06;
	vertices[5].pos.y -= 0.06;

	vertices[2].pos.x -= 0.02;
	vertices[3].pos.x += 0.02;
	vertices[4].pos.z -= 0.02;
	vertices[5].pos.z += 0.02;
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
};


void _3dModelsBuilder::Sample3DSceneRenderer::sliderMoveX(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].moveAbs(float3(val, models[i].position.y, models[i].position.z));
	}
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderMoveY(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].moveAbs(float3(models[i].position.x, val, models[i].position.z));
	}
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderMoveZ(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].moveAbs(float3(models[i].position.x, models[i].position.y, val));
	}
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderRotateX(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].rotateAbs(float3(val, models[i].rotation.y, models[i].rotation.z));
	}
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderRotateY(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].rotateAbs(float3(models[i].rotation.x, val, models[i].rotation.z));
	}
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderRotateZ(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].rotateAbs(float3(models[i].rotation.x, models[i].rotation.y, val));
	}
};
void _3dModelsBuilder::Sample3DSceneRenderer::sliderScale(float val) {
	for (size_t i = 0; i < models.size(); ++i) {
		if (models[i].isSelected())
			models[i].scaleAbs(val);
	}
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
std::vector<UINT> _3dModelsBuilder::Sample3DSceneRenderer::RemoveSelected()
{
	std::vector<UINT> titlesRemoved;
	for (int i = models.size()-1; i >= 0; --i) {
		if (models[i].isSelected()) {
			titlesRemoved.push_back(models[i].title());
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
void _3dModelsBuilder::Sample3DSceneRenderer::setWireframeMode()
{
	drawingMode = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}
void _3dModelsBuilder::Sample3DSceneRenderer::setSolidMode()
{
	drawingMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}
;