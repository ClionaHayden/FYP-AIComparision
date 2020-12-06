#include "Game.h"
using namespace std;

Game::Game():
	m_window{ sf::VideoMode{ 970 , 950 , 32 }, "FYP - AI Comparison" },
	m_car(Vector2f(90.0f,350.0f), 2.0f)
{
}

Game::~Game()
{
}


void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		processKeys(newEvent);
	}
}

void Game::processKeys(sf::Event t_event)
{
	m_car.handleInput(t_event);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window.close();
	}
}

void Game::update(sf::Time t_deltaTime)
{
	m_car.update(t_deltaTime);
}

void Game::render()
{
	m_window.clear(sf::Color::Black);
	m_track.render(m_window);
	m_car.render(m_window);
	m_window.display();
}