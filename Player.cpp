#include "Player.hpp"

Player::Player(Vec2 screenSize) : Entity("assets/player/skin.png"), inventory(10), textFont("assets/font/font.png", Vec2(5, 8)) {
	/* Offset body --> feet */
	Player::feetOffset = Vec2(7.0, 37.0);
	Player::headOffset = Vec2(6.0, -28.0);
	Player::legAnimation = 0.0;

	/* Jumping, Walking Speed */
	Player::jumpstrength = 9.5; // TODO: Make a little random
	Player::acc = Vec2(0.75, 0.0);
	Player::maxvel = Vec2(3.5, 7.75);
	
	/* define render order */
	Player::limb_z_index = std::vector<size_t>();
	Player::limb_z_index.push_back( 1);
	Player::limb_z_index.push_back( 4);
	Player::limb_z_index.push_back( 3);
	Player::limb_z_index.push_back(-1);
	Player::limb_z_index.push_back( 0);
	Player::limb_z_index.push_back( 2);

	/* Inventory */
	Player::setPlaceMode(Player::PLACE_FOREGROUND);
	Player::currentItemSelected = 0;
	/*
	Player::inventory.add(InventoryStack( 6, BLOCK_TNT));
	Player::inventory.add(InventoryStack(24, BLOCK_GRASS));
	Player::inventory.add(InventoryStack(35, BLOCK_WOOD));
	Player::inventory.add(InventoryStack( 2, BLOCK_OVEN));
	Player::inventory.add(InventoryStack( 1, BLOCK_WORKBENCH));
	Player::inventory.add(InventoryStack( 8, BLOCK_TREE));
	Player::inventory.add(InventoryStack(64, BLOCK_GRASS_SNOWY));
	Player::inventory.add(InventoryStack( 8, BLOCK_GLASS));
	Player::inventory.add(InventoryStack(30, BLOCK_COBBLESTONE));
	Player::inventory.add(InventoryStack(900, BLOCK_ICE, 900));
	*/
	
	if (!Player::inventoryHotbarTexture.loadFromFile("assets/gui/inventory.png")) {
		puts("[Player] failed loading 'assets/gui/inventory.png'!");
	}
	
	Player::inventoryHotbarSprite.setTexture(Player::inventoryHotbarTexture);
	Player::inventoryHotbarSprite.setPosition(screenSize.x / 2.0 - (float)Player::inventoryHotbarTexture.getSize().x / 2.0, screenSize.y - (float)Player::inventoryHotbarTexture.getSize().y * 1.25);

	/* Create Limbs and attach to body */
	Player::addLimb(     Limb(Entity::skin, sf::IntRect( 5, 29,  8, 24), Vec2(0.5, 0.5), Vec2( 0,  1)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect( 5,  5, 22, 22), Vec2(0.5, 0.9), Vec2( 0, -4)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect(33, 29,  8, 24), Vec2(0.5, 0.1), Vec2( 0, -2)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect(19, 29,  8, 24), Vec2(0.5, 0.1), Vec2( 0, -2)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect(33,  3,  8, 24), Vec2(0.5, 0.1), Vec2( 0,  7)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect(46,  3,  8, 24), Vec2(0.5, 0.1), Vec2( 0,  7)));

	/* Keep pointers to provide easy access  */
	Player::head = &Player::body.getChild(0);
	Player::armb = &Player::body.getChild(1);
	Player::armf = &Player::body.getChild(2);
	Player::legf = &Player::body.getChild(3);
	Player::legb = &Player::body.getChild(4);
}

void Player::animate(Grid& grid) {
	if (Player::walkstate == Entity::WalkState::WALKING) {
		Player::legAnimation += Player::walkdir;

		Player::legf->getAngle() =  sin(Player::legAnimation / 6.0) * 30.0;
		Player::legb->getAngle() = -Player::legf->getAngle();
		Player::armf->getAngle() =  cos(Player::legAnimation / 7.0) * 20.0;
		Player::armb->getAngle() = -Player::armf->getAngle();
	} else if (Player::walkstate == Entity::WalkState::STANDING) {
		if (fabs(Player::vel.x) > 0.0) {
			Player::legAnimation += Player::walkdir * 1.5;
		} else {
			Player::legAnimation = 0.0;
		}

		Player::legf->getAngle() *= 0.75;
		Player::legb->getAngle() = -Player::legf->getAngle();
	
	}
}

