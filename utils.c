GLuint create_shader(GLenum shader_type, const char *source) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (GL_FALSE == status) {
        GLint log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

        GLchar *info_log = calloc(log_len+1, sizeof(GLchar));
        glGetShaderInfoLog(shader, log_len, NULL, info_log);

        const char *str_type = NULL;
        switch (shader_type) {
            case GL_VERTEX_SHADER: str_type = "vertex"; break;
            case GL_GEOMETRY_SHADER: str_type = "geometry"; break;
            case GL_FRAGMENT_SHADER: str_type = "fragment"; break;
            default: str_type = "unknown";
        }

        fprintf(stderr, "Compile failure in %s shader:\n%s\n", str_type, info_log);

        free(info_log);
    }

    return shader;
}

GLuint create_program(size_t count, GLuint *shaders) {
    GLuint program = glCreateProgram();

    for (size_t i = 0; i < count; i++) {
        glAttachShader(program, shaders[i]);
    }

    glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		GLint log_len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

		GLchar *info_log = calloc(log_len+1, sizeof(GLchar));

		glGetProgramInfoLog(program, log_len, NULL, info_log);
		fprintf(stderr, "Linker failure: %s\n", info_log);

        free(info_log);
	}

	return program;
}
