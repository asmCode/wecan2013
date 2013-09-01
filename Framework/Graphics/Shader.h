#ifndef load_model_test_Shader_h
#define load_model_test_Shader_h

#include "../Math/Matrix.h"
#include "../Math/Vec3.h"
#include "../Math/Vec4.h"
#include <string>
#include <Windows.h>
#include <GL/glew.h>
#include <gl/gl.h>

class Shader
{
public:
	Shader();
	~Shader();
	
	static Shader* LoadFromFile(const char *vertexShaderFile,
								const char *fragmentShaderFile);
	
	void UseProgram();
	
	void BindVertexChannel(unsigned channel, const char *name);
	
	void SetParameter(const char *name, float val);
	void SetParameter(const char *name, float val1, float val2, float val3);
	void SetParameter(const char *name, float val1, float val2, float val3, float val4);
	
	void SetParameter(const char *name, const sm::Vec3 &val);
	void SetParameter(const char *name, const sm::Vec4 &val);
	void SetTextureParameter(const char *name, unsigned channel, unsigned texId);
	void SetMatrixParameter(const char *name, const sm::Matrix &matrix);

	void LinkProgram();
	
public:
	GLuint m_programId;
	GLuint m_vertShaderId;
	GLuint m_fragShaderId;
	
	static GLuint CompileShader(GLenum shaderType, const char* file);
	static bool ValidateProgram(GLuint programId);
};

#endif
