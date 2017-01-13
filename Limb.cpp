#include "Limb.hpp"

Limb::Limb() {
	Limb::children = std::vector<Limb>();

	Limb::size = Vec2(0, 0);
	Limb::pos = Vec2(0, 0);
	Limb::offset = Vec2(0, 0);

	Limb::angle = 0;
	Limb::flipx = 1;
	Limb::flipy = 1;

	//Limb::sprite.setOrigin(Limb::size.x * Limb::anchor.x, Limb::size.y * Limb::anchor.y);
	Limb::setAnchor(Vec2(0, 0));
}

Limb::Limb(sf::Texture& texture, sf::IntRect rect, Vec2 anchor, Vec2 offset, Vec2 pos) {
	Limb::children = std::vector<Limb>();

	Limb::sprite = sf::Sprite(texture, rect);

	Limb::size = Vec2(rect.width, rect.height);
	Limb::pos = offset;
	Limb::offset = offset;

	Limb::angle = 0;
	Limb::flipx = 1;
	Limb::flipy = 1;

	//Limb::sprite.setOrigin(Limb::size.x * Limb::anchor.x, Limb::size.y * Limb::anchor.y);
	Limb::setAnchor(anchor);
}

Limb::~Limb() {

}

void Limb::addChild(Limb child) {
	Limb::children.push_back(child);
}

float& Limb::getAngle() {
	return Limb::angle;
}

Vec2 Limb::getPos() {
	return Limb::pos;
}

Vec2 Limb::getAnchor() {
	return Limb::anchor;
}

Vec2 Limb::getSize() {
	return Limb::size;
}

void Limb::setPos(Vec2 pos) {
	Limb::pos = pos;
	for (size_t i = 0; i < Limb::children.size(); ++i) {
		Limb::children.at(i).pos = Limb::pos + Limb::children.at(i).offset;
	}
}

void Limb::addVel(Vec2 vel) {
	Limb::setPos(Limb::getPos() + vel);
}

void Limb::setAnchor(Vec2 anchor) {
	Limb::anchor = anchor;
	Limb::sprite.setOrigin(Limb::size.x * anchor.x, Limb::size.y * anchor.y);
}

void Limb::setSize(Vec2 size) {
	Limb::size = size;
}

std::vector<Limb>& Limb::getChildren() {
	return Limb::children;
}

void Limb::update() {
	Limb::sprite.setRotation(Limb::angle);

	for (size_t i = 0; i < Limb::children.size(); ++i) {
		Limb& child = Limb::children.at(i);
		
		child.update();
	}
}

void Limb::render(sf::RenderWindow& window, Vec2 off) {
	Limb::sprite.setPosition(Limb::pos.x + Limb::offset.x + off.x, Limb::pos.y + Limb::offset.y + off.y);
	window.draw(Limb::sprite);
}

void Limb::render(sf::RenderWindow& window, sf::Shader& shader, Vec2 off) {
	Limb::sprite.setPosition(Limb::pos.x + Limb::offset.x + off.x, Limb::pos.y + Limb::offset.y + off.y);
	window.draw(Limb::sprite, &shader);
}

void Limb::renderChild(size_t idx, sf::RenderWindow& window, Vec2 off) {
	Limb& child = Limb::children.at(idx);

	child.sprite.setPosition(child.pos.x + child.offset.x + off.x, child.pos.y + child.offset.y + off.y);
	window.draw(child.sprite);
}

void Limb::renderChild(size_t idx, sf::RenderWindow& window, sf::Shader& shader, Vec2 off) {
	Limb& child = Limb::children.at(idx);

	child.sprite.setPosition(child.pos.x + child.offset.x + off.x, child.pos.y + child.offset.y + off.y);
	window.draw(child.sprite, &shader);
}

void Limb::renderAll(sf::RenderWindow& window, std::vector<size_t> childidx, Vec2 offset) {
	for (size_t i = 0; i < childidx.size(); ++i) {
		const size_t child_at = childidx.at(i);

		if (child_at == (size_t)(-1))
			Limb::render(window, offset);
		else
			Limb::renderChild(child_at, window, offset);
	}
}

void Limb::renderAll(sf::RenderWindow& window, std::vector<size_t> childidx, sf::Shader& shader, Vec2 offset) {
	for (size_t i = 0; i < childidx.size(); ++i) {
		const size_t child_at = childidx.at(i);

		if (child_at == (size_t)(-1))
			Limb::render(window, shader, offset);
		else
			Limb::renderChild(child_at, window, shader, offset);
	}
}
Limb& Limb::getChild(size_t index) {
	return Limb::children.at(index);
}