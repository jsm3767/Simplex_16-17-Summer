#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		vector3(0.0f, 0.0f, 100.0f), //Position
		vector3(0.0f, 0.0f, 99.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

#ifdef DEBUG
	uint uInstances = 300; // decreased number in order to deal with laptop speed
#else
	uint uInstances = 1849;
#endif
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	m_uObjects = nSquare * nSquare;
	uint uIndex = -1;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			uIndex++;
			m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
			vector3 v3Position = vector3(glm::sphericalRand(34.0f));
			matrix4 m4Position = glm::translate(v3Position);
			m_pEntityMngr->SetModelMatrix(m4Position);
		}
	}
	m_uOctantLevels = 1;

	tree = new OctTree(vector3(0, 0, 0), 68);
	DistributeEntity(tree);

	m_pEntityMngr->Update();
}

void Simplex::Application::DistributeEntity(OctTree * currTree)
{
	if (currTree->HasChildren()) {
		for (int i = 0; i < 8; i++) {
			DistributeEntity(currTree->GetChild(i));
		}
	}
	else {
		for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++) {
			if (currTree->IsColliding(i)) {
				currTree->AddEntityToList(i);
			}
		}
	}
}

void Simplex::Application::RecreateTree()
{
	tree = new OctTree(vector3(0, 0, 0), 68);
	tree->ConstructTree(numLevels);
	DistributeEntity(tree);
}

void Simplex::Application::CheckCollisions(OctTree* currTree) {
	if (currTree->HasChildren()) {
		for (int i = 0; i < 8; i++) {
			CheckCollisions(currTree->GetChild(i));
		}
	}
	else {
		std::vector<uint> entities = currTree->GetEntityList();
		if (entities.size() <= 0) {
			return;
		}
		for (int i = 0; i < entities.size() - 1; i++) {
			for (int j = i + 1; j < entities.size(); j++) {
				m_pEntityMngr->GetEntity(entities[i])->IsColliding(m_pEntityMngr->GetEntity(entities[j]));
			}
		}
	}
}

void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	//Update Entity Manager
	m_pEntityMngr->Update();

	// I sear I would not be doing this if I did not get the ridiculous set of errors that I did.
	// This is where I will be checking for collision, using the OctTree I created
	// For Some Reason I Was unable to make an OctTree in the Entity Manager class
	CheckCollisions(tree);
	

	if (drawTree)
		tree->Display(C_YELLOW);
	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//display octree
	//m_pRoot->Display();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui,
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}