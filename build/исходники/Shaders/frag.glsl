#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

in vec3 fragPos;

out vec4 color;

uniform sampler2D ourTexture;

float getDist(vec3 a, vec3 b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

vec3 powVec3(vec3 a, float b)
{
    return vec3(pow(a.x, b), pow(a.y, b), pow(a.z, b));
}

void main()
{
    color = texture(ourTexture, TexCoord);
    color.w = 0.95f;
}