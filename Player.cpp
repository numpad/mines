#include "Player.hpp"

Player::Player() : Entity("assets/player/skin.png") {
	/* define render order */
	Player::limb_z_index = std::vector<size_t>();
	Player::limb_z_index.push_back( 1);
	Player::limb_z_index.push_back( 4);
	Player::limb_z_index.push_back( 3);
	Player::limb_z_index.push_back(-1);
	Player::limb_z_index.push_back( 0);
	Player::limb_z_index.push_back( 2);

	/* Create Limbs and attach to body */
	Player::addLimb(Limb(Player::skin, sf::IntRect( 5, 29,  8, 24), Vec2(0.5, 0.5), Vec2( 0,  -2)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect( 5,  5, 22, 22), Vec2(0.5, 0.9), Vec2( 0,  -6)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect(33, 29,  8, 24), Vec2(0.5, 0.1), Vec2( 0,  -4)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect(19, 29,  8, 24), Vec2(0.5, 0.1), Vec2( 0,  -4)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect(33,  3,  8, 24), Vec2(0.5, 0.1), Vec2( 0,  6)));
	Player::addChildLimb(Limb(Player::skin, sf::IntRect(46,  3,  8, 24), Vec2(0.5, 0.1), Vec2( 0,  6)));
	
	/* Keep pointers to provide easy access  */
	Player::head = &Player::body.getChild(0);
	Player::armb = &Player::body.getChild(1);
	Player::armf = &Player::body.getChild(2);
	Player::legf = &Player::body.getChild(3);
	Player::legb = &Player::body.getChild(4);

	Player::acc.x = 0.75;
	Player::maxvel = Vec2(5.0, 7.75);
}

void Player::update(Grid& grid) {
	Player::body.update();
	Player::legf->getAngle() =  sin(Player::timeAlive.getElapsedTime().asMilliseconds() / 120.0) * 30.0;
	Player::legb->getAngle() = -sin(Player::timeAlive.getElapsedTime().asMilliseconds() / 120.0) * 30.0;
	Player::armf->getAngle() =  cos(Player::timeAlive.getElapsedTime().asMilliseconds() / 120.0) * 20.0;
	Player::armb->getAngle() = -cos(Player::timeAlive.getElapsedTime().asMilliseconds() / 120.0) * 20.0;
	

}

void Player::render(sf::RenderWindow& window, Vec2 off) {
	Player::body.renderAll(window, Player::limb_z_index, off);
}

void Player::render(sf::RenderWindow& window, sf::Shader& shader, Vec2 off) {
	Player::body.renderAll(window, Player::limb_z_index, shader, off);
}
