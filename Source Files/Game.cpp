#include "Game.h"

// Private Functions
void Game::initVariables()
{
	/**
	 *	@return void
	 *
	 *	Initialize game variables as:
	 *	- window pointer;
	 *	- points;
	 *	- enemy spawn timer and timer maximum;
	 *	- max enemies.
	 */

	this->window = nullptr;

	// Game Logic
	this->endGame = false;
	this->points = 0;
	this->health = 20;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
	this->mouseHeld = false;
}
void Game::initWindow()
{
	/**
	 *	@return void
	 *
	 *	Initializes game window:
	 *	- Sets height to 600;
	 *	- Sets width to 800;
	 *	- Sets title and set window to not be resizeble
	 *	- Sets framerate limit to 60FPS.
	 */

	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new sf::RenderWindow(this->videoMode, "Falling Blocks", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initFonts()
{
	/**
	 *	@return void
	 *	Load .ttf font from Resources folder
	 *	Prints a ERROR message if it fails.
	 *
	 */

	// This filepath is dynamic to avoid the file to not be found.
	// Inside debug and release folders, there is a copy of the
	// Dosis-Light.ttf font file.
	std::string currentPath = std::filesystem::current_path().string();

	std::string filePath = currentPath + "/Source Files/Dosis-Light.ttf";

	if (!this->font.loadFromFile(filePath))
	{
		std::cout << "ERROR::GAME::INITFONTS::Error while loading fonts." << std::endl;
	}

}

void Game::initText()
{
	/**
	 *	@return void
	 *
	 *	Initializes texts to be rendered at screen.
	 *	- Set the font to this->font
	 *	- set character size to 24
	 *	- Set fill color to white
	 *	- Set default value to "NONE".
	 */
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
}

// Constructors & Destructors
Game::Game()
{
	/**
	 *	Game class constructor.
	 *	- Initialize variables;
	 *	- Initialize window;
	 *	- Initialize game enemies.
	 */
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initText();
}

Game::~Game()
{
	/**
	 *	Game class destructor.
	 */
	delete this->window;
}

// Accessors
const bool Game::isRunning() const
{
	/**
	 *	@return bool
	 *
	 *	Returns if the game instance is running.
	 */

	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	/**
	 *	@return boool
	 *
	 *	Returns if the game has ended.
	 */
	return this->endGame;
}

// Functions
void Game::spawnEnemy()
{
	/**
	 *	@return void
	 *
	 *	Spawns enemies at random positions and set their types and color.
	 *	- Sets a random position;
	 *	- Sets a random type;
	 *	- Sets a respective color;
	 *	- Adds enemy to enemies vector.
	 */

	this->enemy.setPosition(
			static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)), 0.f);

	// Randomize enemy type
	int type = rand() % 5;

	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(10.f, 10.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	}

	// Spawn Enemy
	this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
	/**
	 *	@return void
	 *
	 *	Poll events from the game window.
	 *	While the window has events to poll:
	 *	- Match event type and executes the respective behavior.
	 */

	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;

		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				window->close();

			break;
		}
	}
}

void Game::updateMousePositions()
{
	/**
	 *	@return void
	 *
	 *	Update Mouse Positions:
	 *	- Relative to window (Vector2i)
	 *	- Relative to view (Vector2f)
	 */

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
	/**
	 *	@return void
	 *
	 *	Updates the text shown in the UI.
	 *	- Creates a stringstream
	 *	- Write points and health into the stream
	 *	- Set UI text as the string.
	 */
	std::stringstream ss;

	ss << "Points: " << this->points << "\n"
			<< "Health: " << this->health << "\n";

	this->uiText.setString(ss.str());
}

void Game::updateEnemies()
{
	/**
	 *	@return void
	 *
	 *	Updates the enemy spawn timer and spawns enemies
	 *	when the total amount of enemies is smaller than
	 *	the maximum.
	 *	Moves the enemies downwards.
	 *	Removes the enemies at the edge of the screen.
	 *	If enemy clicked, delete it and add points to player.
	 */

	// Updating the timer for enemy spawning
	if (static_cast<int>(this->enemies.size()) < (this->maxEnemies))
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			// Spawn an enemy and reset timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	// Moving and updating enemies
	for (int i = 0; i < static_cast<int>(this->enemies.size()); i++)
	{
		this->enemies[i].move(0.f, 4.f);

		// If enemy is past the screen
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			// Delete it and subtract from health
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			i -= 1;
		}
	}

	// Check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		// If the mouse is not being held
		if (this->mouseHeld == false)
		{
			// Set mouse being held to true
			this->mouseHeld = true;

			bool deleted = false;

			// For each enemy
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				// Check which one was clicked and delete it
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					// Add points according to enemy color
					if (this->enemies[i].getFillColor() == sf::Color::Magenta)
						this->points += 10;

					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
						this->points += 7;

					else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
						this->points += 5;

					else if (this->enemies[i].getFillColor() == sf::Color::Red)
						this->points += 3;

					else if (this->enemies[i].getFillColor() == sf::Color::Green)
						this->points += 1;

					// Delete enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
					i -= 1;
				}
			}
		}
	}
	// If the mouse is not being clicked
	else
	{
		// Set held state to false
		this->mouseHeld = false;
	}
}

void Game::update()
{
	/**
	 *	@return void
	 *
	 *	Update game state:
	 *	- Poll events;
	 *	- Update mouse positions;
	 *	- Update enemies state.
	 */
	this->pollEvents();

	if (this->endGame == false)
	{
		this->updateMousePositions();

		this->updateText();

		this->updateEnemies();
	}

	if (this->health <= 0)
	{
		this->endGame = true;
	}
}

void Game::renderText(sf::RenderTarget &target)
{
	/**
	 *	@return void
	 *
	 *	Renders text into a target.
	 *	If the game has ended, render the final points and exit
	 *	message.
	 */
	if (this->getEndGame())
	{
		std::stringstream ss;

		ss << "Final points: " << this->points << ".\n"
				<< "Press ESC to exit.";

		this->uiText.setString(ss.str());
	}

	target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget &target)
{
	/**
	 *	@return void
	 *
	 *	Renders enemies at the target.
	 */
	for (auto &e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::render()
{
	/**
	 * @return void
	 *
	 * Render the game screen:
	 * - Clear window frame;
	 * - Render enemies to the frame;
	 * - Render text into the frame
	 * - Display the new frame into the screen.
	 */
	this->window->clear();

	this->renderEnemies(*this->window);

	this->renderText(*this->window);

	this->window->display();
}
