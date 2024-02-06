#version 330 core

in vec2 fragTexCoord;
uniform sampler2D texture0;
uniform float time;

out vec4 FragColor;

void main()
{
    FragColor = texture(texture0, fragTexCoord) * vec4(
        abs(sin(time*5)),
        abs(sin(time*4+1)),
        abs(sin(time*4+2)),
        1
    );
}