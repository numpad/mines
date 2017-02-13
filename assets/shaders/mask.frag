uniform sampler2D texture;
uniform sampler2D mask;

void main() {
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	vec4 mask = texture2D(mask, gl_TexCoord[0].xy);
	float alpha = mask.a * pixel.a;
	
	if (mask.rgb != vec3(0, 0, 0)) {
		gl_FragColor = mask;
	} else {
		gl_FragColor = vec4(pixel.rgb, alpha);
	}
}