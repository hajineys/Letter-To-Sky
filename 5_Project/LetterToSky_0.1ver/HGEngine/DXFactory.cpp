#include "DXFactory.h"
#include "DirectXDefine.h"
#include "DXDevice.h"
#include "DXGeometryBuilder.h"
#include "ASEParser.h"

// Helper Object
#include "HelperObject.h"

// VertexColor Object
#include "VertexColorObject.h"

// Texture Object
#include "TextureObject.h"
#include "TexWave.h"
#include "TexHills.h"

// Lighting 예제
#include "LightingHillAndWave.h"
#include "LightingSkull.h"

// ASE Object
#include "ASEObject.h"

// Bitmap Object
#include "BitmapObject.h"

DXFactory::DXFactory(DXDevice* pDirectX)
	:m_pDirectX(pDirectX),
	m_pDXGeometry(nullptr),
	m_pASEParser(nullptr)
{
	m_pDXGeometry = new DXGeometryBuilder(pDirectX);

	// ASE Parser 생성
	m_pASEParser = new CASEParser;
	m_pASEParser->Init();
}

DXFactory::~DXFactory()
{
	SafeDelete(m_pASEParser);
}

IDXObjectBase* DXFactory::CreateHelperObject(eDXHelperObject object)
{
	IDXObjectBase* newObject = nullptr;

	switch (object)
	{
	case eDXHelperObject::GRID:
	{
		newObject = new HelperObject(m_pDirectX);
		SGeoBuffer* newGBuffer = m_pDXGeometry->BuildGridGeometryBuffers();
		newObject->LoadGeometry(newGBuffer);
	}
	break;

	case eDXHelperObject::AXIS:
	{
		newObject = new HelperObject(m_pDirectX);
		SGeoBuffer* newGBuffer = m_pDXGeometry->BuildAxisGeometryBuffers();
		newObject->LoadGeometry(newGBuffer);
	}
	break;
	}

	return newObject;
}

IDXObjectBase* DXFactory::CreateVertexColorObject(eDXVertexColorObject object)
{
	IDXObjectBase* newObject = nullptr;

	switch (object)
	{
	case eDXVertexColorObject::BOX:
	{
		newObject = new VertexColorObject(m_pDirectX);
		SGeoBuffer* newGBuffer = m_pDXGeometry->BuildBoxGeometryBuffers();
		newObject->LoadGeometry(newGBuffer);
	}
	break;

	case eDXVertexColorObject::CAR:
	{
		newObject = new VertexColorObject(m_pDirectX);
		SGeoBuffer* newGBuffer = m_pDXGeometry->BuildCarGeometryBuffers();
		newObject->LoadGeometry(newGBuffer);
	}
	break;

	case eDXVertexColorObject::SKULL:
	{
		newObject = new VertexColorObject(m_pDirectX);
		SGeoBuffer* newGBuffer = m_pDXGeometry->BuildSkullsGeometryBuffers();
		newObject->LoadGeometry(newGBuffer);
	}
	break;

	case eDXVertexColorObject::WAVES:
	{
		newObject = new VertexColorObject(m_pDirectX);
		SGeoBuffer* newGBuffer = m_pDXGeometry->BuildWavesGeometryBuffers();
		newObject->LoadGeometry(newGBuffer);
	}
	break;

	case eDXVertexColorObject::HILLS:
	{
		newObject = new VertexColorObject(m_pDirectX);
		SGeoBuffer* newGBuffer = m_pDXGeometry->BuildHillsGeometryBuffers();
		newObject->LoadGeometry(newGBuffer);
	}
	break;

	case eDXVertexColorObject::MIKEWAZOWSKI:
	{
		newObject = new VertexColorObject(m_pDirectX);
		SGeoBuffer* newGBuffer = m_pDXGeometry->BuildMikeWazowskisGeometryBuffers();
		newObject->LoadGeometry(newGBuffer);
	}
	break;
	}

	return newObject;
}

IDXObjectBase* DXFactory::CreateTextureObject(eDXTextureObject object, const wchar_t* fileName)
{
	IDXObjectBase* newObject = nullptr;

	switch (object)
	{
	case eDXTextureObject::TBOX:
	{
		newObject = new TextureObject(m_pDirectX);
		SGeoBuffer* newGBuffer = m_pDXGeometry->BuildTexBoxGeometryBuffers();
		newObject->LoadGeometry(newGBuffer);
		newObject->LoadTextureFile(fileName);
	}
	break;

	case eDXTextureObject::THILLS:
	{
		newObject = new TextureObject(m_pDirectX);
		SGeoBuffer* newGBuffer = m_pDXGeometry->BuuildTexHillGeometryBuffers();
		newObject->LoadGeometry(newGBuffer);
		newObject->LoadTextureFile(fileName);
	}
	break;
	}

	return newObject;
}

IDXObjectBase* DXFactory::CreateASEObject(LPSTR pFile, const wchar_t* textureFile)
{
	IDXObjectBase* newObject = new ASEObject(m_pDirectX);

	m_pASEParser->Load(pFile);
	newObject->Initialize();
	newObject->LoadGeometry(m_pASEParser);
	newObject->LoadTextureFile(textureFile);
	return newObject;
}

IDXObjectBase* DXFactory::CreateBitmapObject(int bitmapWidth, int bitmapHeight, const wchar_t* textureFile)
{
	IDXObjectBase* newObject = new BitmapObject(m_pDirectX);
	newObject->Initialize(bitmapWidth, bitmapHeight);
	SGeoBuffer* newGBuffer = m_pDXGeometry->BuildBitmapGeometryBuffers();
	newObject->LoadGeometry(newGBuffer);
	newObject->LoadTextureFile(textureFile);
	return newObject;
}

void DXFactory::Finalize()
{
	delete m_pASEParser;
	m_pASEParser = nullptr;

	delete m_pDXGeometry;
	m_pDXGeometry = nullptr;

	delete m_pDirectX;
	m_pDirectX = nullptr;
}