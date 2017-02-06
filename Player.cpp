#include "Player.hpp"

Player::Player(Vec2 screenSize) : Entity("assets/player/skin.png"), textFont("assets/font/font.png", Vec2(5, 8)) {
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
	Player::showInventory = false;
	
	if (!Player::inventoryHotbarTexture.loadFromFile("assets/gui/inventory.png")) {
		puts("[Player] failed loading 'assets/gui/inventory.png'!");
	}
	
	Player::inventoryHotbarSprite.setTexture(Player::inventoryHotbarTexture);
	Player::inventoryHotbarSprite.setPosition(screenSize.x / 2.0 - (float)Player::inventoryHotbarTexture.getSize().x / 2.0, screenSize.y - (float)Player::inventoryHotbarTexture.getSize().y * 1.25);

	/* Full inventory */
	if (!Player::inventoryFullTexture.loadFromFile("assets/gui/inventory_full.png")) {
		puts("[Player] failed loading 'assets/gui/inventory_full.png'!");
	}

	Player::inventoryFullSprite.setTexture(Player::inventoryFullTexture);
	Player::inventoryFullSprite.setOrigin(Player::inventoryFullTexture.getSize().x / 2.0, Player::inventoryFullTexture.getSize().y / 2.0);
	Player::inventoryFullSprite.setPosition(screenSize.x / 2.0, screenSize.y / 2.0);
	
	Vec2 gridStart(4, 124);
	Vec2 hotbarStart(4, 264);

	Vec2 gridStepX(40, 0);
	Vec2 gridStepY(0, 40);
	for (size_t x = 0; x < 10; ++x) {
		Vec2 gridCellPos = hotbarStart + (gridStepX * x);
		Player::inventoryGui.addCell(gridCellPos);
	}
	for (size_t y = 0; y < 3; ++y) {
		for (size_t x = 0; x < 10; ++x) {
			Vec2 gridCellPos = gridStart + (gridStepX * x) + (gridStepY * y);
			Player::inventoryGui.addCell(gridCellPos);
		}
	}
	
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

	Player::key_up = sf::Keyboard::W;
	Player::key_down = sf::Keyboard::S;
	Player::key_left = sf::Keyboard::A;
	Player::key_right = sf::Keyboard::D;
	Player::key_place_background = sf::Keyboard::Y;
	Player::key_place_foreground = sf::Keyboard::X;
	Player::key_inventory = sf::Keyboard::Tab;
	
}

void Player::handleInput(bool *isKeyClicked) {
	if (sf::Keyboard::isKeyPressed(Player::key_up)) {
		Player::jump(1.1);
	}
	if (sf::Keyboard::isKeyPressed(Player::key_left)) {
		Player::walk(Entity::WalkState::LEFT);
	}
	if (sf::Keyboard::isKeyPressed(Player::key_right)) {
		Player::walk(Entity::WalkState::RIGHT);
	}
	
	if ((!sf::Keyboard::isKeyPressed(Player::key_left) && !sf::Keyboard::isKeyPressed(Player::key_right)) || (sf::Keyboard::isKeyPressed(Player::key_left) && sf::Keyboard::isKeyPressed(Player::key_right))) {
		Player::walkstate = Entity::WalkState::STANDING;
	}

	if (sf::Keyboard::isKeyPressed(Player::key_place_background)) {
		Player::setPlaceMode(Player::PLACE_BACKGROUND);
	} else if (sf::Keyboard::isKeyPressed(Player::key_place_foreground)) {
		Player::setPlaceMode(Player::PLACE_FOREGROUND);
	}

	if (isKeyClicked[Player::key_inventory]) {
		Player::showInventory = !Player::showInventory;
	}

	for (int i = 0; i < 10; ++i) {
		if (sf::Keyboard::isKeyPressed(
			/* Don't make me explain this, it works. No idea why (i - 5) works, but it does. */
			static_cast<sf::Keyboard::Key>((static_cast<int>(sf::Keyboard::Num0) + (i - 5)) % 10 + static_cast<int>(sf::Keyboard::Num0))
		)) {
			Player::selectItem(i);
		}
	}
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
	Player::armb->flipx = Player::walkdir;
	Player::armf->flipx = Player::walkdir;
	Player::legb->flipx = Player::walkdir;
	Player::legf->flipx = Player::walkdir;
	Player::body.flipx = Player::walkdir;
}

