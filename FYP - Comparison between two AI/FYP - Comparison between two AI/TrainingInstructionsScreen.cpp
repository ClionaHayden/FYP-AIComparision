#include "TrainingInstructionsScreen.h"

TainingInstructionsScreen::TainingInstructionsScreen():
	m_buttons
{
	Button(Vector2f(500.0f,700.0f), "Start"),
	Button(Vector2f(500.0f,800.0f), "Back")
},
m_clickTimer(Time::Zero)
{
	setup();
}

TainingInstructionsScreen::~TainingInstructionsScreen()
{
}

void TainingInstructionsScreen::update(Time t_deltaTime)
{
	m_clickTimer += t_deltaTime;
	checkButtonPress();
}

void TainingInstructionsScreen::render(RenderWindow& t_window)
{
	t_window.draw(m_titleText);
	t_window.draw(m_instructions);
	t_window.draw(m_keys);
	for (Button& b : m_buttons)
	{
		b.render(t_window);
	}
}

void TainingInstructionsScreen::handleInput(Event& e, RenderWindow& t_window)
{
	for (Button& b : m_buttons)
	{
		b.handleInput(e, t_window);
	}
}

void TainingInstructionsScreen::setup()
{
	setupSprite();
	setupText();
}

void TainingInstructionsScreen::setupSprite()
{
	if(!m_keyTex.loadFromFile("ASSETS/keys.png")) { cout << "Error loading key Texture" << endl; }
	m_keys.setTexture(m_keyTex);
	m_keys.setPosition(Vector2f{ 270.0f,350.0f });
}

void TainingInstructionsScreen::setupText()
{
	if (!m_font.loadFromFile("ASSETS/FONT/font.ttf"))
		cout << "Error loading font" << endl;

	m_titleText.setFont(m_font);
	m_titleText.setString("Collecting Training \n             Data");
	m_titleText.setFillColor(Color::White);
	m_titleText.setPosition(Vector2f(250.0f, -50.0f));
	m_titleText.setCharacterSize(150);

	m_instructions.setFont(m_font);
	m_instructions.setString("Try to do 3 laps of the track as best as you can.");
	m_instructions.setFillColor(Color::White);
	m_instructions.setPosition(Vector2f(130.0f, 180.0f));
	m_instructions.setCharacterSize(90);
}

void TainingInstructionsScreen::checkButtonPress()
{
	if (m_clickTimer >= seconds(0.5))
	{
		//play
		if (m_buttons[0].getButtonState() == ButtonState::Clicked)
		{
			m_clickTimer = Time::Zero;
			s_gameState = GameState::TrainingDataCollection;
		}
		//back
		if (m_buttons[1].getButtonState() == ButtonState::Clicked)
		{
			m_clickTimer = Time::Zero;
			s_gameState = GameState::ChoiceScreen;
		}
	}
}
