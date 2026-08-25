#ifdef STAND_DEBUG

#include "Commands/Stand/CommandSavedata.hpp"

#include <fstream>

#include "lib/soup/json.hpp"
#include "lib/soup/string.hpp"

#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/CommandTogglePointer.hpp"
#include "AntiCheat/ComponentSavedata.hpp"
#include "Rendering/drawDebugText.hpp"
#include "Util/get_appdata_path.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	class CommandSavedataShow : public CommandToggle
	{
	public:
		explicit CommandSavedataShow(CommandList* const parent)
			: CommandToggle(parent, LIT("Show"))
		{
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, []
			{
				drawDebugText(g_comp_savedata.obj.encodePretty());
			});
		}
	};

	CommandSavedata::CommandSavedata(CommandList* const parent)
		: CommandList(parent, LIT("Savedata"))
	{
		this->createChild<CommandTogglePointer>(&g_comp_savedata.record, LIT("Record"));
		this->createChild<CommandTogglePointer>(&g_comp_savedata.replay, LIT("Replay"));
		this->createChild<CommandSavedataShow>();
		this->createChild<CommandLambdaAction>(LIT("Export"), {}, NOLABEL, [](Click&)
		{
			std::ofstream of(get_appdata_path("Savedata.json"));
			of << g_comp_savedata.obj.encodePretty();
		});
		this->createChild<CommandLambdaAction>(LIT("Import"), {}, NOLABEL, [](Click&)
		{
			auto data = soup::string::fromFile(StringUtils::utf16_to_utf8(get_appdata_path("Savedata.json")));
			auto json = soup::json::decode(data);
			g_comp_savedata.obj = std::move(*reinterpret_cast<soup::JsonObject*>(json.release()));
		});
		this->createChild<CommandLambdaAction>(LIT("Clear"), {}, NOLABEL, [](Click&)
		{
			g_comp_savedata.obj.children.clear();
		});
	}
}

#endif
