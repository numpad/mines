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

	sf::Texture noise;
	if (!noise.loadFromFile("assets/shaders/noise.png")) {
		puts("perlin noise not found");
	}
		
	sf::RenderTexture grid_framebuffer;
	if (!grid_framebuffer.create(window.getSize().x, window.getSize().y)) {
		puts("failed to create grid framebuffer");
	}
	grid_framebuffer.clear(sf::Color(0, 0, 0, 0));

	/* Day/Night cycle */
	DayCycle daycycle(500, RGB(61, 159, 203));

	/* World generation */
	Grid grid(grid_framebuffer, 120, 50, "assets/tileset.png");
	grid.generate();
	
	/* Outline Shader */
	sf::Shader outlineShader;
	if (!outlineShader.loadFromFile("assets/shaders/outline.frag", sf::Shader::Fragment)) {
		puts("failed to load shader!");
	}
	outlineShader.setUniform("texture", sf::Shader::CurrentTexture);
	outlineShader.setUniform("step", 1.0f / 75.0f);

	/* Fragment Shader */
	sf::Shader wateryShader;
	if (!wateryShader.loadFromFile("assets/shaders/watery.frag", sf::Shader::Fragment)) {
		puts("failed to load shader #2!");
	}
	wateryShader.setUniform("texture", sf::Shader::CurrentTexture);
	wateryShader.setUniform("time", 0.0f);

	Player *player = new Player;
	player->setPos(Vec2(400.0, 400.0));

	sf::Clock clock;
	while (window.isOpen()) {
		handle_events(window);
		Vec2 mouse(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			Block b(BLOCK_ICE);
			Block& current = grid.atPoint(mouse.x, mouse.y);
			
			if (!current.collides()) {
				current = b;
			}
		}
		
		float elapsed = clock.getElapsedTime().asMilliseconds() / 120.0;
		wateryShader.setUniform("time", elapsed);
	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			grid.moveCamera(-5.0,  0.0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			grid.moveCamera( 5.0,  0.0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			grid.moveCamera( 0.0, -5.0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			grid.moveCamera( 0.0,  5.0);
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			player->pos = mouse - grid.offset;
			player->vel.y = 0.0;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			player->jump();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			player->walk(Entity::WalkState::LEFT);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player->walk(Entity::WalkState::RIGHT);
		}
		
		if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))) {
			player->walkstate = Entity::WalkState::STANDING;
		}

		//player->head->getAngle() = mouse.angle(player->head->pos + player->pos + player->head->offset);
		player->update(grid);
		grid.offset = screenSize / 2.0 - player->pos + Vec2(0.0, 50.0);

		/* Rendering: */
		daycycle.render(window, grid);
		grid.window->clear(sf::Color(0, 0, 0, 0));

		grid.render();
		grid.window->display();
		sf::Sprite grid_fb_sprite(grid.window->getTexture());
		window.draw(grid_fb_sprite);
		
		//outlineShader.setUniform("step", 1.0f / 75.0f);
		player->render(window, outlineShader, grid.offset);

		window.display();
	}

	return 0;

}