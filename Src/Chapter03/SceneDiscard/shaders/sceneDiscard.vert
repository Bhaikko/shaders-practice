#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out vec3 FrontColor;
out vec3 BackColor;
out vec2 TexCoord;

uniform struct LightInfo {
    vec4 Position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
} Light;

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
} Material;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

void GetCamSpace(out vec3 norm, out vec3 position) 
{
    norm = normalize(NormalMatrix * VertexNormal);
    position = (ViewMatrix * ModelMatrix * vec4(VertexPosition.xyz, 1.0)).xyz;
}

vec3 PhongModel(vec3 position, vec3 n)
{
    vec3 ambient = Light.La * Material.Ka;
    
    // vec3 s = normalize(Light.Position.xyz - position);
    // float sDotN = max(dot(s, n), 0.0);
    // vec3 diffuse = Light.Ld * Material.Kd * sDotN;

    // vec3 spec = vec3(0.0);
    // if (sDotN > 0.0) {
    //     vec3 v = normalize(-position.xyz);
    //     vec3 r = reflect(-s, n);

    //     spec = Light.Ls * Material.Ks * pow(max(dot(r, v), 0.0), Material.Shininess);
    // }

    return ambient  ;
}

void main()
{
    vec3 camNorm, camPosition;
    TexCoord = VertexTexCoord;

    GetCamSpace(camNorm, camPosition);

    // FrontColor = PhongModel(camPosition, camNorm);
    // BackColor = PhongModel(camPosition, -camNorm);

    FrontColor = vec3(1.0, 0.0, 0.0);
    BackColor = vec3(0.0, 1.0, 0.0);

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);
}
