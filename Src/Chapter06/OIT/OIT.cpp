#include "OIT.h"

OIT::OIT(GLint width, GLint height)
{
    this->width = width;
    this->height = height;
}

void OIT::Init()
{
    shader.CreateFromFiles(
        "./Src/Chapter06/OIT/shaders/OIT.vert",
        "./Src/Chapter06/OIT/shaders/OIT.frag"
    );

    cube.LoadModel("./Data/cube.obj");
    sphere.LoadModel("./Data/sphere.obj");
    screenQuad.LoadModel("./Data/plane.obj");

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.UseShader();

    InitShaderStorage();

    GLuint programHandle = shader.GetShaderProgramID();
    pass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "pass1");
    pass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "pass2");


}

void OIT::Render(glm::mat4& view, glm::mat4& projection)
{
    shader.UseShader();

    ClearBuffers();
    Pass1(view, projection);
    glFlush();
    Pass2();
}


void OIT::SetMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    shader.SetUniform("ModelMatrix", model);
    shader.SetUniform("ViewMatrix", view);
    shader.SetUniform("ProjectionMatrix", projection);
    shader.SetUniform("NormalMatrix", glm::mat3(glm::inverseTranspose(view * model)));
}

void OIT::DrawScene(glm::mat4& view, glm::mat4& projection)
{
    shader.SetUniform("LightPosition", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    shader.SetUniform("LightIntensity", glm::vec3(0.9f));

    shader.SetUniform("Kd", glm::vec4(0.2f, 0.2f, 0.9f, 0.55f));

    glm::mat4 model(1.0f);
    float size = 0.75f;
    for (int i = 0; i <= 6; i++) {
        for (int j = 0; j <= 6; j++) {
            for (int k = 0; k <= 6; k++) {
                if ((i + j + k) % 2 == 0) {
                    model = glm::translate(glm::mat4(1.0f), glm::vec3(i - 3, j - 3, k - 3));
                    model = glm::scale(model, glm::vec3(size));

                    SetMatrices(model, view, projection);
                    cube.RenderModel();
                }
            }
        }
    }

    shader.SetUniform("Kd", glm::vec4(0.9f, 0.2f, 0.2f, 0.4f) );
    size = 2.0f;
    float pos = 1.75f;
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-pos, -pos, pos));
    model = glm::scale( model, glm::vec3(size));
    SetMatrices(model, view, projection);
    cube.RenderModel();
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-pos, -pos, -pos));
    model = glm::scale( model, glm::vec3(size));
    SetMatrices(model, view, projection);
    cube.RenderModel();
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-pos, pos, pos));
    model = glm::scale( model, glm::vec3(size));
    SetMatrices(model, view, projection);
    cube.RenderModel();
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-pos, pos, -pos));
    model = glm::scale( model, glm::vec3(size));
    SetMatrices(model, view, projection);
    cube.RenderModel();
    model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, pos, pos));
    model = glm::scale( model, glm::vec3(size));
    SetMatrices(model, view, projection);
    cube.RenderModel();
    model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, pos, -pos));
    model = glm::scale( model, glm::vec3(size));
    SetMatrices(model, view, projection);
    cube.RenderModel();
    model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, -pos, pos));
    model = glm::scale( model, glm::vec3(size));
    SetMatrices(model, view, projection);
    cube.RenderModel();
    model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, -pos, -pos));
    model = glm::scale( model, glm::vec3(size));
    SetMatrices(model, view, projection);
    cube.RenderModel();
}

void OIT::Pass1(glm::mat4& view, glm::mat4& projection)
{
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // disabling writing into the depth buffer
    glDepthMask(GL_FALSE);

    DrawScene(view, projection);

    glFinish();
}

void OIT::Pass2()
{
    /* 
        glMemoryBarrier defines a barrier ordering the memory transactions 
        issued prior to the command relative to those issued after the barrier. 
        For the purposes of this ordering, memory transactions performed by shaders are considered to be 
        issued by the rendering command that triggered the execution of the shader. 
        barriers is a bitfield indicating the set of operations that are synchronized with shader stores
    */

   glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

   glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glm::mat4 model(1.0f), view(1.0f), projection(1.0f);
   model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

   SetMatrices(model, view, projection);
   screenQuad.RenderModel();
}

void OIT::ClearBuffers()
{
    GLuint zero = 0;
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, buffers[COUNTER_BUFFER]);
    glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
    glBindTexture(GL_TEXTURE_2D, headPtrTex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
}

void OIT::InitShaderStorage()
{
    glGenBuffers(2, buffers);
    GLuint maxNodes = 20 * width * height;
    GLint nodeSize = 5 * sizeof(GLfloat) + sizeof(GLuint);  // Size of linked list node as in ListNode struct

    // Atomic Counter
    // an atomic counter is memory buffer (actually a buffer object that stores one or several unsigned int) 
    // that can be incremented (GLSL built-in function atomicCounterIncrement()) or 
    // decremented (GLSL built-in function atomicCounterDecrement()) by a shader.

    // binds the buffer object buffer to the binding point at index index of the array of targets specified by target.
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, buffers[COUNTER_BUFFER]);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

    // The buffer for head pointers, as an image texture
    glGenTextures(1, &headPtrTex);
    glBindTexture(GL_TEXTURE_2D, headPtrTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, width, height);

    // bind a level of a texture to an image unit
    glBindImageTexture(0, headPtrTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

    // Buffer for linked list
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffers[LINKED_LIST_BUFFER]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, maxNodes * nodeSize, NULL, GL_DYNAMIC_DRAW);

    shader.SetUniform("MaxNodes", static_cast<int>(maxNodes));

    std::vector<GLuint> headPtrClearBuf(width * height, 0xffffffff);
    glGenBuffers(1, &clearBuf);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, headPtrClearBuf.size() * sizeof(GLuint), &headPtrClearBuf[0], GL_STATIC_DRAW);



}
