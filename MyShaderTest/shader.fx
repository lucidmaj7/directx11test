
cbuffer VS_CONST_BUFFER : register(b0)
{
	float4x4 worldMatrix;
	float4x4 projectionMatrix;
	float4x4 viewMatrix;
	float4 lightPos;
	float4 camPos;
};
struct VS_INPUT
{
	float4 mPosition : SV_POSITION;
	float2 mTexCoord : TEXCOORD0;
	float3 mNormal : NORMAL;
};

struct VS_OUTPUT {
	float4 mPosition : SV_POSITION;
	float3 mDiffuse: TEXCOORD1;
	float3 mViewDir: TEXCOORD2; //카메라 방향 벡터
	float3 mReflection: TEXCOORD3; //반사 벡터
	float2 mTexCoord : TEXCOORD0;
};

struct PS_INPUT
{
	float4 mPosition : SV_POSITION;
	float3 mDiffuse : TEXCOORD1;  //디퓨즈 난반사 양
	float3 mViewDir: TEXCOORD2;  // 카메라 벡터
	float3 mReflection: TEXCOORD3; // 반사광 벡터
	float2 mTexCoord : TEXCOORD0;
};

cbuffer PS_CONST_BUFFER : register(b0) 
{
	float4 LightColor;
};

Texture2D ObjTexture[2];
SamplerState ObjSamplerState[2];

VS_OUTPUT vs_main(VS_INPUT Input )
{
	VS_OUTPUT Output;


	Output.mPosition = mul(Input.mPosition, worldMatrix); //정점 월드 변환


	float3 lightDir = Output.mPosition.xyz - lightPos.xyz; // 입사광 벡터 구하기
	lightDir = normalize(lightDir); //입사광 벡터 노멀라이즈 

	float3 viewDir = normalize(Output.mPosition.xyz - camPos.xyz); //카메라 벡터 노멀라이즈
	Output.mViewDir = viewDir; //output에 전달

	Output.mPosition = mul(Output.mPosition, viewMatrix); // 정점 뷰 변환
	Output.mPosition = mul(Output.mPosition, projectionMatrix); // 정점 투영 변환




	float3 worldNormal = mul(Input.mNormal, (float3x3)worldMatrix); //노멀 벡터 월드 변환
	worldNormal = normalize(worldNormal); //노멀 벡터 노멀라이즈 

	Output.mTexCoord = Input.mTexCoord;
	Output.mDiffuse = dot(-lightDir, worldNormal); //난반사 
	
	return Output;
	
}

float4 ps_main(PS_INPUT Input) : SV_TARGET
{
	
	float3 diffuse = saturate(Input.mDiffuse);

	diffuse = ceil(diffuse * 5) / 5.0f;
  

   return float4(diffuse.xyz* LightColor.xyz, 1); //출력 
   
}