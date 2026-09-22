#include "Rendering/MenuNavigation.hpp"
#include "Rendering/MenuFocus.hpp"

namespace Stand::Rendering
{
	std::vector<MenuNavigation::Level> MenuNavigation::s_Stack{};
	std::unordered_map<size_t, std::vector<MenuNavigation::Level>> MenuNavigation::s_SavedStacks{};

	void MenuNavigation::Reset(std::string rootLabel, Grid* rootContent)
	{
		s_Stack.clear();
		s_Stack.push_back(Level{std::move(rootLabel), rootContent});
	}

	void MenuNavigation::Push(std::string label, Grid* content)
	{
		MenuFocus::SaveFor(s_Stack.empty() ? nullptr : s_Stack.back().Content);
		s_Stack.push_back(Level{std::move(label), content});
		MenuFocus::RestoreFor(s_Stack.back().Content);
	}

	void MenuNavigation::Pop()
	{
		// The root level (index 0) always stays - there's nowhere further
		// back to go once you're there, same as Stand's own address bar.
		if (s_Stack.size() > 1)
		{
			MenuFocus::SaveFor(s_Stack.back().Content);
			s_Stack.pop_back();
			MenuFocus::RestoreFor(s_Stack.back().Content);
		}
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
