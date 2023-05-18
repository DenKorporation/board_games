#include "States/MenuState.h"
#include "GUI/Button.h"
#include "GUI/Label.h"
#include "Utility.h"
#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/SpriteNode.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack &stack, Context context)
	: State(stack, context),
	  mSceneGraph(),
	  mGUIContainer()
{
	unsigned int winX = context.window->getSize().x, winY = context.window->getSize().y;

	mSceneGraph.setPosition(winX / 2.f, 0.f);

	sf::Texture &texture = context.textures->get(Textures::ID(Textures::Global::Background));
	SpriteNode::Ptr backgroundSprite(new SpriteNode(texture));
	backgroundSprite->setScale((float)winX / texture.getSize().x, (float)winY / texture.getSize().y);
	backgroundSprite->setPosition(0.f, winY / 2.f);
	mSceneGraph.attachChild(std::move(backgroundSprite));

	GUI::Container::Ptr GUIContainer(new GUI::Container);
	mGUIContainer = GUIContainer.get();
	mSceneGraph.attachChild(std::move(GUIContainer));

	GUI::Label::Ptr logo(new GUI::Label("The Fool", *context.fonts, Fonts::Label));
	logo->setFontSize(winY / 3);
	logo->setOutlineThickness(7.f);
	logo->setLetterSpacing(2);
	logo->setFillColor(sf::Color(200, 200, 200));
	logo->setPosition(0.f, winY / 4.f);
	mGUIContainer->attachChild(std::move(logo));

	GUI::Container::Ptr GUIMenu(new GUI::Container);
	GUIMenu->setPosition(0.f, winY * 2.5f / 5.f);

	GUI::Button::Style normalStyle(winY / 20, sf::Color(255, 255, 255), sf::Color(105, 105, 105), sf::Color::Black, 1.f);
	GUI::Button::Style selectedStyle(winY / 20, sf::Color::Red, sf::Color(105, 105, 105), sf::Color::Black, 1.f);

	GUI::Button::Ptr playButton(new GUI::Button(*context.fonts, *context.sounds));
	playButton->setSize(sf::Vector2f(winX / 5.f, winY / 10.f));
	playButton->setText("Play");
	playButton->setNormalStyle(normalStyle);
	playButton->setSelectedStyle(selectedStyle);
	playButton->setCallback([this]()
							{
								requestStackPop(); 
								requestStackPush(States::Game); });
	GUIMenu->attachChild(std::move(playButton));

	GUI::Button::Ptr onlineButton(new GUI::Button(*context.fonts, *context.sounds));
	onlineButton->setPosition(0.f, (winY / 10.f) * 1.2f);
	onlineButton->setSize(sf::Vector2f(winX / 5.f, winY / 10.f));
	onlineButton->setText("Online");
	onlineButton->setNormalStyle(normalStyle);
	onlineButton->setSelectedStyle(selectedStyle);
	onlineButton->setCallback([this]()
							  {
								requestStackPop(); 
								requestStackPush(States::MultiplayerConnection); });
	GUIMenu->attachChild(std::move(onlineButton));

	GUI::Button::Ptr statisticButton(new GUI::Button(*context.fonts, *context.sounds));
	statisticButton->setPosition(0.f, 2 * (winY / 10.f) * 1.2f);
	statisticButton->setSize(sf::Vector2f(winX / 5.f, winY / 10.f));
	statisticButton->setText("Settings");
	statisticButton->setNormalStyle(normalStyle);
	statisticButton->setSelectedStyle(selectedStyle);
	statisticButton->setCallback([this]()
								 {
								requestStackPop();
								requestStackPush(States::Settings); });
	GUIMenu->attachChild(std::move(statisticButton));

	GUI::Button::Ptr exitButton(new GUI::Button(*context.fonts, *context.sounds));
	exitButton->setPosition(0.f, 3 * (winY / 10.f) * 1.2f);
	exitButton->setSize(sf::Vector2f(winX / 5.f, winY / 10.f));
	exitButton->setText("Exit");
	exitButton->setNormalStyle(normalStyle);
	exitButton->setSelectedStyle(selectedStyle);
	exitButton->setCallback([this]()
							{ requestStackClear(); });

	GUIMenu->attachChild(std::move(exitButton));

	mGUIContainer->attachChild(std::move(GUIMenu));
}

void MenuState::draw()
{
	sf::RenderWindow &window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mSceneGraph);
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event &event)
{
	mGUIContainer->handleEvent(event);
	return true;
}
