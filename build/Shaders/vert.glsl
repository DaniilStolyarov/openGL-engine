#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 rotation;

uniform mat4 translation;

void main()
{
    gl_Position = projection * view * model * translation * rotation * vec4(position, 1.0f);
    fragPos = position;
    ourColor = color;
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
} 

//vec4(position.x * cos(fi) - position.y * sin(fi) , position.x * sin(fi) + position.y * cos(fi), position.z, 1.0f)