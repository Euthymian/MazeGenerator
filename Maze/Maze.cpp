#include "MazeGenerator.h"

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;

int main()
{
	srand(time(NULL));

	MazeGenerator* mg = new MazeGenerator(GeneratorAlgorithm::Eller);

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Maze");

	while (window.isOpen())
	{
		// Event handle
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		mg->Update();
		mg->Render(&window);
	}

	return 0;
}