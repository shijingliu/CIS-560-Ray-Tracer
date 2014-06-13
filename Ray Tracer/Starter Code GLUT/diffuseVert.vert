#version 150

//there's no rules about the prefixes but they do help
//keep track of things
uniform mat4 u_modelMatrix;
uniform mat4 u_projMatrix;
uniform vec3 u_pos;
uniform vec3 u_col;
uniform vec3 eye_pos;

in vec4 vs_normal;
in vec4 vs_position;
in vec3 vs_color;

out vec3 fs_normal;
out vec3 fs_light_vector;
out vec3 fs_color;
out vec4 P;
out vec4 eye;
out vec4 lightColor;

void main() { 
	lightColor = vec4(u_col, 1.0);

	//const vec3 lightPos = vec3(2.0, 0.0, 2.0);
	const vec3 lightPos = vec3(2.0, 0.0, 2.0);
    
    // Set the color and normal as just the input from the VBOs,
	// they only get interesting after they've been interpolated by the rasterizer
    fs_color = vs_color * u_col;
	
	//fs_normal = vs_normal.xyz;
	fs_normal = (u_modelMatrix * vs_normal).xyz;

    // Set up our vector for the light
    //fs_light_vector = vec3(lightPos - (u_modelMatrix * vs_position).xyz);
	fs_light_vector = vec3(u_pos - (u_modelMatrix * vs_position).xyz);

	
	//built-in things to pass down the pipeline

	P = vs_position;
	eye = vec4(eye_pos, 1.0);

    gl_Position = u_projMatrix * u_modelMatrix * vs_position;
}