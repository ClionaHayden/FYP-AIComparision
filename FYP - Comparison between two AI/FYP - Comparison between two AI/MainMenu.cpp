#include "MainMenu.h"

MainMenu::MainMenu() :
	m_buttons
{
	Button(Vector2f(500.0f,300.0f), "Start New"),
	Button(Vector2f(500.0f,500.0f), "Load Data"),
	Button(Vector2f(500.0f,700.0f), "Quit")
},
m_clickTimer(Time::Zero)
{
	setup();
}

MainMenu::~MainMenu()
{
}

void MainMenu::update(Time t_deltaTime)
{
	m_clickTimer += t_deltaTime;
	checkButtonPress();
}

void MainMenu::render(RenderWindow& t_window)
{
	t_window.draw(m_titleText);
	for (Button& b : m_buttons)
	{
		b.render(t_window);
	}
}

void MainMenu::handleInput(Event& e, RenderWindow& t_window)
{
	for (Button& b : m_buttons)
	{
		b.handleInput(e,t_window);
	}
}

void MainMenu::setup()
{
	if (!m_font.loadFromFile("ASSETS/FONT/font.ttf")) { cout << "Error loading font" << endl; }
	m_titleText.setFont(m_font);
	m_titleText.setString("Machine Learning \n             Demo");
	m_titleText.setFillColor(Color::White);
	m_titleText.setPosition(Vector2f(250.0f, -50.0f));
	m_titleText.setCharacterSize(150);
}

void MainMenu::checkButtonPress()
{
	if (m_clickTimer >= seconds(0.5))
	{
		//play
		if (m_buttons[0].getButtonState() == ButtonState::Clicked)
		{
			m_clickTimer = Time::Zero;
			s_gameState = GameState::ChoiceScreen;
		}
		//load
		if (m_buttons[1].getButtonState() == ButtonState::Clicked)
		{
			m_clickTimer = Time::Zero;
		}
		//exit
		if (m_buttons[2].getButtonState() == ButtonState::Clicked)
		{
			m_clickTimer = Time::Zero;
			s_gameState = GameState::Exit;
		}
	}
}
