#include "pch.h"
#include <glad/glad.h>

void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
	while (true)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			CB_CORE_ASSERT(false, "OpenGL error {}, at {}:{} - for {}\n", err, fname, line, stmt);
			break;
		}
		else
		{
			break;
		}
	}
}