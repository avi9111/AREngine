#pragma once
#include <Windows.h>
//#include <DirectXMath.h>
#include <D3D11.h>
#include <xnamath.h>
struct DirectionalLight
{
	DirectionalLight()
	{
		ZeroMemory(this, sizeof(this));
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad;	// Ìî²¹×îºóÒ»Î»£¬±£Ö¤128bit¶ÔÆë

};
struct Material
{
	Material() {
		ZeroMemory(this, sizeof(this));
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT4 Reflect;

};
struct PointLight
{
	PointLight()
	{
		ZeroMemory(this, sizeof(this));
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 Att;
	float Pad;
};

struct SpotLight
{
	SpotLight()
	{
		ZeroMemory(this, sizeof(this));
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Position;
	float Range;
	XMFLOAT3 Direction;
	float Spot;
	XMFLOAT3 Att;
	float Pad;
};
class Light
{
	//struct DirectionalLight
	//{
	//	float4 Ambient;
	//	float4 Diffuse;
	//	float4 Specular;
	//	float3 Direction;
	//	float pad;
	//};


	//cbuffer cbPerFrame
	//{
	//DirectionalLight gDirLight;
	////PointLight gPointLight;
	////SpotLight gSpotLight;
	////float3 gEyePosW;
	//};

};

//class DirectionalLight
//{
//
//};

