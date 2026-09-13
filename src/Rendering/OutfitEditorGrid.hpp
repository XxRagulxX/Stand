#pragma once
#include "Ped/Outfit.hpp"
#include "Rendering/Grid.hpp"

#include <string>
#include <vector>

namespace Stand::Rendering
{
	class GridItemTextInput;
	
	class OutfitEditorGrid : public Grid
	{
	public:
		OutfitEditorGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		void RefreshStats();
		void RefreshList();

		void AddComponentDrawableRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddComponentTextureRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddPropDrawableRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddPropTextureRows(std::vector<std::unique_ptr<GridItem>>& items_draft);

		Outfit::OutfitComponents m_Components{};
		Outfit::OutfitProps m_Props{};

		std::vector<std::string> m_Folders{};
		std::vector<std::string> m_Files{};
		std::string m_Folder{};
		std::string m_File{};

		GridItemTextInput* m_OutfitNameInput = nullptr;
		GridItemTextInput* m_NewFolderInput = nullptr;
		bool m_ApplyHair = false;
	};
}
