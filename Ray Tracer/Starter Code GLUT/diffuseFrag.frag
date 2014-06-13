#version 150 

//these are the interpolated values out of the rasterizer, so you can't know
//their specific values without knowing the vertices that contributed to them
in vec3 fs_normal;
in vec3 fs_light_vector;
in vec3 fs_color;
in vec4 eye;
in vec4 P;
in vec4 lightColor;

out vec4 out_Color;

void main() {
    //base colors for materials
    vec4 diffuseColor = vec4(fs_color, 1.0);
    
    //calculate diffuse term and clamp to the range [0, 1]
    float diffuseTerm = clamp(dot(normalize(fs_normal), normalize(fs_light_vector)), 0.0, 1.0);
	vec4 lightVector;
	lightVector = vec4(fs_light_vector, 0.0);
	vec4 I = P - lightVector;
	vec4 N = vec4(fs_normal, 0.0);
	vec4 R = I - 2 * dot(I, N) * N;
	vec4 V = eye - P;
	float specularFactor = clamp(dot(normalize(R), normalize(V)), 0.0, 1.0);
	specularFactor = pow(specularFactor, 10);
    out_Color = diffuseColor * diffuseTerm + diffuseColor * 0.4; // + 0.7 * specularFactor * lightColor;    //Ambient light
}