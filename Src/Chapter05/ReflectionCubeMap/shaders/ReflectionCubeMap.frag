#version 450

in vec3 ReflectDir;
in vec3 RefractDir;

layout (binding = 0) uniform samplerCube CubeMapTex;

uniform float ReflectionFactor;
uniform vec4 MaterialColor;

layout (location = 0) out vec4 FragColor;

void main()
{
    vec3 reflectiveColor = texture(CubeMapTex, ReflectDir).rgb;
    vec3 refractiveColor = texture(CubeMapTex, RefractDir).rgb;

    vec3 color = mix(refractiveColor, reflectiveColor, ReflectionFactor);
        
    // Gamma Correction
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);

}