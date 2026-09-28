#pragma once
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/World/Places/Particles.hpp"

namespace Stand
{
    enum CommandListSelectParticleFlags : uint8_t
    {
        INCLUDE_NONE = 0b1
    };

    class CommandListSelectParticle : public CommandListSelect
    {
    public:
        [[nodiscard]] static std::vector<std::pair<long long, Label>> getOptions(bool include_none = false)
        {
            std::vector<std::pair<long long, Label>> options;
            if (include_none)
                options.emplace_back(0LL, LIT("None"));
            for (const auto& particle : Particles::particles)
                options.emplace_back(reinterpret_cast<long long>(&particle), LIT(particle.rawName));
            return options;
        }

        explicit CommandListSelectParticle(CommandList* parent, Label&& menu_name, Label&& help_text = NOLABEL, uint8_t flags = 0, long long default_value = reinterpret_cast<long long>(&Particles::particles[0]))
            : CommandListSelect(parent, std::move(menu_name), {}, std::move(help_text), getOptions(flags & INCLUDE_NONE), default_value)
        {
        }
    };
}
