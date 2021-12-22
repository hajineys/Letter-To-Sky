#pragma once
#include "VertexColorObject.h"

class Hills : public VertexColorObject
{
public:
	Hills(DXDevice* pDevice);
	~Hills();

	float GetHeight(float x, float z);

private:
	virtual void LoadGeometry(SGeoBuffer* geoBuffer) override;
};
