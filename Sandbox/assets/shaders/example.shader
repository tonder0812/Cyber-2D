#type vertex
#version 330 core

layout(location = 0) in vec3 a_pos;
uniform mat4 u_camera;
out vec4 v_pos;

void main() {
	gl_Position = u_camera * vec4(a_pos, 1.0);
	v_pos = u_camera * vec4(a_pos, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 v_pos;
uniform vec3 u_color;
uniform int u_useColor;
void main() {
	if (u_useColor != 0) {
		color = vec4(u_color, 1.0);
	}
	else {
		color = v_pos * 0.5 + 0.5;
	}
}

