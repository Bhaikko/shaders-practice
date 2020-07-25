#version 450

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

layout (binding = 0) uniform sampler2D BaseTex;
layout (binding = 1) uniform sampler2D AlphaTex;

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

vec3 blinnPhong( vec3 position, vec3 n ) {
  vec3 texColor = texture(BaseTex, TexCoord).rgb;

  vec3 ambient = Light.La * texColor;
  vec3 s = normalize( Light.Position.xyz - position );
  float sDotN = max( dot(s,n), 0.0 );
  vec3 diffuse = texColor * sDotN;
  
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
    vec4 alphaMap = texture(AlphaTex, TexCoord);
    vec4 baseMap = texture(BaseTex, TexCoord);

    if (alphaMap.a < 0.15) {
        discard;
    } else {
        vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
        if (gl_FrontFacing) {
            lightColor = vec4(blinnPhong(Position, normalize(Normal)), 1.0);
        } else {
            lightColor = vec4(blinnPhong(Position, normalize(-Normal)), 1.0);
        }

        FragColor = lightColor;
    }
}