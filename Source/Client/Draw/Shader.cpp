#include <Draw/Shader.hpp>

#include <File.h>

#include <spdlog/spdlog.h>
#include <GL/gl3w.h>

namespace Engine
{

	Shader::Shader(const std::string &path)
	{
		std::string fShaderCode = File::slurp((path + ".frag").c_str());
		std::string vShaderCode = File::slurp((path + ".vert").c_str());

		Shader(fShaderCode.c_str(), vShaderCode.c_str());
	}

	Shader::Shader(const char *fShaderCode, const char *vShaderCode)
	{
		unsigned int vertexShader, fragmentShader;
		int success;
		char infoLog[512];

		// Compile Vertex Shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, 0);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			spdlog::info("Vertex shader compilation failed: {}", infoLog);
		}

		// Compile Fragment Shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, 0);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			spdlog::info("Fragment shader compilation failed: {}", infoLog);
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			spdlog::info("Shader link failed: {}", infoLog);
		}

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
	}

	void Shader::use()
	{
		glUseProgram(ID);
	}

	void Shader::setBool(const char *name, bool value)
	{
		glUniform1i(glGetUniformLocation(ID, name), (int)value);
	}

	void Shader::setInt(const char *name, int value)
	{
		glUniform1i(glGetUniformLocation(ID, name), value);
	}

	void Shader::setFloat(const char *name, GLfloat value)
	{
		glUniform1f(glGetUniformLocation(ID, name), value);
	}

	void Shader::setVec2(const char *name, float *value)
	{

		glUniform2fv(glGetUniformLocation(ID, name), 1, value);
	}

	void Shader::setMat4(const char *name, const glm::mat4 &value)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, &value[0][0]);
	}

	void Shader::setMat4Offset(const char *name, const glm::mat4 &value, int offset)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name) + offset, 1, false, &value[0][0]);
	}

	void Shader::fillSamplers()
	{
		glUseProgram(ID);
		int samplers[32];
		for (int i = 0; i < 32; i++)
			samplers[i] = i;

		glUniform1iv(glGetUniformLocation(ID, "u_Textures"), 32, samplers);
	}

}