#include "MyOctant.h"
using namespace Simplex;

uint MyOctant:: m_uOctantCount = 0;
uint MyOctant:: m_uMaxLevel = 3;
uint MyOctant:: m_uIdealEntityCount = 5 ;
uint MyOctant::GetOctantCount(void) { return m_uOctantCount; };

void MyOctant::Init(void)
{
	m_uChildren = 0;

	m_fSize = 0;

	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Max = vector3(0.0f);
	m_v3Min = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;

	for (uint i = 0; i < 8; i++) {
		m_pChild[i] = nullptr;
	}

}

void MyOctant::Swap(MyOctant& other)
{
	std::swap(m_uChildren,other.m_uChildren); 
	std::swap(m_fSize,other.m_fSize);
	std::swap(m_uID,other.m_uID); 
	std::swap(m_uLevel,other.m_uLevel);
	std::swap(m_v3Center,other.m_v3Center);
	std::swap(m_v3Max,other.m_v3Max);
	std::swap(m_v3Min,other.m_v3Min);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	std::swap(m_lChild, other.m_lChild); 
	std::swap(m_pRoot, other.m_pRoot); 
	std::swap(m_pParent, other.m_pParent); 

	for (uint i = 0; i < 8; i++) {
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}

MyOctant* MyOctant::GetParent(void) { return m_pParent; }

void MyOctant::Release(void)
{
	if (m_uLevel == 0) {
		KillBranches();
	}
	m_uChildren = 0;
	m_fSize = 0;
	m_EntityList.clear();
	m_lChild.clear();
}

MyOctant::MyOctant(uint maxLevel, uint idealEntityCount){
	Init();

	m_uOctantCount = 0;
	m_uMaxLevel = maxLevel;
	m_uIdealEntityCount = idealEntityCount;
	m_uID = m_uOctantCount;

	m_pRoot = this;
	m_lChild.clear();

	std::vector<vector3> minMax; // will hold all min max in here

	uint objectCount = m_pEntityMngr->GetEntityCount();

	for (uint i = 0; i < objectCount; i++)
	{
		MyEntity* pEntity = m_pEntityMngr->GetEntity(i);
		MyRigidBody* pRigidBody = pEntity->GetRigidBody();
		minMax.push_back(pRigidBody->GetMinGlobal());
		minMax.push_back(pRigidBody->GetMaxGlobal());
	}

	MyRigidBody* pRigidBody = new MyRigidBody(minMax);

	vector3 vHalfWidth = pRigidBody->GetHalfWidth();
	float max = vHalfWidth.x;
	for (int i = 1; i < 3; i++) {
		if (max < vHalfWidth[i])
			max = vHalfWidth[i];
	}
	vector3 center = pRigidBody->GetCenterLocal();
	minMax.clear();
	SafeDelete(pRigidBody);

	m_fSize = max * 2.0f;
	m_v3Center = center;
	m_v3Min = m_v3Center - (vector3(max));
	m_v3Max = m_v3Center + (vector3(max));

	m_uOctantCount++;

	ConstructTree(m_uMaxLevel);
}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize) 
{
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - (vector3(a_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(a_fSize) / 2.0f);

	m_uOctantCount++;
}

MyOctant::MyOctant(MyOctant const& other)
{
	m_uChildren = other.m_uChildren;

	m_fSize = other.m_fSize;

	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;

	m_v3Center = other.m_v3Center;
	m_v3Max = other.m_v3Max;
	m_v3Min = other.m_v3Min;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_pRoot = other.m_pRoot;
	m_lChild = other.m_lChild;
	m_pParent = other.m_pParent;

	for (uint i = 0; i < 8; i++) {
		m_pChild[i] = other.m_pChild[i];
	}

}

MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}

MyOctant::~MyOctant() { Release(); };

// just accessors
float MyOctant::GetSize(void) { return m_fSize; };
vector3 MyOctant::GetCenterGlobal(void) { return m_v3Center; };
vector3 MyOctant::GetMaxGlobal(void) { return m_v3Max; };
vector3 MyOctant::GetMinGlobal(void) { return m_v3Min; };

void MyOctant::Display(uint index, vector3 color) {

	if (m_uID == index)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center)*
		glm::scale(vector3(m_fSize)),color,RENDER_WIRE);

		return;
	}
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->Display(index);
	}
}

void MyOctant::Display(vector3 color)
{
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), color, RENDER_WIRE);
}

