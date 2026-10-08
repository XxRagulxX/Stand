#pragma once

#include "Game/gta_fwddecl.hpp"
#include "Game/gta_extensible.hpp"

class CPedHeadBlendData : public rage::fwExtension
{
public:
	char pad[0x10];
	int hair_colour_id;
};
