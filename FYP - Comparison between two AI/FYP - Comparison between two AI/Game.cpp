#include "Game.h"
using namespace std;

extern enum GameState s_gameState = GameState::MainMenu;

Game::Game():
	m_window{ sf::VideoMode{ 970 , 950 , 32 }, "FYP - AI Comparison" }
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

void Game::processKeys(sf::Event& t_event)
{
	switch (s_gameState)
	{
	case GameState::MainMenu:
		m_mainMenu.handleInput(t_event, m_window);
		break;
	case GameState::ChoiceScreen:
		m_choiceScreen.handleInput(t_event, m_window);
		break;
	case GameState::TrainingDataInstructions:
		m_trainingInstructionsScreen.handleInput(t_event, m_window);
		break;
	case GameState::TrainingDataCollection:
		m_track.handleInput(t_event);
		break;
	case GameState::Reinforcement:
		m_track.handleInput(t_event);
		break;
	case GameState::Backprop:
		break;
	case GameState::Success:
		break;
	case GameState::Fail:
		break;
	case GameState::Race:
		break;
	case GameState::Results:
		break;
	case GameState::Exit:
		m_window.close();
		break;
	case GameState::LoadWeights:
		m_track.handleInput(t_event);
		break;
	default:
		break;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window.close();
	}
}

void Game::update(sf::Time t_deltaTime)
{
	switch (s_gameState)
	{
	case GameState::MainMenu:
		m_mainMenu.update(t_deltaTime);
		break;
	case GameState::ChoiceScreen:
		m_choiceScreen.update(t_deltaTime);
		break;
	case GameState::TrainingDataInstructions:
		m_trainingInstructionsScreen.update(t_deltaTime);
		break;
	case GameState::TrainingDataCollection:
		m_track.update(t_deltaTime);
		break;
	case GameState::Reinforcement:
		m_track.update(t_deltaTime);
		break;
	case GameState::Backprop:
		break;
	case GameState::Success:
		break;
	case GameState::Fail:
		break;
	case GameState::Race:
		break;
	case GameState::Results:
		break;
	case GameState::Exit:
		break;
	case GameState::LoadWeights:
		m_track.update(t_deltaTime);
		break;
	default:
		break;
	}
}

void Game::render()
{
	m_window.clear(sf::Color::Black);
	switch (s_gameState)
	{
	case GameState::MainMenu:
		m_mainMenu.render(m_window);
		break;
	case GameState::ChoiceScreen:
		m_choiceScreen.render(m_window);
		break;
	case GameState::TrainingDataInstructions:
		m_trainingInstructionsScreen.render(m_window);
		break;
	case GameState::TrainingDataCollection:
		m_track.render(m_window);
		break;
	case GameState::Reinforcement:
		m_track.render(m_window);
		break;
	case GameState::Backprop:
		break;
	case GameState::Success:
		break;
	case GameState::Fail:
		break;
	case GameState::Race:
		break;
	case GameState::Results:
		break;
	case GameState::Exit:
		break;
	case GameState::LoadWeights:
		m_track.render(m_window);
		break;
	default:
		break;
	}
	m_window.display();
}