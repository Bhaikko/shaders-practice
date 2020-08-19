#version 450

uniform struct LightInfo {
    vec4 Position;
    vec3 Intensity;
} Light;

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
} Material;

// As this texture's sampler type is sampler2DShadow , 
// it is treated as texture containing depth values, and rather than returning a value from the texture
layout (binding = 0) uniform sampler2DShadow ShadowMap;

in vec3 Position;
in vec3 Normal;
in vec4 ShadowCoord;

layout (location = 0) out vec4 FragColor;

vec3 PhongModelDiffAndSpec()
{
    vec3 n = Normal;
    vec3 s = normalize(vec3(Light.Position) - Position);
    vec3 v = normalize(-Position.xyz);
    vec3 r = reflect( -s, n );
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffuse = Light.Intensity * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Intensity * Material.Ks *
            pow( max( dot(r,v), 0.0 ), Material.Shininess );

    return diffuse + spec;
}

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

subroutine(RenderPassType)
void ShadeWithShadow()
{
    vec3 ambient = Light.Intensity * Material.Ka;
    vec3 diffAndSpec = PhongModelDiffAndSpec();

    float shadow = 1.0;

    // This ensure that the fragment is inside the view frustum
    if (ShadowCoord.z >= 0.0) {
        // This returns either 0 or 1 depending on the compare funciton in application
        /* 
            defined in openGL applicaion
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
        */
        shadow = textureProj(ShadowMap, ShadowCoord);
    }

    // If fragment is in shadow, use ambient light only
    FragColor = vec4(diffAndSpec * shadow + ambient, 1.0);

    // Gamma Correction
    FragColor = pow(FragColor, vec4(1.0 / 2.2));
}

subroutine(RenderPassType)
void RecordDepth()
{
    // DO NOTHING, DEPTH WILL BE WRITTEN ONLY AND AUTOMATICALLY
}

void main()
{
    RenderPass();
}