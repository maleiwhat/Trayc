#ifndef EN_GLOBAL_RENDERING_PARAMS_H
#define EN_GLOBAL_RENDERING_PARAMS_H

#include <string>

namespace engine
{
    // Connects texture types with texture slots
    enum TextureType
    {
        // Model texture maps
        DIFFUSE_MAP = 0,
        NORMAL_MAP = 1,
        SPECULAR_MAP = 2,
        HEIGHT_MAP = 3,

        CT_MAT_TEXTURE_TYPES = 4,

        // Geometry pass textures
        G_DEPTH_TEXTURE = 5,
        G_NORMAL_TEXTURE = 6,
        G_SPEC_GLOSS_TEXTURE = 7,
        G_ALBEDO_TEXTURE = 8,

        DEBUG_SLOT = 31,

        CT_TEX_SLOTS = 32
    };

    // Index of the g-pass texture in g-framebuffer
    int GetMRTIdx(TextureType type);
}

#endif
