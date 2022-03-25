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
Texture2D _BumpMap:register(t1);
SamplerState _BumpMapType:register(s1);

struct VertexIn
{
	float4 Pos:POSITION;
	//float4 color:COLOR;//TODO:有color会导致uv采样的问题。。。。。。。。。。。
    float2 uv : TEXCOORD0;
	float3 normal:NORMAL;
	float4 tangent : TANGENT;
};


struct VertexOut
{
	float4 Pos:SV_POSITION;
	float4 color:COLOR;
	float2 TextureUV:TEXCOORD0;   // vertex texture coords 
	float3 normal:NORMAL;
	float4 TtoW0 : TEXCOORD1;
	float4 TtoW1 : TEXCOORD2;
	float4 TtoW2 : TEXCOORD3;
};


VertexOut VS(VertexIn ina)
{
	VertexOut outa;
	outa.Pos = mul(ina.Pos, World);
	outa.Pos = mul(outa.Pos, View);
	outa.Pos = mul(outa.Pos, Proj);
	//outa.color = ina.color;//测试，顶点色，没有的
	outa.TextureUV = ina.uv;//测试 通过
	outa.normal = ina.normal;
	outa.color = float4(ina.normal * 0.5+float3(0.5,0.5,0.5),1);//之前测试color 输出normal 用的

	//UnityObjectToWorldNormal 解析
	//https://blog.csdn.net/zengjunjie59/article/details/109516184
	//float3 worldPos = mul(_Object2World, v.vertex).xyz;
	float4 worldPos = outa.Pos;
	//float3 worldNormal = UnityObjectToWorldNormal(ina.normal);
	float4 worldNormal = mul(ina.Pos, World);
	worldNormal = mul(worldNormal, View);
	worldNormal = mul(worldNormal, Proj);
	worldNormal = normalize(worldNormal);
	
	//float3 worldTangent = UnityObjectToWorldDir(ina.tangent.xyz);
	float3 worldTangent = mul(ina.tangent.xyz,World);
	worldTangent = mul(worldTangent, View);
	worldTangent = mul(worldTangent, Proj);
	float3 worldBinormal = cross(worldNormal, worldTangent) * ina.tangent.w;

	outa.TtoW0 = float4(worldTangent.x, worldBinormal.x, worldNormal.x, worldPos.x);
	outa.TtoW1 = float4(worldTangent.y, worldBinormal.y, worldNormal.y, worldPos.y);
	outa.TtoW2 = float4(worldTangent.z, worldBinormal.z, worldNormal.z, worldPos.z);
	return outa;
}
//unpack 函数
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
	float4 l;
	//ComputeLight(gDirLight, i.normal, l);//计算光线强度

	//fixed3 lightDir = normalize(UnityWorldSpaceLightDir(worldPos));
	float3 lightDir = gDirLight.Direction;
	float4 color = ShaderTexture.Sample(SampleType,i.TextureUV);//测试贴图，实际没用的代码，因为下面一句代码会覆盖掉这句
	//float4 color = float4(0.5, 0.5, 0.5, 1);
	float3 bump = UnpackNormalmapRGorAG(_BumpMap.Sample(_BumpMapType, i.TextureUV));
	//bump = normalize(half3(dot(i.TtoW0.xyz, bump), dot(i.TtoW1.xyz, bump), dot(i.TtoW2.xyz, bump)));

	//fixed3 diffuse = _LightColor0.rgb * albedo * max(0, dot(bump, lightDir));
	float4 diffuse =color * max(0, dot(bump, lightDir));
	
	//float4 diffuse = color;
	//float4 color = _BumpMap.Sample(_BumpMapType, i.TextureUV);

	return diffuse;
}