#pragma once
#include "Rendering/GridItem.hpp"
#include "Rendering/GridItemFocusTracker.hpp"
#include "Util/Joaat.hpp"

#include <atomic>
#include <memory>
#include <string>

namespace Stand::Rendering
{
    class VehiclePreviewItem : public GridItem
    {
    public:
        VehiclePreviewItem(int16_t width, int16_t height, joaat_t hash);
        ~VehiclePreviewItem() override;

        void draw() override;
        void drawText() override;
        bool isFocusable() const override { return true; }
        void onClick(int16_t, int16_t) override;
        void activate() override;

    private:
        joaat_t m_Hash;
        std::string m_ModelName;
        std::string m_MakerName;
        bool m_NameLoaded = false;

        struct PreviewFiber
        {
            std::atomic<bool> active{true};
        };
        std::shared_ptr<PreviewFiber> m_Fiber;
        GridItemFocusTracker m_FocusTracker;

        void EnsureDisplayName();
        std::string CurrentDisplayName() const;
        void StartPreview();
        void StopPreview();
    };
}
