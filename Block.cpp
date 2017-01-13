#include "Block.hpp"

sf::Texture *Block::tileset = NULL;

void Block::setGlobalTileset(sf::Texture& texture) {
	Block::tileset = &texture;
}

Block::Block(int block_id) : tint(255, 255, 255) {
	Block::id = (blockid)block_id;
	Block::damage = 0;

	Block::damage_max = block_max_damages[id];
	if (Block::damage_max == 0)
		Block::damage_max = 50;
	
	Block::sprite.setTexture(*Block::tileset);
	Block::sprite.setTextureRect(sf::IntRect((Block::id % 10) * 32, (Block::id / 10) * 32, 32, 32));
}

void Block::updateId(int newid) {
	Block::id = (blockid)newid;
	Block::sprite.setTextureRect(sf::IntRect((Block::id % 10) * 32, (Block::id / 10) * 32, 32, 32));
}

bool Block::collides() {
	if (id < 0)
		return false;
	return block_does_collide[id];
}

void Block::render(sf::RenderTexture& window, int x, int y, float xoff, float yoff) {
	if (Block::id < 0)
		return;
	
	Block::sprite.setColor(Block::tint.toColor());
	
	Block::sprite.setPosition((int)(xoff + (float)x * 32.0), (int)(yoff + (float)y * 32.0));
	window.draw(Block::sprite);

	float damage_percent = (float)Block::damage / (float)Block::damage_max;
	if (damage_percent > 0) {
		//int damage_id = 9 * 10 + damage_percent * 10;
		//qw_drawimage_quad(spr, (int)(xoff + (float)x * (float)block_size), (int)(yoff + (float)y * (float)block_size), block_size, block_size, (int)(damage_id % 10) * 32, (int)(damage_id / 10) * 32, 32, 32);
	}
	Block::sprite.setColor(sf::Color::White);
}

void Block::setTint(RGB tint) {
	Block::tint = tint;
}

RGB& Block::getTint() {
	return Block::tint;
}
