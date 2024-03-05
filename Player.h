#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h> 
#include<string>
class Player
{
	sf::CircleShape p;
	sf::Vector2f currDir;
	bool turn;
	int arrPos;
	int txtPos;
	int move;
	sf::Vector2f pos;
public:
	std::string playerName;
	Player(sf::Color c);
	void reset();
	void updateAllArrPos();
	int getArrPos();
	int getTxtPos();
	void setPos(float x, float y);
	sf::Vector2f calcTarget(int b,int b2);
	void draw(sf::RenderTarget&win);
	void updateTurn();
	bool Turn();
	void setDirection();
	sf::Vector2f pathMove(bool sl);
};

