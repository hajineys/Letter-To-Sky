#pragma once
#include "IScene.h"
#include <vector>
using namespace std;

class IDXObjectBase;

/// <summary>
/// Stage Tuto Choice Scene
/// 
/// 2021. 08. 01 Hacgeum
/// </summary>
class AreaTutoChoiceScene : public IScene
{
public:
	AreaTutoChoiceScene();
	virtual ~AreaTutoChoiceScene();

	virtual void Initialize(DXDevice* pDevice, DXFactory* pFactory) override;
	virtual void Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera) override;
	virtual void Render(DXRenderer* pRenderer) override;
	virtual void Finalize() override;
	virtual void ChangeSceneState(SceneManager* pSceneManager) override;

private:
	vector<IDXObjectBase*> m_pUI;

	bool m_CharacterOn;

	int m_NowTextIndex;

	bool m_TutoCutImage;
	bool m_CollidedLetter;

	int m_UI07Click;
	bool m_NextScene;
};