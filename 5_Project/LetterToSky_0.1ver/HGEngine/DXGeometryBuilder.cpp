#include <memory.h>
#include "DXGeometryBuilder.h"
#include "DXDevice.h"
#include "Vertex.h"
#include "GeometryGenerator.h"

// Object
#include "Waves.h"
#include "Hills.h"
#include "TexHills.h"

DXGeometryBuilder::DXGeometryBuilder(DXDevice* pDevice)
	: m_pD3dDevice(pDevice->GetD3dDevice()),
	m_pWaves(nullptr), m_pHills(nullptr), m_pTexHill(nullptr)
{
	m_pWaves = new Waves(pDevice);
	m_pHills = new Hills(pDevice);
	m_pTexHill = new TexHills(pDevice);
}

DXGeometryBuilder::~DXGeometryBuilder()
{

}

SGeoBuffer* DXGeometryBuilder::BuildGridGeometryBuffers()
{
	ID3D11Buffer* VB = nullptr;
	ID3D11Buffer* IB = nullptr;
	UINT indexCount = 0;

	// 원본 정점 자료를 정의한다 
	VertexInputLayout::PosColor vertices[121];
	for (int i = 0; i < 121; i++)
	{
		vertices[i].Pos = XMFLOAT3((float)(i % 11) - 5.0f, 0.0f, (float)(i / 11) - 5.0f);
		vertices[i].Color = XMFLOAT4((const float*)&Colors::DarkGray);	// (어두운 회색)
	}

	vertices[5].Color = XMFLOAT4((const float*)&Colors::Black);
	vertices[55].Color = XMFLOAT4((const float*)&Colors::Black);
	vertices[65].Color = XMFLOAT4((const float*)&Colors::Black);
	vertices[115].Color = XMFLOAT4((const float*)&Colors::Black);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosColor) * 121;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;

	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &VB));

	UINT indices[44];
	for (int i = 0; i < 11; i++)
	{
		indices[i * 2] = i;
		indices[i * 2 + 1] = i + 110;
	}

	for (int i = 0; i < 11; i++)
	{
		indices[22 + (i * 2)] = i * 11;
		indices[22 + (i * 2) + 1] = i * 11 + 10;
	}

	indexCount = 44;

	// 색인 버퍼를 서술하는 구조체를 채운다, 결속 플래그로
	// D3D11_BIND_INDEX_BUFFER를 지정한다는 점에 주목할 것
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// 색인 버퍼를 초기화할 자료를 지정한다
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// 색인 버퍼 생성
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &IB));

	// 여기에서 생성
	SGeoBuffer* newGBuffer = new SGeoBuffer();
	newGBuffer->m_VB = VB;
	newGBuffer->m_IB = IB;
	newGBuffer->m_IndexCount = indexCount;
	newGBuffer->m_VertexOffset = 0;
	newGBuffer->m_IndexOffset = 0;

	// 연관이 되는 자료구조에 넣는다.
	m_GeoBuffers.insert(make_pair(eDXObjectType::HELPER, newGBuffer));

	return newGBuffer;
}

SGeoBuffer* DXGeometryBuilder::BuildAxisGeometryBuffers()
{
	ID3D11Buffer* VB = nullptr;
	ID3D11Buffer* IB = nullptr;
	UINT indexCount = 0;

	// 원본 정점 자료를 정의한다 
	VertexInputLayout::PosColor vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },	// x축 (빨강)
		{ XMFLOAT3(5.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},	// y축 (초록)
		{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)	},	// z축 (파랑)
		{ XMFLOAT3(0.0f, 0.0f, 5.0f), XMFLOAT4((const float*)&Colors::Blue) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosColor) * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;

	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &VB));

	// 인덱스 버퍼를 생성
	UINT indices[] =
	{
		// x축
		0, 1,

		// y축
		2, 3,

		// z축
		4, 5,
	};

	indexCount = 6;

	// 색인 버퍼를 서술하는 구조체를 채운다, 결속 플래그로
	// D3D11_BIND_INDEX_BUFFER를 지정한다는 점에 주목할 것
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// 색인 버퍼를 초기화할 자료를 지정한다
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// 색인 버퍼 생성
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &IB));

	// 여기에서 새로운 Geometry Buffer 생성
	SGeoBuffer* newGBuffer = new SGeoBuffer();
	newGBuffer->m_VB = VB;
	newGBuffer->m_IB = IB;
	newGBuffer->m_IndexCount = indexCount;
	newGBuffer->m_VertexOffset = 0;
	newGBuffer->m_IndexOffset = 0;

	// 연관이 되는 자료구조에 넣는다.
	m_GeoBuffers.insert(make_pair(eDXObjectType::HELPER, newGBuffer));

	return newGBuffer;
}

