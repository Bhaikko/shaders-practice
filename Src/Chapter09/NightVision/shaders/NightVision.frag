#version 450

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

uniform int Width;
uniform int Height;
uniform float Radius;

layout (binding = 0) uniform sampler2D RenderTex;
layout (binding = 1) uniform sampler2D NoiseTex;

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

vec3 PhongModel( vec3 pos, vec3 norm )
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

float Luminance(vec3 color) {
    return dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
}

subroutine(RenderPassType)
vec4 Pass1()
{
    return vec4(PhongModel(Position, Normal), 1.0);
}

subroutine(RenderPassType)
vec4 Pass2()
{
    vec4 noise = texture(NoiseTex, TexCoord);
    vec4 color = texture(RenderTex, TexCoord);
    float green = Luminance(color.rgb);

    // Creating Binocular
    float dist1 = length(gl_FragCoord.xy - vec2(Width / 4.0, Height / 2.0));
    float dist2 = length(gl_FragCoord.xy - vec2(3.0 * Width / 4.0, Height / 2.0));

    if (dist1 > Radius && dist2 > Radius) {
        green = 0.0;
    }

    return vec4(0.0, green * clamp(noise.a, 0.0, 1.0), 0.0, 1.0);
}

void main()
{
    FragColor = RenderPass();
}