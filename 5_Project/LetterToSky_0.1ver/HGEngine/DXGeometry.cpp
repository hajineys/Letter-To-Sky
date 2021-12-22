#include "DXGeometry.h"
#include "Vertex.h"

DXGeometry::DXGeometry(ID3D11Device* pDevice)
	:m_pD3dDevice(pDevice),
	mVB(0), mIB(0),
	mIndexCount(0)
{

}

DXGeometry::~DXGeometry()
{

}

void DXGeometry::BuildAxisGeometry()
{
	// ���� ���� �ڷḦ �����Ѵ� 
	VertexInputLayout::PosColor vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },	// x�� (����)
		{ XMFLOAT3(5.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},	// y�� (�ʷ�)
		{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)	},	// z�� (�Ķ�)
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

	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
	m_VBVector.push_back(mVB);

	// �ε��� ���۸� ����
	UINT indices[] =
	{
		// x��
		0, 1,

		// y��
		2, 3,

		// z��
		4, 5,
	};

	mIndexCount = 6;
	m_IndexCountVector.push_back(mIndexCount);

	// ���� ���۸� �����ϴ� ����ü�� ä���, ��� �÷��׷�
	// D3D11_BIND_INDEX_BUFFER�� �����Ѵٴ� ���� �ָ��� ��
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// ���� ���۸� �ʱ�ȭ�� �ڷḦ �����Ѵ�
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// ���� ���� ����
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	m_IBVector.push_back(mIB);
}

void DXGeometry::BuildGridGeometry()
{
	// ���� ���� �ڷḦ �����Ѵ� 
	VertexInputLayout::PosColor vertices[121];
	for (int i = 0; i < 121; i++)
	{
		vertices[i].Pos = XMFLOAT3((float)(i % 11) - 5.0f, 0.0f, (float)(i / 11) - 5.0f);
		vertices[i].Color = XMFLOAT4((const float*)&Colors::DarkGray);	// (��ο� ȸ��)
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

	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
	m_VBVector.push_back(mVB);

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

	mIndexCount = 44;
	m_IndexCountVector.push_back(mIndexCount);

	// ���� ���۸� �����ϴ� ����ü�� ä���, ��� �÷��׷�
	// D3D11_BIND_INDEX_BUFFER�� �����Ѵٴ� ���� �ָ��� ��
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// ���� ���۸� �ʱ�ȭ�� �ڷḦ �����Ѵ�
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// ���� ���� ����
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	m_IBVector.push_back(mIB);
}

SGeoBuffer* DXGeometry::BuildAxisGeometryBuffers()
{
	ID3D11Buffer* _VB = nullptr;
	ID3D11Buffer* _IB = nullptr;
	UINT _indexCount = 0;

	// ���� ���� �ڷḦ �����Ѵ� 
	VertexInputLayout::PosColor vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },	// x�� (����)
		{ XMFLOAT3(5.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},	// y�� (�ʷ�)
		{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)	},	// z�� (�Ķ�)
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

	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &_VB));


	// �ε��� ���۸� ����
	UINT indices[] =
	{
		// x��
		0, 1,

		// y��
		2, 3,

		// z��
		4, 5,
	};

	_indexCount = 6;

	// ���� ���۸� �����ϴ� ����ü�� ä���, ��� �÷��׷�
	// D3D11_BIND_INDEX_BUFFER�� �����Ѵٴ� ���� �ָ��� ��
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * _indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// ���� ���۸� �ʱ�ȭ�� �ڷḦ �����Ѵ�
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// ���� ���� ����
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &_IB));


	// ���⿡�� ����
	SGeoBuffer* _newGBuffer = new SGeoBuffer();
	_newGBuffer->mVB = _VB;
	_newGBuffer->mIB = _IB;
	_newGBuffer->mIndexCount = _indexCount;

	// ������ �Ǵ� �ڷᱸ���� �ִ´�.
	m_GeoBuffers.insert(make_pair(eDXObjectType::LineListObject, _newGBuffer));

	return _newGBuffer;
}

SGeoBuffer* DXGeometry::BuildGridGeometryBuffers()
{
	ID3D11Buffer* _VB = nullptr;
	ID3D11Buffer* _IB = nullptr;
	UINT _indexCount = 0;

	// ���� ���� �ڷḦ �����Ѵ� 
	VertexInputLayout::PosColor vertices[121];
	for (int i = 0; i < 121; i++)
	{
		vertices[i].Pos = XMFLOAT3((float)(i % 11) - 5.0f, 0.0f, (float)(i / 11) - 5.0f);
		vertices[i].Color = XMFLOAT4((const float*)&Colors::DarkGray);	// (��ο� ȸ��)
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

	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &_VB));

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

	_indexCount = 44;

	// ���� ���۸� �����ϴ� ����ü�� ä���, ��� �÷��׷�
	// D3D11_BIND_INDEX_BUFFER�� �����Ѵٴ� ���� �ָ��� ��
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * _indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// ���� ���۸� �ʱ�ȭ�� �ڷḦ �����Ѵ�
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// ���� ���� ����
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &_IB));


	// ���⿡�� ����
	SGeoBuffer* _newGBuffer = new SGeoBuffer();
	_newGBuffer->mVB = _VB;
	_newGBuffer->mIB = _IB;
	_newGBuffer->mIndexCount = _indexCount;

	// ������ �Ǵ� �ڷᱸ���� �ִ´�.
	m_GeoBuffers.insert(make_pair(eDXObjectType::LineListObject, _newGBuffer));

	return _newGBuffer;
}
