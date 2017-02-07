#include "Input.hpp"

bool *Input::isKeyClickedStates = (bool *)calloc(sf::Keyboard::KeyCount, sizeof(sf::Keyboard::Key));
bool *Input::isMouseClickedStates = (bool *)calloc(sf::Mouse::ButtonCount, sizeof(sf::Mouse::Button));

void Input::clearKeysClicked() {
	for (size_t i = 0; i < sf::Keyboard::KeyCount; ++i) {
		Input::isKeyClickedStates[i] = false;
	}
}

void Input::updateKeysClicked(sf::Keyboard::Key key, bool state) {
	Input::isKeyClickedStates[key] = state;
}

bool Input::isKeyClicked(sf::Keyboard::Key key) {
	return Input::isKeyClickedStates[key];
}

void Input::clearMouseClicked() {
	for (size_t i = 0; i < sf::Mouse::ButtonCount; ++i) {
		Input::isMouseClickedStates[i] = false;
	}
}

void Input::updateMouseClicked(sf::Mouse::Button btn, bool state) {
	Input::isMouseClickedStates[btn] = state;
}

bool Input::isMouseClicked(sf::Mouse::Button btn) {
	return Input::isMouseClickedStates[btn];
}
