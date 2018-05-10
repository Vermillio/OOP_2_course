//light info


// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;

	float4x4 World: WORLD;
};

// A pass-through function for the (interpolated) color data.
float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 normal = normalize(input.normal);

	float3 DiffuseLightDirection = normalize(float3(0, 1, 1));
	float DiffuseIntensity = 1.0;
	float lightIntensity = dot(normal, DiffuseLightDirection);
	float4 diffuse = saturate(float4(input.color,1.0f) * DiffuseIntensity * lightIntensity);

	float4 AmbientColor = float4(1, 1, 1, 1);
	float AmbientIntensity = 0.05;
	float Shininess = 200;
	float4 SpecularColor = float4(1, 1, 1, 1);
	float SpecularIntensity = 0.4;
	float3 ViewVector = float3(0, 0, -1);

	float3 light = normalize(DiffuseLightDirection);
	float3 r = normalize(2 * dot(light, normal) * normal - light);
	float3 v = normalize(mul(normalize(ViewVector), input.World));

	float dotProduct = dot(r, v);
	float4 specular = SpecularIntensity * SpecularColor * max(pow(dotProduct, Shininess), 0) * length(diffuse);

	float gamma = 1.2;

	float4 ambient= saturate(diffuse + AmbientColor * AmbientIntensity + specular);
	ambient = saturate(pow(ambient, float4(1 / gamma, 1/gamma, 1/gamma, 1.0f)));

	return ambient;//specular;
		//float4(input.normal.xyz, 1.0f);
}