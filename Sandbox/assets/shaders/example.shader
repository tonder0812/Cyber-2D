#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
uniform mat4 u_camera;
out vec4 v_pos;
out vec2 v_TexCoord;

void main() {
	gl_Position = u_camera * vec4(a_Position, 1.0);
	v_pos = u_camera * vec4(a_Position, 1.0);
	v_TexCoord = a_TexCoord;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 v_pos;
in vec2 v_TexCoord;
uniform vec3 u_color;
uniform int u_useColor;
uniform int u_useImage;
uniform sampler2D u_sampler;
uniform int u_channels;
void main() {
	if (u_useColor != 0) {
		color = vec4(u_color, 1.0);
	}
	else if (u_useImage != 0) {
		vec4 c = texture(u_sampler, v_TexCoord);
		switch (u_channels) {
		case 1:
			color = vec4(c.x, c.x, c.x, 1);
			break;
		case 2:
			color = vec4(c.x, c.x, c.x, c.y);
			break;
		case 3:
			color = vec4(c.x, c.y, c.z, 1);
			break;
		case 4:
			color = vec4(c.x, c.y, c.z, c.w);
			break;
		}
	}
	else {
		color = v_pos * 0.5 + 0.5;
	}
}