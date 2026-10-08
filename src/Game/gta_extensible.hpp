#pragma once

#include "Game/struct_base.hpp"
#include "Game/fwRefAwareBase.hpp"

#pragma pack(push, 1)
namespace rage
{
	class fwExtension
	{
	public:
		virtual ~fwExtension() = default;
		virtual void _0x08() {}
		virtual void _0x10() {}
		virtual uint32_t GetExtensionId() { return 0; };
	};
	static_assert(sizeof(fwExtension) == 0x08);

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
					if (node->data->GetExtensionId() == id)
					{
						return node->data;
					}
				}
			}
			return nullptr;
		}
	};
	static_assert(sizeof(fwExtensionList) == 0x10);

	class fwExtensibleBase : public fwRefAwareBase
	{
	public:
		fwExtensionList extensions;
	};
	static_assert(sizeof(fwExtensibleBase) == 0x20);
}
#pragma pack(pop)
