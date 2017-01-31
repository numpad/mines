#include "Grid.hpp"

/*
Grid::Grid(sf::RenderTexture& window, const char *filename, const char *tileset) {
	Grid::offset = Vec2(0.0f, 0.0f);
	Grid::blocksize = 32;
	Grid::blocks = std::vector<Block>();
	Grid::background_blocks = std::vector<Block>();
	Grid::window = &window;

	if (!Grid::tileset.loadFromFile(tileset)) {
		puts("failed to load tileset!");
		Grid::tileset.create(320, 320);
	}

	Grid::tileset.setSmooth(false);
	Block::setGlobalTileset(Grid::tileset);
	

	FILE *fp = fopen(filename, "r+");
	if (!fp) {
		printf("could not load from \"%s\"!\n", filename);
		return;
	}
	
	int loaded_dimensions[2] = {0};
	fread(loaded_dimensions, sizeof(int), 2, fp);
	Grid::width = loaded_dimensions[0];
	Grid::height = loaded_dimensions[1];
	
	const int loadedblocks_count = loaded_dimensions[0] * loaded_dimensions[1];

	int *loadedblocks = (int*)malloc(loadedblocks_count * sizeof(int));
	fread(loadedblocks, sizeof(int), loadedblocks_count, fp);

	fclose(fp);

	for (int i = 0; i < loadedblocks_count; ++i) {
		Grid::blocks.push_back(Block(loadedblocks[i]));
		Grid::background_blocks.push_back(Block(loadedblocks[i]));
	}

	free(loadedblocks);
}
*/

Grid::Grid(Vec2 screenSize, int w, int h, const char *tileset) {
	Grid::width = w;
	Grid::height = h;
	Grid::offset = Vec2(0.0f, 0.0f);
	Grid::blocksize = 32;
	Grid::blocks = std::vector<Block>();
	Grid::background_blocks = std::vector<Block>();
	
	if (!Grid::framebuffer.create(screenSize.x, screenSize.y)) {
		puts("failed to create grid framebuffer");
	}
	Grid::framebuffer.clear(sf::Color::Transparent);
	Grid::foreground_sprite.setTexture(Grid::framebuffer.getTexture());

	if (!Grid::background_framebuffer.create(screenSize.x, screenSize.y)) {
		puts("failed to create grid background framebuffer");
	}
	Grid::background_framebuffer.clear(sf::Color::Transparent);
	Grid::background_sprite.setTexture(Grid::background_framebuffer.getTexture());

	if (!Grid::tileset.loadFromFile(tileset)) {
		puts("failed to load tileset!");
		Grid::tileset.create(320, 320);
	}

	Grid::tileset.setSmooth(false);
	Block::setGlobalTileset(Grid::tileset);
	
	for (int i = 0; i < w * h; ++i) {
		blocks.push_back(Block(-1));
		background_blocks.push_back(Block(-1));
	}

}

Grid::~Grid() {
	
}

void Grid::save(const char *filename) {
	FILE *fp = fopen(filename, "w+");
	if (!fp) {
		printf("could not write to \"%s\"!\n", filename);
		return;
	}
	
	fwrite(&(Grid::width), sizeof(int), 1, fp);
	fwrite(&(Grid::height), sizeof(int), 1, fp);

	for (size_t i = 0; i < Grid::blocks.size(); ++i) {
		fwrite(&(Grid::at(i).id), sizeof(int), 1, fp);
	}

	fclose(fp);
}



Block &Grid::at(int idx, bool foreground) {
	if (foreground)
		return blocks.at(idx);
	
	return background_blocks.at(idx);
}

Block& Grid::at(int x, int y, bool foreground) {
	if (x < 0 || y < 0 || x >= Grid::width || y >= Grid::height)
		return at(0, foreground);
	
	return Grid::at(x + y * Grid::width, foreground);
}

void Grid::set(int x, int y, Block block, bool foreground) {
	if (x < 0 || y < 0 || x >= Grid::width || y >= Grid::height)
		return;
	
	if (foreground)
		Grid::blocks.at(x + y * Grid::width) = block;
	else
		Grid::background_blocks.at(x + y * Grid::width) = block;
}

void Grid::moveCamera(float x, float y) {
	Grid::offset.x -= x;
	Grid::offset.y -= y;
}

void Grid::setCamera(float x, float y) {
	Grid::offset.x = x;
	Grid::offset.y = y;
}

int Grid::neighbors(int xp, int yp) {
	int sum = 0;
	for (int y = yp - 1; y <= yp + 1; ++y) {
		for (int x = xp - 1; x <= xp + 1; ++x) {
			if (x == xp && y == yp)
				continue;

			if (x < 0 || y < 0 || x >= Grid::width || y >= Grid::height) {
				continue;
			}
			
			if (Grid::at(x, y).id >= BLOCK_GRASS) {
				++sum;
			}
		}
	}

	return sum;
}


