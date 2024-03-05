#pragma once
#include<SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include"GUI.h"
#include<string>
#include<iostream>
class Connection
{
protected:
	sf::Text info;
	bool threadReturn,threadReturn1;
	int portNo;
	int type;
	// Create a server socket to accept new connections
	sf::TcpListener listener;
	// Wait for a connection on server side, or for communication with the server
	sf::TcpSocket socket;
	sf::IpAddress server;
	sf::Packet packet;
	Connection();
    int runTcpServer(GUI&targ);
	int runTcpClient(GUI& targ);
	int sendPacket(sf::Int32 i);
	int sendPacket(std::string s);
	int sendPacket(bool s);
	int recievePacket(sf::Int32 &i);
	int recievePacket(std::string&s);
	int recievePacket(bool& s);
	int reconnectServer(GUI& targ);
	int reconnectClient(GUI& targ);
};