// THIS KILLED ME
void MyOctant::Subdivide(void)
{
	// maximum depth
	if (m_uLevel >= m_uMaxLevel)
		return;

	// already subdivided
	if (m_uChildren != 0)
		return;

	m_uChildren = 8;

	float size = m_fSize / 4.0f;
	float size2 = size * 2.0f;

	vector3 center;

	// octant 0, left back bottom
	center = m_v3Center;
	center.x -= size;
	center.y -= size;
	center.z -= size;

	m_pChild[0] = new MyOctant(center, size2);

	// octant 1, right front bottom
	center.x += size2;
	m_pChild[1] = new MyOctant(center, size2);

	// octant 2, right front bottom
	center.z += size2;
	m_pChild[2] = new MyOctant(center, size2);

	// octant 3, left front bottom
	center.x -= size2;
	m_pChild[3] = new MyOctant(center, size2);

	// octant 4, left front top
	center.y += size2;
	m_pChild[4] = new MyOctant(center, size2);

	// octant 5, left back bottom
	center.z -= size2;
	m_pChild[5] = new MyOctant(center, size2);

	// octant 6, right back top
	center.x += size2;
	m_pChild[6] = new MyOctant(center, size2);

	// octant 7, Right front top
	center.z += size2;
	m_pChild[7] = new MyOctant(center, size2);

	// setting the child's root as the root and parent this so we can move further
	for (uint i = 0; i < 8; i++) {
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		// subdivide if contains more 
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
		{
			m_pChild[i]->Subdivide();
		}
	}
}

MyOctant* MyOctant::GetChild(uint nChild) 
{
	if (nChild > 7)return nullptr;
	return m_pChild[nChild];
}
bool MyOctant::IsColliding(uint index)
{
	uint objectCount = m_pEntityMngr->GetEntityCount();

	if (index >= objectCount)
		return false;

	MyEntity* pEntity = m_pEntityMngr->GetEntity(index);
	MyRigidBody* pRigidbody = pEntity->GetRigidBody();
	vector3 minG = pRigidbody->GetMinGlobal();
	vector3 maxG = pRigidbody->GetMaxGlobal();

	// Simple collision checking

	//x
	if (m_v3Max.x < minG.x)
		return false;
	if (m_v3Min.x > maxG.x)
		return false;

	//y
	if (m_v3Max.y < minG.y)
		return false;
	if (m_v3Min.y > maxG.y)
		return false;

	//z
	if (m_v3Max.z < minG.z)
		return false;
	if (m_v3Min.z > maxG.z)
		return false;

	return true;
}

bool MyOctant::IsLeaf(void) { return m_uChildren == 0; }
bool MyOctant::ContainsMoreThan(uint entities)
{
	// simple contains more than check with entitites
	uint count = 0;
	uint objectCount = m_pEntityMngr->GetEntityCount();
	for (uint i = 0; i < objectCount; i++)
	{
		if (IsColliding(i))
			count++;
		if (count > entities)
			return true;
	}
	return false;

}
void MyOctant::KillBranches(void)
{
	// helps in killing the tree
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->KillBranches();
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}
	m_uChildren = 0;
}

void MyOctant::DisplayLeafs(vector3 color)
{
	// displaying leafs when there are more nodes
	uint nLeafs = m_lChild.size();
	for (uint numberChild = 0; numberChild < nLeafs; numberChild++)
	{
		m_lChild[numberChild]->DisplayLeafs(color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), color, RENDER_WIRE);
}

void MyOctant::ClearEntityList(void)
{
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ClearEntityList();
	}
	m_EntityList.clear();
}

void MyOctant::ConstructTree(uint maxLevel)
{
	if (m_uLevel != 0)
		return;

	m_uMaxLevel = maxLevel;
	m_uOctantCount = 1;

	// clear entity leist
	m_EntityList.clear();

	//clear the tree made
	KillBranches();
	m_lChild.clear();

	// start subdividing
	if (ContainsMoreThan(m_uIdealEntityCount))
	{
		Subdivide();
	}

	// add id to entities
	AssignIDtoEntity();

	//construct list of objects
	ConstructList();
}
void MyOctant::AssignIDtoEntity(void)
{
	// looping through child
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->AssignIDtoEntity();
	}
	if (m_uChildren == 0) // means its a leaf
	{
		uint entities = m_pEntityMngr->GetEntityCount();
		for (uint i = 0; i < entities; i++)
		{
			if (IsColliding(i))
			{
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}

void MyOctant::ConstructList(void)
{
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ConstructList();
	}

	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}

}

