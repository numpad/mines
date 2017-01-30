uniform sampler2D texture;
uniform const int lights_count;
uniform vec3 lights[];
uniform float global_illumination;

float hard(float v) {
	return v < 0.5 ? 0.0 : 1.0;
}

float light(const int i, vec2 pos) {
	float dist = distance(lights[i].xy, pos) * (lights[i].z * global_illumination);
	return dist;
}

void main() {
	
	float alpha = 1.0;
	alpha *= light(0, gl_TexCoord[0].xy);
	

	gl_FragColor = vec4(texture2D(texture, gl_TexCoord[0].xy).rgb, alpha);
}