#version 100

precision mediump float;
varying vec2 fragTexCoord;

const float vignette_add = 0.9;
const float vignette_multiplier = 1.2;
const float vignette_exp = 1.25;

const float warp = 0.0;

const float sharpness = 0.0;
const float sharpness_offset = 0.005;

uniform sampler2D texture0;
uniform float time;

uniform sampler2D noise_texture;
const float noise_zoom = 0.5;
const float noise_strength = 0.0;
const float noise_anim_speed = 6.0;

uniform vec2 camera_offset;
uniform float aspect_ratio;

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
     
    vec2 noise_ofst = texture2D(noise_texture,
        (fragTexCoord + camera_offset) * noise_zoom + rand(vec2(noise_anim, noise_anim))

    ).rg * noise_strength;

    warped_uv += noise_ofst;
    vec3 center = texture2D(texture0, warped_uv).rgb;
    vec3 up     = texture2D(texture0, warped_uv + vec2(0, -sharpness_offset * aspect_ratio)).rgb;
    vec3 down   = texture2D(texture0, warped_uv + vec2(0,  sharpness_offset * aspect_ratio)).rgb;
    vec3 left   = texture2D(texture0, warped_uv + vec2(-sharpness_offset, 0)).rgb;
    vec3 right  = texture2D(texture0, warped_uv + vec2( sharpness_offset, 0)).rgb;
    
    center = center * (sharpness * 4.0 + 1.0);
    vec3 neighbours = (up + down + left + right) * sharpness;
    vec3 calculated = center - neighbours;

    vec4 pixel = vec4(calculated.r, calculated.g, calculated.b, 1.0);
    pixel.rgb *= vignette;

    gl_FragColor = pixel;

    if (warped_uv.y > 1.0 || warped_uv.x < 0.0 || warped_uv.x > 1.0 || warped_uv.y < 0.0) {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}