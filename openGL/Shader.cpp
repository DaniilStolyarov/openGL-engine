#include "Shader.h"

void Shader::getSourceFromFile()
{
	ifstream in(this->path);
	char temp;
	if (in.is_open())
	{
		while (1)
		{
			in.get(temp);
			if (in.eof()) break;
			this->source += temp;
		}
	}
}
Shader::Shader()
{

}
Shader::Shader(int ShaderType, string path)
{
	this->path = path;
	this->getSourceFromFile();

	this->glShader = glCreateShader(ShaderType);
	const GLchar* glSource = this->source.c_str();
	glShaderSource(this->glShader, 1, &glSource, NULL);
	glCompileShader(glShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(glShader, GL_COMPILE_STATUS, &success); 
	if (!success)
	{
		string errorShaderType;
		switch (ShaderType)
		{
		case GL_VERTEX_SHADER:
			errorShaderType = "VERTEX";
			break;
		case GL_FRAGMENT_SHADER:
			errorShaderType = "FRAGMENT";
		}
		glGetShaderInfoLog(glShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << errorShaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}