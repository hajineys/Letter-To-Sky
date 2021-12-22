#pragma once
#include "IScene.h"
#include <vector>
using namespace std;

class IDXObjectBase;

/// <summary>
/// Main InGame Scene °¡±â Àü Cut Scene
/// 
/// 2021. 08. 20 Hacgeum
/// </summary>
class MainEndCutScene : public IScene
{
public:
	MainEndCutScene();
	virtual ~MainEndCutScene();

	virtual void Initialize(DXDevice* pDevice, DXFactory* pFactory) override;
	virtual void Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera) override;
	virtual void Render(DXRenderer* pRenderer) override;
	virtual void Finalize() override;
	virtual void ChangeSceneState(SceneManager* pSceneManager) override;

private:
	vector<IDXObjectBase*> m_pUI;

	int m_NowTextIndex;
	bool m_MainCutImageOn;
	bool m_SendLetterImage;

	bool m_NextScene;
	float m_ClearImageDelayTime;
};