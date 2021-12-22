#pragma once
#include "IDXObject.h"
#include "Vertex.h"
#include "Effects.h"

class GameTimer;

/// <summary>
/// Color Object
/// 
/// Position과 Color값으로 구성되어있다
/// 2021. 06. 09 Hacgeum
/// </summary>
class ColorObject : public IDXObject
{
public:
	ColorObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~ColorObject();

public:
	virtual void Initialize();
	virtual void Render();

private:
	virtual void BuildGeometryBuffers() abstract;
};
