#version 330 core

in vec2 fragTexCoord;

const float vignette_add = .75;
const float vignette_multiplier = 1.25;
const float vignette_exp = 1.25;

const float warp = 1.0;

uniform sampler2D texture0;
uniform float time;

out vec4 FragColor;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    // -- Warp
    // Squared distance from centre
    vec2 dc = abs(vec2(0.5, 0.5)-fragTexCoord);
    dc *= dc;

    vec2 warped_uv = fragTexCoord;
    // warp the fragment coordinates
    warped_uv.x -= 0.5; warped_uv.x *= 1.0+(dc.y*(0.3*warp));
    warped_uv.x += 0.5;

    warped_uv.y -= 0.5; warped_uv.y *= 1.0+(dc.x*(0.4*warp));
    warped_uv.y += 0.5;

    float dist = distance(vec2(.5, .5), warped_uv) * 2.0;

    // -- Vignette
    float vignette = (1.0 - dist) * vignette_multiplier;

    vignette += vignette_add + rand(warped_uv) * 0.02;
    vignette = pow(vignette * vignette_multiplier, vignette_exp);
    vignette = max(min(vignette, 1.0), 0.0);

    vec4 pixel = texture(texture0, warped_uv);
    pixel.rgb *= vignette;

    FragColor = pixel;

    if (warped_uv.y > 1.0 || warped_uv.x < 0.0 || warped_uv.x > 1.0 || warped_uv.y < 0.0) {
        FragColor = vec4(.0, .0, .0,1.0);
    }
}