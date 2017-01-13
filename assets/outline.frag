uniform sampler2D texture;
uniform float step;

vec2 vec2_ceil(vec2 v) {
	return vec2(v.x, v.y);
}

void main() {
	vec2 down = vec2(0.0, step);
	vec2 right = vec2(step, 0.0);
	vec2 vert1 = vec2(step, step);
	vec2 vert2 = vec2(step, -step);
	
	float alpha = 8.0 * texture2D(texture, gl_TexCoord[0].xy).a;
	alpha -= texture2D(texture, vec2_ceil(gl_TexCoord[0].xy + down)).a;
	alpha -= texture2D(texture, vec2_ceil(gl_TexCoord[0].xy - down)).a;
	alpha -= texture2D(texture, vec2_ceil(gl_TexCoord[0].xy + right)).a;
	alpha -= texture2D(texture, vec2_ceil(gl_TexCoord[0].xy - right)).a;

	alpha -= texture2D(texture, vec2_ceil(gl_TexCoord[0].xy + vert1)).a;
	alpha -= texture2D(texture, vec2_ceil(gl_TexCoord[0].xy - vert1)).a;
	alpha -= texture2D(texture, vec2_ceil(gl_TexCoord[0].xy + vert2)).a;
	alpha -= texture2D(texture, vec2_ceil(gl_TexCoord[0].xy - vert2)).a;
	
	if (alpha < 0.01)
		gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
	else
		gl_FragColor = vec4(0.13, 0.13, 0.13, 1.0);
}