#version 450

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 PositionData;
layout (location = 2) out vec3 NormalData;
layout (location = 3) out vec3 ColorData;
layout (location = 4) out float AoData;

uniform mat4 ProjectionMatrix;

const int kernelSize = 64;

uniform int Pass;
uniform vec3 SampleKernel[kernelSize];
uniform float Radius = 0.55;

uniform struct LightInfo {
    vec4 Position;
    vec3 L;
    vec3 La;
} Light;


uniform struct MaterialInfo {
    vec3 Kd;
    bool UseTex;
} Material;

layout (binding = 0) uniform sampler2D PositionTex;
layout (binding = 1) uniform sampler2D NormalTex;
layout (binding = 2) uniform sampler2D ColorTex;
layout (binding = 3) uniform sampler2D AoTex;
layout (binding = 4) uniform sampler2D RandTex;
layout (binding = 5) uniform sampler2D DiffTex;

const vec2 randScale = vec2(200.0, 150.0);

vec3 AmbAndDiffuse(vec3 pos, vec3 norm, vec3 diff, float ao) 
{
    ao = pow(ao, 4);
    vec3 ambient = Light.La * diff * ao;
    vec3 s = normalize(vec3(Light.Position) - pos);
    float sDotN = max(dot(s, norm), 0.0);

    return ambient + Light.L * diff * sDotN;
}

// Storing data for deferred shading
void Pass1()
{
    PositionData = Position;
    NormalData = normalize(Normal);

    if (Material.UseTex) {
        ColorData = pow(texture(DiffTex, TexCoord.xy).rgb, vec3(2.2));
    } else {
        ColorData = Material.Kd;
    }
}

// SSAO Pass
void Pass2()
{
    vec3 randDir = normalize(texture(RandTex, TexCoord.xy * randScale).xyz);
    vec3 n = normalize(texture(NormalTex, TexCoord).xyz);
    vec3 biTang = cross(n, randDir);

    if (length(biTang) < 0.0001) {
        biTang = cross(n, vec3(0.0, 0.0, 1.0));
    }

    biTang = normalize(biTang);
    vec3 tang = cross(biTang, n);
    mat3 toCamSpace = mat3(tang, biTang, n);

    float occlusionSum = 0.0;
    vec3 camPos = texture(PositionTex, TexCoord).xyz;
    for (int i = 0; i < kernelSize; i++) {
        vec3 samplePos = camPos + Radius * (toCamSpace * SampleKernel[i]);

        // Project Point
        vec4 p = ProjectionMatrix * vec4(samplePos, 1);
        p *= 1.0 / p.w;
        p.xyz = p.xyz * 0.5 + 0.5;

        // Access camera space z-coordinate at that point
        float surfaceZ = texture(PositionTex, p.xy).z;
        float zDist = surfaceZ - camPos.z;

        // Count points that are occluded
        if (zDist >= 0.0 && zDist <= Radius && surfaceZ > samplePos.z) {
            occlusionSum += 1.0;
        }
    }

    float occ = occlusionSum / kernelSize;
    AoData = 1.0 - occ;
}


// Blur pass
void Pass3()
{
    ivec2 pix = ivec2(gl_FragCoord.xy);
    float sum = 0.0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            sum += texelFetch(AoTex, pix + ivec2(x, y), 0).r;
        }
    }

    float ao = sum * 0.11111;
    AoData = ao;
}

// Final Color Pass
void Pass4()
{
    vec3 pos = texture(PositionTex, TexCoord).xyz;
    vec3 norm = texture(NormalTex, TexCoord).xyz;
    vec3 diffColor = texture(ColorTex, TexCoord).rgb;
    float aoVal = texture(AoTex, TexCoord).r;

    vec3 col = AmbAndDiffuse(pos, norm, diffColor, aoVal);
    col = pow(col, vec3(1.0 / 2.2));

    FragColor = vec4(col, 1.0);
}

void main()
{
    if (Pass == 1) {
        Pass1();
    } else if (Pass == 2) {
        Pass2();
    } else if (Pass == 3) {
        Pass3();
    } else if (Pass == 4) {
        Pass4();
    }
}