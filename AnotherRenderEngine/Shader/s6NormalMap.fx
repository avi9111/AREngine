#include "LightHelper.fx"
cbuffer CBMatrix:register(b0)//b0 ����vertex constant buff
{
	//row_major matrix gWorld;	// Ĭ����������
matrix World;
matrix View;
matrix Proj;
matrix WorldInvTranpose;

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
Texture2D _BumpMap:register(t1);
SamplerState _BumpMapType:register(s1);

struct VertexIn
{
	float4 Pos:POSITION;
	//float4 color:COLOR;//TODO:��color�ᵼ��uv���������⡣��������������������
    float2 uv : TEXCOORD0;
	float3 normal:NORMAL;
	float4 tangent : TANGENT;
};


struct VertexOut
{
	float4 Pos:SV_POSITION;
	float4 color:COLOR;
	float2 TextureUV:TEXCOORD0;   // vertex texture coords 
	//float3 normal:NORMAL;
	float3 Normal_W:NORMAL;
	float3 Tangent_W:TANGENT;
	float3 Binormal_W:BINORMAL;
};


VertexOut VS(VertexIn ina)
{
	VertexOut outa;
	outa.Pos = mul(ina.Pos, World);
	outa.Pos = mul(outa.Pos, View);
	outa.Pos = mul(outa.Pos, Proj);
	//outa.color = ina.color;//���ԣ�����ɫ��û�е�
	outa.TextureUV = ina.uv;//���� ͨ��
	//outa.normal = ina.normal;
	outa.color = float4(ina.normal * 0.5+float3(0.5,0.5,0.5),1);//֮ǰ����color ���normal �õ�

	//����
	//https://blog.csdn.net/qq_29523119/article/details/52776960?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522164822757016782089345647%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fblog.%2522%257D&request_id=164822757016782089345647&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_ecpm_v1~rank_v31_ecpm-1-52776960.nonecase&utm_term=normal+map&spm=1018.2226.3001.4450
	//���������ɾֲ��ռ�任������ռ䣬�����й��
	outa.Normal_W = mul(ina.normal, (float3x3)WorldInvTranpose);
	//outa.Normal_W = mul(ina.normal, (float3x3)World);
	outa.Normal_W = normalize(outa.Normal_W);
	
	//���������ɾֲ��ռ�任������ռ䣬���ҽ��й��
	outa.Tangent_W = mul(ina.tangent, (float3x3)World);
	outa.Tangent_W = normalize(outa.Tangent_W);

	//���������ɾֲ��ռ�任������ռ䣬���ҽ��й��
	//outa.Binormal_W = mul(ina.Binormal, (float3x3)World);
	//outa.Binormal_W = normalize(outa.Binormal_W);
	outa.Binormal_W = cross(outa.Normal_W, outa.Tangent_W) * ina.tangent.w;

	return outa;
}
//unpack ����
//https://blog.csdn.net/wodownload2/article/details/105298282
float3 UnpackNormalmapRGorAG(float4 packednormal)
{
	// This do the trick
	packednormal.x *= packednormal.w;

	float3 normal;
	normal.xy = packednormal.xy * 2 - 1;
	normal.z = sqrt(1 - saturate(dot(normal.xy, normal.xy)));
	return normal;
}

float4 PS(VertexOut i) : SV_Target
{
	//float4 l;
	//ComputeLight(gDirLight, i.normal, l);//�������ǿ��

	//fixed3 lightDir = normalize(UnityWorldSpaceLightDir(worldPos));
	float3 lightDir = gDirLight.Direction;
	float4 color = ShaderTexture.Sample(SampleType,i.TextureUV);//������ͼ��ʵ��û�õĴ��룬��Ϊ����һ�����Ḳ�ǵ����
	//float4 color = float4(0.5, 0.5, 0.5, 1);
	float3 bump = UnpackNormalmapRGorAG(_BumpMap.Sample(_BumpMapType, i.TextureUV));
	bump = (2.0f * bump) - 1.0f;

	//-----���TBN�����Ѿ�������任��������һ��--------
	float3 N = i.Normal_W;
	float3 T = i.Tangent_W;
	float3 B = i.Binormal_W;

	//��¡�����������߿ռ�任���ֲ��ռ䣬�ٵ�����ռ�,Ȼ����
	bump = mul(bump, float3x3(T, B, N));
	bump = normalize(bump);


	float DiffuseFactor = saturate(dot(bump, gDirLight.Direction));
	color = color + float4(1, 1, 1, 1) * DiffuseFactor;
	return color;
}