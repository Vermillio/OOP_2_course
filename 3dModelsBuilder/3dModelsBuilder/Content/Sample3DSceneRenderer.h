#pragma once

#include "Vertex.h"
#include <string>
#include <vector>
#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "..\Common\DirectXHelper.h"
#include "pch.h"
#include <DirectXMath.h>
#include "VectorGeometry.h"

using std::vector;
using Microsoft::WRL::ComPtr;
using std::shared_ptr;
using std::make_shared;
using Windows::UI::Input::PointerPointProperties;


namespace _3dModelsBuilder
{
	class Model {
	protected:

		float len;

		UINT _title;
		bool _isSelected;

		//0 - default - rotate free
		//1 - move x
		//2 - move y
		//3 - move z
		int selectionAction=0;

		vector<Model> axes;

		shared_ptr<Model> projXY, projZX, projZY;
		bool isProjXYrendering, isProjZXrendering, isProjZYrendering;


		void updateProjXY();
		void updateProjZX();
		void updateProjZY();

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

		vector<Vertex> vertices;
		vector<unsigned short> indices;


		void init();
		Model();
		Model(const vector<float3> &vertices, const vector<unsigned short> &indices);
		Model(const vector<Vertex> &vertices, const vector<unsigned short> &indices);
		~Model() {};

		void updateMesh(vector<Vertex> &_vertices, vector<unsigned short> & _indices);

		UINT title() const { return _title; }
		bool isSelected() const { return _isSelected; }
		void select() 
		{ 
			_isSelected = true; 
			if (isProjXYrendering)
				updateProjXY();
			if (isProjZXrendering)
				updateProjZX();
			if (isProjZYrendering)
				updateProjZY();
		}
		void deselect() { _isSelected = false; selectionAction = 0; }

		void updateBuffers(const shared_ptr<DX::DeviceResources>&deviceResources);
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
		ModelViewProjectionConstantBuffer getConstantBuffer() const { return m_constantBufferData; };
		void createAxes();
		void render(shared_ptr<DX::DeviceResources> &m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer> &m_constantBuffer, ComPtr<ID3D11VertexShader> &m_vertexShader, ComPtr<ID3D11PixelShader> &m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout, D3D11_PRIMITIVE_TOPOLOGY drawingMode, const float4x4 &world) ;
		void renderAxes(shared_ptr<DX::DeviceResources> &m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer> &m_constantBuffer, ComPtr<ID3D11VertexShader> &m_vertexShader, ComPtr<ID3D11PixelShader> &m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout, D3D_PRIMITIVE_TOPOLOGY drawingMode, const float4x4 &world) ;
		void renderProjections(shared_ptr<DX::DeviceResources> &m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer> &m_constantBuffer, ComPtr<ID3D11VertexShader> &m_vertexShader, ComPtr<ID3D11PixelShader> &m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout, D3D11_PRIMITIVE_TOPOLOGY drawingMode, const float4x4 &world);
		bool checkRayCollision(float x, float y, float screenWidth, float screenHeight, float4x4 world);
		bool checkAxesCollision(float x, float y, float screenWidth, float screenHeight, float4x4 world);

		void applyAction(float2 prevMP, float2 curMP);

		void intersect(const Model &model, Model & intersection);

		void startRenderProjXY() { isProjXYrendering = true; updateProjXY(); };
		void stopRenderProjXY() { isProjXYrendering = false; };

		void startRenderProjZX() { isProjZXrendering = true; updateProjZX(); };
		void stopRenderProjZX() { isProjZXrendering = false; };

		void startRenderProjZY() { isProjZYrendering = true; updateProjZY(); };
		void stopRenderProjZY() { isProjZYrendering = false; };
	};
	
	class Axis;

	// This sample renderer instantiates a basic rendering pipeline.
	class Sample3DSceneRenderer
	{
	protected:

//		std::string currentFilename;
	public:
		Sample3DSceneRenderer(const shared_ptr<DX::DeviceResources>& deviceResources);


		float2 normalizeCoordinates(float2 mousePoint);

		bool parseModelFile(std::string filename);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();
		void render( vector<Model> &models, shared_ptr<DX::DeviceResources> &m_deviceResources, ID3D11DeviceContext3 * context, ComPtr<ID3D11Buffer> &m_constantBuffer, ComPtr<ID3D11VertexShader> &m_vertexShader, ComPtr<ID3D11PixelShader> &m_pixelShader, ComPtr<ID3D11InputLayout>& m_inputLayout, D3D_PRIMITIVE_TOPOLOGY drawingMode, const float4x4 &world);
		void StartTracking();
		void StartPointerMove() { m_pointerMove = true; }
		void StopPointerMove() { m_pointerMove = false; }
		bool IsPointerMove() { return m_pointerMove; }
		void TrackingUpdate(float2 prevPos, float2 curPos, bool LeftButtonPressed, bool RightButtonPressed, int MouseWheelDelta);
		void StopTracking();
		bool IsTracking() { return m_tracking; }

		void startIntersectionsRendering() { intersectionsRendering = true; recalcIntersections(); }
		void stopIntersectionsRendering() { intersectionsRendering = false; intersections.clear(); }
		
		void addCube(UINT title);
		void addCube(UINT title, float3 startPoint, float sideLen, float3 rotation, float3 color);
		void addTetrahedron(UINT title);
		void addTetrahedron(UINT title, float3 startPoint, float sideLen, float3 rotation, float3 color);
		
		bool isVertexBufferSet;
		bool isIndexBufferSet;

		vector<UINT> rayCasting(float x, float y);


		//orthogonal projection switch
		bool switchRenderProjZY();
		bool switchRenderProjZX();
		bool switchRenderProjXY();

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

		vector<UINT> RemoveSelected();
		void ResetSelected();
		void ResetWorldModel();

		D3D11_PRIMITIVE_TOPOLOGY drawingMode;

		//drawing mode
		void setWireframeMode();
		void setSolidMode();

	private:

		bool intersectionsRendering;

		float4x4 world;

		vector<Model> models;

		Model worldModel;

		void initWorldModel();

		vector<Model> intersections;


		void recalcIntersections();

		// Cached pointer to device resources.
		shared_ptr<DX::DeviceResources> m_deviceResources;

		// Direct3D resources for cube geometry.
		ComPtr<ID3D11InputLayout>	m_inputLayout;
		ComPtr<ID3D11Buffer>		m_vertexBuffer;
		ComPtr<ID3D11Buffer>		m_indexBuffer;
		ComPtr<ID3D11VertexShader>	m_vertexShader;
		ComPtr<ID3D11PixelShader>	m_pixelShader;
		ComPtr<ID3D11PixelShader>	m_flatPixelShader;

		ComPtr<ID3D11Buffer>		m_constantBuffer;
		ComPtr<ID3D11DepthStencilState> depthDisabledState;
		ComPtr<ID3D11RasterizerState> m_pRasterState;
		UINT depthStateArg;

		float4 cameraPos;

		bool isZYprojRendering;
		bool isZXprojRendering;
		bool isXYprojRendering;

		// System resources for cube geometry.
		ModelViewProjectionConstantBuffer	m_constantBufferData;
		uint32	m_indexCount;

		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		
		float	m_degreesPerSecond;

		bool	m_tracking;

		bool m_pointerMove;

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