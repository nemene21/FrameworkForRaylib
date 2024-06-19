#version 330 core

in vec2 fragTexCoord;
uniform sampler2D texture0;
uniform float time;

uniform float jolly;
uniform sampler2D noise;

out vec4 FragColor;

void main()
{
    FragColor = texture(texture0, fragTexCoord);
    FragColor.rb *= vec2(jolly, jolly);

    FragColor.rgb *= texture(noise, fragTexCoord).rgb;
}