#include "draw.hpp"
#include "net.hpp"

sf::RenderWindow window;
int state = 0;

void draw(sf::RenderWindow& window, sf::Clock& clock) {
	sf::Lock lock(copyMutex);
	float alpha = clock.GetElapsedTime();
	if (alpha > 1.0 || copyTanks.size() < state+2 || copyShells.size() < state+2) {
		if (alpha > 1.0) ++state;
		clock.Reset();
		return;
	}
	if (state+1 == copyTanks.size() || state+1 == copyShells.size()) return;

	window.Clear(sf::Color(255, 255, 255));
	for (int x = -10; x <= 10; ++x)
		window.Draw(sf::Shape::Line(x*100, -1000, x*100, 1000, 6, sf::Color(200, 200, 200)));
	for (int y = -10; y <= 10; ++y)
		window.Draw(sf::Shape::Line(-1000, y*100, 1000, y*100, 6, sf::Color(200, 200, 200)));

	drawTanks(alpha);
	drawShells(alpha);
}

void drawTanks(float alpha) {
	std::vector<Tank> &start = copyTanks[state], &end = copyTanks[state+1];
	for (int i = 0; i < start.size(); ++i) {
		int other = -1;
		for (int j = 0; j < end.size(); ++j)
			if (end[j].id == start[i].id) other = j;
		if (other == -1)
			LOG("Something went horribly, horribly wrong.",);

		int x = start[i].x + alpha*(end[other].x-start[i].x);
		int y = start[i].y + alpha*(end[other].y-start[i].y);
		window.Draw(sf::Shape::Circle(x, y, 50, sf::Color(start[i].r, start[i].g, start[i].b), 5));
	}
}

void drawShells(float alpha) {
	std::vector<Shell> &start = copyShells[state], &end = copyShells[state+1];
	for (int i = 0; i < start.size(); ++i) {
		int other = -1;
		for (int j = 0; j < end.size(); ++j)
			if (end[j].id == start[i].id) other = j;
		if (other == -1)
			continue;

		int x = start[i].sx+(start[i].x-start[i].sx)*(1-(start[i].t-alpha)/(start[i].st));
		int y = start[i].sy+(start[i].y-start[i].sy)*(1-(start[i].t-alpha)/(start[i].st));
		window.Draw(sf::Shape::Circle(x, y, 10, sf::Color(0, 0, 0)));
	}
}