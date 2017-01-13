#ifndef GRID_HPP
#define GRID_HPP

#include "rgb.hpp"
#include "Block.hpp"
#include "Vec2.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

class Grid {
	int blocksize;
public:
	sf::RenderTexture *window;
	sf::Texture tileset;
	int width, height;
	Vec2 offset;
	
	std::vector<Block> blocks;

	Grid(sf::RenderTexture&, const char *filename);
	Grid(sf::RenderTexture&, int width, int height, const char *tileset);
	~Grid();

	void moveCamera(float x, float y);
	void setCamera(float x, float y);

	void save(const char *filename);

	void generate();

	Block& at(int idx);
	Block& at(int x, int y);
	void set(int x, int y, Block b);
	
	int neighbors(int x, int y);

	void point_to_grid(int px, int py, int& x, int& y);
	Block& atPoint(int px, int py);

	void render();
	void render(int xs, int ys, int width, int height);
};

#endif