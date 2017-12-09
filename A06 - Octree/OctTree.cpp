#include "OctTree.h"

using namespace Simplex;

OctTree::OctTree(vector3 center, float size)
{
	m_pMeshMngr = m_pMeshMngr->GetInstance();
	m_pEntityMngr = m_pEntityMngr->GetInstance();

	m_v3Center = center;
	m_fSize = size;
	m_v3Min = vector3(center.x - (size/2), center.y - (size/2), center.z - (size/2));
	m_v3Max = vector3(center.x + (size/2), center.y + (size/2), center.z + (size/2));

	std::vector<vector3> pointList;
	pointList.push_back(vector3(m_v3Min.x, m_v3Min.y, m_v3Min.z));
	pointList.push_back(vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z));
	pointList.push_back(vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z));
	pointList.push_back(vector3(m_v3Min.x, m_v3Min.y, m_v3Max.z));
	pointList.push_back(vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z));
	pointList.push_back(vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z));
	pointList.push_back(vector3(m_v3Max.x, m_v3Min.y, m_v3Max.z));
	pointList.push_back(vector3(m_v3Max.x, m_v3Max.y, m_v3Max.z));

	for (int i = 0; i < 8; i++) {
		m_pChild[i] = nullptr;
	}

	m_pRigidBody = new MyRigidBody(pointList);
}

OctTree::~OctTree(void)
{
	delete[] m_pChild;
}

float OctTree::GetSize(void)
{
	return m_fSize;
}

vector3 OctTree::GetCenterGlobal(void)
{
	return m_v3Center;
}

vector3 OctTree::GetMinGlobal(void)
{
	return m_v3Min;
}

vector3 OctTree::GetMaxGlobal(void)
{
	return m_v3Max;
}

bool OctTree::IsColliding(uint a_uRBIndex)
{
	return m_pRigidBody->IsColliding(m_pEntityMngr->GetRigidBody(a_uRBIndex));
}

void OctTree::Display(vector3 a_v3Color)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_v3Center) * glm::scale(vector3(m_fSize, m_fSize, m_fSize)), a_v3Color);
	if (HasChildren()) {
		for (int i = 0; i < 8; i++) {
			m_pChild[i]->Display(a_v3Color);
		}
	}
}

void OctTree::ClearEntityList(void)
{

}

void OctTree::Subdivide(void)
{
	if (HasChildren()) {
		return;
	}
	
	m_pChild[0] = new OctTree(vector3(m_v3Center.x + m_fSize / 4, m_v3Center.y + m_fSize / 4, m_v3Center.z + m_fSize / 4), m_fSize / 2);
	m_pChild[1] = new OctTree(vector3(m_v3Center.x - m_fSize / 4, m_v3Center.y + m_fSize / 4, m_v3Center.z + m_fSize / 4), m_fSize / 2);
	m_pChild[2] = new OctTree(vector3(m_v3Center.x + m_fSize / 4, m_v3Center.y - m_fSize / 4, m_v3Center.z + m_fSize / 4), m_fSize / 2);
	m_pChild[3] = new OctTree(vector3(m_v3Center.x + m_fSize / 4, m_v3Center.y + m_fSize / 4, m_v3Center.z - m_fSize / 4), m_fSize / 2);
	m_pChild[4] = new OctTree(vector3(m_v3Center.x + m_fSize / 4, m_v3Center.y - m_fSize / 4, m_v3Center.z - m_fSize / 4), m_fSize / 2);
	m_pChild[5] = new OctTree(vector3(m_v3Center.x - m_fSize / 4, m_v3Center.y + m_fSize / 4, m_v3Center.z - m_fSize / 4), m_fSize / 2);
	m_pChild[6] = new OctTree(vector3(m_v3Center.x - m_fSize / 4, m_v3Center.y - m_fSize / 4, m_v3Center.z + m_fSize / 4), m_fSize / 2);
	m_pChild[7] = new OctTree(vector3(m_v3Center.x - m_fSize / 4, m_v3Center.y - m_fSize / 4, m_v3Center.z - m_fSize / 4), m_fSize / 2);
}

OctTree * OctTree::GetChild(uint a_nChild)
{
	return m_pChild[a_nChild];
}

bool OctTree::HasChildren()
{
	return (m_pChild[0] != nullptr);
}

void OctTree::ConstructTree(uint a_nMaxLevel)
{
	if (a_nMaxLevel == 0) {
		return;
	}

	for (int i = 0; i < a_nMaxLevel; i++) {
		if (!HasChildren()) {
			Subdivide();
		}
		else {
			for (int j = 0; j < 8; j++) {
				m_pChild[j]->ConstructTree(a_nMaxLevel - 1);
			}
		}
	}

}

void OctTree::AddEntityToList(uint index)
{
	m_EntityList.push_back(index);
}

std::vector<uint> OctTree::GetEntityList()
{
	return m_EntityList;
}
