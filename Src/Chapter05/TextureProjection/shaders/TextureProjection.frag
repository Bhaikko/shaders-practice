#version 450

in vec3 EyeNormal;
in vec4 EyePosition;
in vec4 ProjTexCoord;

layout (binding = 0) uniform sampler2D ProjectorTex;

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

void main()
{
    vec3 color = blinnPhong(EyePosition.xyz, normalize(EyeNormal));

    vec3 projTexColor = vec3(0.0);
    if (ProjTexCoord.z > 0.0) {
        projTexColor = textureProj(ProjectorTex, ProjTexCoord).rgb;
    }

    FragColor = vec4(color + projTexColor * 0.5, 1.0);
}