#include "LightSystem.hpp"

LightSystem::LightSystem(unsigned int width, unsigned int height) {
	LightSystem::lights_count = 3;
	LightSystem::lights = (sf::Glsl::Vec3 *)malloc(LightSystem::lights_count * sizeof(sf::Glsl::Vec3));

	LightSystem::lights[0] = sf::Glsl::Vec3(0.0, 0.0, 0.3);
	LightSystem::lights[1] = sf::Glsl::Vec3(1.0, 1.0, 0.1);
	LightSystem::lights[2] = sf::Glsl::Vec3(0.5, 0.5, 0.6);

	LightSystem::darkness.create(width, height);
	LightSystem::darkness.clear(sf::Color::Black);

	if (!LightSystem::lightshader.loadFromFile("assets/shaders/illumination.frag", sf::Shader::Fragment)) {
		puts("failed loading lightsystem shader!");
	}

	LightSystem::darkness_sprite.setTexture(LightSystem::darkness.getTexture());

	LightSystem::lightshader.setUniform("texture", sf::Shader::CurrentTexture);
	LightSystem::lightshader.setUniformArray("lights", LightSystem::lights, LightSystem::lights_count);
}

void LightSystem::render(sf::RenderWindow& window) {
	/* Update shader uniform */
	LightSystem::lightshader.setUniform("time", LightSystem::time++);
	
	LightSystem::darkness.display();
	
	window.draw(LightSystem::darkness_sprite, &(LightSystem::lightshader));
}
