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
#include "Item.hpp"
#include "Random.hpp"
#include "BitmapFont.hpp"
#include "BitmapText.hpp"
#include "Clouds.hpp"

void handle_events(sf::RenderWindow& window, Vec2 &screenSize, bool *keypressed) {
	for (size_t i = 0; i < sf::Keyboard::KeyCount; ++i)
		keypressed[i] = false;
	
	sf::Event event;
	while (window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				keypressed[event.key.code] = true;
				break;
			case sf::Event::Resized:
				screenSize.x = event.size.width;
				screenSize.y = event.size.height;
				break;
			default:
				break;
		};
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
}

int main(int argc, char *argv[]) {
	Vec2 screenSize = Vec2(800, 740); // 800 x 740
	sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "Mines!", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	/* Day/Night cycle */
	DayCycle daycycle(1000, RGB(61, 159, 203));
	daycycle.generateNightsky(screenSize);
	if (!daycycle.load("saves/world0/daycycle.sav")) {
		puts("DayCycle not loaded!");
	}

	LightSystem lightsystem(screenSize.x, screenSize.y);

	/* World generation */
	Grid grid(screenSize, 150, 50, "assets/tileset.png");
	if (!grid.load("saves/world0/world.sav")) {
		grid.generate();
	}

	/* Player */
	Player player(screenSize);
	if (!player.load("saves/world0/player.sav")) {
		player.setPos(Vec2(400, 400));
	}

	/* Items */
	std::vector<Item> items = std::vector<Item>();
	
	/* Weather */
	Clouds clouds(screenSize);

	/* Text */
	BitmapFont defaultfont("assets/font/font.png", Vec2(5, 8), 1);

	bool isKeyClicked[sf::Keyboard::KeyCount] = {false};
	sf::Clock clock;
	while (window.isOpen()) {
		handle_events(window, screenSize, isKeyClicked);
		Vec2 mouse(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		//float elapsed = clock.getElapsedTime().asMilliseconds() / 120.0;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			Block& current = grid.atPoint(mouse.x, mouse.y, player.getPlaceMode());
			
			if (!current.collides()) {
				Block blockToPlace(player.takeItem());
				if (blockToPlace.id != BLOCK_AIR)
					current = Block(blockToPlace);
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			int bx, by;
			grid.point_to_grid(mouse.x, mouse.y, bx, by);
			Block& current = grid.at(bx, by, player.getPlaceMode());

			if (current.id >= 0) {
				current.applyDamage();

				if (current.damage > current.maxDamage()) {
					/* Jump in random direction */
					Random velx(-2.0, 2.0);
					Random vely(1.75, 3.75);
					Vec2 vel(velx(), -vely());

					Item destroyedBlockItem(Vec2(bx * 32 + 16, by * 32 + 16), current.id, vel);
					destroyedBlockItem.setCollectTimeout(0.0);

					items.push_back(destroyedBlockItem);
					//player.collectItems(current.id);
					current = Block(BLOCK_AIR);
				}
			}
		}

		player.handleInput();

		if (isKeyClicked[sf::Keyboard::Q]) {
			Random throwVelx(2.5, 3.45);
			Random throwVely(1.5, 4.5);
			blockid thrownItem = player.takeItem();
			if (thrownItem != BLOCK_AIR)
				items.push_back(Item(player.pos, thrownItem, Vec2(throwVelx() * player.walkdir, -throwVely())));
		}

		clouds.update(grid);

		//player.head->getAngle() = mouse.angle(grid.offset + player.head->pos + player.pos + player.head->offset) - 180.0;
		player.update(grid);
		grid.offset = screenSize / 2.0 - player.pos + Vec2(0.0, 50.0);
		
		for (size_t i = 0; i < items.size(); ++i) {
			Item &item = items.at(i);
			item.update(grid);

			/* Check if the player can collect the item */
			if (item.collectableBy(player)) {
				/* Collect item and remove it */
				Vec2 dist = (item.pos - player.pos);
				item.pos -= dist * 0.2;

				if (dist.length() < 20.0) {
					player.collectItems(item.getType());
					items.erase(items.begin() + i);
				}
			}
		}

		/* Rendering: */
		daycycle.render(window, grid);
		clouds.render(window);
		
		grid.render(window);
		player.render(window, grid.offset);
		
		for (size_t i = 0; i < items.size(); ++i) {
			items.at(i).render(window, grid.offset);
		}

		lightsystem.setLight(0, sf::Glsl::Vec3(0.5, 0.5, 2.0));
		lightsystem.setGlobalLight(1.0 - daycycle.get_darkness());
		lightsystem.render(window);

		/* GUI */
		player.renderInventory(window);

		if (daycycle.is_night()) {
			defaultfont.write(window, Vec2(10, 10), L"Nighttime");
		}
		
		window.display();
	}

	grid.save("saves/world0/world.sav");
	player.save("saves/world0/player.sav");
	daycycle.save("saves/world0/daycycle.sav");

	return 0;

}