#pragma once

namespace Render {

    /*
     * These headers will be attached to every shader that the shaderserver compiles.
     */
    const char VertexShaderHeader[] =
            "#version 430\n"
            "layout (std140, binding = 0) uniform GlobalBlock\n"
            "{\n"
            "mat4 View;\n"
            "mat4 Projection;\n"
            "};\n"
            "\n"
            "uniform mat4 Model;\n";

    const char FragmentShaderHeader[] =
            "#version 430\n";
}