#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

class Shader
{
private:
	GLuint m_RendererID;
	std::string vertFile;
	std::string fragFile;
	std::unordered_map<std::string, unsigned int> m_UniformLocationCache;

	unsigned int GetUniformLocation(const std::string& name);
	GLuint CompileShader(GLuint type, const std::string& source);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	std::string ParseShader(const std::string& filepath);
public:
	Shader(const std::string& vertFile, const std::string& fragFile);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix);
};