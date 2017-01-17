uniform sampler2D texture;
uniform sampler2D noiseTexture;
uniform float time;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float perlin(vec2 pos) {
	return texture2D(noiseTexture, pos).r;
}

float hardedge(float v) {
	return v > 0.5 ? 1.0 : 0.0;
}

vec3 rainbow(vec2 pos, float off) {
	return vec3(sin(6.93 * pos.x + off), sin(3.6 * pos.y + off), cos(5.27 * pos.x + off));
}

void main() {
	float xtimeoff = cos(time * 0.01 + gl_TexCoord[0].y) * 0.01;
	float ytimeoff = sin(time * (gl_TexCoord[0].x * 0.05)) * 0.01;
	vec4 pixel = texture2D(texture, vec2(gl_TexCoord[0].x + sin(time * 0.1) * sin(gl_TexCoord[0].y * 30.0) / 60.0 + xtimeoff, gl_TexCoord[0].y + cos(gl_TexCoord[0].y * 30.0) / 60.0 + ytimeoff));

	gl_FragColor = pixel;
}