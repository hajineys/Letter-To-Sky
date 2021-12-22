#include "HelperObject.h"
#include "DirectXStruct.h"

HelperObject::HelperObject(DXDevice* pDevice)
	: VertexColorObject(pDevice)
{
	mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}

HelperObject::~HelperObject()
{

}
