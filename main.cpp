#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <SFML/Graphics.hpp>

#include "rgb.hpp"
#include "rect.hpp"
#include "Vec2.hpp"
#include "Block.hpp"
#include "Grid.hpp"
#include "ParticleConfig.hpp"
#include "Particle.hpp"
#include "DayCycle.hpp"
#include "Limb.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Util.hpp"
#include "LightSystem.hpp"

void handle_events(sf::RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
}

int main(int argc, char *argv[]) {
	Vec2 screenSize = Vec2(800.0, 740.0);
	sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "Mines!", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	/* Day/Night cycle */
	DayCycle daycycle(1000, RGB(61, 159, 203));
	LightSystem lightsystem(screenSize.x, screenSize.y);

	/* World generation */
	Grid grid(screenSize, 150, 50, "assets/tileset.png");
	grid.generate();

	/* Fragment Shader */
	sf::Shader backgroundShader;
	if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
		puts("failed to load shader #2!");
	}
	backgroundShader.setUniform("texture", sf::Shader::CurrentTexture);

	Player player(screenSize);
	player.setPos(Vec2(400.0, 400.0));

	sf::Clock clock;
	while (window.isOpen()) {
		handle_events(window);
		Vec2 mouse(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		float elapsed = clock.getElapsedTime().asMilliseconds() / 120.0;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			Block& current = grid.atPoint(mouse.x, mouse.y, player.getPlaceMode());
			
			if (!current.collides()) {
				Block blockToPlace(player.takeItem());
				if (blockToPlace.id != BLOCK_AIR)
					current = Block(blockToPlace);
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			Block& current = grid.atPoint(mouse.x, mouse.y, player.getPlaceMode());

			current.damage += 1;
			if (current.damage > current.maxDamage()) {
				player.collectItems(current.id);
				current = Block(BLOCK_AIR);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			player.jump(1.1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			player.walk(Entity::WalkState::LEFT);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player.walk(Entity::WalkState::RIGHT);
		}
		
		if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))) {
			player.walkstate = Entity::WalkState::STANDING;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
			player.setPlaceMode(Player::PLACE_BACKGROUND);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
			player.setPlaceMode(Player::PLACE_FOREGROUND);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			player.vel.y -= 0.65;
			if (player.vel.y < -5.0)
				player.vel.y = -5.0;
		}

		for (int i = 0; i < 10; ++i) {
			if (sf::Keyboard::isKeyPressed(
				/* Don't make me explain this, it works. No idea why (i - 5) works, but it does. */
				static_cast<sf::Keyboard::Key>((static_cast<int>(sf::Keyboard::Num0) + (i - 5)) % 10 + static_cast<int>(sf::Keyboard::Num0))
			)) {
				player.selectItem(i);
			}
		}

		//player.head->getAngle() = mouse.angle(grid.offset + player.head->pos + player.pos + player.head->offset) - 180.0;
		player.update(grid);
		grid.offset = screenSize / 2.0 - player.pos + Vec2(0.0, 50.0);

		/* Rendering: */
		daycycle.render(window, grid);

		grid.render(window, backgroundShader);
		
		player.render(window, grid.offset);
		
		lightsystem.setLight(0, sf::Glsl::Vec3(0.5, 0.5, 2.0));
		lightsystem.setGlobalLight(1.0 - daycycle.get_darkness());
		lightsystem.render(window);
		
		/* GUI */

		player.renderInventory(window);
		window.display();
	}

	//grid.save("world0.sav");

	return 0;

}