#include "MyRigidBody.h"
using namespace Simplex;
//Allocation
void MyRigidBody::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_bVisibleBS = false;
	m_bVisibleOBB = true;
	m_bVisibleARBB = false;

	m_fRadius = 0.0f;

	m_v3ColorColliding = C_RED;
	m_v3ColorNotColliding = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;
	m_v3ARBBSize = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& a_pOther)
{
	std::swap(m_pMeshMngr, a_pOther.m_pMeshMngr);
	std::swap(m_bVisibleBS, a_pOther.m_bVisibleBS);
	std::swap(m_bVisibleOBB, a_pOther.m_bVisibleOBB);
	std::swap(m_bVisibleARBB, a_pOther.m_bVisibleARBB);

	std::swap(m_fRadius, a_pOther.m_fRadius);

	std::swap(m_v3ColorColliding, a_pOther.m_v3ColorColliding);
	std::swap(m_v3ColorNotColliding, a_pOther.m_v3ColorNotColliding);

	std::swap(m_v3Center, a_pOther.m_v3Center);
	std::swap(m_v3MinL, a_pOther.m_v3MinL);
	std::swap(m_v3MaxL, a_pOther.m_v3MaxL);

	std::swap(m_v3MinG, a_pOther.m_v3MinG);
	std::swap(m_v3MaxG, a_pOther.m_v3MaxG);

	std::swap(m_v3HalfWidth, a_pOther.m_v3HalfWidth);
	std::swap(m_v3ARBBSize, a_pOther.m_v3ARBBSize);

	std::swap(m_m4ToWorld, a_pOther.m_m4ToWorld);

	std::swap(m_CollidingRBSet, a_pOther.m_CollidingRBSet);
}
void MyRigidBody::Release(void)
{
	m_pMeshMngr = nullptr;
	ClearCollidingList();
}
//Accessors
bool MyRigidBody::GetVisibleBS(void) { return m_bVisibleBS; }
void MyRigidBody::SetVisibleBS(bool a_bVisible) { m_bVisibleBS = a_bVisible; }
bool MyRigidBody::GetVisibleOBB(void) { return m_bVisibleOBB; }
void MyRigidBody::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
bool MyRigidBody::GetVisibleARBB(void) { return m_bVisibleARBB; }
void MyRigidBody::SetVisibleARBB(bool a_bVisible) { m_bVisibleARBB = a_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColorColliding(void) { return m_v3ColorColliding; }
vector3 MyRigidBody::GetColorNotColliding(void) { return m_v3ColorNotColliding; }
void MyRigidBody::SetColorColliding(vector3 a_v3Color) { m_v3ColorColliding = a_v3Color; }
void MyRigidBody::SetColorNotColliding(vector3 a_v3Color) { m_v3ColorNotColliding = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix)
{
	//to save some calculations if the model matrix is the same there is nothing to do here
	if (a_m4ModelMatrix == m_m4ToWorld)
		return;

	//Assign the model matrix
	m_m4ToWorld = a_m4ModelMatrix;

	//Calculate the 8 corners of the cube
	vector3 v3Corner[8];
	//Back square
	v3Corner[0] = m_v3MinL;
	v3Corner[1] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);
	v3Corner[2] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);
	v3Corner[3] = vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);

	//Front square
	v3Corner[4] = vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[5] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[6] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);
	v3Corner[7] = m_v3MaxL;

	//Place them in world space
	for (uint uIndex = 0; uIndex < 8; ++uIndex)
	{
		v3Corner[uIndex] = vector3(m_m4ToWorld * vector4(v3Corner[uIndex], 1.0f));
	}

	//Identify the max and min as the first corner
	m_v3MaxG = m_v3MinG = v3Corner[0];

	//get the new max and min for the global box
	for (uint i = 1; i < 8; ++i)
	{
		if (m_v3MaxG.x < v3Corner[i].x) m_v3MaxG.x = v3Corner[i].x;
		else if (m_v3MinG.x > v3Corner[i].x) m_v3MinG.x = v3Corner[i].x;

		if (m_v3MaxG.y < v3Corner[i].y) m_v3MaxG.y = v3Corner[i].y;
		else if (m_v3MinG.y > v3Corner[i].y) m_v3MinG.y = v3Corner[i].y;

		if (m_v3MaxG.z < v3Corner[i].z) m_v3MaxG.z = v3Corner[i].z;
		else if (m_v3MinG.z > v3Corner[i].z) m_v3MinG.z = v3Corner[i].z;
	}

	//we calculate the distance between min and max vectors
	m_v3ARBBSize = m_v3MaxG - m_v3MinG;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	//Count the points of the incoming list
	uint uVertexCount = a_pointList.size();

	//If there are none just return, we have no information to create the BS from
	if (uVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3MaxL = m_v3MinL = a_pointList[0];

	//Get the max and min out of the list
	for (uint i = 1; i < uVertexCount; ++i)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;

		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;

		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	//with the max and the min we calculate the center
	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;

	//we calculate the distance between min and max vectors
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

	//Get the distance between the center and either the min or the max
	m_fRadius = glm::distance(m_v3Center, m_v3MinL);
}
MyRigidBody::MyRigidBody(MyRigidBody const& a_pOther)
{
	m_pMeshMngr = a_pOther.m_pMeshMngr;

	m_bVisibleBS = a_pOther.m_bVisibleBS;
	m_bVisibleOBB = a_pOther.m_bVisibleOBB;
	m_bVisibleARBB = a_pOther.m_bVisibleARBB;

	m_fRadius = a_pOther.m_fRadius;

	m_v3ColorColliding = a_pOther.m_v3ColorColliding;
	m_v3ColorNotColliding = a_pOther.m_v3ColorNotColliding;

	m_v3Center = a_pOther.m_v3Center;
	m_v3MinL = a_pOther.m_v3MinL;
	m_v3MaxL = a_pOther.m_v3MaxL;

	m_v3MinG = a_pOther.m_v3MinG;
	m_v3MaxG = a_pOther.m_v3MaxG;

	m_v3HalfWidth = a_pOther.m_v3HalfWidth;
	m_v3ARBBSize = a_pOther.m_v3ARBBSize;

	m_m4ToWorld = a_pOther.m_m4ToWorld;

	m_CollidingRBSet = a_pOther.m_CollidingRBSet;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& a_pOther)
{
	if (this != &a_pOther)
	{
		Release();
		Init();
		MyRigidBody temp(a_pOther);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody() { Release(); };
//--- a_pOther Methods
void MyRigidBody::AddCollisionWith(MyRigidBody* a_pOther)
{
	/*
		check if the object is already in the colliding set, if
		the object is already there return with no changes
	*/
	auto element = m_CollidingRBSet.find(a_pOther);
	if (element != m_CollidingRBSet.end())
		return;
	// we couldn't find the object so add it
	m_CollidingRBSet.insert(a_pOther);
}
void MyRigidBody::RemoveCollisionWith(MyRigidBody* a_pOther)
{
	m_CollidingRBSet.erase(a_pOther);
}
void MyRigidBody::ClearCollidingList(void)
{
	m_CollidingRBSet.clear();
}
bool MyRigidBody::IsColliding(MyRigidBody* const a_pOther)
{
	//check if spheres are colliding as pre-test
	bool bColliding = (glm::distance(GetCenterGlobal(), a_pOther->GetCenterGlobal()) < m_fRadius + a_pOther->m_fRadius);
	
	//if they are colliding check the SAT
	if (bColliding)
	{
		int satResult = SAT(a_pOther);
		if(satResult != eSATResults::SAT_NONE)
			bColliding = false;// reset to false
	}

	if (bColliding) //they are colliding
	{
		this->AddCollisionWith(a_pOther);
		a_pOther->AddCollisionWith(this);
	}
	else //they are not colliding
	{
		this->RemoveCollisionWith(a_pOther);
		a_pOther->RemoveCollisionWith(this);
	}

	return bColliding;
}
void MyRigidBody::AddToRenderList(void)
{
	if (m_bVisibleBS)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
		else
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
	}
	if (m_bVisibleOBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorColliding);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorNotColliding);
	}
	if (m_bVisibleARBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
	}
}

