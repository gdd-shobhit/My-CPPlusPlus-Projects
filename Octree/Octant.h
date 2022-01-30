#pragma once
#ifndef __OCTANTCLASS_H_
#define __OCTANTCLASS_H_

#endif // !__OCTANTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex {



	class Octant {
		static uint octantCount;
		static uint maxLevelToGo;
		static uint idealEntityCount;

		// with respect to octants
		uint currentID = 0;
		uint currentLevel = 0;
		
		float sizeOfOctant = 0.0f;
		
		uint octantChildren = 0; // children of octant

		// singletons
		MeshManager* m_pMeshMngr = nullptr;
		MyEntityManager* m_pEntityMngr = nullptr;
		
		vector3 max = vector3(0.0f);
		vector3 min = vector3(0.0f);
		vector3 center = vector3(0.0f);

		Octant* child[8];
		Octant* parent = nullptr;

		std::vector<uint> entityList;

		Octant* root = nullptr;
		std::vector<Octant*> childNode;

	public:

	};
}