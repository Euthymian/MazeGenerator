#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include <iostream>
#include <set>
#include <map>
#include <stack>
#include <random>
#include <SFML/Graphics.hpp>

enum CellProperty
{
	E_PATH = 0x01,
	W_PATH = 0x02,
	N_PATH = 0x04,
	S_PATH = 0x08,
	CELL_VISITING = 0x10,
	CELL_VISITED = 0x20
};

enum GeneratorAlgorithm
{
	BackTracking = 0,
	RandomizedPrim = 1,
	Eller = 2,
};

class MazeGenerator {
protected:
	GeneratorAlgorithm type;

	const float DELAY = 0.02f;

	const int MAZE_WIDTH = 30;
	const int MAZE_HEIGHT = 24;
	const int CELL_SIZE = 15;
	const int WALL_WIDTH = 5;
	const int OFFSET = 5;

	// BackTracking algo 
	std::stack<std::pair<int, int>> stack;
	sf::RectangleShape head;

	// RandomizePrim algo
	std::vector<std::pair<int, int>> set;

	// Eller algo
	int curHeight;
	std::vector<int> row;

	int* maze;
	sf::RectangleShape cellSprite;
	std::vector<sf::RectangleShape> paths;
	sf::Clock clock;
	float timer;

	int coordinateToCell(int x, int y) {
		return (stack.top().first + x) * MAZE_WIDTH + (stack.top().second + y);
	};

	int coordinateToCell(int x, int y, std::pair<int, int> chosen) {
		return (chosen.first + x) * MAZE_WIDTH + (chosen.second + y);
	};

	void BackTrackingUpdate() {
		timer += clock.restart().asSeconds();
		if (!stack.empty()) {
			if (timer >= DELAY) {
				timer -= DELAY;

				std::vector<int> neighbours;

				// East
				if (stack.top().second < (MAZE_WIDTH - 1) && (maze[coordinateToCell(0, 1)] & (CELL_VISITING | CELL_VISITED)) == 0)
					neighbours.push_back(0);
				// West
				if (stack.top().second > 0 && (maze[coordinateToCell(0, -1)] & (CELL_VISITING | CELL_VISITED)) == 0)
					neighbours.push_back(1);
				// North
				if (stack.top().first > 0 && (maze[coordinateToCell(-1, 0)] & (CELL_VISITING | CELL_VISITED)) == 0)
					neighbours.push_back(2);
				// South
				if (stack.top().first < (MAZE_HEIGHT - 1) && (maze[coordinateToCell(1, 0)] & (CELL_VISITING | CELL_VISITED)) == 0)
					neighbours.push_back(3);

				//std::cout << "TOP: " << "{" << stack.top().first << "," << stack.top().second << "} Neighbours: ";
				//for (int i : neighbours) {
				//	std::cout << i << " ";
				//}
				//std::cout << std::endl;

				if (neighbours.empty()) {
					maze[coordinateToCell(0, 0)] |= CELL_VISITED;
					maze[coordinateToCell(0, 0)] &= ~CELL_VISITING;
					stack.pop();
				}
				else {
					int nextCell = neighbours[rand() % neighbours.size()];

					switch (nextCell)
					{
					case 0:
						maze[coordinateToCell(0, 1)] |= CELL_VISITING | W_PATH;
						maze[coordinateToCell(0, 0)] |= E_PATH;
						stack.push({ stack.top().first, stack.top().second + 1 });
						break;
					case 1:
						maze[coordinateToCell(0, -1)] |= CELL_VISITING | E_PATH;
						maze[coordinateToCell(0, 0)] |= W_PATH;
						stack.push({ stack.top().first, stack.top().second - 1 });
						break;
					case 2:
						maze[coordinateToCell(-1, 0)] |= CELL_VISITING | S_PATH;
						maze[coordinateToCell(0, 0)] |= N_PATH;
						stack.push({ stack.top().first - 1, stack.top().second });
						break;
					case 3:
						maze[coordinateToCell(1, 0)] |= CELL_VISITING | N_PATH;
						maze[coordinateToCell(0, 0)] |= S_PATH;
						stack.push({ stack.top().first + 1, stack.top().second });
						break;
					}
				}
			}
		}
	}

