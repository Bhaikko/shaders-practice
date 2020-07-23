#version 450

// Used to define subroutine and its function agruments which can be assigned to indiviual functions
subroutine vec3 shaderModelType(vec3 position, vec3 normal);
subroutine uniform shaderModelType shaderModel;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out vec3 LightIntensity;

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
    position = (ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0)).xyz;
}

// Assigning subroutine to a function with same arguments and return type
subroutine(shaderModelType)
vec3 PhongModel(vec3 position, vec3 normal)
{
    vec3 ambient = Light.La * Material.Ka;

    vec3 s = normalize(Light.Position.xyz - position);
    float sDotN = max(dot(s, normal), 0.0);
    vec3 diffuse = Light.Ld * Material.Kd * sDotN;

    vec3 spec = vec3(0.0);
    if (sDotN > 0.0) {
        vec3 v = normalize(-position.xyz);
        vec3 r = reflect(-s, normal);

        spec = Light.Ls * Material.Ks * pow(max(dot(r, v), 0.0), Material.Shininess);
    }

    ambient = Light.Position.xyz;

    return ambient + diffuse + spec;
}

subroutine(shaderModelType)
vec3 DiffuseModel(vec3 position, vec3 normal)
{
    vec3 s = normalize(Light.Position.xyz - position);
    return Light.Ld * Material.Kd * max(dot(s, normal), 0.0);
}


void main()
{
    vec3 camNorm, camPosition;
    GetCamSpace(camNorm, camPosition);

    LightIntensity = shaderModel(camPosition, camNorm);
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);
}