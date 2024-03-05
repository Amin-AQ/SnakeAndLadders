#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h> 
#include <iostream>
#include <fstream>
#include<string>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
class GUI
{
public:
	sf::RenderWindow window;
	sf::RenderTexture rendText;
	sf::Font font;
	sf::Text*txt;
	sf::Text moveTxt,rollTxt;
	sf::Texture spriteSheet;
	sf::Texture bck;
	GUI() :window(sf::VideoMode(960, 680), "Snake And Ladders")
	{
		window.setPosition(sf::Vector2i(0, 0));
		if (!font.loadFromFile("Resources/Font/SHOWG.ttf"))
			std::cout << "\nFont loading failed\n";
		rollTxt.setFont(font);
		rollTxt.setFillColor(sf::Color::Green);
		rollTxt.setCharacterSize(30);
		txt = new sf::Text[100];
		bool leftToRight = true;
		for (int i = 0; i < 100;)
		{
			if (!leftToRight)
			{
				int j = i + 10;
				for (i; i < j; i++)
				{
					txt[i].setFont(font);
					txt[i].setFillColor(sf::Color::Black);
					txt[i].setString(to_string(i + 1));
				}
			}
			else
			{
				int j = i + 10, k = i + 10;
				for (i; i < k; j--,i++)
				{
					txt[i].setFont(font);
					txt[i].setFillColor(sf::Color::Black);
					txt[i].setString(to_string(j));
				}
			}
			leftToRight = !leftToRight;
		}
		/*for (int i = 0; i < 100; i++)
		{
			txt[i].setFont(font);
			txt[i].setFillColor(sf::Color::Black);
			txt[i].setString(to_string(i + 1));
		}*/
		if(!spriteSheet.loadFromFile("Resources/Tilemap/spritesheet.png"))
			std::cout<<"\nSpritesheet Loading failed.\n";
		if (!bck.loadFromFile("Resources/Tilemap/bck.png", sf::IntRect(0, 0, 960, 680)))
			std::cout << "\nBackground Loading failed.\n";
	}
	int getBoardNumber(int i)
	{
		return stoi((string)txt[i].getString());
	}
	~GUI()
	{
	
	}

	

};