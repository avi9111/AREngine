#include "LightHelper.fx"
cbuffer CBMatrix:register(b0)//b0 代表vertex constant buff
{
	//row_major matrix gWorld;	// 默认列主矩阵
matrix World;
matrix View;
matrix Proj;
};
// 片元着色器的常量缓存
cbuffer cbPerFrame:register(b1)//b1 代表 pixel constant buff
{
	DirectionalLight gDirLight;	// 直射光
	PointLight gPointLight;	// 点光源
	SpotLight gSpotLight;	// 聚光灯
	Material gMaterial;	// 物体材质
	float3 gEyePosW;	// 当前点到相机向量
};
cbuffer cbPre2:register(b2)//上面cbPerFrame 结构体传不过来，直接用floa4测试
{
	float4 gLiteColor;
};

////////////// 前4章节用 //////////////////////////
Texture2D ShaderTexture:register(t0);  
SamplerState SampleType:register(s0);


struct VertexIn
{
	float4 Pos:POSITION;
	//float4 color:COLOR;//TODO:有color会导致uv采样的问题。。。。。。。。。。。
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
	//outa.color = ina.color;//测试
	outa.TextureUV = ina.uv;//测试
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
	//ComputeLight(litStruct, outa.normal, l);//测试DirectionLight结构（LightHelper.fx定义）
	
	//litStruct.Direction = gLiteColor;//测试c++直接传入 gLiteColor 参数
	ComputeLight(gDirLight, outa.normal, l);
// 
// 
	//l = l * gDirLight.Ambient;
	float4 color = ShaderTexture.Sample(SampleType,outa.TextureUV);//测试贴图，实际没用的代码，因为下面一句代码会覆盖掉这句
	//color = outa.color;//使用顶点颜色
	// 
	//return color;

	return color;
	//return float4(gLiteColor.xyz,1);
	return l;
}