/*
* GBcc: Game Boy (DMG) Emulator
* Copyright (C) 2025 Daniel Frias
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utility>
#include <array>

#include "Types.hpp"
#include "VideoConstants.hpp"

namespace GBcc {
    class Video 
    {
        private:
        using Framebuffer = std::array<u8, VideoConstants::FRAMEBUFFER_SIZE>;

        const static GLfloat m_OUTPUT_QUAD_VERTICES[]; 
        const static GLuint  m_OUTPUT_QUAD_INDICES[];

        const static char* m_OUTPUT_QUAD_VERT_SHADER;
        const static char* m_OUTPUT_QUAD_FRAG_SHADER;

        GLFWwindow* m_Window;
        GLFWmonitor* m_Monitor;

        std::pair<GLfloat, GLfloat> m_WindowScale;

        GLuint m_OutputQuadVBO;
        GLuint m_OutputQuadEBO;
        GLuint m_OutputQuadVAO;

        GLuint m_OutputQuadTexture;

        GLuint m_ShaderProgram;

        public:
        static Video& getInstance();
        void UpdateTexture(const Framebuffer& pixels);
        void Draw();
        bool ShouldClose();

        private:
        Video();
        ~Video();

        void InitializeVertexObjects();
        void InitializeTexture();
        void InitializeShader();

        public:
        Video(Video const&)             = delete;
        void operator=(Video const&)    = delete;
    };
}