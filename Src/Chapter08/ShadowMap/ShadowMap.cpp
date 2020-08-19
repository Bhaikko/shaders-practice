#include "ShadowMap.h"

ShadowMap::ShadowMap(GLint width, GLint height)
{
    this->width = width;
    this->height = height;

    this->shadowMapHeight = 800;
    this->shadowMapWidth = 800;
}

void ShadowMap::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter08/ShadowMap/shaders/ShadowMap.vert",
        "./Src/Chapter08/ShadowMap/shaders/ShadowMap.frag"
    );

    
    solidShader.CreateFromFiles(
        "./Src/Chapter08/ShadowMap/shaders/Solid.vert",
        "./Src/Chapter08/ShadowMap/shaders/Solid.frag"
    );

    teapot.LoadModel("./Data/teapot.obj");
    plane.LoadModel("./Data/plane.obj");
    torus.LoadModel("./Data/Torus.obj");

    SetupFBO();

    // Subroutine Setup
    GLuint shaderrSmHandle = shader.GetShaderProgramID();
    pass1Index = glGetSubroutineIndex(shaderrSmHandle, GL_FRAGMENT_SHADER, "RecordDepth");
    pass2Index = glGetSubroutineIndex(shaderrSmHandle, GL_FRAGMENT_SHADER, "ShadeWithShadow");

    shadowBias = glm::mat4(
        glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),  
        glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),  
        glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),  
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)
    );

    float c = 1.65f;
    glm::vec3 lightPos = glm::vec3(0.0f, c * 1.25f, c * 1.5f);
    lightFrustum.Orient(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightFrustum.SetPerspective(50.0f, 1.0f, 1.0f, 25.0f);
    lightPOV = shadowBias * lightFrustum.GetProjectionMatrix() * lightFrustum.GetViewMatrix();

    shader.UseShader();

    shader.SetUniform("Light.Intensity", glm::vec3(0.85f));
    shader.SetUniform("ShadowMap", 0);

}

void ShadowMap::Render(glm::mat4& view, glm::mat4& projection)
{
    shader.UseShader();

    // Pass 1, shadow map generation
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, shadowMapWidth, shadowMapHeight);
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);

        /*  
            When creating the shadow map, we only rendered back faces. This is because of the fact that
            if we were to render front faces, points on certain faces would have nearly the same depth
            as the shadow map's depth, which can cause fluctuations between light and shadow across
            faces that should be completely lit
        */
       
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

            /*  
                This of course will only work correctly if your meshes are completely closed. If that is not the
                case, glPolygonOffset can be used to help the situation by offsetting the depth of the
                geometry from that in the shadow map.
            */

            // setting the scale and units used to calculate depth values
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(2.5f, 10.0f);

            glm::mat4 lightView = lightFrustum.GetViewMatrix();
            glm::mat4 lightProjection = lightFrustum.GetProjectionMatrix();
            DrawScene(lightView, lightProjection);
        glCullFace(GL_BACK);   
    glFlush();

    // Pass 2, Rendering scene with shadow
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, width, height);
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);
        DrawScene(view, projection);

    // Draw light frustum
    glm::mat4 model(1.0f);
    
    solidShader.UseShader();
    
    solidShader.SetUniform("Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    solidShader.SetUniform("ModelMatrix", lightFrustum.GetInverseViewMatrix());
    solidShader.SetUniform("ViewMatrix", view);
    solidShader.SetUniform("ProjectionMatrix", projection);
    lightFrustum.Render();

}


void ShadowMap::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glm::mat4 mv = view * model;

    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(
        glm::vec3(mv[0]),
        glm::vec3(mv[1]),
        glm::vec3(mv[2])
    ));
    shader.SetUniform("ShadowMatrix", lightPOV * model);
}

void ShadowMap::SetupFBO()
{
    GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };

    // Depth Buffer Texture
    // This depth texture act as shadow map
    // Depth values are not actual distances from near plane to object
    GLuint depthTex;
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);

        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, shadowMapWidth, shadowMapHeight);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

        // Specifies the texture comparison mode for currently bound depth textures
        // These lines are responseile for textureProj lookup in shader to return either 0 or 1
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

        // Assign depth buffer texture to texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthTex);

        // Create and setup FBO
        glGenFramebuffers(1, &shadowFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

        GLenum drawBuffers = GL_NONE;
        glDrawBuffers(1, &drawBuffers);

        GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (result == GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Framebuffer Complete." << std::endl;
        } else {
            std::cout << "Framebuffer Error." << std::endl;
        }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::DrawScene(glm::mat4& view, glm::mat4& projection)
{
    glm::vec3 color = glm::vec3(0.7f, 0.5f, 0.3f);
    shader.SetUniform("Material.Ka", color * 0.05f);
    shader.SetUniform("Material.Kd", color);
    shader.SetUniform("Material.Ks", glm::vec3(0.9f, 0.9f, 0.9f));
    shader.SetUniform("Material.Shininess", 150.0f);

    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, -8.0f));
    SetMatrices(model, view, projection);
    teapot.RenderModel();
    
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, -5.0f));
    model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    SetMatrices(model, view, projection);
    torus.RenderModel();

    shader.SetUniform("Material.Kd", glm::vec3(0.25f, 0.25f, 0.25f));
    shader.SetUniform("Material.Ks", glm::vec3(0.0f, 0.0f, 0.0f));
    shader.SetUniform("Material.Ka", glm::vec3(0.05f, 0.05f, 0.05f));
    shader.SetUniform("Material.Shininess", 1.0f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.8f, -5.0f));
    SetMatrices(model, view, projection);
    plane.RenderModel();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -5.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f,0.0f,1.0f));
    SetMatrices(model, view, projection);
    plane.RenderModel();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.5f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f,0.0f,0.0f));
    SetMatrices(model, view, projection);
    plane.RenderModel();



}

void ShadowMap::SpitoutDepthBuffer()
{

}

