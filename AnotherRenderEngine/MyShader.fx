

//VertexShader
cbuffer CBMatrix:register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
};
/*对c++不熟悉，所以必须对应 CommomVertexFormat.h 的结构提 VertexPCNTT
虽然做到到反射Shader Input,不会造成Input的溢出，健壮性增加了
但是没法做到动态映射C++ 的 Struct
所以还需要同步 VertexPCNTT*/
struct VertexIn
{
	float4 Pos:POSITION;
	float2 TextureUV:TEXCOORD0;
	float4 normal:TEXCOORD1;

	//float4 color:COLOR;//加上之后，因为没解耦。。。。旧加载方法（会提示failed，并完全失败）
};


struct VertexOut
{
	float4 Pos:SV_POSITION;
	float4 color:COLOR;
};


VertexOut VS(VertexIn ina)
{
	VertexOut outa;
	outa.Pos = mul(ina.Pos, World);
	outa.Pos = mul(outa.Pos, View);
	outa.Pos = mul(outa.Pos, Proj);
	//outa.color = ina.color;
	outa.color = float4(ina.TextureUV,0,1);
	return outa;
}

float4 PS(VertexOut outa) : SV_Target
{
	//return outa.color*0.5f;
	return outa.color;
}