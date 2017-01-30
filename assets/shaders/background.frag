uniform sampler2D texture;

void main() {
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	pixel.rgb *= 0.65;

	gl_FragColor = pixel;
}