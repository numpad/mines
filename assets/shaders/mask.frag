uniform sampler2D texture;
uniform sampler2D mask;

void main() {
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	float alpha = texture2D(mask, gl_TexCoord[0].xy).a * pixel.a;
	gl_FragColor = vec4(pixel.rgb, alpha);
}