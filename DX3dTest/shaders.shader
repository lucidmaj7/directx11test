
cbuffer cbPerObject
{
    float4x4 WVP;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;
struct VOut
{
    float4 position : SV_POSITION;
 //   float4 color : COLOR;
    float2 TexCoord : TEXCOORD;

};

VOut VShader(float4 position : POSITION, float2 inTexCoord : TEXCOORD)
{
    VOut output;

    output.position = mul(position, WVP);
    
    output.TexCoord = inTexCoord;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float2 inTexCoord : TEXCOORD) : SV_TARGET
{
    return ObjTexture.Sample(ObjSamplerState, inTexCoord);
}

