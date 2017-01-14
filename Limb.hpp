#ifndef LIMB_HPP
#define LIMB_HPP

#include "Vec2.hpp"
#include "Grid.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Limb {
	Vec2 anchor, size;
	float angle;
	bool dead;

	sf::Sprite sprite;

	std::vector<Limb> children;
public:
	int flipx, flipy;
	Vec2 pos;
	Vec2 offset;
	Limb();
	Limb(sf::Texture& texture, sf::IntRect rect, Vec2 anchor, Vec2 offset, Vec2 pos = Vec2(0, 0));
	~Limb();

	float& getAngle();
	void addVel(Vec2 vel);
	Vec2 getPos();
	Vec2 getAnchor();
	Vec2 getSize();
	void setPos(Vec2 pos);
	void setAnchor(Vec2 anchor);
	void setSize(Vec2 size);

	void addChild(Limb limb);

	std::vector<Limb>& getChildren();

	void update();
	void render(sf::RenderWindow& window, Vec2 off = Vec2(0.0f, 0.0f));
	void render(sf::RenderWindow& window, sf::Shader& shader, Vec2 off = Vec2(0.0f, 0.0f));
	
	void renderChild(size_t idx, sf::RenderWindow& window, Vec2 off = Vec2(0.0f, 0.0f));
	void renderChild(size_t idx, sf::RenderWindow& window, sf::Shader& shader, Vec2 off = Vec2(0.0f, 0.0f));
	
	void renderAll(sf::RenderWindow& window, std::vector<size_t> childidx, Vec2 off = Vec2(0.0f, 0.0f));
	void renderAll(sf::RenderWindow& window, std::vector<size_t> childidx, sf::Shader& shader, Vec2 off = Vec2(0.0f, 0.0f));

	Limb& getChild(size_t index);
};

#endif