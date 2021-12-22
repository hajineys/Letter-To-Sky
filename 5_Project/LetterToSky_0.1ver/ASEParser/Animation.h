#pragma once
#include "AlignedAllocationPolicy.h"

///--------------------------------------------------
/// Animation Data�� �����ϱ� ���� Ŭ����
///--------------------------------------------------
class Animation_pos : public AlignedAllocationPolicy<16>
{
public:
	int m_time;
	Vector3	m_pos;
};

class Animation_rot : public AlignedAllocationPolicy<16>
{
public:
	int m_time;
	Vector3 m_rot;
	float m_angle;
	Quaternion m_rotQT_accumulation;		// ��뺯���̱� ������ ���� ����� �����ؼ� ������ �ִ´� (�� ���ݱ����� ���뷮���� ��ȯ�Ѵ�)
};

class Animation_scl : public AlignedAllocationPolicy<16>
{
public:
	int m_time;
	Vector3 m_scale;
	Vector3 m_scaleaxis;
	float m_scaleaxisang;
	Quaternion m_scalerotQT_accumulation;
};

///--------------------------------------------------
/// �� Ŭ������ ������ Animation Ŭ����
///--------------------------------------------------
class Animation : public AlignedAllocationPolicy<16>
{
	// �ִϸ��̼� �������� �̸�, ������ ���������̴�
public:
	std::string m_nodename;
	vector<Animation_pos*> m_position;
	vector<Animation_rot*> m_rotation;
	vector<Animation_scl*> m_scale;

	int m_ticksperFrame = 0;	// �� �������� ƽ ũ��, �̰��� AnimationTable�� ������ ������ ������Ʈ���� �ٸ� �� �ִ�
	int m_minKeyframe = 0;		// Ű �����Ӱ��� �ּ�ũ��, �̰��� ���ǰͰ� �� ��ġ������ �ʴ´�
};
