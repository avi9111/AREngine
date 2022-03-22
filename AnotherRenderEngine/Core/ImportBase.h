/*  测试 include 而已，无实际作用的类*/
#pragma once
//有毒不能用
//#include <DirectXMath.h>
//using namespace DirectX;
//用的很旧的 d3d,而用xnamth之前必须include <d3d11.h>
#include<d3d11.h>
#include<xnamath.h>
class ImportBase
{
public:
	void ReadVectorPosition(XMFLOAT3 pos);
};

