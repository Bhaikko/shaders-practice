#version 450

in vec3 Position;
in vec3 Normal;

uniform struct LightInfo {
    vec4 Position;
    vec3 La;
    vec3 L;
} Light;

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
} Material;

uniform struct FogInfo {
    float MaxDistance;
    float MinDistance;
    vec3 Color;
} Fog;

layout (location = 0) out vec4 FragColor;

vec3 Phong(vec3 position, vec3 n)
{
    vec3 ambient = Light.La * Material.Ka;
    vec3 s = normalize( Light.Position.xyz - position );
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffuse = Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 ) {
        vec3 v = normalize(-position.xyz);
        vec3 h = normalize( v + s );
        spec = Material.Ks *
                pow( max( dot(h,n), 0.0 ), Material.Shininess );
    }
    return ambient + Light.L * (diffuse + spec);
}

void main()
{
    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDistance - dist) / (Fog.MaxDistance - Fog.MinDistance);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec3 lightColor = Phong(Position, normalize(Normal));
    vec3 color = mix(Fog.Color, lightColor, fogFactor);
    FragColor = vec4(color, 1.0);
}