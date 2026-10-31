#pragma once

#include "Game/fwExtension.hpp"
#include "Game/fwRefAwareBase.hpp"

#include <soup/structing.hpp>

#pragma pack(push, 1)
namespace rage
{
	class fwExtensionList
	{
	public:
		class LinkedListNode
		{
		public:
			fwExtension* data;
			LinkedListNode* next;
		};
		static_assert(sizeof(LinkedListNode) == 0x10);

		LinkedListNode* head;
		uint32_t bitflag;
		PAD(0x08 + 4, 0x10);

		[[nodiscard]] fwExtension* Get(uint32_t id)
		{
			if (id >= 32 || ((bitflag >> id) & 1))
			{
				for (LinkedListNode* node = head; node != nullptr; node = node->next)
				{
					if (node->data->GetId() == id)
					{
						return node->data;
					}
				}
			}
			return nullptr;
		}
	};
	static_assert(sizeof(fwExtensionList) == 0x10);

	class fwExtensibleBase2 : public fwRefAwareBase
	{
	public:
		fwExtensionList extensions;
	};
	static_assert(sizeof(fwExtensibleBase2) == 0x20);
}
#pragma pack(pop)
