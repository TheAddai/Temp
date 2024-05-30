#pragma once

// sprite shader
static const char* s_spriteVertexShaderSource = R"(

    #version 330 core

    layout (location = 0) in vec2 a_position;
    layout (location = 1) in vec4 a_color;
    layout (location = 2) in vec2 a_textureCoords;
    layout (location = 3) in float a_textureIndex;

    out vec4 v_color;
    out vec2 v_textureCoords;
    out float v_textureIndex;

    layout(std140) uniform cameraBlock
    {
	    mat4 u_viewProjection;
    };

    void main()
    {
        v_color = a_color;
        v_textureCoords = a_textureCoords;
        v_textureIndex = a_textureIndex;
        gl_Position = u_viewProjection * vec4(a_position, 0.0, 1.0);
    }
)";

static const char* s_spritePixelShaderSource = R"(

    #version 330 core

    layout (location = 0) out vec4 color;

    in vec4 v_color;
    in vec2 v_textureCoords;
    in float v_textureIndex;

    uniform sampler2D u_textures[16];

    void main()
    {
        switch(int(v_textureIndex))
        {
            case 0: color = texture(u_textures[0], v_textureCoords) * v_color; break;
            case 1: color = texture(u_textures[1], v_textureCoords) * v_color; break;
            case 2: color = texture(u_textures[2], v_textureCoords) * v_color; break;
            case 3: color = texture(u_textures[3], v_textureCoords) * v_color; break;
            case 4: color = texture(u_textures[4], v_textureCoords) * v_color; break;
            case 5: color = texture(u_textures[5], v_textureCoords) * v_color; break;
            case 6: color = texture(u_textures[6], v_textureCoords) * v_color; break;
            case 7: color = texture(u_textures[7], v_textureCoords) * v_color; break;
            case 8: color = texture(u_textures[8], v_textureCoords) * v_color; break;
            case 9: color = texture(u_textures[9], v_textureCoords) * v_color; break;
            case 10: color = texture(u_textures[10], v_textureCoords) * v_color; break;
            case 11: color = texture(u_textures[11], v_textureCoords) * v_color; break;
            case 12: color = texture(u_textures[12], v_textureCoords) * v_color; break;
            case 13: color = texture(u_textures[13], v_textureCoords) * v_color; break;
            case 14: color = texture(u_textures[14], v_textureCoords) * v_color; break;
            case 15: color = texture(u_textures[15], v_textureCoords) * v_color; break;
        }
    }
)";

// line shader
static const char* s_lineVertexShaderSource = R"(

    #version 330 core

    layout (location = 0) in vec2 a_position;
    layout (location = 1) in vec4 a_color;

    layout(std140) uniform CameraBlock
    {
	    mat4 u_viewProjection;
    };

    out vec4 v_color;

    void main()
    {
        v_color = a_color;
        gl_Position = u_viewProjection * vec4(a_position, 0.0, 1.0);
    }
)";

static const char* s_linePixelShaderSource = R"(

    #version 330 core

    layout (location = 0) out vec4 color;

    in vec4 v_color;
   
    void main()
    {
        color = v_color;
    }
)";

