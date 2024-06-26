#version 330 core

in vec2 fragTexCoord;
uniform sampler2D texture0;
uniform float time;
uniform float tint = vec4(1, 1, 1, 1);


out vec4 FragColor;

void main()
{
    FragColor = texture(texture0, fragTexCoord) * tint;
}