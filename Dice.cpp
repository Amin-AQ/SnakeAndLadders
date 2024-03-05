#include "Dice.h"
#include <SFML/Graphics.hpp>
#include <windows.h> 
#include<ctime>
Dice::Dice(): dices(sf::Quads,4)
{
	diceTexture.loadFromFile("Resources/Tilemap/tileset.png", sf::IntRect(252, 0, 378, 63));
	for (int i = 0,col=0,row=630,tcol=0,trow=0;i < 4;i++)
	{
		dices[i].position = sf::Vector2f(col, row);
		dices[i].texCoords = sf::Vector2f(tcol, trow);
		if (i == 0)
		{
			col += 63;
			tcol += 63;
		}
		else if (i == 1)
		{
			row += 63;
			trow += 63;
		}
		else
		{
			col -= 63;
			tcol -= 63;
		}
	}
}
void Dice::setPosition(sf::Vector2f p)
{
	for (int i = 0, col = p.x, row = p.y;i < 4;i++)
	{
		dices[i].position = sf::Vector2f(col, row);
		if (i == 0)
			col += 63;
		else if (i == 1)
			row += 63;
		else
			col -= 63;
	}
}
int Dice::roll()
{
	int j = (rand() % (5 - 0 + 1)) + 0;
	setTexture(j);
	return j;
}
void Dice::setTexture(int i)
{
	for (int j = 0, tcol = i*63, trow = 0;j < 4;j++)
	{
		dices[j].texCoords = sf::Vector2f(tcol, trow);
		if (j == 0)
			tcol += 63;
		else if (j == 1)
			trow += 63;
		else
			tcol -= 63;
	}
}
void Dice::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();
	// apply the tileset texture
	states.texture = &diceTexture;
	target.draw(dices, states);
}
void Dice::draw2(sf::RenderTarget& win, int i, sf::Vector2f p)
{
	setTexture(i);
	setPosition(p);
	draw(win, dstate);
}