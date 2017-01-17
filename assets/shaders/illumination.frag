uniform sampler2D texture;
uniform float time;
uniform vec3 lights[];

float hard(float v) {
	return v < 0.5 ? 0.0 : 1.0;
}

float light(int i, vec2 pos) {
	return distance(lights[i].xy, pos) * (1.5 + sin(cos(time * 0.03) * 0.1));
}

void main() {
	
	float alpha = 1.0;
	alpha *= light(0, gl_TexCoord[0].xy);
	alpha *= light(1, gl_TexCoord[0].xy);
	alpha *= light(2, gl_TexCoord[0].xy);
	
	gl_FragColor = vec4(texture2D(texture, gl_TexCoord[0].xy).rgb, alpha);
}