#version 450

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex;

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
  vec3 texColor = texture(RenderTex, TexCoord).rgb;

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
    FragColor = vec4(blinnPhong(Position, normalize(Normal)), 1.0);
}