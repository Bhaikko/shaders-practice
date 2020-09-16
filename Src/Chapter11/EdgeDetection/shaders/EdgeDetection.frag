#version 450

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

layout (binding = 0) uniform sampler2D EdgeTex;

subroutine vec4 RenderPassType();
subroutine uniform RenderPassType RenderPass;

uniform struct LightInfo {
    vec4 Position;
    vec3 Intensity;
} Light;

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
} Material;

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

subroutine(RenderPassType)
vec4 Pass1() {
    return vec4(phongModel(Position, Normal), 1.0);
}

subroutine(RenderPassType) 
vec4 Pass2() {
    return texture(EdgeTex, TexCoord);
}

void main()
{
    FragColor = RenderPass();
}