	void RandomizedPrimUpdate() {
		timer += clock.restart().asSeconds();
		if (!set.empty()) {
			if (timer >= DELAY) {
				timer -= DELAY;

				int randIndex = rand() % set.size();
				std::pair<int, int> chosen = set[randIndex];

				std::vector<int> neighbours;

				// East
				if (chosen.second < (MAZE_WIDTH - 1) && (maze[coordinateToCell(0, 1, chosen)] & (CELL_VISITING | CELL_VISITED)) == 0)
					neighbours.push_back(0);
				// West
				if (chosen.second > 0 && (maze[coordinateToCell(0, -1, chosen)] & (CELL_VISITING | CELL_VISITED)) == 0)
					neighbours.push_back(1);
				// North
				if (chosen.first > 0 && (maze[coordinateToCell(-1, 0, chosen)] & (CELL_VISITING | CELL_VISITED)) == 0)
					neighbours.push_back(2);
				// South
				if (chosen.first < (MAZE_HEIGHT - 1) && (maze[coordinateToCell(1, 0, chosen)] & (CELL_VISITING | CELL_VISITED)) == 0)
					neighbours.push_back(3);

				//std::cout << "CHOSEN: " << "{" << chosen.first << "," << chosen.second << "} Neighbours: ";
				//for (int i : neighbours) {
				//	std::cout << i << " ";
				//}
				//std::cout << std::endl;

				if (neighbours.empty()) {
					maze[coordinateToCell(0, 0, chosen)] |= CELL_VISITED;
					maze[coordinateToCell(0, 0, chosen)] &= ~CELL_VISITING;
					set.erase(set.begin() + randIndex);
				}
				else {
					int nextCell = neighbours[rand() % neighbours.size()];

					switch (nextCell)
					{
					case 0:
						maze[coordinateToCell(0, 1, chosen)] |= CELL_VISITING | W_PATH;
						maze[coordinateToCell(0, 0, chosen)] |= E_PATH;
						set.push_back({ chosen.first,chosen.second + 1 });
						break;
					case 1:
						maze[coordinateToCell(0, -1, chosen)] |= CELL_VISITING | E_PATH;
						maze[coordinateToCell(0, 0, chosen)] |= W_PATH;
						set.push_back({ chosen.first, chosen.second - 1 });
						break;
					case 2:
						maze[coordinateToCell(-1, 0, chosen)] |= CELL_VISITING | S_PATH;
						maze[coordinateToCell(0, 0, chosen)] |= N_PATH;
						set.push_back({ chosen.first - 1, chosen.second });
						break;
					case 3:
						maze[coordinateToCell(1, 0, chosen)] |= CELL_VISITING | N_PATH;
						maze[coordinateToCell(0, 0, chosen)] |= S_PATH;
						set.push_back({ chosen.first + 1, chosen.second });
						break;
					}
				}
			}
		}
	}

	bool True() {
		return rand() % 2;
	}

	void EllerUpdate() {
		timer += clock.restart().asSeconds();
		if (timer >= DELAY) {
			timer -= DELAY;

			if (curHeight < MAZE_HEIGHT) {
				// Setup map
				std::map<int, std::vector<int>> m;
				for (int i = 0; i < MAZE_WIDTH; i++) {
					m[row[i]].push_back(i);
				}

				// Horizontal connections
				for (int i = 0; i < MAZE_WIDTH - 1; i++) {
					maze[curHeight * MAZE_WIDTH + i] |= CELL_VISITED;
					if (row[i] != row[i + 1] && (True() || curHeight == MAZE_HEIGHT - 1)) {
						//std::cout << "Connect ";
						int tmp = row[i + 1];
						for (int e : m[row[i + 1]]) {
							row[e] = row[i];
							m[row[i]].push_back(e);
							maze[curHeight * MAZE_WIDTH + e] |= CELL_VISITED;
						}
						m[tmp].clear();

						maze[curHeight * MAZE_WIDTH + i] |= E_PATH;
						maze[curHeight * MAZE_WIDTH + i + 1] |= W_PATH;
					}
					//else std::cout << "| ";
				}
				maze[curHeight * MAZE_WIDTH + MAZE_WIDTH - 1] |= CELL_VISITED;

				// Re-setup map
				m.clear();
				for (int i = 0; i < MAZE_WIDTH; i++) {
					m[row[i]].push_back(i);
				}

				// Vertical connections
				if (curHeight < MAZE_HEIGHT - 1) {
					for (auto e : m) {
						int numEleOfEachSet = e.second.size();
						int numOfDown = rand() % numEleOfEachSet + 1;
						std::set<int> s;
						while (s.size() != numOfDown)
							s.insert(rand() % numEleOfEachSet);
						for (int i : s) {
							maze[curHeight * MAZE_WIDTH + e.second[i]] |= S_PATH;
							maze[(curHeight + 1) * MAZE_WIDTH + e.second[i]] |= N_PATH;
						}
					}
				}

				// Copy down cells to next row 
				int currentPopulatedValue = 0;
				std::vector<int> tmp(row.begin(), row.end());
				for (int i = 0; i < MAZE_WIDTH; i++) {
					row[i] = 0;
				}
				for (int i = 0; i < MAZE_WIDTH; i++) {
					bool secondProcess = false;
					if (row[i] == 0) {
						secondProcess = true;
						row[i] = ++currentPopulatedValue;
					}
					if (maze[curHeight * MAZE_WIDTH + i] & S_PATH && secondProcess) {
						for (int e : m[tmp[i]]) {
							if (maze[curHeight * MAZE_WIDTH + e] & S_PATH)
								row[e] = currentPopulatedValue;
						}
					}
				}

				curHeight++;
			}
		}
	}

public:
	MazeGenerator(GeneratorAlgorithm type) 
	{
		this->type = type;

		timer = 0.0f;

		maze = new int[MAZE_WIDTH * MAZE_HEIGHT];
		std::memset(maze, 0x00, MAZE_WIDTH * MAZE_HEIGHT * sizeof(int));

		cellSprite = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
		for (int i = 0; i < 4; i++) {
			sf::RectangleShape tmp;
			if (i < 2)
				tmp.setSize(sf::Vector2f(WALL_WIDTH, CELL_SIZE));
			else
				tmp.setSize(sf::Vector2f(CELL_SIZE, WALL_WIDTH));
			tmp.setFillColor(sf::Color::White);
			paths.push_back(tmp);
		}

		if (this->type == BackTracking) {
			stack.push({ 0,0 });
			maze[0] = CELL_VISITING;
			head = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
			head.setFillColor(sf::Color::Green);
		}
		else if (this->type == RandomizedPrim) {
			set.push_back({ MAZE_HEIGHT / 2, MAZE_WIDTH / 2 });
			maze[set[0].first * MAZE_WIDTH + set[0].second] = CELL_VISITING;
		}
		else if (this->type == Eller) {
			curHeight = 0;
			for (int i = 1; i <= MAZE_WIDTH; i++)
				row.push_back(i);
			row.push_back(-1);
		}
	}

