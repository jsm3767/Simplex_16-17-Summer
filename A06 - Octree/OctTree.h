/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2017/07
----------------------------------------------*/
#ifndef __OctTreeCLASS_H_
#define __OctTreeCLASS_H_

#include "Simplex\Physics\EntityManager.h"
#include "MyRigidBody.h"
#include "MyEntityManager.h"

using namespace Simplex;


	//System Class
	class SimplexDLL OctTree
	{
		static uint m_uOctTreeCount; //will store the number of OctTrees instantiated
		static uint m_uMaxLevel;//will store the maximum level an OctTree can go to
		static uint m_uIdealEntityCount; //will tell how many ideal Entities this object will contain

		uint m_uID = 0; //Will store the current ID for this OctTree
		uint m_uLevel = 0; //Will store the current level of the OctTree
		uint m_uChildren = 0;// Number of children on the OctTree (either 0 or 8)

		float m_fSize = 0.0f; //Size of the OctTree

		MeshManager* m_pMeshMngr = nullptr;
		MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager Singleton
		MyRigidBody* m_pRigidBody = nullptr;

		vector3 m_v3Center = vector3(0.0f); //Will store the center point of the OctTree
		vector3 m_v3Min = vector3(0.0f); //Will store the minimum vector of the OctTree
		vector3 m_v3Max = vector3(0.0f); //Will store the maximum vector of the OctTree

		OctTree* m_pParent = nullptr;// Will store the parent of current OctTree
		OctTree* m_pChild[8];//Will store the children of the current OctTree

		std::vector<uint> m_EntityList; //List of Entities under this OctTree (Index in Entity Manager)

		OctTree* m_pRoot = nullptr;//Root OctTree
		std::vector<OctTree*> m_lChild; //list of nodes that contain objects (this will be applied to root only)

	public:
		OctTree(vector3 center, float size);
		~OctTree(void);
		/*
		USAGE: Gets this OctTree's size
		ARGUMENTS: ---
		OUTPUT: size of OctTree
		*/
		float GetSize(void);
		/*
		USAGE: Gets the center of the OctTree in global scape
		ARGUMENTS: ---
		OUTPUT: Center of the OctTree in global space
		*/
		vector3 GetCenterGlobal(void);
		/*
		USAGE: Gets the min corner of the OctTree in global space
		ARGUMENTS: ---
		OUTPUT: Minimum in global space
		*/
		vector3 GetMinGlobal(void);
		/*
		USAGE: Gets the max corner of the OctTree in global space
		ARGUMENTS: ---
		OUTPUT: Maximum in global space
		*/
		vector3 GetMaxGlobal(void);
		/*
		USAGE: Asks if there is a collision with the Entity specified by index from
		the Bounding Object Manager
		ARGUMENTS:
		- int a_uRBIndex -> Index of the Entity in the Entity Manager
		OUTPUT: check of the collision
		*/
		bool IsColliding(uint a_uRBIndex);
		/*
		USAGE: Displays the OctTree volume in the color specified
		ARGUMENTS:
		- vector3 a_v3Color = REYELLOW -> Color of the volume to display.
		OUTPUT: ---
		*/
		void Display(vector3 a_v3Color = C_YELLOW);
		/*
		USAGE: Clears the Entity list for each node
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ClearEntityList(void);
		/*
		USAGE: allocates 8 smaller OctTrees in the child pointers
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Subdivide(void);
		/*
		USAGE: returns the child specified in the index
		ARGUMENTS: uint a_nChild -> index of the child (from 0 to 7)
		OUTPUT: OctTree object (child in index)
		*/
		OctTree* GetChild(uint a_nChild);

		bool HasChildren();
		/*
		USAGE: Creates a tree using subdivisions, the max number of objects and levels
		ARGUMENTS:
		- uint a_nMaxLevel = 3 -> Sets the maximum level of the tree while constructing it
		OUTPUT: ---
		*/
		void ConstructTree(uint a_nMaxLevel = 3);

		void AddEntityToList(uint index);

		std::vector<uint> GetEntityList();
	};//class

#endif //__OctTreeCLASS_H_

  /*
  USAGE:
  ARGUMENTS: ---
  OUTPUT: ---
  */