uint MyRigidBody::SAT(MyRigidBody* const a_pOther)
{
	
	glm::vec3 v3MyCorner[8];
	// I need all 8 corners of both boxes so I can calculate their x y and z axis each
	v3MyCorner[0] = glm::vec3(m_v3MinL.x, m_v3MinL.y, m_v3MinL.z);
	v3MyCorner[1] = glm::vec3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);
	v3MyCorner[2] = glm::vec3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);
	v3MyCorner[3] = glm::vec3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);

	v3MyCorner[4] = glm::vec3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);
	v3MyCorner[5] = glm::vec3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);
	v3MyCorner[6] = glm::vec3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);
	v3MyCorner[7] = glm::vec3(m_v3MaxL.x, m_v3MaxL.y, m_v3MaxL.z);

	glm::vec3 v3OtherCorner[8];
	v3OtherCorner[0] = glm::vec3(a_pOther->m_v3MinL.x, a_pOther->m_v3MinL.y, a_pOther->m_v3MinL.z);
	v3OtherCorner[1] = glm::vec3(a_pOther->m_v3MaxL.x, a_pOther->m_v3MinL.y, a_pOther->m_v3MinL.z);
	v3OtherCorner[2] = glm::vec3(a_pOther->m_v3MinL.x, a_pOther->m_v3MaxL.y, a_pOther->m_v3MinL.z);
	v3OtherCorner[3] = glm::vec3(a_pOther->m_v3MaxL.x, a_pOther->m_v3MaxL.y, a_pOther->m_v3MinL.z);

	v3OtherCorner[4] = glm::vec3(a_pOther->m_v3MinL.x, a_pOther->m_v3MinL.y, a_pOther->m_v3MaxL.z);
	v3OtherCorner[5] = glm::vec3(a_pOther->m_v3MaxL.x, a_pOther->m_v3MinL.y, a_pOther->m_v3MaxL.z);
	v3OtherCorner[6] = glm::vec3(a_pOther->m_v3MinL.x, a_pOther->m_v3MaxL.y, a_pOther->m_v3MaxL.z);
	v3OtherCorner[7] = glm::vec3(a_pOther->m_v3MaxL.x, a_pOther->m_v3MaxL.y, a_pOther->m_v3MaxL.z);

	for (int i = 0; i < 8; i++) {
		v3MyCorner[i] = vector3(m_m4ToWorld * vector4(v3MyCorner[i], 1.0f));
		v3OtherCorner[i] = vector3(a_pOther->m_m4ToWorld * vector4(v3OtherCorner[i], 1.0f));
	}

	glm::vec3 myXYZ[3];
	myXYZ[0] = glm::normalize(v3MyCorner[1] - v3MyCorner[0]); // my x axis | ax
	myXYZ[1] = glm::normalize(v3MyCorner[2] - v3MyCorner[0]); // my y axis | ay
	myXYZ[2] = glm::normalize(v3MyCorner[4] - v3MyCorner[0]); // my z axis | az

	// now I have all 8 corners of each, and I have all 15 axis to project onto (however we'll start with these axis so we dont have to do cross if we can find a seperating axis first)
	// I simply need to loop through each axis and:
	//		loop through each corner projecting each onto the axis
	//	find the min and max, and compare those to the min and max of the other box
	//	if either max < the other's min then there is a collision and we break

	for (int axis = 0; axis < myXYZ->length(); axis++) {
		float aMinProjection, aMaxProjection, bMinProjection, bMaxProjection;

		GetMinMaxProjection(v3MyCorner, myXYZ[axis], aMinProjection, aMaxProjection);
		GetMinMaxProjection(v3OtherCorner, myXYZ[axis], bMinProjection, bMaxProjection);

		// if one min is greater than the other's max that's not a collision
		if (aMinProjection > bMaxProjection || bMinProjection > aMaxProjection)
			return (axis + 1); // this should return the proper axis that seperated the two
	}

	glm::vec3 otherXYZ[3];
	otherXYZ[0] = glm::normalize(v3OtherCorner[1] - v3OtherCorner[0]); // other x axis | bx
	otherXYZ[1] = glm::normalize(v3OtherCorner[2] - v3OtherCorner[0]); // other y axis | by
	otherXYZ[2] = glm::normalize(v3OtherCorner[4] - v3OtherCorner[0]); // other z axis | bz

	for (int axis = 0; axis < otherXYZ->length(); axis++) {
		float aMinProjection, aMaxProjection, bMinProjection, bMaxProjection;

		GetMinMaxProjection(v3MyCorner, otherXYZ[axis], aMinProjection, aMaxProjection);
		GetMinMaxProjection(v3OtherCorner, otherXYZ[axis], bMinProjection, bMaxProjection);

		// if one min is greater than the other's max that's not a collision
		if (aMinProjection > bMaxProjection || bMinProjection > aMaxProjection)
			return (axis + 4); // this should return the proper axis that seperated the two
	}

	glm::vec3 crosses[9];
	crosses[0] = glm::normalize(glm::cross(myXYZ[0], otherXYZ[3] + FLT_EPSILON)); // cross of each x | ax x bx
	crosses[1] = glm::normalize(glm::cross(myXYZ[0], otherXYZ[4] + FLT_EPSILON)); //  ax x by
	crosses[2] = glm::normalize(glm::cross(myXYZ[0], otherXYZ[5] + FLT_EPSILON)); //  ax x bz

	crosses[3] = glm::normalize(glm::cross(myXYZ[1], otherXYZ[3] + FLT_EPSILON)); //  ay x bx
	crosses[4] = glm::normalize(glm::cross(myXYZ[1], otherXYZ[4] + FLT_EPSILON)); //  ay x by
	crosses[5] = glm::normalize(glm::cross(myXYZ[1], otherXYZ[5] + FLT_EPSILON)); //  ay x bz

	crosses[6] = glm::normalize(glm::cross(myXYZ[2], otherXYZ[3] + FLT_EPSILON)); //  az x bx
	crosses[7] = glm::normalize(glm::cross(myXYZ[2], otherXYZ[4] + FLT_EPSILON)); //  az x by
	crosses[8] = glm::normalize(glm::cross(myXYZ[2], otherXYZ[5] + FLT_EPSILON)); //  az x bz

	for (int axis = 0; axis < crosses->length(); axis++) {
		float aMinProjection, aMaxProjection, bMinProjection, bMaxProjection;

		GetMinMaxProjection(v3MyCorner, crosses[axis], aMinProjection, aMaxProjection);
		GetMinMaxProjection(v3OtherCorner, crosses[axis], bMinProjection, bMaxProjection);

		// if one min is greater than the other's max that's not a collision
		if (aMinProjection > bMaxProjection || bMinProjection > aMaxProjection)
			return (axis + 7); // this should return the proper axis that seperated the two
	}

	//there is no axis test that separates these two objects
	return eSATResults::SAT_NONE;
}

void Simplex::MyRigidBody::GetMinMaxProjection(glm::vec3 corners[], vector3 axis, float & min, float & max)
{
	// here we set the first of the projections to min, and we'll update min and max as we loop through each corner of the box
	min = glm::dot(corners[0], axis);
	max = min;

	for (int i = 1; i < 8; i++) { // we can skip zero since it's our base
		float proj = glm::dot(corners[i], axis); // lets see where this corner lies on the axis
		if (proj < min) {
			min = proj;
		}
		if (proj > max) {
			max = proj;
		}
	}

	// this should give us the min and max values of every projection, and put them on the pointers we took in
}

