//
//Texture2D ShaderTexture;
//SamplerState SampleType
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = Wrap;
//    AddressV = Wrap;
//};
Texture2D ShaderTexture:register(t0);  
SamplerState SampleType:register(s0);
//VertexShader
cbuffer CBMatrix:register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
};

struct VertexIn
{
	float4 Pos:POSITION;
	//float4 color:COLOR;//TODO:有color会导致uv采样的问题。。。。。。。。。。。
    float2 uv : TEXCOORD0;
	float4 normal:TEXCOORD1;
};


struct VertexOut
{
	float4 Pos:SV_POSITION;
	float4 color:COLOR;
	float2 TextureUV:TEXCOORD0;   // vertex texture coords 
};


VertexOut VS(VertexIn ina)
{
	VertexOut outa;
	outa.Pos = mul(ina.Pos, World);
	outa.Pos = mul(outa.Pos, View);
	outa.Pos = mul(outa.Pos, Proj);
	//outa.color = ina.color;//测试
	//outa.TextureUV = ina.uv;//测试
	outa.TextureUV = ina.normal;
	outa.color = ina.normal * 0.5+float4(0.5,0.5,0.5,1);
	return outa;
}

float4 PS(VertexOut outa) : SV_Target
{
	float4 color = ShaderTexture.Sample(SampleType,outa.TextureUV);//测试贴图，实际没用的代码，因为下面一句代码会覆盖掉这句
	color = outa.color;
	return color;
}