//***************************************************************************************
// RenderStates.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines render state objects.  
//***************************************************************************************
#pragma once
#include "DirectXDefine.h"

/// <summary>
/// DirectX RasterizerState들을 모아놓은 클래스
/// 
/// 2021. 07. 19 Hacgeum
/// </summary>
class DXRenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	// 렌더 상태 객체들을 생성
	static ID3D11RasterizerState* m_pWireframeRS;
	static ID3D11RasterizerState* m_pSolidRS;
	static ID3D11DepthStencilState* m_pNormalDSS;
	static ID3D11RasterizerState* m_pNoCullRS;

	static ID3D11BlendState* m_pAlphaToCoverageBS;
	static ID3D11BlendState* m_pTransparentBS;

	static ID3D11DepthStencilState* m_pLessEqualDSS;
};