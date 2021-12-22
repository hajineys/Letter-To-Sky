//***************************************************************************************
// Vertex.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Defines vertex structures and input layouts.
//***************************************************************************************
#pragma once
#include "DirectXDefine.h"

namespace VertexInputLayout
{
	struct Position
	{
		XMFLOAT3 Pos;
	};

	struct PosColor
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	struct PosTexture
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Texture;
	};

	struct PosNormal
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
	};

	// Basic 32-byte vertex structure.
	struct PosNomTexture
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Texture;
	};
}

class InputLayoutDesc
{
public:
	static const D3D11_INPUT_ELEMENT_DESC Position[1];
	static const D3D11_INPUT_ELEMENT_DESC PosColor[2];
	static const D3D11_INPUT_ELEMENT_DESC PosTexture[2];
	static const D3D11_INPUT_ELEMENT_DESC PosNormal[2];
	static const D3D11_INPUT_ELEMENT_DESC PosNomTexture[3];
};

class InputLayouts
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11InputLayout* Position;
	static ID3D11InputLayout* PosColor;
	static ID3D11InputLayout* PosTexture;
	static ID3D11InputLayout* PosNormal;
	static ID3D11InputLayout* PosNomTexture;
};