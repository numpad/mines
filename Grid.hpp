#ifndef GRID_HPP
#define GRID_HPP

#include "rgb.hpp"
#include "Block.hpp"
#include "Vec2.hpp"
#include "Random.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

class Grid {
	int blocksize;

public:
	sf::RenderTexture framebuffer, background_framebuffer;
	sf::Sprite background_sprite, foreground_sprite;
	sf::Shader backgroundShader;

	sf::Texture tileset;
	int width, height;
	Vec2 offset;
	
	std::vector<Block> blocks;
	std::vector<Block> background_blocks;

	Grid(Vec2 screenSize, int width, int height, const char *tileset);
	~Grid();

	void moveCamera(float x, float y);
	void setCamera(float x, float y);

	bool load(const char *filename);
	void save(const char *filename);

	void generate();

	Block& at(int idx, bool foreground = true);
	Block& at(int x, int y, bool foreground = true);
	void set(int x, int y, Block b, bool foreground = true);
	
	int neighbors(int x, int y);
	float raycast(Vec2 start, Vec2 dir, float len = 100.0, float infinity = -1.0);

	void point_to_grid(int px, int py, int& x, int& y);
	Block& atPoint(int px, int py, bool foreground = true);
	Block& atPoint(Vec2 p, bool foreground = true);

	void render();
	void render(int xs, int ys, int width, int height);

	void render(sf::RenderWindow &window);
};

#endif