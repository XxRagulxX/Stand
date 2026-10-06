#pragma once
#include "Rendering/StandPort/GridItem.hpp"
#include "Rendering/StandPort/TextureDynamic.hpp"
#include "Rendering/StandPort/ThemeIcons.hpp"

#include <DirectXMath.h>

#include <string>
#include <vector>

#include "Game/typedecl.hpp"

namespace Stand
{
    class Command;
    class CommandList;

#pragma pack(push, 1)
    struct DrawCommandData
    {
        DirectX::XMFLOAT4 textColour{};
        DirectX::XMFLOAT4 rightTextColour{};
        bool focused = false;
        bool hasRectColourOverride = false;
        DirectX::XMFLOAT4 rectColourOverride{};
        bool centered = false;
        std::wstring rightbound_text{};
        float rightbound_text_width = 0.f;
        std::wstring trimmed_name{};
        Rendering::IconSlot leftIcon = Rendering::IconSlot::Count;
        Rendering::IconSlot rightIcon = Rendering::IconSlot::Count;
        DirectX::XMFLOAT4 iconColour{1.f, 1.f, 1.f, 1.f};

        void setSlider(std::wstring&& value, std::wstring& name, float& command_name_max_width);
    };

    struct DrawListData
    {
        cursor_t offset = 0;
        bool has_extra_top = false;
        std::vector<DrawCommandData> list{};
    };

    class GridItemList : public Rendering::GridItem
    {
    public:
        CommandList* const view;
        const cursor_t offset;
        TextureDynamic contentsTex{};

        explicit GridItemList(CommandList* view, int16_t height, uint8_t priority, Rendering::Alignment alignment_relative_to_last, cursor_t offset);

        void update();

        [[nodiscard]] bool containsCommand(const Command* target) const noexcept;

        void draw() override;
        void drawText() override;

    private:
        DrawListData m_draw_data{};

        static bool trimTextH(std::wstring& text, float scale, float maxWidth);
    };
#pragma pack(pop)
}
