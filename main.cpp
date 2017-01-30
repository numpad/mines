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
	//Grid grid(grid_framebuffer, "world0.sav", "assets/tileset.png");

	/* Outline Shader */
	sf::Shader druggedShader;
	if (!druggedShader.loadFromFile("assets/shaders/drugged.frag", sf::Shader::Fragment)) {
		puts("failed to load shader!");
	}
	druggedShader.setUniform("texture", sf::Shader::CurrentTexture);
	druggedShader.setUniform("time", 0.0f);
	druggedShader.setUniform("resolution", sf::Vector2f(screenSize.x, screenSize.y));
	

	/* Fragment Shader */
	sf::Shader backgroundShader;
	if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
		puts("failed to load shader #2!");
	}
	backgroundShader.setUniform("texture", sf::Shader::CurrentTexture);
	
	/* Inventory Hotbar */
	sf::Texture invHotbarTex;
	if (!invHotbarTex.loadFromFile("assets/inventory.png")) {
		puts("failed to load invhotbartex!");
	}

	sf::Sprite invHotbar;
	invHotbar.setTexture(invHotbarTex);
	invHotbar.setPosition(screenSize.x / 2.0 - (float)invHotbarTex.getSize().x / 2.0, screenSize.y - (float)invHotbarTex.getSize().y * 1.25);

	Player player;
	player.setPos(Vec2(400.0, 400.0));

	sf::Clock clock;
	int current_block_id = 0;
	while (window.isOpen()) {
		handle_events(window);
		Vec2 mouse(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		float elapsed = clock.getElapsedTime().asMilliseconds() / 120.0;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			Block b(current_block_id);
			Block& current = grid.atPoint(mouse.x, mouse.y);
			
			if (!current.collides()) {
				current = b;
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			Block& current = grid.atPoint(mouse.x, mouse.y);

			current.damage += 1;
			if (current.damage > current.maxDamage()) {
				current = Block(-1);
			}
		}
		
		druggedShader.setUniform("time", elapsed);

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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			player.vel.y -= 0.65;
			if (player.vel.y < -5.0)
				player.vel.y = -5.0;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			current_block_id += 1;
			printf("Block: %d\n", current_block_id);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			current_block_id -= 1;
			printf("Block: %d\n", current_block_id);
		}

		//player.head->getAngle() = mouse.angle(player.head->pos + player.pos + player.head->offset);
		player.update(grid);
		grid.offset = screenSize / 2.0 - player.pos + Vec2(0.0, 50.0);

		/* Rendering: */
		daycycle.render(window, grid);

		//druggedShader.setUniform("offset", sf::Vector2f(grid.offset.x, grid.offset.y));
		
		grid.render(window, backgroundShader);
		
		player.render(window, grid.offset);
		
		lightsystem.setLight(0, sf::Glsl::Vec3(0.5, 0.5, 2.0 + sin(elapsed * 0.2) * 0.075));
		lightsystem.setGlobalLight(1.0 - daycycle.get_darkness());
		lightsystem.render(window);
		
		/* GUI */

		window.draw(invHotbar);

		window.display();
	}

	//grid.save("world0.sav");

	return 0;

}