#include "Rendering/MenuNavigation.hpp"

namespace Stand::Rendering
{
	std::vector<MenuNavigation::Level> MenuNavigation::s_Stack{};

	void MenuNavigation::Reset(std::string rootLabel, Grid* rootContent)
	{
		s_Stack.clear();
		s_Stack.push_back(Level{std::move(rootLabel), rootContent});
	}

	void MenuNavigation::Push(std::string label, Grid* content)
	{
		s_Stack.push_back(Level{std::move(label), content});
	}

	void MenuNavigation::Pop()
	{
		// The root level (index 0) always stays - there's nowhere further
		// back to go once you're there, same as Stand's own address bar.
		if (s_Stack.size() > 1)
			s_Stack.pop_back();
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
}
