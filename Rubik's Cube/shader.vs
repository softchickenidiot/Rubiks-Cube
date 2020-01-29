#version 460

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

uniform mat4 gWorld;
uniform mat4 rotateSide;

out vec2 TexCoord0;

void main()
{
    if (Position.y < 0.0) { 
        gl_Position = gWorld * vec4(Position, 1.0);
    }
    else {
        gl_Position = rotateSide * vec4(Position, 1.0);
    }
    TexCoord0 = TexCoord;
};