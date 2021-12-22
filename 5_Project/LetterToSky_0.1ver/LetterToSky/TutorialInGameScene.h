#pragma once
#include "IScene.h"
#include <vector>
using namespace std;

#include "../DXTK/Inc/SimpleMath.h"
using namespace DirectX::SimpleMath;

class IDXObjectBase;
class Stair;
class Bird;
class PlayerNew;
struct SavePoint;

/// <summary>
/// Tutorial InGame Scene
/// 
/// 2021. 08. 01 Hacgeum
/// </summary>
class TutorialInGameScene : public IScene
{
public:
	TutorialInGameScene();
	virtual ~TutorialInGameScene();

	virtual void Initialize(DXDevice* pDevice, DXFactory* pFactory, ResourceManager* pResourceManager) override;
	virtual void Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera) override;
	virtual void Render(DXRenderer* pRenderer) override;
	virtual void Finalize() override;
	virtual void ChangeSceneState(SceneManager* pSceneManager) override;

	virtual BoxCollider* GetFootStepStair() override;

private:
	void CameraMove(D3Camera* pCamera, float dTime);
	Vector3 m_DelayCamPos;	// �ణ ������ �ΰ� �ε巴�� ���󰡴� ��ġ

	/// ���̺�����Ʈ ����
	void AddSavePoint(std::string name, Vector3 pos);

	// �� ���������� ���̺�����Ʈ��
	std::vector<SavePoint*> m_pVecSavePoints;

	/// ĳ������ ��ġ�� �������� �浹���� �Լ�
	bool GetCollisionPos(Vector3* position);
	void CheckPlayerPos();

private:
	vector<IDXObjectBase*> m_pBackground;

	vector<Stair*> m_pStairList;
	vector<Bird*> m_pBirdList;
	PlayerNew* m_pPlayerNew;
	vector<IDXObjectBase*> m_pUI;

	int CharacterY;
	int m_NowTextIndex;

	BoxCollider* m_pResetBirds;
	BoxCollider* m_pResetForwardClouds;
	BoxCollider* m_pResetBackwardClouds;

	float m_ClearDelayTime;
	bool m_GameClearImageOn;
	bool m_GameClear;
	BoxCollider* m_pClearStage;

	bool LetterBasic;
	bool Letter8;
	bool Letter6;
	bool Letter4;
	bool Letter2;

	float m_TimeLimit;
	bool m_TimeImageOn = false;
	bool m_TimeOut;

public:
	static bool m_CharacterMoveStart;
};