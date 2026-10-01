#include "Rendering/MenuNavigation.hpp"
#include "Rendering/MenuFocus.hpp"

namespace Stand::Rendering
{
	std::vector<MenuNavigation::Level> MenuNavigation::s_Stack{};
	std::unordered_map<size_t, std::vector<MenuNavigation::Level>> MenuNavigation::s_SavedStacks{};
	std::mutex MenuNavigation::s_Mutex{};

	void MenuNavigation::Reset(std::string rootLabel, Grid* rootContent)
	{
		std::lock_guard lock(s_Mutex);
		s_Stack.clear();
		s_Stack.push_back(Level{std::move(rootLabel), rootContent});
	}

	void MenuNavigation::Push(std::string label, Grid* content)
	{
		Grid* prev;
		{
			std::lock_guard lock(s_Mutex);
			prev = s_Stack.empty() ? nullptr : s_Stack.back().Content;
			s_Stack.push_back(Level{std::move(label), content});
		}
		MenuFocus::SaveFor(prev);
		MenuFocus::RestoreFor(content);
	}

	void MenuNavigation::Pop()
	{
		Grid* saved = nullptr;
		Grid* restored = nullptr;
		{
			std::lock_guard lock(s_Mutex);
			if (s_Stack.size() > 1)
			{
				saved = s_Stack.back().Content;
				s_Stack.pop_back();
				restored = s_Stack.back().Content;
			}
		}
		if (saved)
		{
			MenuFocus::SaveFor(saved);
			MenuFocus::RestoreFor(restored);
		}
	}

	bool MenuNavigation::IsDescendantActive(const Grid* g)
	{
		std::lock_guard lock(s_Mutex);
		for (const auto& level : s_Stack)
			if (level.Content == g)
				return true;
		return false;
	}

	Grid* MenuNavigation::Current()
	{
		return s_Stack.empty() ? nullptr : s_Stack.back().Content;
	}

	std::string MenuNavigation::BreadcrumbPath()
	{
		std::string path;
		for (const auto& level : s_Stack)
		{
			if (!path.empty())
				path += " > ";
			path += level.Label;
		}
		return path;
	}

	void MenuNavigation::SaveStackFor(size_t sidebarIndex)
	{
		s_SavedStacks[sidebarIndex] = s_Stack;
	}

	bool MenuNavigation::RestoreStackFor(size_t sidebarIndex)
	{
		const auto it = s_SavedStacks.find(sidebarIndex);
		if (it == s_SavedStacks.end() || it->second.empty())
			return false;
		s_Stack = it->second;
		return true;
	}
}
