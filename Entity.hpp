#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "rgb.hpp"
#include "rect.hpp"
#include "Vec2.hpp"
#include "Block.hpp"
#include "Grid.hpp"
#include "Limb.hpp"
#include "Util.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Entity {
private:
	void physicsHandleJumping(Grid& grid);
	void physicsHandleFalling(Grid& grid);
	void physicsHandleWalking(Grid& grid);

protected:
	Vec2 feetOffset, headOffset;

public:

	/* Handle collision along y-Axis */
	struct JumpState {
		enum State {
			STANDING, JUMPING, FALLING
		};
	};

	/* Handle collision along x-Axis */
	struct WalkState {
		enum State {
			STANDING, WALKING
		};
		
		enum Direction {
			LEFT = -1, RIGHT = 1
		};
	};

	/* Physics */
	Vec2 pos, vel, lastvel;
	Vec2 acc, maxvel;
	float jumpvel, jumpstrength;
	
	/* Rendering */
	Limb body;
	sf::Texture skin;

	/* Logic */
	JumpState::State jumpstate;
	WalkState::State walkstate;
	WalkState::Direction walkdir;

	Entity(const char *skinpath);
	void addLimb(Limb body);
	void addChildLimb(Limb body);
	void setPos(Vec2 pos);

	void physicsUpdate(Grid& grid);
	
	void jump(float factor = 1.0);
	void walk(WalkState::Direction dir);

	virtual void update(Grid& grid) =0;
	virtual void render(sf::RenderWindow& window, Vec2 offset = Vec2(0.0f, 0.0f)) =0;
	virtual void render(sf::RenderWindow& window, sf::Shader& shader, Vec2 offset = Vec2(0.0f, 0.0f)) =0;
	
};

#endif