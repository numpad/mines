#ifndef LIGHTSYSTEM_HPP
#define LIGHTSYSTEM_HPP

#include <SFML/Graphics.hpp>

class LightSystem {
	sf::RenderTexture darkness;
	sf::Shader lightshader;
	sf::Sprite darkness_sprite;
	float time;

	sf::Glsl::Vec3 *lights;
	size_t lights_count;
public:

	LightSystem(unsigned int, unsigned int);

	void render(sf::RenderWindow& window);
};

#endif