SGeoBuffer* DXGeometryBuilder::BuildBoxGeometryBuffers()
{
	ID3D11Buffer* VB = nullptr;
	ID3D11Buffer* IB = nullptr;
	UINT indexCount = 0;

	// 원본 정점 자료를 정의한다 
	VertexInputLayout::PosColor vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4((const float*)&Colors::White)  },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4((const float*)&Colors::Black)  },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4((const float*)&Colors::Red)    },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4((const float*)&Colors::Green)  },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Blue)   },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Yellow) },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Cyan)   },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Magenta)}
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosColor) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;

	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &VB));

	// 인덱스 버퍼를 생성
	UINT indices[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	indexCount = 36;

	// 색인 버퍼를 서술하는 구조체를 채운다, 결속 플래그로
	// D3D11_BIND_INDEX_BUFFER를 지정한다는 점에 주목할 것
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// 색인 버퍼를 초기화할 자료를 지정한다
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// 색인 버퍼 생성
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &IB));

	// 여기에서 새로운 Geometry Buffer 생성
	SGeoBuffer* newGBuffer = new SGeoBuffer();
	newGBuffer->m_VB = VB;
	newGBuffer->m_IB = IB;
	newGBuffer->m_IndexCount = indexCount;
	newGBuffer->m_VertexOffset = 0;
	newGBuffer->m_IndexOffset = 0;

	// 연관이 되는 자료구조에 넣는다.
	m_GeoBuffers.insert(make_pair(eDXObjectType::VERTEXCOLOR, newGBuffer));

	return newGBuffer;
}

SGeoBuffer* DXGeometryBuilder::BuildCarGeometryBuffers()
{
	ID3D11Buffer* VB = nullptr;
	ID3D11Buffer* IB = nullptr;
	UINT indexCount = 0;

	std::ifstream fin("../Data/Models/Car.txt");

	//if (!fin)
	//{
	//	MessageBox(0, L"Models/Car.txt not found.", 0, 0);
	//	return false;
	//}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	float nx, ny, nz;
	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);

	std::vector<VertexInputLayout::PosColor> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;

		vertices[i].Color = black;

		// Normal not used in this demo.
		fin >> nx >> ny >> nz;
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	indexCount = 3 * tcount;
	std::vector<UINT> indices(indexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosColor) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &VB));

	// Pack the indices of all the meshes into one index buffer.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &IB));

	// 여기에서 새로운 Geometry Buffer 생성
	SGeoBuffer* newGBuffer = new SGeoBuffer();
	newGBuffer->m_VB = VB;
	newGBuffer->m_IB = IB;
	newGBuffer->m_IndexCount = indexCount;
	newGBuffer->m_VertexOffset = 0;
	newGBuffer->m_IndexOffset = 0;

	// 연관이 되는 자료구조에 넣는다.
	m_GeoBuffers.insert(make_pair(eDXObjectType::VERTEXCOLOR, newGBuffer));

	return newGBuffer;
}

SGeoBuffer* DXGeometryBuilder::BuildSkullsGeometryBuffers()
{
	ID3D11Buffer* VB = nullptr;
	ID3D11Buffer* IB = nullptr;
	UINT indexCount = 0;

	std::ifstream fin("../Data/Models/skull.txt");

	//if (!fin)
	//{
	//	MessageBox(0, L"Models/skull.txt not found.", 0, 0);
	//	return false;
	//}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	float nx, ny, nz;
	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);

	std::vector<VertexInputLayout::PosColor> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;

		vertices[i].Color = black;

		// Normal not used in this demo.
		fin >> nx >> ny >> nz;
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	indexCount = 3 * tcount;
	std::vector<UINT> indices(indexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosColor) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &VB));

	// Pack the indices of all the meshes into one index buffer.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &IB));

	// 여기에서 새로운 Geometry Buffer 생성
	SGeoBuffer* newGBuffer = new SGeoBuffer();
	newGBuffer->m_VB = VB;
	newGBuffer->m_IB = IB;
	newGBuffer->m_IndexCount = indexCount;
	newGBuffer->m_VertexOffset = 0;
	newGBuffer->m_IndexOffset = 0;

	// 연관이 되는 자료구조에 넣는다.
	m_GeoBuffers.insert(make_pair(eDXObjectType::VERTEXCOLOR, newGBuffer));

	return newGBuffer;
}

