#pragma once

namespace _3dModelsBuilder
{
	using float3 = DirectX::XMFLOAT3;
	using float2 = DirectX::XMFLOAT2;
	using float4 = DirectX::XMFLOAT4;
	using float4X4 = DirectX::XMFLOAT4X4;

	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// Constant buffer used to send MVP matrices to the vertex shader.
	struct ModelViewProjectionConstantBuffer
	{
		float4X4 model;
		float4X4 view;
		float4X4 projection;
		float3 lightPos;
		float3 viewPos;
		float3 lightColor;
		float3 tmp;
	};

	// Used to send per-vertex data to the vertex shader.
	struct VertexPositionColor
	{
		float3 pos;
		float3 color;
	};



}