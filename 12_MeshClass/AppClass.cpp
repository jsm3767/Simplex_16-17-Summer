#include "AppClass.h"

int meshSize = 48;
float frame = 0;
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Make MyMesh object
	m_pMesh = new MyMesh[meshSize];
	for (int i = 0; i < meshSize; i++) {
		m_pMesh[i].GenerateCube(1.0f, C_BLACK);
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	frame ++;
	if (frame > 360) {
		frame = 0;
	}

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	static matrix4 m4Model = matrix4() * glm::translate(IDENTITY_M4, vector3(-40, 0, -20));
	m4Model = glm::translate(m4Model, vector3(glm::abs(glm::sin(glm::radians(frame)))/20, glm::cos(glm::radians(frame))/30, 0));

	for (int i = 0; i < meshSize; i++) {
		if (i < 2) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(i*6 + 2, 8, 0)));
		}
		else if (i < 4) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i-2) * 4 + 3, 7, 0)));
		}
		else if (i < 11) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 4) + 2, 6, 0)));
		}
		else if (i < 13) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 11) + 1, 5, 0)));
		}
		else if (i < 16) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 13) + 4, 5, 0)));
		}
		else if (i < 18) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 16) + 8, 5, 0)));
		}
		else if (i < 29) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 18), 4, 0)));
		}
		else if (i < 30) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 29), 3, 0)));
		}
		else if (i < 37) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 30) + 2, 3, 0)));
		}
		else if (i < 38) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 37) + 10, 3, 0)));
		}
		else if (i < 40) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 38) * 2, 2, 0)));
		}
		else if (i < 42) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 40) * 2 + 8, 2, 0)));
		}
		else if (i < 44) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 42) + 3, 1, 0)));
		}
		else if (i < 46) {
			m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3((i - 44) + 6, 1, 0)));
		}
		
	}
		
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete[] m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}