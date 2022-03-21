#pragma once
#ifndef _TEX_CLASS_H
#define _TEX_CLASS_H

#include<d3d11.h>
#include<D3DX11.h> //创建纹理资源的API
#include "D3DClass.h"
class TexClass
{
private:
	ID3D11ShaderResourceView* mTexture;

public:
	TexClass();
	TexClass(const TexClass&);
	~TexClass();

public:
	bool Initilize(ID3D11Device* d3dDevice, WCHAR const* TextureFilename);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();

};
#endif // !_TEX_CLASS_H
