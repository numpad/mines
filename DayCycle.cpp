#include "DayCycle.hpp"

DayCycle::DayCycle(unsigned int daylength, RGB color) {
	DayCycle::daytime = 0;
	DayCycle::daylength = daylength;
	DayCycle::sunrise = true;
	DayCycle::days = 0;
	DayCycle::color = color;
}

sf::Color DayCycle::get_color() {
	return sf::Color(DayCycle::color.r * DayCycle::get_darkness(),
	                 DayCycle::color.g * DayCycle::get_darkness(),
	                 DayCycle::color.b * DayCycle::get_darkness()
	                 );
}

float DayCycle::get_darkness() {
	return ((float)DayCycle::daytime / (float)DayCycle::daylength);
}

unsigned int DayCycle::get_day() {
	return DayCycle::days;
}

void DayCycle::update() {
	if (DayCycle::sunrise) ++daytime;
	else                   --daytime;

	if (DayCycle::daytime >= DayCycle::daylength && DayCycle::sunrise) {
		DayCycle::days++;
		DayCycle::sunrise = false;
	} else if (DayCycle::daytime <= 0 && !DayCycle::sunrise) {
		DayCycle::days++;
		DayCycle::sunrise = true;
	}
}

void DayCycle::render(sf::RenderWindow& window, Grid &grid) {
	DayCycle::update();
	window.clear(DayCycle::get_color());

}