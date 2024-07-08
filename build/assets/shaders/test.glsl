#version 100
precision mediump float;

varying vec2 fragTexCoord;
uniform sampler2D texture0;
uniform float time;

const float PI = 3.14;

void main()
{
    gl_FragColor = texture2D(texture0, fragTexCoord);
}
