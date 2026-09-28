#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Core/types.hpp"

namespace Stand
{
    class CommandVector3 : public CommandList
    {
    protected:
        using CommandList::CommandList;
    public:
        [[nodiscard]] virtual Vector3 getVec() = 0;
        virtual void setVec(Vector3 vec) = 0;
    };
}
