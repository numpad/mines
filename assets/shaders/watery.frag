uniform sampler2D texture;
uniform float time;

void main() {
	
	gl_FragColor = texture2D(texture, vec2(
		gl_TexCoord[0].x + sin(gl_TexCoord[0].y * 20.0 + time * 2.0) / 40.0,
		gl_TexCoord[0].y
	));
	
}