//light info


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

// A pass-through function for the (interpolated) color data.
float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 lightDir = normalize(input.lightPos - input.pos_);
	float3 viewDir = normalize(input.viewPos - input.pos_);
	float3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(input.normal, halfwayDir), 0.0), 6.0);
	float4 specular = float4(input.color * spec, 1.0f);
	float gamma = 2.2;
	specular = pow(specular, float4(1.0 / gamma, 1.0 / gamma, 1.0 / gamma, 1.0f));
	return specular;
}