#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/**
 * Game.h
 *
 * Class that acts as the game engine.
 * Wrapper class.
 *
 *  Created on: 7 de abr. de 2024
 *      Author: Renan Andrade
 */

#ifndef GAME_H_
#define GAME_H_

class Game
{
private:
	/*VARIABLES*/

	// Window
	sf::RenderWindow *window;
	sf::Event ev;
	sf::VideoMode videoMode;

	// Mouse Positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// Resources
	sf::Font font;

	// Text
	sf::Text uiText;

	// Game Logic
	bool endGame;
	unsigned int points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;

	// Game Objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	// Private Functions
	void initVariables();
	void initWindow();
	void initFonts();
	void initText();

public:
	// Constructors & Destructors
	Game();
	virtual ~Game();

	// Accessors
	const bool isRunning() const;
	const bool getEndGame() const;

	// Functions
	void spawnEnemy();

	void pollEvents();

	void updateMousePositions();
	void updateText();
	void updateEnemies();
	void update();

	void renderText(sf::RenderTarget &target);
	void renderEnemies(sf::RenderTarget &target);
	void render();
};

#endif /* GAME_H_ */
