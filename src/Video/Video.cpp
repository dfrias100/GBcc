#include "Video/Video.hpp"

#include <iostream>

namespace GBcc
{
    const std::array<GLfloat, 20U> Video::m_OUTPUT_QUAD_VERTICES = {
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f
    };
    
    const std::array<GLuint, 6U> Video::m_OUTPUT_QUAD_INDICES = {
        0, 1, 2,
        2, 3, 0
    };
    
    const char* Video::m_OUTPUT_QUAD_VERT_SHADER = "#version 330\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "out vec2 texCoord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "   texCoord = aTexCoord;\n"
        "}\0";
    
    const char* Video::m_OUTPUT_QUAD_FRAG_SHADER = "#version 330\n"
        "out vec4 fragColor;\n"
        "in vec2 texCoord;\n"
        "uniform sampler2D textureSampler;"
        "void main()\n"
        "{\n"
        "   fragColor = texture(textureSampler, texCoord);\n"
        "}\0";
    
    Video& Video::getInstance()
    {
        static Video instance;
        return instance;
    }
    
    Video::Video()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VideoConstants::GL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VideoConstants::GL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
        constexpr i32 windowWidth  = VideoConstants::GAMEBOY_SCREEN_WIDTH  * VideoConstants::GAMEBOY_SCREEN_SCALE;
        constexpr i32 windowHeight = VideoConstants::GAMEBOY_SCREEN_HEIGHT * VideoConstants::GAMEBOY_SCREEN_SCALE;

        m_Window = glfwCreateWindow(
            windowWidth,
            windowHeight,
            "GBcc - gl_test", 
            NULL, 
            NULL
        );

        m_Monitor = glfwGetPrimaryMonitor();
    
        auto& [xScale, yScale] = m_WindowScale;
        glfwGetMonitorContentScale(m_Monitor, &xScale, &yScale);

        GLsizei trueWidth  = xScale * windowWidth;
        GLsizei trueHeight = yScale * windowHeight;

        if (m_Window == NULL)
        {
            std::cerr << "ERROR::GLFW: Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "ERROR::GLAD: Failed to initialize OpenGL!" << std::endl;
            glfwTerminate();
            exit(-1);
        }

        glViewport(0, 0, trueWidth, trueHeight);

        InitializeVertexObjects();
        InitializeTexture();
        InitializeShader();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    Video::~Video()
    {
        glDeleteBuffers(1, &m_OutputQuadVBO);
        glDeleteBuffers(1, &m_OutputQuadEBO);
        glDeleteVertexArrays(1, &m_OutputQuadVAO);
        glDeleteTextures(1, &m_OutputQuadTexture);
        glDeleteProgram(m_ShaderProgram);
        glfwTerminate();
    }

    void Video::InitializeVertexObjects()
    {
        glGenBuffers(1, &m_OutputQuadVBO);
        glGenBuffers(1, &m_OutputQuadEBO);
        glGenVertexArrays(1, &m_OutputQuadVAO);

        glBindVertexArray(m_OutputQuadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_OutputQuadVBO);
        glBufferData(
            GL_ARRAY_BUFFER, 
            sizeof(m_OUTPUT_QUAD_VERTICES), 
            m_OUTPUT_QUAD_VERTICES.data(), 
            GL_STATIC_DRAW
        );
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OutputQuadEBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, 
            sizeof(m_OUTPUT_QUAD_INDICES), 
            m_OUTPUT_QUAD_INDICES.data(), 
            GL_STATIC_DRAW
        );

        constexpr GLuint vertexStride = 5 * sizeof(GLfloat);
        constexpr GLuint indicesBegin = 3 * sizeof(GLfloat); 

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexStride, (void*)indicesBegin);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void Video::InitializeTexture()
    {
        constexpr Framebuffer emptyData = { 0 };

        glGenTextures(1, &m_OutputQuadTexture);
        glBindTexture(GL_TEXTURE_2D, m_OutputQuadTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(
            GL_TEXTURE_2D, 
            0, 
            GL_RGB, 
            VideoConstants::GAMEBOY_SCREEN_WIDTH, 
            VideoConstants::GAMEBOY_SCREEN_HEIGHT, 
            0, 
            GL_RGB, 
            GL_UNSIGNED_BYTE, 
            emptyData.data()
        );

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Video::InitializeShader()
    {
        GLint  success;
        GLuint vertexShaderId, fragmentShaderId;
        std::array<GLchar, 512U> infoLog;

        vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderId, 1, &m_OUTPUT_QUAD_VERT_SHADER, 0);
        glCompileShader(vertexShaderId);
        glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vertexShaderId, 512U, nullptr, infoLog.data());
            std::cerr << "ERROR::SHADER::VERTEx: Compilation failed! \n" << infoLog.data() << std::endl;
        }

        fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderId, 1, &m_OUTPUT_QUAD_FRAG_SHADER, 0);
        glCompileShader(fragmentShaderId);
        glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(fragmentShaderId, 512U, nullptr, infoLog.data());
            std::cerr << "ERROR::SHADER::FRAGMENT: Compilation failed! \n" << infoLog.data() << std::endl;
        }

        m_ShaderProgram = glCreateProgram();
        glAttachShader(m_ShaderProgram, vertexShaderId);
        glAttachShader(m_ShaderProgram, fragmentShaderId);
        glLinkProgram(m_ShaderProgram);
        glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(fragmentShaderId, 512U, nullptr, infoLog.data());
            std::cerr << "ERROR::SHADER::PROGRAM: Linking failed! \n" << infoLog.data() << std::endl;
        }

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
    }

    void Video::UpdateTexture(const Framebuffer& pixels)
    {
        glBindTexture(GL_TEXTURE_2D, m_OutputQuadTexture);
        glTexSubImage2D(
            GL_TEXTURE_2D, 
            0, 
            0, 
            0, 
            VideoConstants::GAMEBOY_SCREEN_WIDTH, 
            VideoConstants::GAMEBOY_SCREEN_HEIGHT, 
            GL_RGB, 
            GL_UNSIGNED_BYTE, 
            pixels.data()
        );
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Video::Draw()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(m_ShaderProgram);

        glBindTexture(GL_TEXTURE_2D, m_OutputQuadTexture);

        glBindVertexArray(m_OutputQuadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

        glfwSwapInterval(1);
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    bool Video::ShouldClose()
    {
        return glfwWindowShouldClose(m_Window);
    }
}