	~MazeGenerator()
	{
		delete[]maze;
	}

	void Update() {
		if (type == BackTracking)
			BackTrackingUpdate();
		else if (type == RandomizedPrim)
			RandomizedPrimUpdate();
		else if (type == Eller)
			EllerUpdate();
	}

	void Render(sf::RenderWindow* window) {
		window->clear(sf::Color::Black);

		for (int i = 0; i < MAZE_HEIGHT; i++) {
			for (int j = 0; j < MAZE_WIDTH; j++) {
				// Draw cell
				if (maze[i * MAZE_WIDTH + j] & CELL_VISITING)
					cellSprite.setFillColor(sf::Color(128, 128, 128));
				else if (maze[i * MAZE_WIDTH + j] & CELL_VISITED)
					cellSprite.setFillColor(sf::Color::White);
				else
					cellSprite.setFillColor(sf::Color::Blue);
				cellSprite.setPosition(j * (CELL_SIZE + WALL_WIDTH) + OFFSET, i * (CELL_SIZE + WALL_WIDTH) + OFFSET);
				window->draw(cellSprite);

				// Draw walls
				if (maze[i * MAZE_WIDTH + j] & E_PATH) {
					paths[0].setPosition(j * (CELL_SIZE + WALL_WIDTH) + CELL_SIZE + OFFSET, i * (CELL_SIZE + WALL_WIDTH) + OFFSET);
					window->draw(paths[0]);
				}
				if (maze[i * MAZE_WIDTH + j] & W_PATH) {
					paths[1].setPosition(j * (CELL_SIZE + WALL_WIDTH) - WALL_WIDTH + OFFSET, i * (CELL_SIZE + WALL_WIDTH) + OFFSET);
					window->draw(paths[1]);
				}
				if (maze[i * MAZE_WIDTH + j] & N_PATH) {
					paths[2].setPosition(j * (CELL_SIZE + WALL_WIDTH) + OFFSET, i * (CELL_SIZE + WALL_WIDTH) - WALL_WIDTH + OFFSET);
					window->draw(paths[2]);
				}
				if (maze[i * MAZE_WIDTH + j] & S_PATH) {
					paths[3].setPosition(j * (CELL_SIZE + WALL_WIDTH) + OFFSET, i * (CELL_SIZE + WALL_WIDTH) + CELL_SIZE + OFFSET);
					window->draw(paths[3]);
				}
				// Iterating the maze downward and rightward so West Path and South Path can be ignored
			}
		}
		if (type == BackTracking && !stack.empty()) {
			head.setPosition(stack.top().second * (CELL_SIZE + WALL_WIDTH) + OFFSET, stack.top().first * (CELL_SIZE + WALL_WIDTH) + OFFSET);
			window->draw(head);
		}

		window->display();
	}
};
#endif // !MAZE_GENERATOR_H
