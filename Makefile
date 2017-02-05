CC=g++ -std=c++14
CFLAGS=-c -Wall -pedantic
LDFLAGS=-lm -lsfml-graphics -lsfml-window -lsfml-system
SOURCES=main.cpp Block.cpp DayCycle.cpp Entity.cpp Grid.cpp Limb.cpp Particle.cpp ParticleConfig.cpp Player.cpp rect.cpp rgb.cpp RNG.cpp Util.cpp Vec2.cpp LightSystem.cpp Item.cpp Random.cpp Inventory.cpp BitmapFont.cpp BitmapText.cpp Clouds.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o