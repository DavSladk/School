#include <Shader.h>

Shader::Shader(const std::string& vertFile, const std::string& fragFile)
	: vertFile(vertFile), fragFile(fragFile), m_RendererID(0)
{
	std::string vertexShader = ParseShader(vertFile);
	std::string fragmentShader = ParseShader(fragFile);
	m_RendererID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
	{
		std::cout << "WArning uniform '" << name << "' does not exist!" << std::endl;
	}

	m_UniformLocationCache[name] = location;

	return location;
}

std::string Shader::ParseShader(const std::string& filepath)
{
	std::ifstream file(filepath);

	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

	return content;
}

GLuint Shader::CompileShader(GLuint type, const std::string& source)
{
	GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, NULL, message);
		std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cerr << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}