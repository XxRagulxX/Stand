#pragma once

#include "Rendering/StandPort/CommandColour.hpp"
#include "Commands/Widgets/CommandEspTags.hpp"
#include "Game/AbstractEntity.hpp"
#include "Game/AbstractPlayer.hpp"

#include <SimpleMath.h>

namespace Stand
{
    template <class T>
    class CommandEspBase : public T
    {
    public:
        using T::T;

        CommandColour* colour = nullptr;
        CommandEspTags* tag_colours = nullptr;

    protected:
        [[nodiscard]] DirectX::SimpleMath::Color getColour(AbstractEntity&) const
        {
            return colour->getRGBA();
        }

        [[nodiscard]] DirectX::SimpleMath::Color getColour(AbstractPlayer& player) const
        {
            return tag_colours->getColour(player);
        }

        [[nodiscard]] DirectX::SimpleMath::Color getColour(AbstractPlayer&& player) const
        {
            return tag_colours->getColour(player);
        }
    };
}
