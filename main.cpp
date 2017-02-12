#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

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
#include "Input.hpp"

void handle_events(sf::RenderWindow& window, Vec2 &screenSize) {
	Input::clearKeysClicked();
	Input::clearMouseClicked();
	
	sf::Event event;
	while (window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				Input::updateKeysClicked(event.key.code, true);
				break;
			case sf::Event::MouseButtonPressed:
				Input::updateMouseClicked(event.mouseButton.button, true);
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

void updateItems(Player &player, Grid &grid, std::vector<Item> &items) {
	for (size_t i = 0; i < items.size(); ++i) {
		Item &item = items.at(i);
		item.update(grid);

		/* Check if the player can collect the item */
		if (player.canCollectItem(item)) {
			/* Collect item and remove it */
			Vec2 dist = (item.pos - player.pos);
			item.pos -= dist * 0.2;

			if (dist.length() < 20.0) {
				player.collectItems(item.getType());
				items.erase(items.begin() + i);
			}
		}
	}
}

int run_server(const int port) {
	printf("[Log] Starting server on port %d...\n", port);

	sf::TcpListener listener;
	if (listener.listen(port) != sf::Socket::Done) {
		printf("[Error] Failed listening on port :%d\n", port);
	}

	sf::TcpSocket client;
	if (listener.accept(client) != sf::Socket::Done) {
		printf("[Error] Failed accepting client!\n");
	}

	puts("[Log] Server stopped!");
	return 0;
}

int run_connect(sf::IpAddress &address, int port) {
	printf("[Log] Connecting to %s:%d\n", address.toString().c_str(), port);

	sf::TcpSocket socket;
	if (socket.connect(address, port) != sf::Socket::Done) {
		printf("[Error] Failed connecting to server!\n");
		return 1;
	}

	

	puts("[Log] Connect stopped!");
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc >= 2) {
		int port = 8123;
		if (!strcmp(argv[1], "--server")) {
			if (argc >= 3 && argv[2][0] == ':')
				port = atoi(argv[2] + 1);
			
			return run_server(port);
		} else if (!strcmp(argv[1], "--connect")) {
			sf::IpAddress serverAddress = sf::IpAddress::getLocalAddress();
			if (argc >= 3)
				serverAddress = sf::IpAddress(argv[2]);
			
			if (argc >= 4 && argv[3][0] == ':')
				port = atoi(argv[3] + 1);

			return run_connect(serverAddress, port);
		}
	}



	Vec2 screenSize = Vec2(800, 760); // 800 x 740
	sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "Mines!", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	/* Day/Night cycle */
	DayCycle daycycle(1000, RGB(61, 159, 203));
	if (!daycycle.load("saves/world0/daycycle.sav")) {
		puts("DayCycle not loaded!");
	}
	daycycle.generateNightsky(screenSize);

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
	BitmapText earlyAlphaText(defaultfont);
	earlyAlphaText.write(L"Early Alpha");

	while (window.isOpen()) {
		handle_events(window, screenSize);
		Vec2 mouse(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !player.showInventory) {
			Block& current = grid.atPoint(mouse.x, mouse.y, player.getPlaceMode());
			
			if (!current.collides()) {
				Block blockToPlace(player.takeItem());
				if (blockToPlace.id != BLOCK_AIR) {
					player.animateArm(((player.pos + grid.offset) - mouse).angle() - 90.0, 6.0);
					current = Block(blockToPlace);
				}
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !player.showInventory) {
			int bx, by;
			grid.point_to_grid(mouse.x, mouse.y, bx, by);
			Block& current = grid.at(bx, by, player.getPlaceMode());

			if (current.id >= 0) {
				current.applyDamage();
				player.animateArm(((player.pos + grid.offset) - mouse).angle() - 90.0, 10.0);
				
				if (current.damage > current.maxDamage()) {
					/* Jump in random direction */
					Random velx(-2.0, 2.0);
					Random vely(1.75, 3.75);
					Vec2 vel(velx(), -vely());

					Item destroyedBlockItem(Vec2(bx * 32 + 16, by * 32 + 16), current.id, vel);
					destroyedBlockItem.setCollectTimeout(0.0);

					items.push_back(destroyedBlockItem);
					current = Block(BLOCK_AIR);
				}
			}
		}

		player.handleInput();

		if (Input::isKeyClicked(sf::Keyboard::Q)) {
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

		updateItems(player, grid, items);

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
		player.renderInventory(window, items);

		earlyAlphaText.drawTo(window, Vec2(10, 10));
		
		window.display();
	}

	grid.save("saves/world0/world.sav");
	player.save("saves/world0/player.sav");
	daycycle.save("saves/world0/daycycle.sav");

	return 0;

}