#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

#include <cstdio>

namespace NoiseTex
{
    int StoreTex(GLubyte* data, int width, int height)
    {
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        return texID;
    }

    int Generate2DTexture(float baseFreq = 4.0f, float persistence = 0.5f, int width = 128, int height = 128, bool periodic = false)
    {
        printf("Generating Noise Texture. \n");
        
        GLubyte* data = new GLubyte[width * height * 4];

        float xFactor = 1.0f / (width - 1);
        float yFactor = 1.0f / (height - 1);

        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                float x = xFactor * col;
                float y = yFactor * row;
                float sum = 0.0f;
                float freq = baseFreq;
                float persist = persistence;

                for (int oct = 0; oct < 4; oct++) {
                    glm::vec2 p(x * freq, y * freq);

                    float val = 0.0f;
                    if (periodic) {
                        val = glm::perlin(p, glm::vec2(freq)) * persist;
                    } else {
                        val = glm::perlin(p) * persist;
                    }

                    sum += val;

                    float result = (sum + 1.0f) / 2.0f;

                    // Clamp Between 0 and 1;

                    result = result > 1.0f ? 1.0f : result;
                    result = result < 0.0f ? 0.0f : result;

                    // Store in texture
                    data[((row * width + col) * 4) + oct] = GLubyte(result * 255.0f);
                    freq *= 2.0f;
                    persist *= persistence;
                }
            }
        }

        int texID = NoiseTex::StoreTex(data, width, height);
        delete[] data;

        printf("Done. \n");
        return texID;
    }

    int GeneratePeriodic2DTex(float baseFreq = 4.0f, float persist = 0.5f, int width = 128, int height = 128)
    {
        return NoiseTex::Generate2DTexture(baseFreq, persist, width, height, true);
    }
};