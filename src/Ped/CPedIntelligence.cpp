#include "Ped/CPedIntelligence.hpp"

#include "Ped/aiTaskTree.hpp"

CTask* CPedIntelligence::FindTaskActiveByType(const int iType) const noexcept
{
	return reinterpret_cast<CTask*>(task_manager.trees[0]->FindTaskByTypeActive(iType));
}
