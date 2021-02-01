#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/Window.hpp"

#include "Track.h"
#include "GameState.h"
#include "MainMenu.h"
#include "ChoiceScreen.h"

using namespace std;

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event& t_event);
	void update(sf::Time t_deltaTime);
	void render();

	sf::RenderWindow m_window; 
	Track m_track;
	MainMenu m_mainMenu;
	ChoiceScreen m_choiceScreen;
};

#endif // !GAME_HPP