float Grid::raycast(Vec2 start, Vec2 dir, float len, float infinity) {
	const float step = dir.length();
	float dist = 0.0;

	while (dist < len || len < 0.0) {
		if (Grid::atPoint(start.x, start.y).collides()) {
			return dist;
		}

		start += dir;
		dist += step;
	}

	return infinity;
}

void Grid::point_to_grid(int px, int py, int& x, int& y) {
	x = (px - Grid::offset.x) / 32;
	y = (py - Grid::offset.y) / 32;
}

Block& Grid::atPoint(int px, int py, bool foreground) {
	int bx, by;
	Grid::point_to_grid(px, py, bx, by);
	return Grid::at(bx, by, foreground);
}

Block& Grid::atPoint(Vec2 p, bool foreground) {
	return Grid::atPoint(p.x, p.y, foreground);
}

void Grid::render() {
	Grid::render(-Grid::offset.x / 32, -Grid::offset.y / 32,
	            ((int)(Grid::framebuffer).getSize().x / 32) + 32,
				((int)(Grid::framebuffer).getSize().y / 32) + 32
				);
}

void Grid::render(int xs, int ys, int width, int height) {
	for (int y = ys; y < ys + height; ++y) {
		for (int x = xs; x < xs + width; ++x) {
			if (x < 0 || x >= Grid::width || y < 0 || y >= Grid::height)
				continue;
			
			Grid::at(x, y, false).render(Grid::background_framebuffer, x, y, offset.x, offset.y);
		}
	}

	for (int y = ys; y < ys + height; ++y) {
		for (int x = xs; x < xs + width; ++x) {
			if (x < 0 || x >= Grid::width || y < 0 || y >= Grid::height)
				continue;
			
			Grid::at(x, y).render(Grid::framebuffer, x, y, offset.x, offset.y);
		}
	}
}

void Grid::render(sf::RenderWindow &window, sf::Shader &shader) {
	/* Clear framebuffer */
	Grid::background_framebuffer.clear(sf::Color::Transparent);
	Grid::framebuffer.clear(sf::Color::Transparent);

	/* Draw blocks to framebuffer */
	Grid::render();

	/* Display content */
	Grid::background_framebuffer.display();
	Grid::framebuffer.display();

	window.draw(Grid::background_sprite, &shader);
	window.draw(Grid::foreground_sprite);
}

void Grid::generate() {
	#define place(name) Grid::set(x, y, Block(BLOCK_ ## name));
	#define placeabove(name) Grid::set(x, y - 1, Block(BLOCK_ ## name));
	#define placenabove(name,n) Grid::set(x, y - n, Block(BLOCK_ ## name));
	#define placebehind(name) Grid::set(x, y, Block(BLOCK_ ## name), false);
	#define placebehindabove(name) Grid::set(x, y - 1, Block(BLOCK_ ## name), false);
	#define placebehindnabove(name,n) Grid::set(x, y - n, Block(BLOCK_ ## name), false);
	#define placebehindabovexy(name,dx,dy) Grid::set(x + dx, y - dy, Block(BLOCK_ ## name), false);
	Random randomHeight(Grid::height * (1./3.) - 2, Grid::height * (1./3.) + 2);
	Random randomHeightOffset(-2, 2);
	Random randomPercent(0.0, 1.0);

	for (int x = 0; x < Grid::width; ++x) {
		int currentMaxHeight = randomHeight();
		for (int y = Grid::height - 1; y >= currentMaxHeight; --y) {
			place(DIRT);
			placeabove(GRASS);
			
			if (y == currentMaxHeight && randomPercent() < 0.075) {
				placebehindabove(DIRT);

				int treeHeight = randomPercent() * 4.0 + 3.0;
				for (int i = 0; i < treeHeight; ++i) {
					placebehindnabove(TREE, i - 2);
				}
				
				placebehindnabove(LEAVES, treeHeight - 2);
				placebehindnabove(LEAVES, treeHeight - 3);

				placebehindabovexy(LEAVES, -1, treeHeight - 2);
				placebehindabovexy(LEAVES, +1, treeHeight - 2);
				
				
			} else {
				placebehindabove(GRASS);
				
			}

			if (y > Grid::height / 5.0 + randomHeight()) {
				placebehind(STONE);
			} else {
				placebehind(DIRT);
			}
		}
	}
	#undef place
	#undef placeabove
	#undef placebehind
	#undef placebehindabove
	/*
	for (int y = 0; y < Grid::height; ++y) {
		for (int x = 0; x < Grid::width; ++x) {

			if (y > Grid::height / 3) {
				Grid::set(x, y, Block(BLOCK_DIRT));
				Grid::set(x, y, Block(BLOCK_DIRT), false);
			} else if (y == Grid::height / 3) {
				Grid::set(x, y, Block(BLOCK_GRASS));
				Grid::set(x, y - 1, Block(BLOCK_GRASS), false);
				Grid::set(x, y, Block(BLOCK_DIRT), false);
				
			}

		}
	}
	*/
}