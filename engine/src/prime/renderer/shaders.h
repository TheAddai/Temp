#pragma once

// sprite shader
static const char* s_spriteVertexShaderSource = R"(

    #version 330 core

    layout (location = 0) in vec2 a_position;
    layout (location = 1) in vec4 a_color;

    out vec4 v_color;

    layout(std140) uniform cameraBlock
    {
	    mat4 u_viewProjection;
    };

    void main()
    {
        v_color = a_color;
        gl_Position = u_viewProjection * vec4(a_position, 0.0, 1.0);
    }
)";

static const char* s_spritePixelShaderSource = R"(

    #version 330 core

    layout (location = 0) out vec4 color;

    in vec4 v_color;

    void main()
    {
        color = v_color;
    }
)";