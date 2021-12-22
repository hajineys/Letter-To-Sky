#include "Transform.h"
#include "MathHelper.h"

Transform::Transform()
	: m_WorldTM(),
	m_Position(0.0f, 0.0f, 0.0f),
	m_TranslationTM(),
	m_RotationTM(),
	m_ScaleTM(),
	m_Angle(0.0f),
	m_Forward(true),
	m_Forward1(true),
	m_Backward(true),
	m_Weight(0.0f),
	m_Per(0.0f),
	m_IsLerping(false)
{
	XMMATRIX I = XMMatrixIdentity();
	m_WorldTM = I;
	m_TranslationTM = I;
	m_RotationTM = I;
	m_ScaleTM = I;
}

Transform::~Transform()
{

}

void Transform::UpdateWorldTM()
{
	m_WorldTM = m_ScaleTM * m_RotationTM * m_TranslationTM;
}

void Transform::Strafe(float start, float end, float value)
{
	// �������̾��µ�, ������ ���� ��� ���������� �ٲ� ��
	if (m_Forward == true && end < m_Position.x)
	{
		m_Forward = false;
	}

	// �������̾��µ�, ������ ���� ��� ���������� �ٲ� ��
	if (m_Forward == false && m_Position.x < start)
	{
		m_Forward = true;
	}

	// �������϶��� ������ ������ ���ؼ� ����. �� ������Ʈ�� �̵��Ѵ�.
	if (m_Forward == true)
	{
		m_TranslationTM *= XMMatrixTranslation(value, 0.0f, 0.0f);
		m_Position.x += value;
	}
	// �������϶��� ������ ���� �ݴ�� �̵��Ѵ�. �� ������Ʈ�� �ݴ�� �̵��Ѵ�.
	else
	{
		m_TranslationTM *= XMMatrixTranslation(-value, 0.0f, 0.0f);
		m_Position.x -= value;
	}
}

void Transform::StrafeInverse(float start, float end, float value)
{
	// �������̾��µ�, ������ ���� ��� ���������� �ٲ� ��
	if (m_Backward == true && m_Position.x < end)
	{
		m_Backward = false;
	}

	// �������̾��µ�, ������ ���� ��� ���������� �ٲ� ��
	if (m_Backward == false && start < m_Position.x)
	{
		m_Backward = true;
	}

	// �������϶��� ������ ���� �ݴ�� �̵��Ѵ�. �� ������Ʈ�� �ݴ�� �̵��Ѵ�.
	if (m_Backward == true)
	{
		m_TranslationTM *= XMMatrixTranslation(-value, 0.0f, 0.0f);
		m_Position.x -= value;
	}
	// �������϶��� ������ ������ ���ؼ� ����. �� ������Ʈ�� �̵��Ѵ�.
	else
	{
		m_TranslationTM *= XMMatrixTranslation(value, 0.0f, 0.0f);
		m_Position.x += value;
	}
}

void Transform::StrafeDiagonal(float startX, float endX, float startY, float endY, float startZ, float endZ, float dTime)
{
	// Lerp�� �����Ѵ�, �� �ѹ� ���´�.
	if (m_EndPos.x - m_NowPos.x < 0.1f &&
		m_EndPos.y - m_NowPos.y < 0.1f &&
		m_EndPos.z - m_NowPos.z < 0.1f && m_IsLerping == false)
	{
		m_IsLerping = true;

		if (m_Forward == true)
		{
			// ������ ����, (�ʱ�ȭ)
			m_StartPos = Vector3(startX, startY, startZ);
			m_EndPos = Vector3(endX, endY, endZ);
			m_Forward = false;
		}
		else
		{
			// ������ ����, (�ʱ�ȭ)
			m_StartPos = Vector3(endX, endY, endZ);
			m_EndPos = Vector3(startX, startY, startZ);
			m_Forward = true;
		}
	}

	// �� ������ ���� ���̴�.
	if (m_IsLerping)
	{
		///Lerp();

		// per (0 ~ 1), �ð��� �������� �ۼ�Ʈ�� ������Ų��.
		m_Per += dTime * 0.5;

		// Lerp -> Start Position, Target Position, per
		// ���� ���൵�� ���� ���� ������...-> per 0% -> nowPos = startPos
		// per -> 100% (1.0) -> nowPos = targetPos
		m_NowPos = MathHelper::Lerp<Vector3>(m_StartPos, m_EndPos, m_Per);

		SetPosition(m_NowPos.x, m_NowPos.y, m_NowPos.z);

		// ..... -> �ۼ�Ʈ�� 1(100%)�� ������ �ȵȴ�.
		if (m_Per > 1.0f)
		{
			m_Per = 1.0f;
		}

		// per�� 1�� �Ѿ����Ƿ� Lerp �ߴ��Ѵ�.
		if (m_Per >= 1.0f)
		{
			m_Per = 0.0f;
			m_IsLerping = false;
		}
	}
}

