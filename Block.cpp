#include "Block.hpp"

sf::Texture *Block::tileset = NULL;


void Block::setGlobalTileset(sf::Texture& texture) {
	Block::tileset = &texture;
}

sf::IntRect Block::getBlockSubRect(blockid b_id) {
	return sf::IntRect((b_id % 10) * 32, (b_id / 10) * 32, 32, 32);
}

Block::Block(int block_id) : tint(255, 255, 255) {
	/* Set ID */
	Block::id = (blockid)block_id;

	/* Load damage */
	Block::damage = 0;
	Block::damage_max = block_max_damages[id];
	if (Block::damage_max == 0)
		Block::damage_max = 50;
	
	/* Friction */
	Block::friction = block_friction[id];

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

int Block::maxDamage() {
	return block_max_damages[Block::id];
}

void Block::render(sf::RenderTexture& window, int x, int y, float xoff, float yoff) {
	if (Block::id < 0)
		return;
	
	Block::sprite.setColor(Block::tint.toColor());
	
	Block::sprite.setPosition((int)(xoff + (float)x * 32.0), (int)(yoff + (float)y * 32.0));
	window.draw(Block::sprite);

	float damage_percent = (float)Block::damage / (float)Block::damage_max;
	if (damage_percent >= 1.0)
		damage_percent = 0.99;
	
	if (damage_percent > 0) {
		int damage_id = 9 * 10 + damage_percent * 10;
		sf::IntRect subrect = Block::sprite.getTextureRect();
		int srx = subrect.left,
			sry = subrect.top;

		subrect.left = (damage_id % 10) * 32;
		subrect.top = (damage_id / 10) * 32;
		Block::sprite.setTextureRect(subrect);

		window.draw(Block::sprite);
		
		subrect.left = srx;
		subrect.top = sry;
		Block::sprite.setTextureRect(subrect);
	}

	Block::sprite.setColor(sf::Color::White);
}


void Block::render(sf::RenderWindow& window, Vec2 pos, Vec2 offset) {
	if (Block::id < 0)
		return;
	
	Block::sprite.setColor(Block::tint.toColor());
	
	Block::sprite.setPosition(pos.x + offset.x, pos.y + offset.y);
	window.draw(Block::sprite);
	Block::sprite.setColor(sf::Color::White);
}

void Block::setTint(RGB tint) {
	Block::tint = tint;
}

RGB& Block::getTint() {
	return Block::tint;
}
