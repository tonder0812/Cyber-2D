void CheckOpenGLError(const char* stmt, const char* fname, int line);
#ifdef _DEBUG
#define GL_CHECK(stmt) stmt;CheckOpenGLError(#stmt, __FILE__, __LINE__);
#else
#define GL_CHECK(stmt) stmt
#endif
