#pragma once

#include "datBitBuffer.hpp"

namespace rage
{
	struct netMessage
	{
		static bool WriteHeader(unsigned int id, datBitBuffer& buf)
		{
			if (buf.writeU32(0x3246, 14))
			{
				if (id > 0xFF)
				{
					return buf.writeBool(true)
						&& buf.writeU32(id, 16)
						;
				}
				else
				{
					return buf.writeBool(false)
						&& buf.writeU32(id, 8)
						;
				}
			}
			return false;
		}

		static unsigned int readHeader(datBitBuffer& buf)
		{
			if (buf.readU32(14) == 0x3246)
			{
				if (buf.readBool())
				{
					return buf.readU16();
				}
				else
				{
					return buf.readU8();
				}
			}
			return -1;
		}

		template <typename Msg>
		static bool Export(const Msg& msg, void* data, unsigned int size, unsigned int* written)
		{
			datExportBuffer buf;
			buf.SetReadWriteBytes(data, size);
			if (WriteHeader(Msg::id, buf)
				&& const_cast<Msg&>(msg).ser(buf)
				)
			{
				if (written)
				{
					*written = buf.GetByteLength();
				}
				return true;
			}
			return false;
		}
	};
}
