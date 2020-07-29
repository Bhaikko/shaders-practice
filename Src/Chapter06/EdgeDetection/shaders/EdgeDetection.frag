#version 450

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (binding = 0) uniform sampler2D RenderTex;

uniform float EdgeThreshold;
uniform int Pass;

uniform struct LightInfo {
    vec4 Position;
    vec3 L;
    vec3 La;
} Light;

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
} Material;

layout (location = 0) out vec4 FragColor;

const vec3 lum = vec3(0.2126, 0.7152, 0.0722);

vec3 BlinnPhong(vec3 position, vec3 n)
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

vec4 Pass1()
{
    return vec4(BlinnPhong(Position, normalize(Normal)), 1.0);
}

float luminance(vec3 color) {
    return dot(lum, color);
}

vec4 Pass2()
{
    ivec2 pix = ivec2(gl_FragCoord.xy);

    // perform a lookup of a single texel within a texture with an offset
    float s00 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,  1)).rgb);
    float s10 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,  0)).rgb);
    float s20 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1, -1)).rgb);
    float s01 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2( 0,  1)).rgb);
    float s21 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2( 0, -1)).rgb);
    float s02 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2( 1,  1)).rgb);
    float s12 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2( 1,  0)).rgb);
    float s22 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2( 1, -1)).rgb);

    float sx = s00 + 2 * s10 + s20 - (s02 + 2 * s12 + s22);
    float sy = s00 + 2 * s01 + s02 - (s20 + 2 * s21 + s22);
    float g = sx * sx + sy * sy;

    if (g > EdgeThreshold) {
        return vec4(1.0);
    } else {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }
}

void main()
{
    if (Pass == 1) {
        FragColor = Pass1();
    } else if (Pass == 2) {
        FragColor = Pass2();
    }
}
