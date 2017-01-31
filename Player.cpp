#include "Player.hpp"

Player::Player() : Entity("assets/player/skin.png"), inventory(10) {
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
	Player::inventory.add(InventoryStack( 6, BLOCK_TNT));
	Player::inventory.add(InventoryStack(24, BLOCK_GRASS));
	Player::inventory.add(InventoryStack(35, BLOCK_WOOD));
	Player::inventory.add(InventoryStack( 2, BLOCK_OVEN));
	Player::inventory.add(InventoryStack( 1, BLOCK_WORKBENCH));
	Player::inventory.add(InventoryStack( 8, BLOCK_TREE));
	Player::inventory.add(InventoryStack(12, BLOCK_LEAVES));
	Player::inventory.add(InventoryStack( 8, BLOCK_GLASS));
	Player::inventory.add(InventoryStack(30, BLOCK_COBBLESTONE));
	
	
	/* Create Limbs and attach to body */
	Player::addLimb(     Limb(Entity::skin, sf::IntRect( 5, 29,  8, 24), Vec2(0.5, 0.5), Vec2( 0,  0)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect( 5,  5, 22, 22), Vec2(0.5, 0.9), Vec2( 0, -6)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect(33, 29,  8, 24), Vec2(0.5, 0.1), Vec2( 0, -4)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect(19, 29,  8, 24), Vec2(0.5, 0.1), Vec2( 0, -4)));
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
		Player::legAnimation += (Player::walkdir == Entity::WalkState::LEFT) ? -1.0 : 1.0;

		Player::legf->getAngle() =  sin(Player::legAnimation / 6.0) * 30.0;
		Player::legb->getAngle() = -Player::legf->getAngle();
		Player::armf->getAngle() =  cos(Player::legAnimation / 7.0) * 20.0;
		Player::armb->getAngle() = -Player::armf->getAngle();
	} else if (Player::walkstate == Entity::WalkState::STANDING) {
		if (fabs(Player::vel.x) > 0.0) {
			Player::legAnimation += (Player::walkdir == Entity::WalkState::LEFT) ? -1.5 : 1.5;
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
}

void Player::render(sf::RenderWindow& window, Vec2 off) {
	Player::body.renderAll(window, Player::limb_z_index, Player::pos + off);
}

void Player::render(sf::RenderWindow& window, sf::Shader& shader, Vec2 off) {
	Player::body.renderAll(window, Player::limb_z_index, shader, Player::pos + off);
}

/* Inventory Management */

void Player::renderInventory(sf::RenderWindow &window, Vec2 off) {
	for (size_t i = 0; i < Player::inventory.getSize(); ++i) {
		Block invblock(Player::inventory.at(i).get());
		
		float yoff = 0;
		if (i == Player::currentItemSelected) {
			float elapsed = Player::timeAlive.getElapsedTime().asSeconds();
			yoff = -fabs(sin(elapsed * 4.0) * 7.0);
		}
		
		invblock.render(window, off + Vec2(i * 40, yoff));
	}
}

blockid Player::getItem() {
	return Player::inventory.at(Player::currentItemSelected).get();
}

blockid Player::takeItem() {
	return Player::inventory.at(Player::currentItemSelected).take();
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