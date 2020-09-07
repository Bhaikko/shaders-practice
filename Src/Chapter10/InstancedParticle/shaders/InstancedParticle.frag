#version 450

struct LightInfo {
    vec4 Position;
    vec3 Intensity;
};

uniform LightInfo Light;

struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 E;
    float Shininess;
};

uniform MaterialInfo Material;

in vec3 fPosition;
in vec3 fNormal;

layout (location = 0) out vec4 FragColor;

vec3 phongModel( vec3 pos, vec3 norm )
{
    vec3 s = normalize(vec3(Light.Position) - pos);
    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect( -s, norm );
    vec3 ambient = Light.Intensity * Material.Ka;
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Light.Intensity * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Intensity * Material.Ks *
               pow( max( dot(r,v), 0.0 ), Material.Shininess );

    return ambient + diffuse + spec;
}



layout (binding = 0) uniform sampler1D RandomTex;

void main()
{
    // FragColor = vec4(Material.E + phongModel(fPosition, fNormal), 1.0);
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);

}