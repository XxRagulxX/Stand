#pragma once
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Game/Pools.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

#include <cstdint>
#include <unordered_map>

namespace Stand
{
    class CommandTrafficModelFlag : public CommandListSelect
    {
        static constexpr int    FLAG_INDEX  = 137;
        static constexpr size_t FLAGS_OFFSET = 0x57C;
        static constexpr size_t FLAG_BYTE   = static_cast<size_t>(FLAG_INDEX / 8);
        static constexpr uint8_t FLAG_BIT   = static_cast<uint8_t>(1u << (FLAG_INDEX % 8));

        std::unordered_map<uint8_t*, uint8_t> m_originals;

        uint8_t* getFlagByte(Vehicle& veh)
        {
            auto* ptr = veh.GetPointer<uint8_t*>();
            if (!ptr) return nullptr;
            auto* model_info = *reinterpret_cast<uint8_t**>(ptr + 0x20);
            if (!model_info) return nullptr;
            return &model_info[FLAGS_OFFSET + FLAG_BYTE];
        }

    public:
        explicit CommandTrafficModelFlag(CommandList* parent)
            : CommandListSelect(parent, LIT("Glide Ability"), CMDNAMES("trafficglide"), NOLABEL, {
                {0, LIT("Don't Override")},
                {1, LIT("On")},
                {2, LIT("Off")}
            }, 0)
        {}

        void onChange(Click& click, long long prev_value) final
        {
            if (value == 0)
            {
                CommandTickDispatch::RemoveCommand(this);
                click.ensureScriptThread([this] {
                    for (auto& [byte_ptr, orig] : m_originals)
                        *byte_ptr = orig;
                    m_originals.clear();
                });
            }
            else
            {
                CommandTickDispatch::AddCommand(this);
            }
        }

        void onTick() final
        {
            int player_veh = Self::GetVehicle().GetHandle();
            for (auto veh : Pools::GetVehicles())
            {
                if (!veh || veh.GetHandle() == player_veh) continue;
                uint8_t* byte_ptr = getFlagByte(veh);
                if (!byte_ptr) continue;
                if (m_originals.find(byte_ptr) == m_originals.end())
                    m_originals[byte_ptr] = *byte_ptr;
                if (value == 1)
                    *byte_ptr |= FLAG_BIT;
                else
                    *byte_ptr &= ~FLAG_BIT;
            }
        }

        ~CommandTrafficModelFlag() override
        {
            CommandTickDispatch::RemoveCommand(this);
        }
    };
}
