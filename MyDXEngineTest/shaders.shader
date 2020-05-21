
cbuffer cbPerObject
{
    float4x4 WVP;
    float4x4 World;
};

struct Light
{
    float3 dir;
    float4 ambient;
    float4 diffuse;
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
};

VOut VShader(float4 position : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
    VOut output;

    output.position =  mul(position, WVP);
    output.normal =  mul(normal, World);
    output.TexCoord = inTexCoord;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL) : SV_TARGET
{
       return ObjTexture.Sample(ObjSamplerState, inTexCoord);;
     /*  normal = normalize(normal);
        float4 diffuse = ObjTexture.Sample(ObjSamplerState, inTexCoord);
        float3 finalColor;

        finalColor = diffuse * light.ambient;
        finalColor += saturate(dot(light.dir, normal) * light.diffuse * diffuse);

        return float4(finalColor, diffuse.a);*/

}

float4 D2D_PS(float2 inTexCoord : TEXCOORD) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample(ObjSamplerState, inTexCoord);

    return diffuse;
}