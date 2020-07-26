#version 450

in vec3 LightDir;
in vec2 TexCoord;
in vec3 ViewDir;

uniform sampler2D ColorTex;
uniform sampler2D NormalMapTex;

uniform struct LightInfo {
    vec4 Position;
    vec3 L;
    vec3 La;
} Light;

uniform struct MaterialInfo {
    vec3 Ks;
    float Shininess;
} Material;

layout (location = 0) out vec4 FragColor;

vec3 BlinnPhong(vec3 n)
{
    vec3 texColor = texture(ColorTex, TexCoord).rgb;

    vec3 ambient = Light.La * texColor;
    vec3 s = normalize(LightDir);

    float sDotN = max(dot(s, n), 0.0);
    vec3 diffuse = texColor * sDotN;

    vec3 spec = vec3(0.0);
    if (sDotN > 0.0) {
        vec3 v = normalize(ViewDir);
        vec3 h = normalize(v + s);

        spec = Material.Ks * pow(max(dot(h, n), 0.0), Material.Shininess);
    }

    return ambient + Light.L * (diffuse + spec);
}

void main()
{
    vec3 norm = texture(NormalMapTex, TexCoord).xyz;

    norm.xy = 2.0 * norm.xy - 1.0;
    FragColor = vec4(BlinnPhong(norm), 1.0);
}