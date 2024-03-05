 #pragma once
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<vector>
#include<ctime>
#include<string>
#include<iostream>
#include"TileMap.h"
#include"GUI.h"
#include"Player.h"
#include"Dice.h"
#include"Connection.h"
class SnakeAndLadders:public Connection
{
	GUI g;
	sf::Thread rcvThrd,sndThrd;
	const int size;
	const int tileMap[100];
	int dno,rollTime,winner,gameMode; //0: single, 1::multi client, 2:: multi server
	bool diceRoll,rollLock,slMode,gameQuit;
	vector<sf::VertexArray>ladderList; 
	vector<sf::VertexArray>snakeList;
	TileMap map;
	Player P;
	int stat;
	Player opp;
	Dice dice;
	sf::Clock clock;
public:
	SnakeAndLadders() :size(10*10), P(sf::Color::White),opp(sf::Color::Black), 
									tileMap{1,0,99,0,1,0,1,-91,52,0,
											0,-78,0,1,0,1,0,1,0,1,
											1,0,1,-48,1,0,1,0,1,0,
											0,1,70,1,0,1,0,1,0,1,
											1,0,1,0,1,0,1,0,-23,0,
											0,1,8,1,0,1,0,1,0,1,
											1,0,1,86,1,0,1,-80,1,0,
											32,1,0,1,0,1,0,1,-11,1,
											-67,0,1,0,1,0,63,0,1,0,
											0,-7,0,1,0,1,0,1,0,2
									}, rcvThrd(&SnakeAndLadders::recieveHelper,this),sndThrd(&SnakeAndLadders::sendHelper,this)
	{
		opp.playerName = "AI";
		g.rendText.create(63 * 10, 63 * 10);
		map.load("Resources/Tilemap/tileset.png", sf::Vector2u(63, 63), tileMap, g.txt, 10, 10, g.rendText, ladderList, snakeList);
		dno = 1;
		winner = -1;
		stat = -1;
		gameQuit = false;
		gameMode = 0;
		rollTime = 0;
		diceRoll = 0;
		rollLock = 0;
		slMode = 0;
		for (int i = 0; i < ladderList.size(); i++)
			g.rendText.draw(ladderList[i], &(g.spriteSheet));
		for (int i = 0; i < snakeList.size(); i++)
			g.rendText.draw(snakeList[i], &(g.spriteSheet));
		g.rendText.display();
		map.setPosition(165, 0);
		g.rollTxt.setPosition(sf::Vector2f(800, 60));
	}
	void recieveHelper()
	{
		Connection::threadReturn = false;
		stat=Connection::recievePacket(dno);
		Connection::threadReturn = true;
		return;
	}
	void sendHelper()
	{
		Connection::threadReturn1 = false;
		stat=Connection::sendPacket(dno);
		Connection::threadReturn1 = true;
		return;
	}
	int menu()
	{
		const int  noOfOptions = 4;
		sf::Text menuOpts[noOfOptions];
		int pos = 270;
		for (int i = 0;i < noOfOptions;i++)
		{
			menuOpts[i].setFont(g.font);
			menuOpts[i].setFillColor(sf::Color::Yellow);
			menuOpts[i].setPosition(250, pos);
			pos += 40;
		}
		menuOpts[0].setString("Single Player");
		menuOpts[1].setString("Join Local Server (Multilayer)");
		menuOpts[2].setString("Create Local Server (Multiplayer)");
		menuOpts[3].setString("Exit");
		int selected = 0, prev=-1;
		sf::Event event;
		while (g.window.isOpen())
		{
			while (g.window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					gameQuit = true;
					g.window.close();
				}
				if (event.type == sf::Event::KeyPressed)
				{
					switch (event.key.code)
					{
					case sf::Keyboard::Up:
						prev = selected;
						menuOpts[prev].setFillColor(sf::Color::Yellow);
						if (selected == 0)
							selected = 3;
						else
							selected--;
						break;
					case sf::Keyboard::Down:
						prev = selected;
						menuOpts[prev].setFillColor(sf::Color::Yellow);
						selected = ((selected + 1) % noOfOptions);
						break;
					case sf::Keyboard::Enter:
						return selected;
					case sf::Keyboard::C:
						Screenshot();
					}
				}
			}
			g.window.clear(sf::Color(0, 0, 0));
			g.window.draw(sf::Sprite(g.bck));
			menuOpts[selected].setFillColor(sf::Color::Black);
			for (int i = 0;i < noOfOptions;i++)
				g.window.draw(menuOpts[i]);
			g.window.display();
		}
	}
	void run()
	{
		int w=63,x = -1,tmp=0;
		sf::Vector2f target(-2,-2),pos(-1,-1);
		stat = -1;
		if (winner == 1)
			P.updateTurn();
		else if (winner == 0)
			opp.updateTurn();
		sf::Time timeSinceLastUpdate = sf::Time::Zero, timePerFrame=(sf::seconds(1.f / 60.f));
		Connection::threadReturn = Connection::threadReturn1 = false;
		bool recieved = false,sending=false;
		while (g.window.isOpen() && (stat != sf::Socket::Disconnected || gameMode == 0))
		{
			sf::Event event;
			timeSinceLastUpdate += clock.restart();
			/*sf::CircleShape shape(100.f);
			shape.setFillColor(sf::Color::Green);*/
			while (g.window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					gameQuit = true;
					g.window.close();
				}
				if (event.type == sf::Event::KeyPressed)
				{
					switch (event.key.code)
					{
					case sf::Keyboard::Space:
						if ((!diceRoll && !rollLock && P.Turn() && gameMode == 0) || (!diceRoll && !rollLock && P.Turn() && gameMode > 0 && !sending))
						{
							diceRoll = 1;
							rollTime = 120;
							rollLock = 1;
						}
						break;
					case sf::Keyboard::C:
						Screenshot();
					}
				}
			}
			if (opp.Turn() && gameMode > 0 && !recieved)
			{
				sf::Int32 q = dno;
				rcvThrd.launch();
				if (Connection::threadReturn)
				{
					recieved = true;
				}
			}
			if (Connection::threadReturn1&&gameMode>0)
			{
					P.updateTurn();
					opp.updateTurn();
					rollLock = 0;
					x = -1;
					w = 63;
					diceRoll = 0;
					pos.x = pos.y = -1;
					target.x = target.y = -2;
					Connection::threadReturn1 = sending =  false;

			}
			if (!rollLock && !diceRoll && opp.Turn() && (recieved||gameMode==0))
			{
				diceRoll = 1;
				rollTime = 300;
				rollLock = 1;
			}
			timeSinceLastUpdate += clock.restart();
			while (timeSinceLastUpdate >= timePerFrame)
			{
				
				if (gameMode == 0 || (gameMode > 0 && ((P.Turn()&&!sending)||(opp.Turn()&&recieved))))
				{
					if (diceRoll && rollTime > 0)
					{
						
						if (gameMode == 0 || P.Turn())
							dno = dice.roll();
						else if (gameMode > 0 && opp.Turn())
						{
							tmp = dice.roll();
						}
						rollTime--;
						//cout << rollTime<<","<<opp.Turn() <<"," << gameMode << endl;
						if (rollTime <= 0)
						{
							diceRoll = 0;
							x = dno + 1;
							//cout << x;
							x *= 63;
						}
					}
					if ((x > 0 && !diceRoll && !slMode) || (pos != target && slMode))
					{
						if (P.Turn())
						{
							if (P.getTxtPos() + dno + 1 > 100 && x == ((dno + 1) * 63))
							{
								goto felse;
							}
							else
								pos = P.pathMove(slMode);

						}
						else if (opp.Turn())
						{
							if (opp.getTxtPos() + dno + 1 > 100 && x == ((dno + 1) * 63))  //need exact number to goto 100
							{
								goto felse;
							}
							else
								pos = opp.pathMove(slMode);
						}
						x--;
						w--;
						if (w <= 0 && !slMode)
						{
							if (P.Turn())
							{
								P.updateAllArrPos();
								if (P.getTxtPos() == 100)
								{
									Connection::sendPacket(dno);
									winner = 1;
									WinState(event, winner);
									P.updateTurn();
									return;
								}
							}
							else if (opp.Turn())
							{
								opp.updateAllArrPos();
								if (opp.getTxtPos() == 100)
								{
									winner = 0;
									WinState(event, winner);
									opp.updateTurn();
									return;
								}
							}
							w = 63;
						}
						if ((x <= 0 && !slMode) || (slMode && target == pos))
						{
							if (target == pos)
							{
								goto felse;
							}

							if (P.Turn() && (tileMap[P.getArrPos()] < 0 || tileMap[P.getArrPos()] > 1))
							{
								int d = P.getArrPos();
								int z = tileMap[d];
								if ((z < 0 && d < abs(z) || (z > 1 && d > z)))
								{
									target = P.calcTarget(abs(z), g.getBoardNumber(99 - abs(z)));
									slMode = true;
								}
								else
									goto felse;
							}
							else if (opp.Turn() && (tileMap[opp.getArrPos()] < 0 || tileMap[opp.getArrPos()] > 1))
							{
								int d = opp.getArrPos();
								int z = tileMap[d];
								if ((z < 0 && d < abs(z) || (z > 1 && d > z)))
								{
									target = opp.calcTarget(abs(z), g.getBoardNumber(99 - abs(z)));
									slMode = true;
								}
								else
									goto felse;
							}
							else
							{
							felse:
								slMode = 0;
								sf::Int32 q = dno;
								if (P.Turn() && gameMode > 0)
								{
									Connection::threadReturn = false;
									recieved = false;
									sending = true;
									sndThrd.launch();
								}
								else
								{
									P.updateTurn();
									opp.updateTurn();
									rollLock = 0;
									x = -1;
									w = 63;
									diceRoll = 0;
									pos.x = pos.y = -1;
									target.x = target.y = -2;
								}
							}
						}
					}
				}
				timeSinceLastUpdate -= timePerFrame;
			}
			g.window.clear(sf::Color(0, 0, 0));
			g.window.draw(sf::Sprite(g.bck));
			g.window.draw(map);
			sf::Sprite temp(g.rendText.getTexture());
			temp.setPosition(165, 0); 
			g.window.draw(temp);
			P.draw(g.window);
			opp.draw(g.window);
			if (P.Turn())
				g.rollTxt.setString("Roll ! ! !");
			else
				g.rollTxt.setString("Waiting . . .");
			g.window.draw(g.rollTxt);
			if (opp.Turn() && gameMode > 0 && diceRoll)
			{
				//cout << rollTime << endl;
				dice.draw2(g.window, tmp, sf::Vector2f(800, 100));
			}
			else 
				dice.draw2(g.window, dno,sf::Vector2f(800,100));
			g.window.display();
			
 		}
		if (gameMode > 0 && stat == sf::Socket::Disconnected)
		{
			gameQuit = true;
			cout << "\nConnection Lost\n";
			return;
		}
	}
	void excgStr(string& sent, string&recieved)
	{
		if (gameMode == 2)
		{
			//while (Connection::socket.NotReady);
			Connection::sendPacket(sent);
			//while (Connection::socket.NotReady);
			Connection::recievePacket(recieved);
			//std::cout << "2" << opp.playerName;
		}
		else if (gameMode == 1)
		{
			//while (Connection::socket.NotReady);
			Connection::recievePacket(recieved);
			//std::cout << "1" << opp.playerName;
			//while (Connection::socket.NotReady);
			Connection::sendPacket(sent);
		}
	}
	void excgInt(sf::Int32& sent, sf::Int32& recieved)
	{
		if (gameMode == 2)
		{
			//while (Connection::socket.NotReady);
			Connection::sendPacket(sent);
			//while (Connection::socket.NotReady);
			Connection::recievePacket(recieved);
			//std::cout << "2" << opp.playerName;
		}
		else if (gameMode == 1)
		{
			//while (Connection::socket.NotReady);
			Connection::recievePacket(recieved);
			//std::cout << "1" << opp.playerName;
			//while (Connection::socket.NotReady);
			Connection::sendPacket(sent);
		}
	}
	void excgBool(bool& sent, bool& recieved)
	{
		if (gameMode == 2)
		{
			//while (Connection::socket.NotReady);
			cout << "srSnd\n";
			Connection::sendPacket(sent);
			//while (Connection::socket.NotReady);
			cout << "srRec\n";
			Connection::recievePacket(recieved);
			//std::cout << "2" << opp.playerName;
		}
		else if (gameMode == 1)
		{
			//while (Connection::socket.NotReady);
			cout << "clRec\n";
			Connection::recievePacket(recieved);
			//std::cout << "1" << opp.playerName;
			//while (Connection::socket.NotReady);
			cout << "clSnd\n";
			Connection::sendPacket(sent);
		}
	}
	int startup()
	{
		sf::Text txt; sf::Event ev;
		std::string inputStr="";
		txt.setFillColor(sf::Color::Yellow);
		txt.setFont(g.font);
		txt.setOrigin(-100, -200);
		txt.setCharacterSize(36);
		bool ex = 0,takeText=true;
		while (g.window.isOpen()&&!ex)
		{
			while (g.window.pollEvent(ev) || inputStr.size() == 0)
			{
				g.window.clear(sf::Color(0, 0, 0));
				g.window.draw(sf::Sprite(g.bck));
				if (ev.type == sf::Event::Closed)
				{
					g.window.close();
					gameQuit = true;
				}
				if (ev.type == sf::Event::LostFocus)
					takeText = false;
				if (ev.type == sf::Event::GainedFocus)
					takeText = true;
				if (ev.type == sf::Event::KeyPressed)
				{
					if (ev.key.code == sf::Keyboard::Enter && inputStr.size() > 0)
						ex = 1;
					else if (ev.key.code == sf::Keyboard::BackSpace && inputStr.size() > 0)
						inputStr.pop_back();
				}
				else if (ev.type == sf::Event::TextEntered &&takeText)
				{
					if (ev.text.unicode >= 32 && ev.text.unicode <= 127)
						inputStr += ev.text.unicode;
				}
				txt.setString("Welcome, Please Enter Your Name: " + inputStr);
				g.window.draw(txt);
				g.window.display();
			}
		}
		P.playerName = inputStr;
		return gameMode;
		//bool nxt = false;
		//txt.setString("Roll the dice (Spacebar): ");
		//int x = -1,dno1=1,dno2=3;
		//sf::Time timeSinceLastUpdate = sf::Time::Zero, timePerFrame(sf::seconds(1.f / 60.f));
		//while (g.window.isOpen())
		//{
		//	timeSinceLastUpdate += clock.restart();
		//	while (g.window.pollEvent(ev))
		//	{
		//		if (ev.type == sf::Event::Closed)
		//			g.window.close();
		//		if (ev.type == sf::Event::KeyPressed)
		//		{
		//			switch (ev.key.code)
		//			{
		//			case sf::Keyboard::Space:
		//				if (!diceRoll && !rollLock)
		//				{
		//					diceRoll = (!diceRoll);
		//					rollTime = 120;
		//					nxt = false;
		//					rollLock = 1;
		//				}
		//			}
		//		}
		//	}
		//	//timeSinceLastUpdate += clock.restart();
		//	while (timeSinceLastUpdate >= timePerFrame)
		//	{
		//		timeSinceLastUpdate -= timePerFrame;
		//		if (diceRoll && rollTime > 0)
		//		{
		//			dno1 = dice.roll();
		//			dno1++;
		//			rollTime--;
		//			if (rollTime <= 0)
		//			{
		//				diceRoll = !diceRoll;
		//				rollTime = 120;
		//				rollLock = 0;
		//				nxt = true;
		//			}
		//		}
		//		if (dno1 > dno2 && nxt)
		//			txt.setString(P.playerName + " will move first ! ! ! ");
		//		else if (dno1 < dno2 && nxt)
		//			txt.setString(opp.playerName + " will move first ! ! ! ");
		//		else if (dno1 == dno2 && nxt)
		//		{
		//			txt.setString("Same number, Roll Again"); nxt = false;
		//		}
		//	}
		//	g.window.clear(sf::Color(0, 0, 0));
		//	g.window.draw(sf::Sprite(g.bck));
		//	dice.draw2(g.window, dno1-1,sf::Vector2f(480,340));
		//	g.window.draw(txt);
		//	g.window.display();
		//	if (dno1 < dno2&&nxt)
		//	{
		//		Sleep(1000);
		//		return 0;
		//	}
		//	else if (dno1 > dno2&& nxt)
		//	{
		//		Sleep(1000);
		//		return 1;
		//	}
		//}
	}
	void WinState(sf::Event& eve, int pno)
	{
		sf::Text msg,msg2;
		msg.setFont(g.font);
		msg2.setFont(g.font);
		msg.setFillColor(sf::Color::Yellow);
		msg.setPosition(250, 270);
		msg2.setPosition(270, 400);
		msg2.setFillColor(sf::Color::Black);
		msg2.setCharacterSize(20);
		msg.setCharacterSize(48);
		while (g.window.isOpen())
		{
			while (!g.window.pollEvent(eve))
			{
				g.window.clear(sf::Color(0, 0, 0));
				g.window.draw(sf::Sprite(g.bck));
				if (pno == 1)
					msg.setString("Congratulation "+P.playerName+",\n\t\tYOU WON !!!");
				else
					msg.setString("YOU LOST :/");
				if(gameMode>0)
					msg2.setString("\nPress Esc to return to Main Menu . . .");
				else
					msg2.setString("\nPress Any key to Play Again, or ESC to Quit . . .");
				g.window.draw(msg);
				g.window.draw(msg2);
				g.window.display();
			}
			if (eve.type == sf::Event::Closed)
			{
				gameQuit = true;
				g.window.close();
			}
			if (eve.type == sf::Event::KeyPressed)
			{
				switch (eve.key.code)
				{
				case sf::Keyboard::Escape:
					gameQuit = true;
					return;
				case sf::Keyboard::C:
					Screenshot();
					break;
				default:
					if (gameMode > 0)
						gameQuit = true;
					else
						gameQuit = false;
					return;
				}
			}
		}
	}
	int fileNo = 0;
	void Screenshot()
	{
		string fileName = "";
		sf::Vector2u windowSize = g.window.getSize();
		sf::Texture texture;
		texture.create(windowSize.x, windowSize.y);
		texture.update(g.window);
		sf::Image screenshot = texture.copyToImage();
		fileName = "Resources/Screenshots/" + to_string(fileNo) + ".jpg";
		ifstream iff(fileName);
		while (iff.is_open() == true)
		{
			iff.close();
			fileNo++;
			fileName = "Resources/Screenshots/" + to_string(fileNo) + ".jpg";
			iff.open(fileName);
		}
		fileNo++;
		screenshot.saveToFile(fileName);
		cout << "\n--> Screenshot Saved at " << fileName << endl;
	}
	void menuProc()
	{
	strt:
		int status = 0;
		int s = menu();
		gameMode = s;
		if (s != 3)
			gameQuit = false;
		if (s == 3)
		{
			gameQuit = true;
			g.window.close();
		}
		else if (s == 1 || s == 2)
		{
			if (s == 1)
				status = Connection::runTcpClient(g);
			else if (s == 2)
				status = Connection::runTcpServer(g);
			if (status == -1)
				goto strt;
		}
		if (status == 0&&!gameQuit)
		{
			int x = startup();
			if (x == 0 || x == 1)
				opp.updateTurn();
			else if (x == 2)
				P.updateTurn();
		}
	}
	void GamePlay()
	{
		menuProc();
		 
		while (!gameQuit&&g.window.isOpen())
		{
			run();
			if (gameMode > 0)
			{	
				listener.close();
				socket.disconnect();
			}
			if (g.window.isOpen())
			{
				rollLock = false;
				diceRoll = false;
				rollTime = 0;
				slMode = 0;
				dno = 1;
				P.reset();
				opp.reset();
				if (gameQuit)
				{
					winner = -1;
					menuProc();
				}
			}
		}
		g.window.close();
	}

	~SnakeAndLadders()
	{

	}
};