#version 460

const uint max_lights = 5;

const uint toon_levels = 3;
const float toon_sf = 1.0 / toon_levels;

in vec3 position;
in vec3 normal;
in vec2 tex_coord;

layout(location = 0) out vec4 color;

layout(binding = 0) uniform sampler2D diffuse_tex;
layout(binding = 1) uniform sampler2D overlay_tex;
layout(binding = 2) uniform sampler2D opacity_tex;

// Lights store a position and diffuse/ambient/specular intensity.
uniform struct LightInfo {
    vec4 position;
    vec3 ld;
    vec3 la;
    vec3 ls;
} lights[max_lights];

uniform uint num_lights;

// Materials store a diffuse/ambient/specular reflectivity,
// and a shininess factor.
uniform struct MaterialInfo {
    vec3 kd;
    vec3 ka;
    vec3 ks;
    float shiny;
    bool toon;
} material;

float toon(float x, bool y) {
    return y ? floor(x * toon_levels) * toon_sf : x;
}

vec3 phong(LightInfo light, MaterialInfo mat, vec3 pos, vec3 normal) {
    // Calculate output color using the Phong lighting model.

    vec3 diffuse_color = texture(diffuse_tex, tex_coord).rgb;
    vec3 overlay_color = texture(overlay_tex, tex_coord).rgb;
    float opacity = texture(opacity_tex, tex_coord).x;
    vec3 tex_color = mix(diffuse_color, overlay_color, opacity);

    vec3 s = normalize(vec3(light.position.xyz - pos));
    float sdn = max(dot(s, normal), 0.0);
    vec3 v = normalize(-pos);
    vec3 r = reflect(-s, normal);

    // Sum of Phong components
    vec3 ambient = light.la * tex_color;
    vec3 diffuse = light.ld * tex_color * toon(sdn, mat.toon);
    vec3 specular = light.ls * mat.ks * toon(pow(max(dot(r, v), 0.0), mat.shiny), mat.toon);
    return ambient + diffuse + specular;
}

void main() {
    color = vec4(0.0);
    for(int i = 0; i < num_lights; i++) {
        color += vec4(phong(lights[i], material, position, normal), 1.0);
    }
}
