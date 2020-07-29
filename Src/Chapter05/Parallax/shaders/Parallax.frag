#version 450

in vec3 LightDir;
in vec2 TexCoord;
in vec3 ViewDir;

layout (binding = 0) uniform sampler2D ColorTex;
layout (binding = 1) uniform sampler2D NormalMapTex;
layout (binding = 2) uniform sampler2D HeightMapTex;

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

const float bumpScale = 0.03;

vec2 FindOffset(vec3 v, out float height)
{
    const int nSteps = int(mix(60, 10, abs(v.z)));
    float htStep = 1.0 / nSteps;

    vec2 deltaT = (v.xy * bumpScale) / (nSteps * v.z);
    float ht = 1.0;
    vec2 tc = TexCoord.xy;
    height = texture(HeightMapTex, tc).r;

    // while (height < ht) {
    //     ht -= htStep;
    //     tc -= deltaT;

    //     height = texture(HeightMapTex, tc).r;
    // }

    return tc;
}

bool IsOccluded(float height, vec2 tc, vec3 s)
{
    const int nShadowSteps = int(mix(60, 10, abs(s.z)));
    float htStep = 1.0 / nShadowSteps;

    vec2 deltaT = (s.xy * bumpScale) / (nShadowSteps * s.z);
    float ht = height + htStep * 0.1;

    while (height < ht && ht < 1.0) {
        ht += htStep;
        tc += deltaT;
        height = texture(HeightMapTex, tc).r;
    }

    return ht < 1.0;
}

vec3 BlinnPhong()
{
    vec3 v = normalize(ViewDir);
    vec3 s = normalize(LightDir);

    float height = 1.0;
    vec2 tc = FindOffset(v, height);

    vec3 texColor = texture(ColorTex, tc).rgb;
    vec3 n = texture(NormalMapTex, tc).xyz;

    n.xy + 2.0 * n.xy - 1.0;
    n = normalize(n);

    float sDotN = max(dot(s, n), 0.0);
    vec3 diffuse = vec3(0.0);
    vec3 ambient = Light.La * texColor;
    vec3 spec = vec3(0.0);

    if (sDotN > 0.0 && !IsOccluded(height, tc, s)) {
        diffuse = texColor * sDotN;
        vec3 h = normalize(v + s);
        spec = Material.Ks * pow(max(dot(h, v), 0.0), Material.Shininess);
    }

    return ambient + Light.L * (diffuse + spec);
}

void main()
{
    vec3 c = BlinnPhong();
    c = pow(c, vec3(1.0 / 2.2));

    FragColor = vec4(c, 1.0);
}