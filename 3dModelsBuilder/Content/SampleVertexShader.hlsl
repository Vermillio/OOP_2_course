// A constant buffer that stores the three basic column-major matrices for composing geometry.
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
	float3 lightPos;
	float3 viewPos;
	float3 lightColor;
	float3 tmp;
};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float3 pos_ : POSITION;
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
	float3 lightPos : LIGHTPOS;
	float3 viewPos : VIEWPOS;
	float3 lightColor : LIGHTCOLOR;
};

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f);

	// Transform the vertex position into projected space.
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;
	output.pos_ = pos.xyz;

	// Pass the color through without modification.
	output.color = input.color;
	output.normal = input.normal;

	output.lightPos=lightPos;
	output.viewPos=viewPos;
	output.lightColor=lightColor;
	return output;
}