SGeoBuffer* DXGeometryBuilder::BuildWavesGeometryBuffers()
{
	ID3D11Buffer* VB = nullptr;
	ID3D11Buffer* IB = nullptr;
	UINT indexCount = 0;

	m_pWaves->Initialize();

	// Create the vertex buffer. Note that we allocate space only, as
	// we will be updating the data every time step of the simulation
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosColor) * m_pWaves->GetVertexCount();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	HR(m_pD3dDevice->CreateBuffer(&vbd, 0, &VB));

	// Create the index buffer. The index buffer is fixed, so we only 
	// need to create and set once
	std::vector<UINT> indices(3 * m_pWaves->GetTriangleCount()); // 3 indices per face

	// Iterate over each quad
	UINT m = m_pWaves->GetNumRows();
	UINT n = m_pWaves->GetNumCols();
	int k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (DWORD j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[k + 1] = i * n + j + 1;
			indices[k + 2] = (i + 1) * n + j;

			indices[k + 3] = (i + 1) * n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1) * n + j + 1;

			k += 6; // next quad
		}
	}

	indexCount = indices.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &IB));

	// 여기에서 새로운 Geometry Buffer 생성
	SGeoBuffer* newGBuffer = new SGeoBuffer();
	newGBuffer->m_VB = VB;
	newGBuffer->m_IB = IB;
	newGBuffer->m_IndexCount = indexCount;
	newGBuffer->m_VertexOffset = 0;
	newGBuffer->m_IndexOffset = 0;

	// 연관이 되는 자료구조에 넣는다.
	m_GeoBuffers.insert(make_pair(eDXObjectType::VERTEXCOLOR, newGBuffer));

	return newGBuffer;
}

SGeoBuffer* DXGeometryBuilder::BuildHillsGeometryBuffers()
{
	ID3D11Buffer* VB = nullptr;
	ID3D11Buffer* IB = nullptr;
	UINT indexCount = 0;

	GeometryGenerator::MeshData grid;

	GeometryGenerator geoGen;

	geoGen.CreateGrid(160.0f, 160.0f, 50, 50, grid);

	indexCount = grid.Indices.size();

	// Extract the vertex elements we are interested and apply the height function to
	// each vertex.  In addition, color the vertices based on their height so we have
	// sandy looking beaches, grassy low hills, and snow mountain peaks.
	std::vector<VertexInputLayout::PosColor> vertices(grid.Vertices.size());
	for (size_t i = 0; i < grid.Vertices.size(); ++i)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;

		p.y = m_pHills->GetHeight(p.x, p.z);

		vertices[i].Pos = p;

		// Color the vertex based on its height.
		if (p.y < -10.0f)
		{
			// Sandy beach color.
			vertices[i].Color = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
		}
		else if (p.y < 5.0f)
		{
			// Light yellow-green.
			vertices[i].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		}
		else if (p.y < 12.0f)
		{
			// Dark yellow-green.
			vertices[i].Color = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
		}
		else if (p.y < 20.0f)
		{
			// Dark brown.
			vertices[i].Color = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
		}
		else
		{
			// White snow.
			vertices[i].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosColor) * grid.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &VB));

	// Pack the indices of all the meshes into one index buffer.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &grid.Indices[0];
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &IB));

	// 여기에서 새로운 Geometry Buffer 생성
	SGeoBuffer* newGBuffer = new SGeoBuffer();
	newGBuffer->m_VB = VB;
	newGBuffer->m_IB = IB;
	newGBuffer->m_IndexCount = indexCount;
	newGBuffer->m_VertexOffset = 0;
	newGBuffer->m_IndexOffset = 0;

	// 연관이 되는 자료구조에 넣는다.
	m_GeoBuffers.insert(make_pair(eDXObjectType::VERTEXCOLOR, newGBuffer));

	return newGBuffer;
}

