#include "Grid.hpp"

Grid::Grid(sf::RenderTexture& window, const char *filename, const char *tileset) {
	Grid::offset = Vec2(0.0f, 0.0f);
	Grid::blocksize = 32;
	Grid::blocks = std::vector<Block>();
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
	}

	free(loadedblocks);
}

Grid::Grid(sf::RenderTexture& window, int w, int h, const char *tileset) {
	Grid::width = w;
	Grid::height = h;
	Grid::offset = Vec2(0.0f, 0.0f);
	Grid::blocksize = 32;
	Grid::blocks = std::vector<Block>();
	
	Grid::window = &window;

	if (!Grid::tileset.loadFromFile(tileset)) {
		puts("failed to load tileset!");
		Grid::tileset.create(320, 320);
	}

	Grid::tileset.setSmooth(false);
	Block::setGlobalTileset(Grid::tileset);
	
	for (int i = 0; i < w * h; ++i) {
		blocks.push_back(Block(-1));
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



Block &Grid::at(int idx) {
	return blocks.at(idx);
}

Block& Grid::at(int x, int y) {
	if (x < 0 || y < 0 || x >= Grid::width || y >= Grid::height)
		return at(0);
	
	return Grid::at(x + y * Grid::width);
}

void Grid::set(int x, int y, Block block) {
	if (x < 0 || y < 0 || x >= Grid::width || y >= Grid::height)
		return;
	
	Grid::blocks.at(x + y * Grid::width) = block;
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

Block& Grid::atPoint(int px, int py) {
	int bx, by;
	Grid::point_to_grid(px, py, bx, by);
	return Grid::at(bx, by);
}

Block& Grid::atPoint(Vec2 p) {
	return Grid::atPoint(p.x, p.y);
}

void Grid::render() {
	Grid::render(-Grid::offset.x / 32, -Grid::offset.y / 32,
	            ((int)(*Grid::window).getSize().x / 32) + 32,
				((int)(*Grid::window).getSize().y / 32) + 32
				);
}

void Grid::render(int xs, int ys, int width, int height) {
	for (int y = ys; y < ys + height; ++y) {
		for (int x = xs; x < xs + width; ++x) {
			if (x < 0 || x >= Grid::width || y < 0 || y >= Grid::height)
				continue;
			
			Grid::at(x, y).render(*Grid::window, x, y, offset.x, offset.y);
		}
	}
}

void Grid::generate() {
	for (int y = 0; y < Grid::height; ++y) {
		for (int x = 0; x < Grid::width; ++x) {
			if (x == 4) {
				Grid::set(x, y, Block(BLOCK_COBBLESTONE));
			}
			
			if (y > Grid::height / 3) {
				Grid::set(x, y, Block(BLOCK_DIRT));
			} else if (y == Grid::height / 3) {
				Grid::set(x, y, Block(BLOCK_GRASS));
			}

		}
	}
}