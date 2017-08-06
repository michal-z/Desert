#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>


static PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
static PFNGLCREATEPROGRAMPIPELINESPROC glCreateProgramPipelines;
static PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
static PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines;
static PFNGLDELETEPROGRAMPROC glDeleteProgram;
static PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages;
static PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
static PFNGLDRAWARRAYSPROC glDrawArrays;

static uint32_t s_Vs;
static uint32_t s_Fs;
static uint32_t s_Vao;
static uint32_t s_Ppo;

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
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Desert", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);
    glCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)glfwGetProcAddress("glCreateVertexArrays");
    glCreateProgramPipelines = (PFNGLCREATEPROGRAMPIPELINESPROC)glfwGetProcAddress("glCreateProgramPipelines");
    glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)glfwGetProcAddress("glCreateShaderProgramv");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glfwGetProcAddress("glDeleteVertexArrays");
    glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)glfwGetProcAddress("glDeleteProgramPipelines");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glfwGetProcAddress("glDeleteProgram");
    glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)glfwGetProcAddress("glUseProgramStages");
    glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)glfwGetProcAddress("glBindProgramPipeline");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
    glDrawArrays = (PFNGLDRAWARRAYSPROC)glfwGetProcAddress("glDrawArrays");

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