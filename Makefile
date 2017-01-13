CC=g++
CFLAGS=-c -Wall -pedantic
LDFLAGS=-lm -lsfml-graphics -lsfml-window -lsfml-system
SOURCES=main.cpp Block.cpp DayCycle.cpp Entity.cpp Grid.cpp Limb.cpp Particle.cpp ParticleConfig.cpp Player.cpp rect.cpp rgb.cpp RNG.cpp Util.cpp Vec2.cpp
OBJECTS=$(SOURCES:%.cpp=bin/%.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o bin/$@

.cpp.o:
	$(CC) $(CFLAGS) $< -o bin/$@
