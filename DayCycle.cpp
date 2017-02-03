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

bool DayCycle::is_night() {
	return DayCycle::get_darkness() < 0.4;
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

	/* Update night sky */
	DayCycle::nightSkySprite.rotate(0.0035);
	if (DayCycle::sunrise) {
		if (DayCycle::nightSkyAlpha > 0.0 && !DayCycle::is_night()) {
			DayCycle::nightSkyAlpha *= 0.995;
		}
	} else {
		if (DayCycle::is_night()) {
			if (DayCycle::nightSkyAlpha < 255) {
				DayCycle::nightSkyAlpha += 0.75;
			} else if (DayCycle::nightSkyAlpha > 255) {
				DayCycle::nightSkyAlpha = 255;
			}
		}
	}
}

void DayCycle::render(sf::RenderWindow& window, Grid &grid) {
	DayCycle::update();
	window.clear(DayCycle::get_color());

	if (DayCycle::nightSkyAlpha > 0.0) {
		DayCycle::nightSkySprite.setColor(sf::Color(255, 255, 255, DayCycle::nightSkyAlpha));
		window.draw(DayCycle::nightSkySprite);
	}
}

void DayCycle::generateNightsky(Vec2 screenSize) {
	/* Skip if nightSky was already generated for this day */
	if (DayCycle::nightSkyId == DayCycle::days)
		return;
	
	DayCycle::nightSkyId = DayCycle::days;
	
	if (!DayCycle::nightSky.create(screenSize.x + 700, screenSize.x + 700)) {
		puts("[DayCycle] failed creating nightsky texture");
		return;
	}

	sf::RectangleShape star;

	float y = 0;
	Random randomStep(10, 70);
	Random randomOffset(-20, 20);
	Random randomStarColor(240, 255);
	Random randomStarAlpha(100, 255);
	Random randomSize(1.25, 3.5);
	Random randomAngle(0.0, 359.0);

	DayCycle::nightSky.clear(sf::Color::Transparent);
	while (y < DayCycle::nightSky.getSize().y) {
		y += randomStep();
		float x = 0;
		while (x < DayCycle::nightSky.getSize().x) {
			x += randomStep();

			const int color = randomStarColor();
			const float size = randomSize();
			const float angle = randomAngle();

			star.setFillColor(sf::Color(color, color, color, randomStarAlpha()));
			star.setSize(sf::Vector2f(size, size));
			star.setOrigin(size / 2.0, size / 2.0);
			star.setPosition(x + randomOffset(), y + randomOffset());
			star.setRotation(angle);

			DayCycle::nightSky.draw(star);
		}
	}

	/* Update sprite */
	DayCycle::nightSky.setSmooth(true);
	DayCycle::nightSkySprite.setTexture(DayCycle::nightSky.getTexture());
	//DayCycle::nightSkySprite.setTextureRect(sf::IntRect(0, 0, screenSize.x, screenSize.y));
	DayCycle::nightSkySprite.setOrigin(DayCycle::nightSky.getSize().x / 2, DayCycle::nightSky.getSize().y / 2);
	DayCycle::nightSkySprite.setPosition(screenSize.x / 2.0, screenSize.y);
	DayCycle::nightSkySprite.setRotation(0.0);

	DayCycle::nightSkyAlpha = 255;
}

void DayCycle::save(const char *fn) {
	FILE *fp = fopen(fn, "w+");
	if (!fp) {
		printf("[DayCycle] Could not save to \"%s\"!\n", fn);
		return;
	}

	fwrite(&(DayCycle::daytime), sizeof(unsigned int), 1, fp);
	fwrite(&(DayCycle::daylength), sizeof(unsigned int), 1, fp);
	fwrite(&(DayCycle::sunrise), sizeof(bool), 1, fp);
	fwrite(&(DayCycle::days), sizeof(unsigned int), 1, fp);
	fwrite(&(DayCycle::color.r), sizeof(unsigned char), 1, fp);
	fwrite(&(DayCycle::color.g), sizeof(unsigned char), 1, fp);
	fwrite(&(DayCycle::color.b), sizeof(unsigned char), 1, fp);
	fwrite(&(DayCycle::nightSkyAlpha), sizeof(float), 1, fp);
	
	

	fclose(fp);
}

bool DayCycle::load(const char *fn) {
	FILE *fp = fopen(fn, "r");
	if (!fp) {
		return false;
	}

	fread(&(DayCycle::daytime), sizeof(unsigned int), 1, fp);
	fread(&(DayCycle::daylength), sizeof(unsigned int), 1, fp);
	fread(&(DayCycle::sunrise), sizeof(bool), 1, fp);
	fread(&(DayCycle::days), sizeof(unsigned int), 1, fp);
	fread(&(DayCycle::color.r), sizeof(unsigned char), 1, fp);
	fread(&(DayCycle::color.g), sizeof(unsigned char), 1, fp);
	fread(&(DayCycle::color.b), sizeof(unsigned char), 1, fp);
	fread(&(DayCycle::nightSkyAlpha), sizeof(float), 1, fp);

	fclose(fp);
	return true;
}