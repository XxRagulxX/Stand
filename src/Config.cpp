#include "Config.hpp"

#include "Core/ExceptionHandler.hpp"
#include "Rendering/Gui.hpp"
#include "Util/get_appdata_path.hpp"

namespace Stand
{
	Config::Config(std::wstring&& name)
		: name(std::move(name))
	{
	}

	Config::~Config() noexcept
	{
	}

	std::wstring Config::getName() const
	{
		return name;
	}

	std::wstring Config::getPath() const
	{
		return get_appdata_path(name + L".txt");
	}

	void Config::setName(std::wstring&& name)
	{
		this->name = std::move(name);
	}

	void Config::load()
	{
	}

	bool Config::canSaveNow() const
	{
		return true;
	}

	void Config::save()
	{
		if (isInitialised() && !save_requested)
		{
			save_requested = true;
			saveJob();
		}
	}

	void Config::saveJob()
	{
		saveJob(std::make_shared<soup::WeakRef<Config>>(this));
	}

	void Config::saveJob(std::shared_ptr<soup::WeakRef<Config>>&& weakref)
	{
		Exceptional::createManagedExceptionalThread([weakref{ std::move(weakref) }]() mutable
		{
			if (auto c = weakref->getPointer())
			{
				if (!c->save_requested || g_gui.unload_state > UnloadState::SAVE_PENDING_CONFIGS)
				{
					return;
				}
				if (!c->canSaveNow())
				{
					return Config::saveJob(std::move(weakref));
				}
				c->saveToFileNow();
				c->setSaveNoLongerNeeded();
			}
		});
	}

	void Config::setSaveNoLongerNeeded() noexcept
	{
		save_requested = false;
	}

	bool Config::isInitialised() const noexcept
	{
		return !name.empty();
	}

	void Config::reset()
	{
		name.clear();
		save_requested = false;
		transient_token.refresh();
	}
}
