#version 450

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;

uniform int Pass;

layout (binding = 0) uniform sampler2D HdrTex;
layout (binding = 1) uniform sampler2D BlurTex1;
layout (binding = 2) uniform sampler2D BlurTex2;

uniform float LumThresh;    // Luminace Threshold

uniform struct LightInfo {
    vec4 Position;
    vec3 L;
    vec3 La;
} Lights[3];

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
} Material;

uniform float PixOffset[10] = float[](0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
uniform float Weights[10];

// XYZ / RGB conversion matrices 
uniform mat3 rgb2xyz = mat3(
    0.4124564, 0.2126729, 0.0193339,
    0.3575761, 0.7151522, 0.1191920,
    0.1804375, 0.0721750, 0.9503041
);

uniform mat3 xyz2rgb = mat3(
    3.2404542, -0.9692660, 0.0556434,
    -1.5371385, 1.8760108, -0.2040259,
    -0.4985314, 0.0415560, 1.0572252
);

uniform float Exposure = 0.35;
uniform float White = 0.928;
uniform float AveLum;

float luminance(vec3 color)
{
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

vec3 blinnPhong( vec3 position, vec3 n, int idx ) {
    vec3 ambient = Lights[idx].La * Material.Ka;
    vec3 s = normalize( Lights[idx].Position.xyz - position );
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffuse = Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 ) {
    vec3 v = normalize(-position.xyz);
    vec3 h = normalize( v + s );
    spec = Material.Ks *
            pow( max( dot(h,n), 0.0 ), Material.Shininess );
    }
    return ambient + Lights[idx].L * (diffuse + spec);
}

vec4 Pass1() 
{
    vec3 n = normalize(Normal);
    vec3 color = vec3(0.0);

    for (int i = 0; i < 3; i++) {
        color += blinnPhong(Position, n, i);
    }

    return vec4(color, 1);
}

// Bright Pass filter which writes to BlurTex1
// Write pixel values above certain Luminance threshold
vec4 Pass2()
{
    vec4 val = texture(HdrTex, TexCoord);
    if (luminance(val.rgb) > LumThresh) {
        return val; 
    } else {
        return vec4(0.0);
    }
}

// First Blur pass along Y axis which reads from BlurTex1 and writes to BlurTex2
vec4 Pass3()
{   
    // retrieve the dimensions of a level of a texture, second parameter is LOD
    float dy = 1.0 / (textureSize(BlurTex1, 0)).y;

    vec4 sum = texture(BlurTex1, TexCoord) * Weights[0];
    for (int i = 1; i < 10; i++) {
        sum += texture(BlurTex1, TexCoord + vec2(0.0, PixOffset[i]) * dy) * Weights[i];
        sum += texture(BlurTex1, TexCoord - vec2(0.0, PixOffset[i]) * dy) * Weights[i];
    }

    return sum;
}

// Second Blur Pass along X axis which reads from BlurTex2 and writes to Blurtex1
vec4 Pass4()
{
    float dx = 1.0 / (textureSize(BlurTex2, 0)).x;

    vec4 sum = texture(BlurTex2, TexCoord) * Weights[0];
    for (int i = 1; i < 10; i++) {
        sum += texture(BlurTex2, TexCoord + vec2(PixOffset[i], 0.0) * dx) * Weights[i];
        sum += texture(BlurTex2, TexCoord - vec2(PixOffset[i], 0.0) * dx) * Weights[i];
    }

    return sum;
}

// Composite Pass, apply tone map to HDR image
// Then combine with the blurred bright - pass filter
// Read from BlurTex1 and HdrTex, write to default buffer
vec4 Pass5()
{
    // Tone Mapping
    vec4 color = texture(HdrTex, TexCoord);

    // Convert to XYZ color space
    vec3 xyzCol = rgb2xyz * vec3(color);

    // Convert to xyY
    float xyzSum = xyzCol.x + xyzCol.y + xyzCol.z;
    vec3 xyYCol = vec3(
        xyzCol.x / xyzSum,
        xyzCol.y / xyzSum,
        xyzCol.y 
    );

    // Apply the tone mapping operation to luminance 
    float L = (Exposure * xyYCol.z) / AveLum;
    L = (L * (1 + L / (White * White))) / (1 + L);

    // Using new Lumninace, convert back to XYZ
    xyzCol.x = (L * xyYCol.x) / (xyYCol.y);
    xyzCol.y = L;
    xyzCol.z = (L * (1 - xyYCol.x - xyYCol.y)) / xyYCol.y;

    // Convert back to RGB
    vec4 toneMapColor = vec4(xyz2rgb * xyzCol, 1.0);

    // Combine with blurred texture
    // Linear filtering on this texture access so that we get extra blurring
    vec4 blurTex = texture(BlurTex1, TexCoord);


    return toneMapColor + blurTex;
}

void main()
{
    if (Pass == 1) {
        FragColor = Pass1();
    } else if (Pass == 2) {
        FragColor = Pass2();
    } else if (Pass == 3) {
        FragColor = Pass3();
    } else if (Pass == 4) {
        FragColor = Pass4();
    } else if (Pass == 5) {
        FragColor = Pass5();
    }
}