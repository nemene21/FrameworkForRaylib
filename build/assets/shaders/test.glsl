#version 330 core

in vec2 fragTexCoord;
uniform sampler2D texture0;

out vec4 FragColor;

void main()
{
    FragColor = texture(texture0, fragTexCoord) * vec4(1, 0, 0, 1);
}