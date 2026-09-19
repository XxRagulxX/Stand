#pragma once
#include "Rendering/GridItemEntityPreview.hpp"
#include "Util/Joaat.hpp"

#include <string>

namespace Stand::Rendering
{
    class VehicleEntityPreview : public GridItemEntityPreview
    {
    public:
        VehicleEntityPreview(int16_t width, int16_t height, joaat_t hash);

        void draw() override;
        void drawText() override;
        bool isFocusable() const override { return true; }
        void onClick(int16_t, int16_t) override;
        void activate() override;

    protected:
        [[nodiscard]] std::uint32_t GetModelHash() const override;
        [[nodiscard]] Entity CreateEntity(const rage::fvector3& pos) const override;
        [[nodiscard]] float GetAdditionalOffset() const override;
        void OnPreviewTick(Entity& entity) override;

    private:
        joaat_t m_Hash;
        std::string m_ModelName;
        std::string m_MakerName;
        bool m_NameLoaded = false;
        mutable Vector3 m_ModelMin{};
        mutable Vector3 m_ModelMax{};
        mutable bool m_DimsLoaded = false;

        void EnsureDisplayName();
        void EnsureModelDims() const;
        std::string CurrentDisplayName() const;
    };
}
