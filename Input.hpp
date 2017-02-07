#ifndef INPUT_HPP
#define INPUT_HPP

#include <SFML/Graphics.hpp>

class Input {
	static bool *isKeyClickedStates;
	static bool *isMouseClickedStates;
	
public:
	static void clearKeysClicked();
	static void updateKeysClicked(sf::Keyboard::Key, bool state = true);
	static bool isKeyClicked(sf::Keyboard::Key);

	static void clearMouseClicked();
	static void updateMouseClicked(sf::Mouse::Button, bool state = true);
	static bool isMouseClicked(sf::Mouse::Button);

	~Input() {
		puts("destroy Input");
	}
};

#endif