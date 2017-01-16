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
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	vec4 orig  = pixel;

	if (pixel.a == 0.0) {
		float noise = rand(gl_TexCoord[0].xy) * 0.015;

		pixel.rgb = vec3(0.0 + noise, 0.0 + noise, 0.1 + noise);
		pixel.a = 1.0 - gl_TexCoord[0].y;
		
	}
	
	gl_FragColor = pixel;
}