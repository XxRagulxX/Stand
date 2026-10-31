#pragma once

#include "Commands/Widgets/tbToggleableBehaviour.hpp"

namespace Stand
{
#pragma pack(push, 1)
	class tbToggleableBehaviourScript : public tbToggleableBehaviour
	{
	public:
		void runTick() { onTick(); }

	protected:
		void onEnable() override;

		virtual void onTick() = 0;
	};
#pragma pack(pop)
}
