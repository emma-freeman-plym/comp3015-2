#version 460

const uint max_lights = 5;

const uint toon_levels = 3;
const float toon_sf = 1.0 / toon_levels;

const float line_weight = 0.75;

const float pi = 3.141592653589793238462643383279502;

in vec3 g_position;
in vec3 g_normal;
noperspective in vec3 g_edge_distance;
in vec2 g_tex_coord;

layout(location = 0) out vec4 color;

layout(binding = 0) uniform sampler2D diffuse_tex;
layout(binding = 1) uniform sampler2D overlay_tex;
layout(binding = 2) uniform sampler2D opacity_tex;

// Lights store a position and diffuse/ambient/specular intensity.
uniform struct LightInfo {
    uint kind;
    vec3 position;
    vec3 intensity;
} lights[max_lights];

uniform uint num_lights;

// Materials store a diffuse/ambient/specular reflectivity,
// and a shininess factor.
uniform struct MaterialInfo {
    vec3 color;
    float rough;
    bool metal;
    bool diffuse;
    bool overlay;
    float frequency;
    float velocity;
    float amplitude;
} material;

uniform bool wireframe;

float toon(float x, bool y) {
    return y ? floor(x * toon_levels) * toon_sf : x;
}

vec3 get_color(MaterialInfo mat) {
    vec3 diffuse_color = texture(diffuse_tex, g_tex_coord).rgb;
    vec3 overlay_color = texture(overlay_tex, g_tex_coord).rgb;
    float opacity = texture(opacity_tex, g_tex_coord).x;
    vec3 tex_color = mat.overlay ? mix(diffuse_color, overlay_color, opacity) : diffuse_color;
    return mat.diffuse ? tex_color : mat.color;
}

float ggx_dist(MaterialInfo mat, float ndh) {
    float alpha2 = mat.rough * mat.rough * mat.rough * mat.rough;
    float d = (ndh * ndh) * (alpha2 - 1) + 1;
    return alpha2 / (pi * d * d);
}

vec3 shlick_fresnel(MaterialInfo mat, float ldh) {
    vec3 f0 = mat.metal ? get_color(mat) : vec3(0.04);
    return f0 + (1 - f0) * pow(1.0 - ldh, 5);
}

float geom_smith(MaterialInfo mat, float d) {
    float k = (mat.rough + 1.0) * (mat.rough + 1.0) / 8.0;
    float denom = d * (1 - k) + k;
    return 1.0 / denom;
}

vec3 microfacet(LightInfo light, MaterialInfo mat, vec3 pos, vec3 n) {
    vec3 diffuse_brdf = mat.metal ? vec3(0.0) : get_color(mat);

    vec3 l = vec3(0);
    vec3 i = light.intensity;
    if(light.kind == 0) {
        // Point
        l = light.position - pos;
        float dist = length(l);
        l = normalize(l);
        i /= dist * dist;
    } else {
        // Directional
        l = normalize(light.position);
    }

    vec3 v = normalize(-pos);
    vec3 h = normalize(v + l);
    float ndh = dot(n, h);
    float ldh = dot(l, h);
    float ndl = max(dot(n, l), 0.0);
    float ndv = dot(n, v);

    vec3 spec_brdf = 0.25 *
        ggx_dist(mat, ndh) *
        shlick_fresnel(mat, ldh) *
        geom_smith(mat, ndl) *
        geom_smith(mat, ndv);

    return (diffuse_brdf + pi * spec_brdf) * ndl * i;
}

void main() {
    vec3 sum = vec3(0.0);
    vec3 n = normalize(g_normal);
    for(int i = 0; i < num_lights; i++) {
        sum += microfacet(lights[i], material, g_position, n);
    }
    sum = pow(sum, vec3(1.0 / 2.2));

    color = vec4(sum, 1.0);

    if (wireframe) {
        float d = min(g_edge_distance.x, g_edge_distance.y);
        d = min(d, g_edge_distance.z);
        float c;
        if (d < line_weight - 1.0) {
            c = 1.0;
        } else if (d > line_weight + 1.0) {
            c = 0.0;
        } else {
            float x = d - (line_weight - 1);
            c = exp2(-2.0 * (x * x));
        }
        color = mix(color, vec4(0.1, 0.1, 0.1, 1.0), c);
    }
}
