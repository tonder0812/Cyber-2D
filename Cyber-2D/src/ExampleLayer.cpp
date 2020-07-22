#include "pch.h"
#include "Cyber.h"
#include <imgui.h>
#include "ExampleLayer.h"
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void DemoLayer::onAttach() {
	glGenVertexArrays(1, &m_VertexArr);
	glBindVertexArray(m_VertexArr);

	glGenBuffers(1, &m_VertexBuff);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuff);

	float vert[] = {
		-0.5f , -0.5f , 0,
		 0.5f , -0.5f , 0,
		 0    ,  0.5f , 0
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	glGenBuffers(1, &m_IndexBuff);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuff);

	unsigned int indeces[] = { 0,1,2 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);


	// Read our shaders into the appropriate buffers
	std::string vertexSource = R"(
		#version 330 core

		layout(location=0) in vec3 a_pos;

		void main(){
			gl_Position=vec4(a_pos,1.0);
		}
	)";// Get source code for vertex shader.
	std::string fragmentSource = R"(
		#version 330 core

		layout(location=0) out vec4 color;	
		uniform vec3 u_color;		

		void main(){
			color=vec4(u_color,1.0);
		}
	)"; // Get source code for fragment shader.

	// Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = (const GLchar*)vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
		CB_CORE_ERROR(&infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertexShader);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		return;
	}

	// Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = (const GLchar*)fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
		CB_CORE_ERROR(&infoLog[0]);
		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);
		return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	m_program = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);

	// Link our program
	glLinkProgram(m_program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]);
		CB_CORE_ERROR(&infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(m_program);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		return;
	}
	

	// Always detach shaders after a successful link.
	glDetachShader(m_program, vertexShader);
	glDetachShader(m_program, fragmentShader);

}

void DemoLayer::onDetach() {
	glDeleteBuffers(1, &m_VertexBuff);
	glDeleteBuffers(1, &m_IndexBuff);
	glDeleteProgram(m_program);
}

void DemoLayer::onImGUI() {
	ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_Color));
	ImGui::ShowDemoWindow();
}
void DemoLayer::onUpdate() {
	glUseProgram(m_program);
	glUniform3f(glGetUniformLocation(m_program, "u_color"),m_Color[0],m_Color[1],m_Color[2]);
	glBindVertexArray(m_VertexArr);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}