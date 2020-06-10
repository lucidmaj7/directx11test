
cbuffer VS_CONST_BUFFER : register(b0)
{
	float4x4 worldMatrix;
	float4x4 projectionMatrix;
	float4x4 viewMatrix;
	float4 lightPos;
};
struct VS_INPUT
{
	float4 position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float3 mDiffuse: TEXCOORD1;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 mDiffuse : TEXCOORD1;
};

VS_OUTPUT vs_main(VS_INPUT input )
{
	VS_OUTPUT output;
	output.position = mul(input.position,worldMatrix);
	float3 lightDirection = output.position.xyz - lightPos.xyz;

	output.position = mul(output.position,viewMatrix);
	output.position = mul(output.position ,projectionMatrix);

	lightDirection = normalize(lightDirection);
	float3 worldNormal = mul(input.normal, (float3x3)worldMatrix);
	worldNormal = normalize(worldNormal);

	output.mDiffuse = dot(-lightDirection, worldNormal);
	return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{

	float3 ambient = float3(0.1f,0.1f,0.1f);
   float3 diffuse = saturate(input.mDiffuse);
   if (diffuse.x < 0.1)
	   diffuse = ambient;
   return float4(diffuse, 1);
   
}