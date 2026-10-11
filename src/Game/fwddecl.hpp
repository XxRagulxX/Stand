#pragma once

#include <cstdint>

namespace rage
{
	class scrThread;
}

enum eEntityType : uint8_t;
enum PedBoneId : int;

namespace Stand
{
	class AbstractEntity;
	class AbstractModel;
	class AbstractPlayer;

	enum name_t : uint8_t;
	enum Direction : uint8_t;

	struct Box;
	struct TargetingData;
}
