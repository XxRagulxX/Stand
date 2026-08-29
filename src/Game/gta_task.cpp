#include "Game/gta_task.hpp"

#include "Ped/eTaskType.hpp"

bool CTask::IsValidNMControlSubTask() const noexcept
{
	return IsNMBehaviourTask() || IsBlendFromNMTask() || type_id == CTaskTypes::TASK_RAGE_RAGDOLL;
}
