#pragma once
#include "Rendering/StandPort/Grid.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Rendering/StandPort/Position2d.hpp"

namespace Stand::Rendering
{
	class TransformModelListGrid : public Grid
	{
	public:
		explicit TransformModelListGrid(Stand::CommandList* list);

		static TransformModelListGrid& GetOrCreate(Stand::CommandList* list);

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		Stand::CommandList* m_List;
	};
}
