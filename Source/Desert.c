#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>


#define IMAGE_WIDTH 1280
#define IMAGE_HEIGHT 720

static PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
static PFNGLCREATEPROGRAMPIPELINESPROC glCreateProgramPipelines;
static PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv;
static PFNGLCREATETEXTURESPROC glCreateTextures;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
static PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines;
static PFNGLDELETEPROGRAMPROC glDeleteProgram;
static PFNGLDELETETEXTURESPROC glDeleteTextures;
static PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages;
static PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
static PFNGLDRAWARRAYSPROC glDrawArrays;
static PFNGLDISPATCHCOMPUTEPROC glDispatchCompute;
static PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D;
static PFNGLTEXTUREPARAMETERIPROC glTextureParameteri;
static PFNGLBINDTEXTUREUNITPROC glBindTextureUnit;
static PFNGLBINDIMAGETEXTUREPROC glBindImageTexture;

static uint32_t s_Vs;
static uint32_t s_Fs;
static uint32_t s_Vao;
static uint32_t s_Ppo;
static uint32_t s_Tex;

int ShouldRecompileShaders(void);

static char *LoadTextFile(const char *fileName)
{
    FILE *file = fopen(fileName, "rb");
    assert(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    assert(size != -1);
    char *content = (char *)malloc(size + 1);
    assert(content);
    fseek(file, 0, SEEK_SET);
    fread(content, 1, size, file);
    fclose(file);
    content[size] = '\0';
    return content;
}

static void CreateShaders(void)
{
    glDeleteProgram(s_Vs);
    glDeleteProgram(s_Fs);

    char *glsl = LoadTextFile("Desert.glsl");

    const char *vsSrc[] = { "#version 450 core\n", "#define VS_FULL_TRIANGLE\n", (const char *)glsl };
    s_Vs = glCreateShaderProgramv(GL_VERTEX_SHADER, 3, vsSrc);

    const char *fsSrc[] = { "#version 450 core\n", "#define FS_DESERT\n", (const char *)glsl };
    s_Fs = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 3, fsSrc);

    free(glsl);

    glUseProgramStages(s_Ppo, GL_VERTEX_SHADER_BIT, s_Vs);
    glUseProgramStages(s_Ppo, GL_FRAGMENT_SHADER_BIT, s_Fs);
}

static void Initialize(void)
{
    glCreateVertexArrays(1, &s_Vao);
    glCreateProgramPipelines(1, &s_Ppo);

    glCreateTextures(GL_TEXTURE_2D, 1, &s_Tex);
    glTextureParameteri(s_Tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(s_Tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureStorage2D(s_Tex, 1, IMAGE_WIDTH, IMAGE_HEIGHT, GL_RGBA8);

    CreateShaders();
}

static void Shutdown(void)
{
    glDeleteProgram(s_Vs);
    glDeleteProgram(s_Fs);
    glDeleteProgramPipelines(1, &s_Ppo);
    glDeleteVertexArrays(1, &s_Vao);
    glfwTerminate();
}

static void Update(void)
{
    glBindProgramPipeline(s_Ppo);
    glBindVertexArray(s_Vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    if (ShouldRecompileShaders())
    {
        CreateShaders();
    }
}

int main(void)
{
    if (!glfwInit())
    {
        return 1;
    }

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    GLFWwindow *window = glfwCreateWindow(IMAGE_WIDTH, IMAGE_HEIGHT, "Desert", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);
    glCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)glfwGetProcAddress("glCreateVertexArrays");
    glCreateProgramPipelines = (PFNGLCREATEPROGRAMPIPELINESPROC)glfwGetProcAddress("glCreateProgramPipelines");
    glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)glfwGetProcAddress("glCreateShaderProgramv");
    glCreateTextures = (PFNGLCREATETEXTURESPROC)glfwGetProcAddress("glCreateTextures");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glfwGetProcAddress("glDeleteVertexArrays");
    glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)glfwGetProcAddress("glDeleteProgramPipelines");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glfwGetProcAddress("glDeleteProgram");
    glDeleteTextures = (PFNGLDELETETEXTURESPROC)glfwGetProcAddress("glDeleteTextures");
    glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)glfwGetProcAddress("glUseProgramStages");
    glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)glfwGetProcAddress("glBindProgramPipeline");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
    glDrawArrays = (PFNGLDRAWARRAYSPROC)glfwGetProcAddress("glDrawArrays");
    glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)glfwGetProcAddress("glDispatchCompute");
    glTextureStorage2D = (PFNGLTEXTURESTORAGE2DPROC)glfwGetProcAddress("glTextureStorage2D");
    glTextureParameteri = (PFNGLTEXTUREPARAMETERIPROC)glfwGetProcAddress("glTextureParameteri");
    glBindTextureUnit = (PFNGLBINDTEXTUREUNITPROC)glfwGetProcAddress("glBindTextureUnit");
    glBindImageTexture = (PFNGLBINDIMAGETEXTUREPROC)glfwGetProcAddress("glBindImageTexture");

    Initialize();

    while (!glfwWindowShouldClose(window))
    {
        Update();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Shutdown();
    return 0;
}
