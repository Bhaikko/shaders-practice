#version 450

in vec3 FrontColor;
in vec3 BackColor;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

void main()
{
    const float scale = 15.0;

    bvec2 toDiscard = greaterThan(fract(TexCoord * scale), vec2(0.1, 0.1));

    if (all(toDiscard)) {
        // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        discard;
    } else {
        if (gl_FrontFacing) {
            FragColor = vec4(FrontColor, 1.0);
        } else {
            FragColor = vec4(BackColor, 1.0);
        }
    }

}