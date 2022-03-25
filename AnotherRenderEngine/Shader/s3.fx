#include "LightHelper.fx"
cbuffer CBMatrix:register(b0)//b0 ����vertex constant buff
{
	//row_major matrix gWorld;	// Ĭ����������
matrix World;
matrix View;
matrix Proj;
};
// ƬԪ��ɫ���ĳ�������
cbuffer cbPerFrame:register(b1)//b1 ���� pixel constant buff
{
	DirectionalLight gDirLight;	// ֱ���
	PointLight gPointLight;	// ���Դ
	SpotLight gSpotLight;	// �۹��
	Material gMaterial;	// �������
	float3 gEyePosW;	// ��ǰ�㵽�������
};
cbuffer cbPre2:register(b2)//����cbPerFrame �ṹ�崫��������ֱ����floa4����
{
	float4 gLiteColor;
};

////////////// ǰ4�½��� //////////////////////////
Texture2D ShaderTexture:register(t0);  
SamplerState SampleType:register(s0);


struct VertexIn
{
	float4 Pos:POSITION;
	//float4 color:COLOR;//TODO:��color�ᵼ��uv���������⡣��������������������
    float2 uv : TEXCOORD0;
	float3 normal:NORMAL;
};


struct VertexOut
{
	float4 Pos:SV_POSITION;
	float4 color:COLOR;
	float2 TextureUV:TEXCOORD0;   // vertex texture coords 
	float3 normal:NORMAL;
};


VertexOut VS(VertexIn ina)
{
	VertexOut outa;
	outa.Pos = mul(ina.Pos, World);
	outa.Pos = mul(outa.Pos, View);
	outa.Pos = mul(outa.Pos, Proj);
	//outa.color = ina.color;//����
	outa.TextureUV = ina.uv;//����
	//outa.TextureUV = ina.normal;
	outa.normal = ina.normal;
	outa.color = float4(ina.normal * 0.5+float3(0.5,0.5,0.5),1);
	return outa;
}

float4 PS(VertexOut outa) : SV_Target
{
	float4 l;
	//DirectionalLight litStruct;
	//litStruct.Direction = gLiteColor.xyz;
	//ComputeLight(litStruct, outa.normal, l);//����DirectionLight�ṹ��LightHelper.fx���壩
	
	//litStruct.Direction = gLiteColor;//����c++ֱ�Ӵ��� gLiteColor ����
	ComputeLight(gDirLight, outa.normal, l);
// 
// 
	//l = l * gDirLight.Ambient;
	float4 color = ShaderTexture.Sample(SampleType,outa.TextureUV);//������ͼ��ʵ��û�õĴ��룬��Ϊ����һ�����Ḳ�ǵ����
	//color = outa.color;//ʹ�ö�����ɫ
	// 
	//return color;

	return color;
	//return float4(gLiteColor.xyz,1);
	return l;
}