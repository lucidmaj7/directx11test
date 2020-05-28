

struct Light
{
    float3 dir;
    float4 ambient;
    float4 diffuse;
}; 
cbuffer cbPerObject
{
    float4x4 WVP;
    float4x4 World;
    float4 lightPos;
};

cbuffer cbPerFrame
{
    Light light;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;
struct VOut
{
    float4 position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 mDiffuse : TEXCOORD1;
};
struct VIn
{
    float4 position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 normal : NORMAL;
};
VOut VShader(float4 position : POSITION, float2 inTexCoord : TEXCOORD0, float3 normal : NORMAL)
{
    VOut output;

    output.position = mul(position, World);
  
    float3 lightDir= output.position.xyz - lightPos.xyz;
    
    output.position = mul(position, WVP);
    lightDir = normalize(lightDir);
    output.normal = normalize(mul(normal, (float3x3)World));
    output.mDiffuse = dot(-lightDir, output.normal);
    output.TexCoord = inTexCoord;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL, float3 inputdiffuse: TEXCOORD1) : SV_TARGET
{
 
        float4 diffuse = ObjTexture.Sample(ObjSamplerState, inTexCoord);
        float3 diffuse2 = saturate(inputdiffuse);
        float3 finalColor = diffuse2 * diffuse;
 

        return float4(finalColor, 1);
        
}
