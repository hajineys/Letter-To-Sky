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
class MainInGameScene : public IScene
{
public:
	MainInGameScene();
	virtual ~MainInGameScene();

	virtual void Initialize(DXDevice* pDevice, DXFactory* pFactory, ResourceManager* pResourceManager) override;
	virtual void Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera) override;
	virtual void Render(DXRenderer* pRenderer) override;
	virtual void Finalize() override;
	virtual void ChangeSceneState(SceneManager* pSceneManager) override;

	virtual BoxCollider* GetFootStepStair() override;

private:
	void CameraMove(D3Camera* pCamera, float dTime);
	Vector3 m_DelayCamPos;	// 약간 간격을 두고 부드럽게 따라가는 위치

	/// 세이브포인트 관련
	void AddSavePoint(std::string name, Vector3 pos);

	// 이 스테이지의 세이브포인트들
	std::vector<SavePoint*> m_pVecSavePoints;

	/// 캐릭터의 위치를 가져오는 충돌감지 함수
	bool GetCollisionPos(Vector3* position);
	void CheckPlayerPos();

private:
	vector<IDXObjectBase*> m_pBackground;

	vector<Stair*> m_pStairList;
	vector<Bird*> m_pBirdList;
	PlayerNew* m_pPlayerNew;
	vector<IDXObjectBase*> m_pUI;

	BoxCollider* m_pResetBirds;
	BoxCollider* m_pResetForwardClouds;
	BoxCollider* m_pResetBackwardClouds;

	int CharacterY;
	float m_TimeLimit;
	bool m_TimeImageOn = false;
	bool m_TimeOut;
	float m_TimeNextScene;

	float m_ClearDelayTime;
	bool m_GameClear;
	BoxCollider* m_pClearStage;
};