void Player::update(Grid& grid) {
	Player::body.update();
	Player::physicsUpdate(grid);
	Player::animate(grid);

	Player::head->flipx = Player::walkdir;
}

void Player::render(sf::RenderWindow& window, Vec2 off) {
	Player::body.renderAll(window, Player::limb_z_index, Player::pos + off);
}

void Player::render(sf::RenderWindow& window, sf::Shader& shader, Vec2 off) {
	Player::body.renderAll(window, Player::limb_z_index, shader, Player::pos + off);
}

/* Inventory Management */

void Player::renderInventory(sf::RenderWindow &window, Vec2 off) {
	window.draw(Player::inventoryHotbarSprite);

	for (size_t i = 0; i < Player::inventory.getSize(); ++i) {
		Block invblock(Player::inventory.at(i).get());
		if (invblock.id < 0)
			continue;
		
		float yoff = 0;
		if (i == Player::currentItemSelected) {
			float elapsed = Player::timeAlive.getElapsedTime().asSeconds();
			yoff = -fabs(sin(elapsed * 4.0) * 7.0);
		}
		
		Vec2 invblockPos = Vec2(Player::inventoryHotbarSprite.getPosition().x + 4.0, Player::inventoryHotbarSprite.getPosition().y + 4.0) + Vec2(i * 40, yoff);
		invblock.render(window, invblockPos);

		/* Blocks left */
		wchar_t itemCountStr[5];
		swprintf(itemCountStr, 4, L"%lu", Player::inventory.at(i).count);

		Player::textFont.write(window, invblockPos + Vec2(1, 1), itemCountStr, sf::Color(80, 80, 80));
		Player::textFont.write(window, invblockPos, itemCountStr);
		
	}
}

blockid Player::getItem() {
	return Player::inventory.at(Player::currentItemSelected).get();
}

blockid Player::takeItem() {
	return Player::inventory.at(Player::currentItemSelected).take();
}

bool Player::canCollect(blockid type) {
	return Player::inventory.hasSpaceFor(type);
}

size_t Player::collectItems(blockid type, size_t count) {
	Player::inventory.add(InventoryStack(count, type));
	return 0;
}

void Player::selectItem(size_t index) {
	Player::currentItemSelected = index;
}

void Player::setPlaceMode(Player::PlaceMode pm) {
	Player::placeMode = pm;
}

bool Player::getPlaceMode() {
	return static_cast<bool>(Player::placeMode);
}

bool Player::load(const char *fn) {
	FILE *fp = fopen(fn, "r");
	if (!fp) {
		return false;
	}

	float loaded_position[2];
	fread(loaded_position, sizeof(float), 2, fp);

	for (size_t i = 0; i < Player::inventory.getSize(); ++i) {
		size_t blockCount;
		blockid blockId;

		fread(&blockCount, sizeof(size_t), 1, fp);
		fread(&blockId, sizeof(blockid), 1, fp);

		Player::collectItems(blockId, blockCount);
	}

	Player::setPos(Vec2(loaded_position[0], loaded_position[1]));

	fclose(fp);
	return true;
}

void Player::save(const char *fn) {
	FILE *fp = fopen(fn, "w+");
	if (!fp) {
		printf("Cannot save to \"%s\"!\n", fn);
		return;
	}
	
	fwrite(&(Player::pos.x), sizeof(float), 1, fp);
	fwrite(&(Player::pos.y), sizeof(float), 1, fp);
	
	for (size_t i = 0; i < Player::inventory.getSize(); ++i) {
		fwrite(&(Player::inventory.at(i).count), sizeof(size_t), 1, fp);
		const int blockId = Player::inventory.at(i).get();

		fwrite(&(blockId), sizeof(blockid), 1, fp);
		
	}

	fclose(fp);
}