void Transform::MoveUpDown(float start, float end, float value)
{
	// �������̾��µ�, ������ ���� ��� ���������� �ٲ� ��
	if (m_Forward == true && end < m_Position.y)
	{
		m_Forward = false;
	}

	// �������̾��µ�, ������ ���� ��� ���������� �ٲ� ��
	if (m_Forward == false && m_Position.y < start)
	{
		m_Forward = true;
	}

	// �������϶��� ������ ������ ���ؼ� ����. �� ������Ʈ�� �̵��Ѵ�.
	if (m_Forward == true)
	{
		m_TranslationTM *= XMMatrixTranslation(0.0f, value, 0.0f);
		m_Position.y += value;
	}
	// �������϶��� ������ ���� �ݴ�� �̵��Ѵ�. �� ������Ʈ�� �ݴ�� �̵��Ѵ�.
	else
	{
		m_TranslationTM *= XMMatrixTranslation(0.0f, -value, 0.0f);
		m_Position.y -= value;
	}
}

void Transform::MoveFrontBack(float start, float end, float value)
{
	// �������̾��µ�, ������ ���� ��� ���������� �ٲ� ��
	if (m_Forward == true && end < m_Position.z)
	{
		m_Forward = false;
	}

	// �������̾��µ�, ������ ���� ��� ���������� �ٲ� ��
	if (m_Forward == false && m_Position.z < start)
	{
		m_Forward = true;
	}

	// �������϶��� ������ ������ ���ؼ� ����. �� ������Ʈ�� �̵��Ѵ�.
	if (m_Forward == true)
	{
		m_TranslationTM *= XMMatrixTranslation(0.0f, 0.0f, value);
		m_Position.z += value;
	}
	// �������϶��� ������ ���� �ݴ�� �̵��Ѵ�. �� ������Ʈ�� �ݴ�� �̵��Ѵ�.
	else
	{
		m_TranslationTM *= XMMatrixTranslation(0.0f, 0.0f, -value);
		m_Position.z -= value;
	}
}

void Transform::Fall(float dTime)
{
	float value = 30.0f * dTime;

	m_TranslationTM *= XMMatrixTranslation(0.0f, -value, 0.0f);
	m_Position.y -= value;
}

void Transform::SetPosition(float posX, float posY, float posZ)
{
	m_TranslationTM = XMMatrixTranslation(posX, posY, posZ);

	m_Position.x = posX;
	m_Position.y = posY;
	m_Position.z = posZ;
}

void Transform::AddPosition(float x, float y, float z)
{
	m_TranslationTM = XMMatrixTranslation(m_Position.x + x, m_Position.y + y, m_Position.z + z);

	m_Position.x += x;
	m_Position.y += y;
	m_Position.z += z;
}

void Transform::SetTranslation(float dx, float dy, float dz)
{
	m_TranslationTM *= XMMatrixTranslation(dx, dy, dz);

	m_Position.x += dx;
	m_Position.y += dy;
	m_Position.z += dz;
}

void Transform::SetRotation(char axis, float angle)
{
	m_Angle = angle;

	float radians = XMConvertToRadians(m_Angle);

	switch (axis)
	{
		case 'X':
			m_RotationTM = XMMatrixRotationX(radians);
			break;

		case 'Y':
			m_RotationTM = XMMatrixRotationY(radians);
			break;

		case 'Z':
			m_RotationTM = XMMatrixRotationZ(radians);
			break;
	}
}

void Transform::SetScale(float px, float py, float pz)
{
	m_ScaleTM += XMMatrixScaling(px, py, pz);
}