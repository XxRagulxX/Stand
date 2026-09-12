#pragma once
#include "Ped/ePedVarComp.hpp"

#include <cstddef>

namespace Stand
{
	struct PedComponent
	{
		int id;
		const char* label;
		const char* cmd_name;
		const char* var_label;
		const char* var_cmd_name;
	};

	struct PedProp
	{
		int id;
		const char* label;
		const char* cmd_name;
		const char* var_label;
		const char* var_cmd_name;
	};

	inline const PedComponent ped_components[] = {
		{PV_COMP_HEAD, "Head",           "head",        "Head Variation",        "headvar"},
		{PV_COMP_BERD, "Facial Hair",    "face",        "Facial Hair Variation", "facevar"},
		{PV_COMP_HAIR, "Hair",           "hair",        "Hair Color",            "hairvar"},
		{PV_COMP_UPPR, "Torso",          "top",         "Torso Variation",       "topvar"},
		{PV_COMP_JBIB, "Top",            "top2",        "Top Variation",         "top2var"},
		{PV_COMP_ACCS, "Accessories",    "top3",        "Accessories Variation", "top3var"},
		{PV_COMP_TASK, "Bag",            "bag",         "Bag Variation",         "bagvar"},
		{PV_COMP_HAND, "Gloves",         "gloves",      "Gloves Variation",      "glovesvar"},
		{PV_COMP_LOWR, "Pants",          "pants",       "Pants Variation",       "pantsvar"},
		{PV_COMP_FEET, "Shoes",          "shoes",       "Shoes Variation",       "feetvar"},
		{PV_COMP_TEEF, "Accessories 2",  "accessories", "Accessories 2 Var",     "accessoriesvar"},
		{PV_COMP_DECL, "Decals",         "decals",      "Decals Variation",      "decalsvar"},
	};

	inline const PedProp ped_props[] = {
		{0, "Hat",      "hat",      "Hat Variation",      "hatvar"},
		{1, "Glasses",  "glasses",  "Glasses Variation",  "glassesvar"},
		{2, "Ear Wear", "ears",     "Ear Wear Variation", "earsvar"},
		{6, "Watch",    "watch",    "Watch Variation",    "watchvar"},
		{7, "Bracelet", "bracelet", "Bracelet Variation", "braceletvar"},
	};
}

#define NUM_PED_COMPONENTS (sizeof(Stand::ped_components) / sizeof(Stand::ped_components[0]))
#define NUM_PED_PROPS      (sizeof(Stand::ped_props) / sizeof(Stand::ped_props[0]))
#define COMPACT_OUTFIT_LEGACY_SIZE ((NUM_PED_COMPONENTS + NUM_PED_PROPS) * 2)
#define COMPACT_OUTFIT_SIZE        (COMPACT_OUTFIT_LEGACY_SIZE + 1)
