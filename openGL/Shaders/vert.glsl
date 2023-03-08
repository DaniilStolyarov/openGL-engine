#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// uniform float fi;
uniform mat4 rotation;

void main()
{
    // gl_Position = projection * view * model * vec4(position.z * cos(fi) - position.x * sin(fi) , position.z * sin(fi) + position.x * cos(fi), position.y, 1.0f);
    gl_Position = projection * view * model * rotation * vec4(position, 1.0f);
    ourColor = color;
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
} 

//vec4(position.x * cos(fi) - position.y * sin(fi) , position.x * sin(fi) + position.y * cos(fi), position.z, 1.0f)