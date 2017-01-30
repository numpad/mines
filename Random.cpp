#include "Random.hpp"

Random::Random(float min, float max) : gen(rd()), dis(min, max) {
	Random::min = min;
	Random::max = max;
}

Random::Random() : Random(0.0, 1.0) {

}

Random::Random(Random &&random) : gen(rd()), dis(random.min, random.max) {

}

Random& Random::operator=(Random&& random) {
	Random::gen = std::mt19937(Random::rd());
	Random::dis = std::uniform_real_distribution<>(random.min, random.max);
	return *this;
}

void Random::setRange(float min, float max) {
	Random::dis = std::uniform_real_distribution<>(min, max);
	Random::min = min;
	Random::max = max;
}

float Random::operator()() {
	return dis(gen);
}
