#version 330 core

in vec2 fragTexCoord;

const float vignette_add = .6;
const float vignette_multiplier = 1.5;
const float vignette_exp = 1.25;

const float warp = 0.5f;

uniform sampler2D texture0;
uniform float time;

uniform sampler2D paper_texture;
uniform float paperness = 0.04;

uniform sampler2D noise_texture;
const float noise_zoom = 0.5;
const float noise_strength = 0.006;
const float noise_anim_speed = 6.0;

uniform vec2 camera_offset = vec2(0, 0);

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

    // -- Noise offset
    float noise_anim = floor(time * noise_anim_speed);
     
    vec2 noise_ofst = texture(noise_texture,
        (fragTexCoord + camera_offset) * noise_zoom + rand(vec2(noise_anim, noise_anim))

    ).rg * noise_strength;

    warped_uv += noise_ofst;
    vec4 pixel = texture(texture0, warped_uv);
    pixel.rgb *= vignette;

    pixel.rgb = mix(pixel.rgb, texture(paper_texture, fragTexCoord).rgb, paperness);

    FragColor = pixel;

    if (warped_uv.y > 1.0 || warped_uv.x < 0.0 || warped_uv.x > 1.0 || warped_uv.y < 0.0) {
        FragColor = vec4(.0, .0, .0, 1.);
    }
}