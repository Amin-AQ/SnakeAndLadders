#include "Player.h"
#include<iostream>
#include<string>
Player::Player(sf::Color c)
{
	turn = 0;
	playerName = ""; 
	p.setFillColor(c);
	p.setRadius(30);
	txtPos = 1;
	arrPos = 90;
	currDir.x = 1;
	currDir.y = 0;
	this->setPos(165, 9*63+0);
}
void Player::reset()
{
	txtPos = 1;
	turn = 0;
	arrPos = 90;
	currDir.x = 1;
	currDir.y = 0;
	this->setPos(165, 9 * 63);
}
void Player::setPos(float x, float y)
{
	pos = sf::Vector2f(x, y);
	p.setPosition(pos); 
}
void Player::draw(sf::RenderTarget&win)
{
	win.draw(p);
}
sf::Vector2f Player::calcTarget (int b, int b2)
{ 
	int i = b % 10, j = b / 10;
	arrPos = b;
	txtPos = b2;
	sf::Vector2f targ(i * 63+165, j * 63);
	currDir = (targ - pos)/63.f;
	return targ;
} 
void Player::updateTurn()
{
	turn = !turn;
}
int Player::getTxtPos() { return txtPos; }
void Player::setDirection()
{
	if (txtPos % 10 == 0)
	{
		currDir.x = 0; currDir.y = -1;
	}
	else if ((txtPos / 10)%2==0)//even left to right
	{
		currDir.x = 1; currDir.y = 0;
	}
	else if ((txtPos / 10) % 2 != 0)//odd right to left
	{
		currDir.x = -1; currDir.y = 0;
	}
}
void Player::updateAllArrPos()
{ 
	if (txtPos % 10 == 0) // to go up on board
		arrPos -= 10;
	else if ((txtPos / 10) % 2 == 0)//even left to right
		arrPos++;
	else if ((txtPos / 10) % 2 != 0)//odd right to left
		arrPos--;
	txtPos++;
}
bool Player::Turn() { return turn; }
int Player::getArrPos() { return arrPos;   }
sf::Vector2f Player::pathMove(bool sl)
{
	if(!sl)
		setDirection();
	setPos(pos.x+(currDir).x,pos.y+(currDir).y);
	return pos ;
}