SGeoBuffer* DXGeometryBuilder::BuildMikeWazowskisGeometryBuffers()
{
	ID3D11Buffer* VB = nullptr;
	ID3D11Buffer* IB = nullptr;
	UINT indexCount = 0;

	// 버텍스 버퍼를 생성한다
	VertexInputLayout::PosColor vertices[] =
	{
		{ XMFLOAT3(-0.0000f, -21.8276f,	51.462f),	XMFLOAT4((const float*)&Colors::Black) },
		{ XMFLOAT3(-0.0000f, -19.0703f,	59.2249f),	XMFLOAT4((const float*)&Colors::White) },
		{ XMFLOAT3(-8.5496f, -19.0703f,	55.6836f),	XMFLOAT4((const float*)&Colors::White) },
		{ XMFLOAT3(-12.0909f, -19.0703f, 47.1340f),	XMFLOAT4((const float*)&Colors::White) },
		{ XMFLOAT3(-8.5496f, -19.0703f,	38.5845f),	XMFLOAT4((const float*)&Colors::White) },
		{ XMFLOAT3(0.0000f,	-19.0703f, 35.0431f),	XMFLOAT4((const float*)&Colors::White) },
		{ XMFLOAT3(8.5496f,	-19.0703f, 38.5845f),	XMFLOAT4((const float*)&Colors::White) },
		{ XMFLOAT3(12.0909f, -19.0703f,	47.1340f),	XMFLOAT4((const float*)&Colors::White) },
		{ XMFLOAT3(8.5496f,	-19.0703f, 55.6836f),	XMFLOAT4((const float*)&Colors::White) },
		{ XMFLOAT3(-0.0000f, -10.6389f,	61.2146f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-10.2615f, -10.6389f, 57.0905f), XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-14.5120f, -10.6389f, 47.1340f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-10.2615f, -10.6389f, 37.1775f), XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(0.0000f,	-10.6389f, 33.0534f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(10.2615f, -10.6389f,	37.1775f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(14.5120f, -10.6389f,	47.1340f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(10.2615f, -10.6389f,	57.0905f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-3.06f, -21.8276f, 50.194f),		XMFLOAT4((const float*)&Colors::Black) },
		{ XMFLOAT3(-4.328f, -21.8276f, 47.134f),	XMFLOAT4((const float*)&Colors::Black) },
		{ XMFLOAT3(-3.06f, -21.8276f, 44.074f),		XMFLOAT4((const float*)&Colors::Black) },
		{ XMFLOAT3(0.0000f,	-21.8276f, 42.806f),	XMFLOAT4((const float*)&Colors::Black) },
		{ XMFLOAT3(3.06f, -21.8276f, 44.074f),		XMFLOAT4((const float*)&Colors::Black) },
		{ XMFLOAT3(4.328f, -21.8276f, 47.134f),		XMFLOAT4((const float*)&Colors::Black) },
		{ XMFLOAT3(3.06f, -21.8276f, 50.194f),		XMFLOAT4((const float*)&Colors::Black) },
		{ XMFLOAT3(-0.0000f, -23.1756f,	47.1340f),	XMFLOAT4((const float*)&Colors::SeaGreen) },
		{ XMFLOAT3(-0.0000f, -10.6389f,	47.1340f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(9.7936f,	-12.8012f, 63.0949f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(11.1716f, 0.0790f, 65.5526f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(10.9969f, 12.9591f, 63.0949f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(-0.0000f, 16.9266f, 63.4588f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(15.7203f, -16.9924f, 45.0263f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(22.8937f, -2.3908f, 43.2949f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(18.6844f, 17.1503f, 45.0263f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-0.0000f, 23.1756f, 45.5606f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(10.8816f, -11.9923f,	26.9906f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(15.5812f, 0.0790f, 26.8726f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(12.0713f, 12.1502f, 28.5308f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(0.0000f,	0.0790f, 23.2576f),		XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(13.3933f, -2.5614f, 25.8171f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(10.8845f, 0.0790f, 25.3183f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(13.3933f, 2.7193f, 25.8171f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(16.3595f, 0.0790f, 17.6294f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(14.5085f, -3.0468f, 16.8317f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(12.6041f, 0.0790f, 16.7308f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(14.5085f, 3.2047f, 16.8317f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(15.3450f, 0.0790f, 7.2620f),		XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(13.3835f, -3.0468f, 7.2523f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(11.4905f, 0.0790f, 7.2571f),		XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(13.3835f, 3.2047f, 7.2523f),		XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(19.1949f, -4.9066f, 0.0002f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(13.3497f, -15.5750f,	-0.0001f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(7.7083f,	-4.9066f, 0.0000f),		XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(13.3497f, 5.7617f, -0.0001f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(12.9371f, -2.5001f, 64.0223f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(14.3983f, 0.0790f, 62.2755f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(12.9371f, 2.6580f, 64.0223f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(14.8238f, 0.0790f, 69.1037f),	XMFLOAT4((const float*)&Colors::LightGoldenrodYellow) },
		{ XMFLOAT3(15.5440f, -0.9732f, 68.4794f),	XMFLOAT4((const float*)&Colors::LightGoldenrodYellow) },
		{ XMFLOAT3(16.1401f, 0.0790f, 67.7668f),	XMFLOAT4((const float*)&Colors::LightGoldenrodYellow) },
		{ XMFLOAT3(15.5440f, 1.1311f, 68.4794f),	XMFLOAT4((const float*)&Colors::LightGoldenrodYellow) },
		{ XMFLOAT3(0.0000f,	0.0790f, 68.3411f),		XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(22.0108f, 0.0790f, 40.8069f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(22.6883f, 2.4805f, 43.3231f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(23.6312f, 0.0790f, 45.6405f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(25.5701f, -2.3908f, 36.5555f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(24.1121f, 0.0790f, 35.8729f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(25.5396f, 2.4805f, 36.7368f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(26.6876f, 0.0790f, 37.6278f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(26.4224f, -2.3908f, 15.9691f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(25.1877f, 0.0790f, 17.0021f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(26.5738f, 2.4805f, 16.0734f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(27.8666f, 0.0790f, 15.4095f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(0.0000f,	-15.7184f, 64.1530f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(-9.7936f, -12.8012f,	63.0949f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(-12.9371f, -2.5001f,	64.0223f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(-11.1716f, 0.0790f, 65.5526f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(-12.9371f, 2.6580f, 64.0223f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(-10.9969f, 12.9591f,	63.0949f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(0.0000f,	-22.2506f, 45.9496f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(-15.7203f, -16.9924f, 45.0263f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(-22.8937f, -2.3908f, 43.2949f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-23.6312f, 0.0790f, 45.6405f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-14.3983f, 0.0790f, 62.2755f),	XMFLOAT4((const float*)&Colors::YellowGreen) },
		{ XMFLOAT3(-22.6883f, 2.4805f, 43.3231f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-18.6844f, 17.1503f, 45.0263f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(0.0000f,	-15.7184f, 26.7028f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(-10.8816f, -11.9923f, 26.9906f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(-13.3933f, -2.5614f,	25.8171f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(-15.5812f, 0.0790f, 26.8726f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(-22.0108f, 0.0790f, 40.8069f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-13.3933f, 2.7193f, 25.8171f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(-12.0713f, 12.1502f,	28.5308f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(-0.0000f, 16.9266f, 26.5626f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(-10.8845f, 0.0790f, 25.3183f),	XMFLOAT4((const float*)&Colors::DarkOliveGreen) },
		{ XMFLOAT3(-14.5085f, -3.0468f,	16.8317f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-16.3595f, 0.0790f, 17.6294f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-12.6041f, 0.0790f, 16.7308f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-14.5085f, 3.2047f, 16.8317f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-13.3835f, -3.0468f,	7.2523f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-15.3450f, 0.0790f, 7.2620f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-11.4905f, 0.0790f, 7.2571f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-13.3835f, 3.2047f, 7.2523f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-13.3497f, -15.5750f, -0.0001f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-19.1949f, -4.9066f,	0.0002f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-7.7083f, -4.9066f, 0.0000f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-13.3497f, 5.7617f, -0.0001f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-15.5440f, -0.9732f,	68.4794f),	XMFLOAT4((const float*)&Colors::LightGoldenrodYellow) },
		{ XMFLOAT3(-14.8238f, 0.0790f, 69.1037f),	XMFLOAT4((const float*)&Colors::LightGoldenrodYellow) },
		{ XMFLOAT3(-16.1401f, 0.0790f, 67.7668f),	XMFLOAT4((const float*)&Colors::LightGoldenrodYellow) },
		{ XMFLOAT3(-15.5440f, 1.1311f, 68.4794f),	XMFLOAT4((const float*)&Colors::LightGoldenrodYellow) },
		{ XMFLOAT3(-24.1121f, 0.0790f, 35.8729f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-25.5701f, -2.3908f,	36.5555f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-25.5396f, 2.4805f, 36.7368f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-26.6876f, 0.0790f, 37.6278f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-25.1877f, 0.0790f, 17.0021f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-26.4224f, -2.3908f,	15.9691f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-26.5738f, 2.4805f, 16.0734f),	XMFLOAT4((const float*)&Colors::OliveDrab) },
		{ XMFLOAT3(-27.8666f, 0.0790f, 15.4095f),	XMFLOAT4((const float*)&Colors::OliveDrab) }
	};

	// Direct는 Y업, 3D Max는 Z업으로 모델링 data를 바로 이식하면 모델링이 눕혀져있다
	// 그래서 Pos Y값과 Pos Z값을 바꾼다
	for (int i = 0; i < 118; i++)
	{
		float temp = vertices[i].Pos.y;
		vertices[i].Pos.y = vertices[i].Pos.z;
		vertices[i].Pos.z = temp;
	}

	// 기본 크기가 너무 크게 나와 사이즈를 축소함
	for (int i = 0; i < 118; i++)
	{
		vertices[i].Pos.x /= 10.0f;
		vertices[i].Pos.y /= 10.0f;
		vertices[i].Pos.z /= 10.0f;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosColor) * 118;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &VB));

	UINT indices[] =
	{
		  2,   0,  17,
		  0,   2,   1,
		  3,  17,  18,
		 17,   3,   2,
		  4,  18,  19,
		 18,   4,   3,
		  5,  19,  20,
		 19,   5,   4,
		  6,  20,  21,
		 20,   6,   5,
		  7,  21,  22,
		 21,   7,   6,
		  8,  22,  23,
		 22,   8,   7,
		  1,  23,   0,
		 23,   1,   8,
		 10,   1,   2,
		  1,  10,   9,
		 11,   2,   3,
		  2,  11,  10,
		 12,   3,   4,
		  3,  12,  11,
		 13,   4,   5,
		  4,  13,  12,
		 14,   5,   6,
		  5,  14,  13,
		 15,   6,   7,
		  6,  15,  14,
		 16,   7,   8,
		  7,  16,  15,
		  9,   8,   1,
		  8,   9,  16,
		 24,  23,  22,
		 23,  24,   0,
		 17,  24,  18,
		 24,  17,   0,
		 19,  24,  20,
		 24,  19,  18,
		 24,  21,  20,
		 21,  24,  22,
		 25,  10,  11,
		 10,  25,   9,
		 16,  25,  15,
		 25,  16,   9,
		 14,  25,  13,
		 25,  14,  15,
		 25,  12,  13,
		 12,  25,  11,
		 60,  26,  72,
		 26,  27,  53,
		 27,  26,  60,
		 60,  55,  27,
		 55,  60,  28,
		 60,  29,  28,
		 26,  78,  72,
		 78,  26,  30,
		 30,  63,  31,
		 26,  63,  30,
		 63,  53,  54,
		 26,  53,  63,
		 54,  62,  63,
		 54,  32,  62,
		 54,  28,  32,
		 55,  28,  54,
		 33,  28,  29,
		 28,  33,  32,
		 34,  78,  30,
		 78,  34,  85,
		 34,  35,  38,
		 30,  35,  34,
		 35,  31,  61,
		 30,  31,  35,
		 61,  40,  35,
		 61,  36,  40,
		 61,  32,  36,
		 62,  32,  61,
		 92,  32,  33,
		 32,  92,  36,
		 37,  85,  34,
		 37,  38,  39,
		 38,  37,  34,
		 36,  39,  40,
		 39,  36,  37,
		 37,  36,  92,
		 35,  42,  38,
		 42,  35,  41,
		 38,  43,  39,
		 43,  38,  42,
		 39,  44,  40,
		 44,  39,  43,
		 40,  41,  35,
		 41,  40,  44,
		 41,  46,  42,
		 46,  41,  45,
		 42,  47,  43,
		 47,  42,  46,
		 43,  48,  44,
		 48,  43,  47,
		 44,  45,  41,
		 45,  44,  48,
		 45,  50,  46,
		 50,  45,  49,
		 46,  51,  47,
		 51,  46,  50,
		 47,  52,  48,
		 52,  47,  51,
		 48,  49,  45,
		 49,  48,  52,
		 51,  49,  52,
		 49,  51,  50,
		 27,  57,  53,
		 57,  27,  56,
		 53,  58,  54,
		 58,  53,  57,
		 54,  59,  55,
		 59,  54,  58,
		 55,  56,  27,
		 56,  55,  59,
		 58,  56,  59,
		 56,  58,  57,
		 31,  65,  61,
		 65,  31,  64,
		 61,  66,  62,
		 66,  61,  65,
		 62,  67,  63,
		 67,  62,  66,
		 63,  64,  31,
		 64,  63,  67,
		 64,  69,  65,
		 69,  64,  68,
		 65,  70,  66,
		 70,  65,  69,
		 66,  71,  67,
		 71,  66,  70,
		 67,  68,  64,
		 68,  67,  71,
		 69,  71,  70,
		 71,  69,  68,
		 60,  72,  73,
		 73,  75,  60,
		 75,  73,  74,
		 60,  76,  77,
		 76,  60,  75,
		 60,  77,  29,
		 78,  73,  72,
		 73,  78,  79,
		 74,  81,  82,
		 73,  81,  74,
		 81,  79,  80,
		 73,  79,  81,
		 83,  82,  81,
		 84,  82,  83,
		 77,  82,  84,
		 76,  82,  77,
		 33,  77,  84,
		 77,  33,  29,
		 78,  86,  79,
		 86,  78,  85,
		 80,  88,  89,
		 79,  88,  80,
		 88,  86,  87,
		 79,  86,  88,
		 90,  89,  88,
		 91,  89,  90,
		 84,  89,  91,
		 83,  89,  84,
		 92,  84,  91,
		 84,  92,  33,
		 37,  86,  85,
		 37,  87,  86,
		 87,  37,  93,
		 91,  93,  37,
		 93,  91,  90,
		 37,  92,  91,
		 88,  94,  95,
		 94,  88,  87,
		 87,  96,  94,
		 96,  87,  93,
		 93,  97,  96,
		 97,  93,  90,
		 90,  95,  97,
		 95,  90,  88,
		 95,  98,  99,
		 98,  95,  94,
		 94, 100,  98,
		100,  94,  96,
		 96, 101, 100,
		101,  96,  97,
		 97,  99, 101,
		 99,  97,  95,
		 99, 102, 103,
		102,  99,  98,
		 98, 104, 102,
		104,  98, 100,
		100, 105, 104,
		105, 100, 101,
		101, 103, 105,
		103, 101,  99,
		104, 103, 102,
		103, 104, 105,
		 75, 106, 107,
		106,  75,  74,
		 74, 108, 106,
		108,  74,  82,
		 82, 109, 108,
		109,  82,  76,
		 76, 107, 109,
		107,  76,  75,
		108, 107, 106,
		107, 108, 109,
		 80, 110, 111,
		110,  80,  89,
		 89, 112, 110,
		112,  89,  83,
		 83, 113, 112,
		113,  83,  81,
		 81, 111, 113,
		111,  81,  80,
		111, 114, 115,
		114, 111, 110,
		110, 116, 114,
		116, 110, 112,
		112, 117, 116,
		117, 112, 113,
		113, 115, 117,
		115, 113, 111,
		114, 117, 115,
		117, 114, 116
	};

	indexCount = 684;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &IB));

	// 여기에서 새로운 Geometry Buffer 생성
	SGeoBuffer* newGBuffer = new SGeoBuffer();
	newGBuffer->m_VB = VB;
	newGBuffer->m_IB = IB;
	newGBuffer->m_IndexCount = indexCount;
	newGBuffer->m_VertexOffset = 0;
	newGBuffer->m_IndexOffset = 0;

	// 연관이 되는 자료구조에 넣는다.
	m_GeoBuffers.insert(make_pair(eDXObjectType::VERTEXCOLOR, newGBuffer));

	return newGBuffer;
}

SGeoBuffer* DXGeometryBuilder::BuildTexBoxGeometryBuffers()
{
	ID3D11Buffer* VB = nullptr;
	ID3D11Buffer* IB = nullptr;
	UINT indexCount = 0;
	int vertexOffset;
	UINT indexOffset;

	GeometryGenerator::MeshData box;

	GeometryGenerator geoGen;
	geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);

	// Cache the vertex offsets to each object in the concatenated vertex buffer
	vertexOffset = 0;

	// Cache the index count of each object
	indexCount = box.Indices.size();

	// Cache the starting index for each object in the concatenated index buffer
	indexOffset = 0;

	UINT totalVertexCount = box.Vertices.size();

	UINT totalIndexCount = indexCount;

	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer
	std::vector<VertexInputLayout::PosNomTexture> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].Texture = box.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosNomTexture) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &VB));

	// Pack the indices of all the meshes into one index buffer
	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &IB));

	// 여기에서 새로운 Geometry Buffer 생성
	SGeoBuffer* newGBuffer = new SGeoBuffer();
	newGBuffer->m_VB = VB;
	newGBuffer->m_IB = IB;
	newGBuffer->m_IndexCount = indexCount;
	newGBuffer->m_VertexOffset = vertexOffset;
	newGBuffer->m_IndexOffset = indexOffset;

	// 연관이 되는 자료구조에 넣는다.
	m_GeoBuffers.insert(make_pair(eDXObjectType::VERTEXCOLOR, newGBuffer));

	return newGBuffer;
}

SGeoBuffer* DXGeometryBuilder::BuuildTexHillGeometryBuffers()
{
	ID3D11Buffer* VB = nullptr;
	ID3D11Buffer* IB = nullptr;
	UINT indexCount = 0;

	GeometryGenerator::MeshData grid;

	GeometryGenerator geoGen;

	geoGen.CreateGrid(160.0f, 160.0f, 50, 50, grid);

	indexCount = grid.Indices.size();

	// Extract the vertex elements we are interested and apply the height function to
	// each vertex
	std::vector<VertexInputLayout::PosNomTexture> vertices(grid.Vertices.size());
	for (size_t i = 0; i < grid.Vertices.size(); ++i)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;

		p.y = m_pTexHill->GetHillHeight(p.x, p.z);

		vertices[i].Pos = p;
		vertices[i].Normal = m_pTexHill->GetHillNormal(p.x, p.z);
		vertices[i].Texture = grid.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosNomTexture) * grid.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &VB));

	// Pack the indices of all the meshes into one index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &grid.Indices[0];
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &IB));

	// 여기에서 새로운 Geometry Buffer 생성
	SGeoBuffer* newGBuffer = new SGeoBuffer();
	newGBuffer->m_VB = VB;
	newGBuffer->m_IB = IB;
	newGBuffer->m_IndexCount = indexCount;

	// 연관이 되는 자료구조에 넣는다.
	m_GeoBuffers.insert(make_pair(eDXObjectType::VERTEXCOLOR, newGBuffer));

	return newGBuffer;
}

SGeoBuffer* DXGeometryBuilder::BuildBitmapGeometryBuffers()
{
	ID3D11Buffer* VB = nullptr;
	ID3D11Buffer* IB = nullptr;
	UINT indexCount = 0;
	int vertexCount = 0;

	// 정점 배열의 정점 수와 인덱스 배열의 인덱스 수를 지정합니다.
	indexCount = vertexCount = 6;

	// 정점 배열을 만듭니다.
	VertexInputLayout::PosTexture* vertices = new VertexInputLayout::PosTexture[vertexCount];

	// 정점 배열을 0으로 초기화합니다.
	memset(vertices, 0, (sizeof(VertexInputLayout::PosTexture) * vertexCount));

	// 인덱스 배열을 만듭니다.
	unsigned long* indices = new unsigned long[indexCount];

	// 데이터로 인덱스 배열을 로드합니다.
	for (UINT i = 0; i < indexCount; i++)
	{
		indices[i] = i;
	}

	// 정적 정점 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexInputLayout::PosTexture) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 이제 정점 버퍼를 만듭니다.
	HR(m_pD3dDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &VB));

	// 정적 인덱스 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성합니다.
	HR(m_pD3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &IB));

	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제합니다.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	// 여기에서 새로운 Geometry Buffer 생성
	SGeoBuffer* newGBuffer = new SGeoBuffer();
	newGBuffer->m_VB = VB;
	newGBuffer->m_IB = IB;
	newGBuffer->m_IndexCount = indexCount;
	newGBuffer->m_VertexCount = vertexCount;

	// 연관이 되는 자료구조에 넣는다.
	m_GeoBuffers.insert(make_pair(eDXObjectType::VERTEXCOLOR, newGBuffer));

	return newGBuffer;
}