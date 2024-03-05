#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h> 
class Dice : public sf::Drawable, public sf::Transformable
{
	sf::Texture diceTexture;
	sf::VertexArray dices;
	sf::RenderStates dstate;
public:
	Dice();
	void setPosition(sf::Vector2f p);
	int roll();
	void setTexture(int i);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw2(sf::RenderTarget& win, int i, sf::Vector2f p);
};

