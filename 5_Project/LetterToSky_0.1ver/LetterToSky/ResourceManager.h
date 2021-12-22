#pragma once
#include <vector>
using namespace std;

class DXDevice;
class DXFactory;

// Object
class Stair;
class Bird;
class PlayerNew;
class IDXObjectBase;

/// <summary>
/// InGame Scene Asset들 관리하는 매니저
/// 
/// 2021. 08. 20 Hacgeum
/// </summary>
class ResourceManager
{
public:
	ResourceManager(DXDevice* pDevice, DXFactory* pFactory);
	~ResourceManager();

	void CreateTutoInGameAssets();
	void CreateMainInGameAssets();
	void Finalize();

private:
	DXDevice* m_pDXDevice;
	DXFactory* m_pDXFactory;

public:
	vector<IDXObjectBase*> m_pTutoBackgroundList;

	vector<Stair*> m_pTutoStairList;
	vector<Bird*> m_pTutoBirdList;
	PlayerNew* m_pTutoPlayer;
	vector<IDXObjectBase*> m_pTutoUI;

public:
	vector<IDXObjectBase*> m_pMainBackgroundList;

	vector<Stair*> m_pMainStairList;
	vector<Bird*> m_pMainBirdList;
	PlayerNew* m_pMainPlayer;
	vector<IDXObjectBase*> m_pMainUI;
};