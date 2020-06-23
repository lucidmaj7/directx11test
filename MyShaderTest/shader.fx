
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
	float3 mViewDir: TEXCOORD2; //ī�޶� ���� ����
	float3 mReflection: TEXCOORD3; //�ݻ� ����
	float2 mTexCoord : TEXCOORD0;
};

struct PS_INPUT
{
	float4 mPosition : SV_POSITION;
	float3 mDiffuse : TEXCOORD1;  //��ǻ�� ���ݻ� ��
	float3 mViewDir: TEXCOORD2;  // ī�޶� ����
	float3 mReflection: TEXCOORD3; // �ݻ籤 ����
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


	Output.mPosition = mul(Input.mPosition, worldMatrix); //���� ���� ��ȯ


	float3 lightDir = Output.mPosition.xyz - lightPos.xyz; // �Ի籤 ���� ���ϱ�
	lightDir = normalize(lightDir); //�Ի籤 ���� ��ֶ����� 

	float3 viewDir = normalize(Output.mPosition.xyz - camPos.xyz); //ī�޶� ���� ��ֶ�����
	Output.mViewDir = viewDir; //output�� ����

	Output.mPosition = mul(Output.mPosition, viewMatrix); // ���� �� ��ȯ
	Output.mPosition = mul(Output.mPosition, projectionMatrix); // ���� ���� ��ȯ




	float3 worldNormal = mul(Input.mNormal, (float3x3)worldMatrix); //��� ���� ���� ��ȯ
	worldNormal = normalize(worldNormal); //��� ���� ��ֶ����� 

	Output.mTexCoord = Input.mTexCoord;
	Output.mDiffuse = dot(-lightDir, worldNormal); //���ݻ� 
	
	return Output;
	
}

float4 ps_main(PS_INPUT Input) : SV_TARGET
{
	
	float3 diffuse = saturate(Input.mDiffuse);

	diffuse = ceil(diffuse * 5) / 5.0f;
  

   return float4(diffuse.xyz* LightColor.xyz, 1); //��� 
   
}