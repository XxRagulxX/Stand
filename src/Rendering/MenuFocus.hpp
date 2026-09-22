#pragma once
#include <cstddef>
#include <cstdint>
#include <unordered_map>

namespace Stand::Rendering
{
	class Grid;
	class GridItem;

	class MenuFocus
	{
	public:
		enum class Region : uint8_t
		{
			Sidebar,
			Content
		};
		static Region GetRegion();
		static void SetRegion(Region region);
		static GridItem* GetFocusedItem(Grid* currentContent);
		static void MoveContent(Grid* currentContent, int delta);
		static void SetFocusedItem(Grid* currentContent, const GridItem* item);

		// Called from MenuNavigation::Push()/Pop() to explicitly save the
		// departing grid's index and restore the arriving grid's saved index.
		static void SaveFor(Grid* grid);
		static void RestoreFor(Grid* grid);

	private:
		static Region s_Region;
		static Grid* s_LastContent;
		static size_t s_ContentIndex;
		static std::unordered_map<Grid*, size_t> s_SavedIndices;
	};
}
