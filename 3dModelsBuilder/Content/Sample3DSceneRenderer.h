#pragma once

#include <string>
#include <vector>
#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "..\Common\DirectXHelper.h"
#include "pch.h"
#include <DirectXMath.h>

namespace _3dModelsBuilder
{

	using float3 = DirectX::XMFLOAT3;
	using float2 = DirectX::XMFLOAT2;
	using float4 = DirectX::XMFLOAT4;
	using float4x4 = DirectX::XMFLOAT4X4;

	template<class T>
	using ComPtr= Microsoft::WRL::ComPtr<T>;

	struct Vertex {
		float3 pos;
		float3 color;
		float3 normal;
	};

	inline float DotProduct(float3 a, float3 b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline float3 CrossProduct(float3 a, float3 b) {
		float x = a.y*b.z - a.z*b.y;
		float y = a.z*b.x - a.x*b.z;
		float z = a.x*b.y - a.y*b.x;
		return float3(x, y, z);
	}

	inline float3 operator-(float3 a) { return float3(-a.x, -a.y, -a.z); };

	inline float3 operator*(float3 a, float k) {
		return float3(a.x *k, a.y *k, a.z *k);
	}

	inline float3 operator+(float3 a, float3 b) {
		return float3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	inline float3 operator-(float3 a, float3 b) {
		return float3(a.x-b.x, a.y-b.y, a.z-b.z);
	}

	inline float3 transformFloat3(const float3 &_vector, const float4x4 &matrix) {
		float4 vector = float4(_vector.x, _vector.y, _vector.z, 1.0f);
		DirectX::XMVECTOR vec = XMLoadFloat4(&vector);
		DirectX::XMMATRIX mat = XMLoadFloat4x4(&matrix);
		vec = XMVector4Transform(vec, mat);
		float4 vec_;
		XMStoreFloat4(&vec_, vec);
		return float3(vec_.x, vec_.y, vec_.z);
	}

	bool intersectRayTriangle(float3 rayOrig, float3 rayDir, float3 v0, float3 v1, float3 v2);


	class Model {
	protected:

		float len;

		UINT _title;
		bool _isSelected;

		//0 - default - rotate free
		//1 - move x
		//2 - move y
		//3 - move z
		//4 - rotate yz
		//5 - rotate xz
		//6 - rotate xy
		//4 - scale
		int selectionAction=0;

		std::vector<Model> axes;

		bool isVertexBufferSet;
		bool isIndexBufferSet;

		ComPtr<ID3D11Buffer> m_vertexBuffers;
		ComPtr<ID3D11Buffer>	m_indexBuffer;

		void calcNormals();
		float3 normal(float3 v1, float3 v2, float3 v3);
	public:

		float3 rotation;
		float3 position;
		float scaleCoeff;

		ModelViewProjectionConstantBuffer		m_constantBufferData;

		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		Model();
		~Model() {};

		UINT title() const { return _title; }
		bool isSelected() const { return _isSelected; }
		void select() { _isSelected = true; }
		void deselect() { _isSelected = false; selectionAction = 0; }

		void updateBuffers(const std::shared_ptr<DX::DeviceResources>&deviceResources);
		void setColor(float3 color);
		//reset
		void setIdentity();

		//absolute
		void rotateAbs(float3 rotation);
		void moveAbs(float3 position);
		void scaleAbs(float k);

		//relative
		void rotate(float3 rotation);
		void move(float3 position);
		void scale(float k);

		void setConstantBuffer(const ModelViewProjectionConstantBuffer &buff);
		bool checkRayCollision(const float3 &rayOrigin, const float3 &rayDirection);

		void createAxes();
		void render(std::shared_ptr<DX::DeviceResources> &m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer> &m_constantBuffer, ComPtr<ID3D11VertexShader> &m_vertexShader, ComPtr<ID3D11PixelShader> &m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout);
		void renderAxes(std::shared_ptr<DX::DeviceResources> &m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer> &m_constantBuffer, ComPtr<ID3D11VertexShader> &m_vertexShader, ComPtr<ID3D11PixelShader> &m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout);

		void checkAxesCollision(float3 rayOrigin, float3 rayDirection);

		void applyAction(float2 prevMP, float2 curMP);
	};
	
	class Axis;



	// This sample renderer instantiates a basic rendering pipeline.
	class Sample3DSceneRenderer
	{
	protected:

//		std::string currentFilename;
	public:
		Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);


		float2 normalizeCoordinates(float2 mousePoint);

		bool parseModelFile(std::string filename);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();
		void StartTracking();
		void StartPointerMove() { m_pointerMove = true; }
		void StopPointerMove() { m_pointerMove = false; }
		bool IsPointerMove() { return m_pointerMove; }
		void TrackingUpdate(float2 prevPos, float2 curPos);
		void StopTracking();
		bool IsTracking() { return m_tracking; }
		
		void addCube(UINT title);
		void addCube(UINT title, float3 startPoint, float sideLen, float3 rotation, float3 color);
		void addTetrahedron(UINT title);
		void addTetrahedron(UINT title, float3 startPoint, float sideLen, float3 rotation, float3 color);
		
		bool isVertexBufferSet;
		bool isIndexBufferSet;

		std::vector<UINT> rayCasting(float x, float y);


		//orthogonal projection switch
		bool changeStateOrthoProjectionX();
		bool changeStateOrthoProjectionY();
		bool changeStateOrthoProjectionZ();

		//sliders

		void sliderMoveX(float val);
		void sliderMoveY(float val);
		void sliderMoveZ(float val);
		void sliderRotateX(float val);
		void sliderRotateY(float val);
		void sliderRotateZ(float val);
		void sliderScale(float val);
		void sliderRed(float val);
		void sliderGreen(float val);
		void sliderBlue(float val);

		std::vector<UINT> removeSelected();
		void resetSelected();

	private:

		std::vector<Model> models;

		std::vector<Axis> axes;

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Direct3D resources for cube geometry.
		ComPtr<ID3D11InputLayout>	m_inputLayout;
		ComPtr<ID3D11Buffer>		m_vertexBuffer;
		ComPtr<ID3D11Buffer>		m_indexBuffer;
		ComPtr<ID3D11VertexShader>	m_vertexShader;
		ComPtr<ID3D11PixelShader>	m_pixelShader;
		ComPtr<ID3D11Buffer>		m_constantBuffer;
		ComPtr<ID3D11DepthStencilState> depthDisabledState;
		UINT depthStateArg;

		bool isXprojRendering;
		bool isYprojRendering;
		bool isZprojRendering;

		// System resources for cube geometry.
		ModelViewProjectionConstantBuffer	m_constantBufferData;
		uint32	m_indexCount;

		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		
		float	m_degreesPerSecond;

		bool	m_tracking;

		bool m_pointerMove;

		void renderProjections();
	};

	class Axis : public Model {
	public:
		Axis() : Model() {};
		Axis(float3 pos, float length);
	};

	class Cube : public Model {
	public:
		Cube(UINT title, const float3 &startPoint, float sideLen);
		void create(const float3 &startPoint, float sideLen);
	};

	class Tetrahedron : public Model {
	public:
		Tetrahedron(UINT title, const float3 &startPoint, float sideLen);
		void create(const float3 &startPoint, float sideLen);
	};
}