#version 450

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform int Pass;

layout (binding = 0) uniform sampler2D RenderTexture;

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

uniform float Weights[5];

vec3 blinnPhong( vec3 position, vec3 n ) {  
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

vec4 Pass1() {
    return vec4(blinnPhong(Position, normalize(Normal)), 1.0);
}

vec4 Pass2() 
{
    ivec2 pix = ivec2(gl_FragCoord.xy);

    vec4 sum = texelFetch(RenderTexture, pix, 0) * Weights[0];

    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(0, 1)) * Weights[1];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(0, 2)) * Weights[2];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(0, 3)) * Weights[3];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(0, 4)) * Weights[4];
    
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(0, -1)) * Weights[1];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(0, -2)) * Weights[2];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(0, -3)) * Weights[3];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(0, -4)) * Weights[4];

    return sum;
}

vec4 Pass3()
{
    ivec2 pix = ivec2(gl_FragCoord.xy);

    vec4 sum = texelFetch(RenderTexture, pix, 0) * Weights[0];

    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(1, 0)) * Weights[1];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(2, 0)) * Weights[2];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(3, 0)) * Weights[3];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(4, 0)) * Weights[4];
    
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(-1, 0)) * Weights[1];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(-2, 0)) * Weights[2];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(-3, 0)) * Weights[3];
    sum += texelFetchOffset(RenderTexture, pix, 0, ivec2(-4, 0)) * Weights[4];

    return sum;
}

void main()
{
    if (Pass == 1) {
        FragColor = Pass1();
    } else if (Pass == 2) {
        FragColor = Pass2();
    } else {
        FragColor = Pass3();
    }
}