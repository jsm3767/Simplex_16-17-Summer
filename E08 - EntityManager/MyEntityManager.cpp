#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
}
void MyEntityManager::Release(void)
{
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	for (int i = 0; i < m_entityList.size(); i++) {
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
			return i;
	}
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	if (a_uIndex < 0)
		return m_entityList[m_entityList.size()]->GetModel();
	else
		return m_entityList[a_uIndex]->GetModel();

}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	return GetModel(GetEntityIndex(a_sUniqueID));
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	if (a_uIndex < 0)
		return m_entityList[m_entityList.size() - 1]->GetRigidBody();
	else
		return m_entityList[a_uIndex]->GetRigidBody();

}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	return GetRigidBody(GetEntityIndex(a_sUniqueID));
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	if (a_uIndex < 0)
		return m_entityList[m_entityList.size() - 1]->GetModelMatrix();
	else
		return m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	return GetModelMatrix(GetEntityIndex(a_sUniqueID));
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	SetModelMatrix(a_m4ToWorld, GetEntityIndex(a_sUniqueID));
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	if (a_uIndex < 0)
		m_entityList[m_entityList.size() - 1]->SetModelMatrix(a_m4ToWorld);
	else
		m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
MyEntityManager::MyEntityManager() { Init(); }
MyEntityManager::MyEntityManager(MyEntityManager const& other) { }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager() { Release(); };
// other methods
void Simplex::MyEntityManager::Update(void)
{

}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	m_entityList.push_back(new MyEntity(a_sFileName, a_sUniqueID));
	m_uEntityCount++;
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	MyEntity* toRemove = m_entityList[a_uIndex];
	std::swap(toRemove, m_entityList.back());
	m_entityList.pop_back();
	delete toRemove;
	m_uEntityCount--;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	RemoveEntity(GetEntityIndex(a_sUniqueID));
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	if (a_uIndex < 0)
		return m_entityList[m_entityList.size() - 1]->GetUniqueID();
	else
		return m_entityList[a_uIndex]->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	if (a_uIndex < 0)
		return m_entityList[m_entityList.size() - 1];
	else
		return m_entityList[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	if (a_uIndex < 0) {
		for (int i = 0; i < m_entityList.size(); i++){
			m_entityList[i]->AddToRenderList(a_bRigidBody);
		}
	}
	else 
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	AddEntityToRenderList(GetEntityIndex(a_sUniqueID), a_bRigidBody);
}