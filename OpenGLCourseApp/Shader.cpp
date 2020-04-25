#include "Shader.h"

Shader::Shader() : shaderID{0}, uniformProjection{0}, uniformModel{0}, uniformView{0} { }

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
	 
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;                
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		std::printf("fail to read %s! File does not exist.", fileLocation);
		return "";
	}

		std::string line = "";
		while (!fileStream.eof())
		{
			std::getline(fileStream, line);
			content.append(line + "\n");
		}

		fileStream.close();
		return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	// create a program and give shader the ID; actual program sits on GPU
	shaderID = glCreateProgram();

	if (!shaderID) {
		printf("Error creating the shader program....\n");
		return;
	}

	// add shaders to program; vShader and fShader that was created above
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	// Get any erros from shaders
	// hard to debug
	GLint result = 0;
	GLchar elog[1024] = { 0 };

	// create all executble on GPU
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);			// we pass the program and which info we want the which is status and store the status in result variable

	// check for errors
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);	// store the error info in the variable elog to debug if there is a problem in vertex and fragment shaders
		printf("Error linking program: %s \n", elog);
		return;
	}

	// validate & check for errors
	glValidateProgram(shaderID);										// check if the shader program is valid for Open GL
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);			// we pass the program and which info we want the which is status and store the status in result variable
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);		// store the error info in the variable elog to debug if there is a problem in vertex and fragment shaders
		printf("Error validating program: %s \n", elog);
		return;
	}

	uniformModel		= glGetUniformLocation(shaderID, "model");
	uniformProjection	= glGetUniformLocation(shaderID, "projection");
	uniformView			= glGetUniformLocation(shaderID, "view");
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{

	return uniformView;
}

void Shader::UseShader()
{
	// add a check if(shaderID!=0;); if not used shaderID will be assigned as zero but no error 
	glUseProgram(shaderID); 
}

void Shader::ClearShader()
{
	if (shaderID != 0) 
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModel = 0;
	uniformProjection = 0;
}


void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	// create an empty shader(fragment or fragment) and pass the ID
	GLuint theShader = glCreateShader(shaderType);

	// required variable format for OpenGL glShaderSource() function
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = static_cast<GLint>(strlen(shaderCode));		// strlen is C version of std::strlen() ; we included string.h C library

	// Load the shader file into GL
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	// Get any erros from shaders
	// hard to debug
	GLint result = 0;
	GLchar elog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);			// we pass the program and which info we want the which is status and store the status in result variable
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);	// store the error info in the variable elog to debug if there is a problem in vertex and fragment shaders
		printf("Error compiling the %d shader: %s \n", shaderType, elog);
		return;
	}
	glAttachShader(theProgram, theShader);
}




Shader::~Shader() { ClearShader(); }