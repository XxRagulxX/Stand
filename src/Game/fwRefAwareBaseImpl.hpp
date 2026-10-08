#pragma once

namespace rage
{
	struct fwKnownRefHolder
	{
		void** m_ppReference;
		fwKnownRefHolder* m_pNext;
	};

	template<typename T>
	class fwRefAwareBaseImpl : public T
	{
	public:
		fwKnownRefHolder* m_pKnownRefHolderHead;
	};
}
