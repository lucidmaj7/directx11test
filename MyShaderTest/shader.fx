
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
	float2 mTexCoord : TEXCOORD;
	float3 mNormal : NORMAL;
};

struct VS_OUTPUT {
	float4 mPosition : SV_POSITION;
	float3 mDiffuse: TEXCOORD1;
	float3 mViewDir: TEXCOORD2; //ī�޶� ���� ����
	float3 mReflection: TEXCOORD3; //�ݻ� ����
};

struct PS_INPUT
{
	float4 mPosition : SV_POSITION;
	float3 mDiffuse : TEXCOORD1;  //��ǻ�� ���ݻ� ��
	float3 mViewDir: TEXCOORD2;  // ī�޶� ����
	float3 mReflection: TEXCOORD3; // �ݻ籤 ����
};

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


	Output.mDiffuse = dot(-lightDir, worldNormal); //���ݻ� 
	Output.mReflection = reflect(lightDir, worldNormal); //�ݻ纤�� ���ϱ�
	return Output;
	
}

float4 ps_main(PS_INPUT Input) : SV_TARGET
{

	float3 diffuse = saturate(Input.mDiffuse);

   float3 reflection = normalize(Input.mReflection); // �ݻ籤 ���� �븻������
   float3 viewDir = normalize(Input.mViewDir); // ī�޶� ���� ��ֶ����� 
   float3 specular = 0; //�ݻ籤�� ���ϱ�
   if (diffuse.x > 0)
   {
	  specular = saturate(dot(reflection, -viewDir));
	  specular = pow(specular, 40.0f); //�ŵ�����
   }


   float3 ambient = float3(0.1f, 0.1f, 0.1f); //�ֺ���

   return float4(ambient + diffuse + specular, 1); //��� 
   
}