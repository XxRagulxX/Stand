#pragma once
#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Core/FileMgr.hpp"

#include <filesystem>
#include <shellapi.h>
#include <windows.h>

namespace Stand::Features
{
    namespace
    {
        static StandWidgets::CommandLambdaAction s_OpenGarageFolder{
            "opengaragefolder",
            "Open Folder",
            "Opens the folder containing your saved garage vehicles.",
            [] {
                auto path = FileMgr::GetProjectFolder("./Vehicles").Path();
                std::filesystem::create_directories(path);
                ShellExecuteA(nullptr, "explore", path.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            }
        };
    }
}
