#ifndef LIGHTSYSTEM_HPP
#define LIGHTSYSTEM_HPP

#include <SFML/Graphics.hpp>

class LightSystem {
	sf::RenderTexture darkness;
	sf::Shader lightshader;
	sf::Sprite darkness_sprite;
	float time, global_illumination;

	sf::Glsl::Vec3 *lights;
	size_t lights_count;
public:

	LightSystem(unsigned int, unsigned int);

	void setLight(size_t idx, sf::Glsl::Vec3 vec);
	void setGlobalLight(float global_illumination);

	void render(sf::RenderWindow& window);
};

#endif