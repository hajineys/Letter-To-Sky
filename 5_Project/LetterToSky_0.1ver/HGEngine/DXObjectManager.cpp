#include "pch.h"
#include "DXObjectManager.h"
#include "IDXObjectBase.h"

DXObjectManager::DXObjectManager()
{

}

DXObjectManager::~DXObjectManager()
{

}

void DXObjectManager::Finalize()
{
	std::map<std::string, IDXObjectBase*>::iterator iter;

	for (iter = m_Objects.begin(); iter != m_Objects.end(); iter++)
	{
		iter->second->Finalize();
		m_Objects.erase(iter);
	}
}
