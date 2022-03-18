

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
	float4 color:COLOR;
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
	outa.color = ina.color;
	return outa;
}

float4 PS(VertexOut outa) : SV_Target
{
	//return outa.color*0.5f;
	return outa.color;
}