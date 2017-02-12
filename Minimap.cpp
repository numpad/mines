#include "Minimap.hpp"

Minimap::Minimap(size_t width, size_t height) {
	/* Generate texture for minimap */
	if (!Minimap::texture.create(width, height)) {
		printf("[Minimap] Failed creating %lux%lu texture!\n", width, height);
	}
	Minimap::width = width;
	Minimap::height = height;

	Minimap::sprite.setTexture(Minimap::texture);
	Minimap::sprite.setScale(4, 4);
	Minimap::pixels = (sf::Uint8 *)calloc(width * height * 4, sizeof(sf::Uint8));
	Minimap::updateTexture();
}

Minimap::~Minimap() {
	free(Minimap::pixels);
}

void Minimap::set(size_t i, sf::Color color) {
	const size_t arridx = i * 4;
	if (arridx >= width * height * 4) {
		printf("[i] arridx = %lu (max. %lu)\n", arridx, width * height * 4);
		return;
	}
	
	Minimap::pixels[arridx + 0] = color.r;
	Minimap::pixels[arridx + 1] = color.g;
	Minimap::pixels[arridx + 2] = color.b;
	Minimap::pixels[arridx + 3] = color.a;
}

void Minimap::set(size_t x, size_t y, sf::Color color) {
	const size_t arridx = x + y * Minimap::width;
	Minimap::set(arridx, color);
}

void Minimap::updateTexture() {
	Minimap::texture.update(Minimap::pixels);
}

void Minimap::update(Grid &grid, size_t x, size_t y) {

}

void Minimap::draw(sf::RenderTarget &target) {
	Minimap::sprite.setPosition(target.getSize().x - (Minimap::sprite.getTextureRect().width * Minimap::sprite.getScale().x) - 10, 10);
	Minimap::sprite.setTexture(Minimap::texture);
	target.draw(Minimap::sprite);
}