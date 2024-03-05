#include "Connection.h"
#include"GUI.h"
#include<SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include<string>
Connection::Connection()
{
    info.setCharacterSize(24);
    threadReturn = false;
    threadReturn1 = false;
    info.setFillColor(sf::Color::Black);
    info.setPosition(250, 270); 
    portNo = 5001;
    type = 0;
}
int Connection::runTcpServer(GUI& targ)
{
    type = 0;
    listener.close();
    // Create a server socket to accept new connections
    info.setFont(targ.font);
    int status = 0;
    // Listen to the given port for incoming connections
    if (listener.listen(portNo) != sf::Socket::Done)
    {
        info.setString("No Connections");
        status = -1;
    }
    else
    {
        info.setString("Your Server address : " + sf::IpAddress::getLocalAddress().toString() + "\n, share this with your friend.\nWaiting for connections... ");
        std::cout << "Your Server address: " << sf::IpAddress::getLocalAddress() << "\n,share this with your friend.\n";
        std::cout << "\nWaiting for connections... " << std::endl;
    }
    targ.window.clear();
    targ.window.draw(sf::Sprite(targ.bck));
    targ.window.draw(info);
    targ.window.display();
    // Wait for a connection
    if (status == 0)
    {
        if (listener.accept(socket) != sf::Socket::Done)
        {
            status = -1;
            info.setString("Connection Error");
        }
        else
        {
            std::cout << "Client connected: " << socket.getRemoteAddress() << std::endl;
            info.setString("Player connected: " + socket.getRemoteAddress().toString());
            socket.setBlocking(true);
        }
        targ.window.clear();
        targ.window.draw(sf::Sprite(targ.bck));
        targ.window.draw(info);
        targ.window.display();
    }
    Sleep(3000);
    return status;
    // Send a message to the connected client

	// Receive a message back from the client
}

int Connection::runTcpClient(GUI&targ)
{
    type = 1;
    int status=0;
    string tmp = "";
    sf::Event ev;
    int ex = 0,attempNo=1;
    info.setFont(targ.font);
    // Ask for the server address
    while (targ.window.isOpen()&&!ex)
    {
        while ((targ.window.pollEvent(ev) || tmp.size() == 0)&&!ex)
        {
            targ.window.clear(sf::Color(0, 0, 0));
            targ.window.draw(sf::Sprite(targ.bck));
            if (ev.type == sf::Event::KeyPressed)
            {
                if (ev.key.code == sf::Keyboard::Enter && tmp.size() > 0)
                {
                    server = tmp;
                    tmp = "";
                    if (server == sf::IpAddress::None)
                    {
                        ex = 0;
                        attempNo++;
                    }
                    else
                        ex = 1;
                }
                else if (ev.key.code == sf::Keyboard::BackSpace && tmp.size() > 0)
                    tmp.pop_back();
            }
            else if (ev.type == sf::Event::TextEntered)
            {
                if (ev.text.unicode >= 32 && ev.text.unicode <= 127)
                    tmp += ev.text.unicode;

            }
            info.setString("Attempt#"+to_string(attempNo)+": Type the address of the server to connect to:\n" + tmp);
            targ.window.draw(info);
            targ.window.display();
        }
    }
    // Create a socket for communicating with the server
    socket.disconnect();
    // Connect to the server
    if (socket.connect(server, portNo) != sf::Socket::Done)
    {
        status = -1;
        info.setString("Connection Error");
    }
    else
    {
        info.setString("Connected to server " + server.toString());
        std::cout << "Connected to server " << server << std::endl;
        socket.setBlocking(true);
    }
    targ.window.clear();
    targ.window.draw(sf::Sprite(targ.bck));
    targ.window.draw(info);
    targ.window.display();
    Sleep(3000);
    return status;
    // Receive a message from the server

    // Send an answer to the server
}
int Connection::sendPacket(sf::Int32 i)
{
    packet << i;
    int atNo = 0, stat = socket.send(packet);
    while (stat != sf::Socket::Done && atNo < 5)
    {
        stat = socket.send(packet);
        atNo++;
        cout << "Connecting . . . Attempt " + to_string(atNo) + "/5";
    }
    packet.clear();
    return stat;
}
int Connection::sendPacket(std::string s)
{
    packet << s;
    int atNo = 0, stat = socket.send(packet);
    while (stat != sf::Socket::Done && atNo < 5)
    {
        stat = socket.send(packet);
        atNo++;
        cout << "Connecting . . . Attempt " + to_string(atNo) + "/5";
    }
    packet.clear();
    return stat;

}
int Connection::sendPacket(bool s)
{
    packet << s;
    int atNo = 0, stat = socket.send(packet);
    while (stat != sf::Socket::Done && atNo < 5)
    {
        stat = socket.send(packet);
        atNo++;
        cout << "Connecting . . . Attempt " + to_string(atNo) + "/5";
    }
    packet.clear();
    return stat;

}

int Connection::recievePacket(sf::Int32& i)
{
    int atNo = 0,stat=socket.receive(packet);
    while (stat != sf::Socket::Done && atNo < 5)
    {
        stat = socket.receive(packet);
        atNo++;
        cout << "Connecting . . . Attempt " + to_string(atNo) + "/5";
    }
    packet >> i;
    packet.clear();
    return stat;

}
int Connection::recievePacket(std::string& s)
{
    int atNo = 0, stat = socket.receive(packet);
    while (stat != sf::Socket::Done && atNo < 5)
    {
        stat = socket.receive(packet);
        atNo++;
        cout << "Connecting . . . Attempt " + to_string(atNo) + "/5";
    }
    packet >> s;
    packet.clear();
    return stat;
}
int Connection::recievePacket(bool& s)
{
    int atNo = 0, stat = socket.receive(packet);
    while (stat != sf::Socket::Done && atNo < 5)
    {
        stat = socket.receive(packet);
        atNo++;
        cout<<"Connecting . . . Attempt " + to_string(atNo) + "/5";
    }
    packet >> s;
    packet.clear();
    return stat;
}

int Connection::reconnectServer(GUI& targ)
{
    return 0;
}
int Connection::reconnectClient(GUI& targ)
{
    return 0;
}