#include "LightHelper.fx"
cbuffer CBMatrix:register(b0)//b0 代表vertex constant buff
{
	//row_major matrix gWorld;	// 默认列主矩阵
matrix World;
matrix View;
matrix Proj;
matrix WorldInvTranpose;

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
	//outa.color = ina.color;//测试，顶点色，没有的
	outa.TextureUV = ina.uv;//测试 通过
	//outa.normal = ina.normal;
	outa.color = float4(ina.normal * 0.5+float3(0.5,0.5,0.5),1);//之前测试color 输出normal 用的

	//出处
	//https://blog.csdn.net/qq_29523119/article/details/52776960?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522164822757016782089345647%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fblog.%2522%257D&request_id=164822757016782089345647&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_ecpm_v1~rank_v31_ecpm-1-52776960.nonecase&utm_term=normal+map&spm=1018.2226.3001.4450
	//将法线量由局部空间变换到世界空间，并进行规格化
	outa.Normal_W = mul(ina.normal, (float3x3)WorldInvTranpose);
	//outa.Normal_W = mul(ina.normal, (float3x3)World);
	outa.Normal_W = normalize(outa.Normal_W);
	
	//将切向量由局部空间变换到世界空间，并且进行规格化
	outa.Tangent_W = mul(ina.tangent, (float3x3)World);
	outa.Tangent_W = normalize(outa.Tangent_W);

	//将切向量由局部空间变换到世界空间，并且进行规格化
	//outa.Binormal_W = mul(ina.Binormal, (float3x3)World);
	//outa.Binormal_W = normalize(outa.Binormal_W);
	outa.Binormal_W = cross(outa.Normal_W, outa.Tangent_W) * ina.tangent.w;

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
	//float4 l;
	//ComputeLight(gDirLight, i.normal, l);//计算光线强度

	//fixed3 lightDir = normalize(UnityWorldSpaceLightDir(worldPos));
	float3 lightDir = gDirLight.Direction;
	float4 color = ShaderTexture.Sample(SampleType,i.TextureUV);//测试贴图，实际没用的代码，因为下面一句代码会覆盖掉这句
	//float4 color = float4(0.5, 0.5, 0.5, 1);
	float3 bump = UnpackNormalmapRGorAG(_BumpMap.Sample(_BumpMapType, i.TextureUV));
	bump = (2.0f * bump) - 1.0f;

	//-----求出TBN矩阵（已经和世界变换矩阵结合在一起）--------
	float3 N = i.Normal_W;
	float3 T = i.Tangent_W;
	float3 B = i.Binormal_W;

	//将隆起法向量由切线空间变换到局部空间，再到世界空间,然后规格化
	bump = mul(bump, float3x3(T, B, N));
	bump = normalize(bump);


	float DiffuseFactor = saturate(dot(bump, gDirLight.Direction));
	color = color + float4(1, 1, 1, 1) * DiffuseFactor;
	return color;
}