#pragma once

#include "texture.hpp"

#include "color.hpp"

namespace gfx {
    class ColorTexture : public Texture
    {
    public:
        /* Constructors */

        // Default Constructor
        ColorTexture() = default;

        // Standard Constructor (Float List)
        ColorTexture(const double r, const double g, const double b)
                : Texture(), m_color{ r, g, b }
        {}

        // Standard Constructor (Color Object)
        explicit ColorTexture(const Color& color)
                : Texture(), m_color{ color }
        {}

        /* Destructor */

        ~ColorTexture() override = default;

        /* Accessors */

        [[nodiscard]] Color getTextureColorAt(const Vector4& object_point,
                                              const TextureMap& mapping) const override
        { return m_color; }

        /* Object Operations */

        // Creates a copy of this texture and returns a smart pointer to the new object
        [[nodiscard]] std::shared_ptr<Texture> clone() const override
        { return std::make_shared<ColorTexture>(*this); }

    private:
        /* Data Members */

        Color m_color{ white() };

        /* Texture Helper Method Overrides */

        [[nodiscard]] bool areEquivalent(const Texture& other_texture) const override;
    };

}
