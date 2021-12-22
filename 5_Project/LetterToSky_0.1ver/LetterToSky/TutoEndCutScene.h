#pragma once
#include "IScene.h"
#include <vector>
using namespace std;

class IDXObjectBase;

class TutoEndCutScene : public IScene
{
public:
	TutoEndCutScene();
	virtual ~TutoEndCutScene();

	virtual void Initialize(DXDevice* pDevice, DXFactory* pFactory) override;
	virtual void Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera) override;
	virtual void Render(DXRenderer* pRenderer) override;
	virtual void Finalize() override;
	virtual void ChangeSceneState(SceneManager* pSceneManager) override;

private:
	vector<IDXObjectBase*> m_pUI;

	int m_NowTextIndex;

	bool m_CharacterOn;
	bool m_TextImageOff;

	float m_ClearImageDelayTime;

	bool m_NextScene = false;
};