void Player::render(sf::RenderWindow& window, Vec2 off) {
	Player::body.renderAll(window, Player::limb_z_index, Player::pos + off);
}

void Player::render(sf::RenderWindow& window, sf::Shader& shader, Vec2 off) {
	Player::body.renderAll(window, Player::limb_z_index, shader, Player::pos + off);
}

/* Inventory Management */

void Player::renderInventory(sf::RenderWindow &window, Vec2 off) {
	/* Render full inventory */
	if (Player::showInventory) {
		const float previousScale = Player::textFont.getScale();

		Player::textFont.setScale(2.0);
		window.draw(Player::inventoryFullSprite);
		sf::Vector2f invPosLeftCorner = Player::inventoryFullSprite.getPosition() - Player::inventoryFullSprite.getOrigin();
		Vec2 invTextPos(invPosLeftCorner.x + 3.0, invPosLeftCorner.y + 4.0);

		Player::textFont.write(window, invTextPos + Vec2(1.0, 1.0), L"Crafting", sf::Color(167, 167, 167));
		Player::textFont.write(window, invTextPos, L"Crafting", sf::Color(67, 67, 67));
		
		invTextPos.y += 103;
		Player::textFont.write(window, invTextPos + Vec2(1.0, 1.0), L"Inventory", sf::Color(167, 167, 167));
		Player::textFont.write(window, invTextPos, L"Inventory", sf::Color(67, 67, 67));
		
		Player::textFont.setScale(previousScale);

		inventoryGui.render(window,
		Vec2(
			Player::inventoryFullSprite.getPosition().x - Player::inventoryFullSprite.getOrigin().x,
			Player::inventoryFullSprite.getPosition().y - Player::inventoryFullSprite.getOrigin().y
		) + off,
		Player::textFont);
	

	}

	/* Render inventory hotbar */
	if (!Player::showInventory) {
		window.draw(Player::inventoryHotbarSprite);

		for (size_t i = 0; i < MIN(10, Player::inventoryGui.getItems().getSize()); ++i) {
			Block invblock(Player::inventoryGui.getItems().at(i).get());
			if (invblock.id < 0)
				continue;
			
			Vec2 invblockPos;
			float yoff = 0;
			if (i == Player::currentItemSelected) {
				float elapsed = Player::timeAlive.getElapsedTime().asSeconds();
				yoff = -fabs(sin(elapsed * 4.0) * 7.0);
			}

			invblockPos = Vec2(Player::inventoryHotbarSprite.getPosition().x + 4.0, Player::inventoryHotbarSprite.getPosition().y + 4.0) + Vec2(i * 40, yoff);
		

			Player::inventoryGui.getItems().at(i).render(window, Player::textFont, invblockPos);
		}
	}

}

blockid Player::getItem() {
	return Player::inventoryGui.getItems().at(Player::currentItemSelected).get();
}

blockid Player::takeItem() {
	return Player::inventoryGui.getItems().at(Player::currentItemSelected).take();
}

bool Player::canCollect(blockid type) {
	return Player::inventoryGui.getItems().hasSpaceFor(type);
}

size_t Player::collectItems(blockid type, size_t count) {
	Player::inventoryGui.getItems().add(InventoryStack(count, type));
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

	for (size_t i = 0; i < Player::inventoryGui.getSize(); ++i) {
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
	
	for (size_t i = 0; i < Player::inventoryGui.getItems().getSize(); ++i) {
		fwrite(&(Player::inventoryGui.getItems().at(i).count), sizeof(size_t), 1, fp);
		const int blockId = Player::inventoryGui.getItems().at(i).get();

		fwrite(&(blockId), sizeof(blockid), 1, fp);
		
	}

	fclose(fp);
}