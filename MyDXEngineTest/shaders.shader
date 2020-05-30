

struct Light
{
    float4 ambient; 
}; 
cbuffer cbPerObject
{
    float4x4 WorldMatrix;
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
    float4 lightPos;
    float4 camPos;
};
struct Material {
    float4  specular;
};

cbuffer cbPerFrame
{
    Material material;
    Light light;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;


struct VOut
{
    float4 position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 mDiffuse : TEXCOORD1;
    float3 mViewDir:TEXCOORD2;
    float3 mReflection: TEXCOORD3;
};
struct VIn
{
    float4 position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 normal : NORMAL;
};
VOut VShader(float4 position : POSITION, float2 inTexCoord : TEXCOORD0, float3 normal : NORMAL)
{
    VOut output;

    output.position = mul(position, WorldMatrix);
  
    float3 lightDir= normalize(output.position.xyz - lightPos.xyz);
    float3 viewDir = normalize(output.position.xyz - camPos.xyz);

    output.position = mul(output.position, ViewMatrix);
    output.position = mul(output.position, ProjectionMatrix);
  
    float3 worldNormal= normalize(mul(normal, (float3x3)WorldMatrix));

    output.mDiffuse = dot(-lightDir, worldNormal);
    output.TexCoord = inTexCoord;

    output.mReflection = reflect(lightDir, worldNormal);
    output.mViewDir = viewDir;


    return output;
}


float4 PShader(float4 position : SV_POSITION, float2 inTexCoord : TEXCOORD0, float3 normal : NORMAL, float3 inputdiffuse: TEXCOORD1, float3 viewDir:TEXCOORD2, float3 reflection: TEXCOORD3) : SV_TARGET
{
      float4 diffuse = ObjTexture.Sample(ObjSamplerState, inTexCoord);
      float3 ambient = diffuse * light.ambient;
      float3 specular = 0;
      float3 diffuse2 = saturate(inputdiffuse)* diffuse;
      float3 r = normalize(reflection);
      float3 v = normalize(viewDir);
      bool bSpecular = false;
        if (diffuse.x > 0)
        {
            specular = saturate(dot(r, -v));
            if (material.specular.x != 0)
            {
                specular.x = pow(specular.x, material.specular.x);
                bSpecular = true;
            }
            if (material.specular.y != 0)
            {
                specular.y = pow(specular.y, material.specular.y);
                bSpecular = true;
            }
            if (material.specular.z != 0)
            {
                specular.z = pow(specular.z, material.specular.z);
                bSpecular = true;
            }

        }
        if (bSpecular == true)
            return float4(ambient + diffuse2 + specular, 1);

        return float4(ambient+diffuse2 , 1);
  
        
}
