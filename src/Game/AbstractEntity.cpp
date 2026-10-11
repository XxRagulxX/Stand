#include "Game/AbstractEntity.hpp"

#include "Core/Pointers.hpp"
#include "Game/CDynamicEntity.hpp"
#include "Game/fwEntity.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
	bool AbstractEntity::isValid() const noexcept
	{
		return ptr != nullptr || handle != INVALID_GUID;
	}

	bool AbstractEntity::isFullyUsable()
	{
		return ensureHandleAndPointer() && ptr != nullptr && handle != INVALID_GUID;
	}

	bool AbstractEntity::hasHandle() const
	{
		return handle != INVALID_GUID;
	}

	bool AbstractEntity::ensureHandle()
	{
		if (handle != INVALID_GUID) return true;
		return findHandle();
	}

	bool AbstractEntity::findHandle()
	{
		if (ptr != nullptr)
		{
			handle = Pointers.PtrToHandle(ptr);
			return handle != INVALID_GUID;
		}
		return false;
	}

	bool AbstractEntity::isHandleEquals(Entity h)
	{
		return getHandle() == h;
	}

	void AbstractEntity::releaseHandle()
	{
		handle = INVALID_GUID;
	}

	bool AbstractEntity::setHandle(Entity desired_handle)
	{
		handle = desired_handle;
		return true;
	}

	void AbstractEntity::setHandleReference(uint8_t reference)
	{
	}

	bool AbstractEntity::ensurePointer()
	{
		if (ptr != nullptr) return true;
		return findPointer();
	}

	bool AbstractEntity::findPointer()
	{
		if (handle != INVALID_GUID)
		{
			ptr = reinterpret_cast<CPhysical*>(Pointers.HandleToPtr(handle));
			return ptr != nullptr;
		}
		return false;
	}

	bool AbstractEntity::ensureHandleAndPointer()
	{
		if (handle == INVALID_GUID)
			return findHandle();
		else if (ptr == nullptr)
			return findPointer();
		return true;
	}

	Entity AbstractEntity::getHandle()
	{
		if (handle == INVALID_GUID) findHandle();
		return handle;
	}

	CPhysical* AbstractEntity::getPointer()
	{
		if (ptr == nullptr) findPointer();
		return ptr;
	}

	void* AbstractEntity::getNetObjectImpl()
	{
		if (ensurePointer())
			return reinterpret_cast<CDynamicEntity*>(ptr)->m_NetObject;
		return nullptr;
	}

	AbstractEntity::operator Entity()
	{
		return getHandle();
	}

	AbstractEntity::operator CPed*()
	{
		return reinterpret_cast<CPed*>(getPointer());
	}

	AbstractEntity::operator CVehicle*()
	{
		return reinterpret_cast<CVehicle*>(getPointer());
	}

	bool AbstractEntity::operator==(AbstractEntity& b)
	{
		if (handle != INVALID_GUID && b.handle != INVALID_GUID)
			return handle == b.handle;
		return getPointer() == b.getPointer();
	}

	bool AbstractEntity::operator==(AbstractEntity&& b)
	{
		return operator==(b);
	}

	bool AbstractEntity::operator!=(AbstractEntity& b)
	{
		return !operator==(b);
	}

	void AbstractEntity::operator=(const AbstractEntity& b)
	{
		this->ptr = b.ptr;
		this->handle = b.handle;
	}

	std::size_t AbstractEntity::hash() const noexcept
	{
		return (((uint64_t)(uintptr_t)ptr) << 32) | (uint32_t)handle;
	}

	void AbstractEntity::reset() noexcept
	{
		this->ptr = nullptr;
		this->handle = INVALID_GUID;
	}

	bool AbstractEntity::exists()
	{
		return ensureHandle() && ENTITY::DOES_ENTITY_EXIST(handle);
	}

	v3 AbstractEntity::getPos()
	{
		if (ensurePointer())
		{
			const auto& row = reinterpret_cast<rage::fwEntity*>(ptr)->m_Transform.rows[3];
			return { row.x, row.y, row.z };
		}
		return {};
	}

	void AbstractEntity::setPos(const v3& pos)
	{
		if (ensureHandle())
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(handle, pos.x, pos.y, pos.z, true, true, true);
	}

	v3 AbstractEntity::getRot()
	{
		if (ensureHandle())
			return ENTITY::GET_ENTITY_ROTATION(handle, 2);
		return {};
	}

	bool AbstractEntity::isOwner()
	{
		if (ensureHandle())
			return NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(handle);
		return true;
	}

	void AbstractEntity::requestControl(Hash reason, std::function<void(AbstractEntity&)>&& callback, std::function<void(AbstractEntity&)>&& timeout_callback)
	{
		if (!ensureHandle()) return;
		if (isOwner())
		{
			callback(*this);
		}
		else
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(handle);
			if (isOwner())
				callback(*this);
			else
				timeout_callback(*this);
		}
	}

	int AbstractEntity::getSeat(AbstractEntity& veh)
	{
		if (ensureHandle() && veh.ensureHandle())
		{
			const int passengers = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(veh.handle);
			for (int i = -1; i < passengers; i++)
			{
				if (VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh.handle, i, 0) == handle)
					return i;
			}
		}
		return -3;
	}
}
