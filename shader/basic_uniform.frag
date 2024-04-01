#version 460

in vec3 position;
in vec3 normal;
layout (location = 0) out vec4 color;

// Lights store a position and diffuse/ambient/specular intensity.
uniform struct LightInfo {
    vec4 position;
    vec3 ld;
    vec3 la;
    vec3 ls;
} light;

// Materials store a diffuse/ambient/specular reflectivity,
// and a shininess factor.
uniform struct MaterialInfo {
    vec3 kd;
    vec3 ka;
    vec3 ks;
    float shiny;
} material;

vec3 phong(LightInfo light, MaterialInfo mat, vec3 pos, vec3 n) {
    // Calculate output color using the Phong lighting model.

    // Light direction is a vector from the light position
    // to this position, normalized.
    vec3 s = normalize(vec3(light.position.xyz - pos));
    float sdn = max(dot(s, n), 0.0);
    vec3 v = normalize(-pos);
    vec3 r = reflect(-s, n);

    // Sum of Phong components
    vec3 ambient  = light.la * mat.ka;
    vec3 diffuse  = light.ld * mat.kd * sdn;
    vec3 specular = light.ls * mat.ks * pow(max(dot(r, v), 0.0), mat.shiny);
    return ambient + diffuse + specular;
}

void main() {
    color = vec4(phong(light, material, position, normal), 1.0);
}
