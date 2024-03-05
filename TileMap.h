#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h> 
#include <iostream>
#include <fstream>
#include<string>
#include<cmath>
#include"GUI.h"
#include<vector>
using namespace std;
#define PI 3.14159265
class TileMap : public sf::Drawable, public sf::Transformable
{
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();
        // apply the tileset texture
        states.texture = &m_tileset;
        // draw the vertex array
        target.draw(m_vertices, states);
    }
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
public:
    void loadText(int x, int y, sf::Text txt, sf::RenderTarget& target)
    {
        txt.setOrigin(-x, -y);
        target.draw(txt);
    }
    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, sf::Text*txt, unsigned int width, unsigned int height, sf::RenderTarget&targ,vector<sf::VertexArray>&ladderList, vector<sf::VertexArray>& snakeList)
    {
        sf::VertexArray temp1(sf::Quads, 4), temp2(sf::Quads, 4);
        
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;
        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);
        // populate the vertex array, with one quad per tile
        int a = 99;
        for (unsigned int i = 0; i < width ; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];
                if (tileNumber > 1)
                    tileNumber = 2;
                else if (tileNumber < 0)
                    tileNumber = 3;
                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);
                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];
                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
                loadText((j * tileSize.y) + 10, (i * tileSize.x) + 10, txt[a], targ);
                a--;
            }
        // i: column no.   j: row no.
        for (int b = 0; b < 100; b++)
        {
            int i = b % 10, j = b / 10,xoff=0,yoff=0,i1=0,j1=0,j2=0;
            sf::Vector2f pos(i * tileSize.x, j * tileSize.y);
            if (tiles[b] > 1&&b<tiles[b])
            {
                if (i < (tiles[b] % 10))
                {
                    xoff = 20; yoff = -20; j1 = 1; j2 = 0; i1 = 1;
                }
                else
                {
                    xoff = 20; yoff = 20; j1 = 0; j2 = 1; i1 = 1;
                }
                temp1[0].position = sf::Vector2f((i * tileSize.x) + xoff, ((j+j1) * tileSize.y) + yoff);
                temp1[0].texCoords = sf::Vector2f(0, 0);
                temp1[1].position = sf::Vector2f((i + i1) * tileSize.x - xoff, (j + j2) * tileSize.y - yoff);
                temp1[1].texCoords = sf::Vector2f(14, 0);
                i = tiles[b] % 10; j = tiles[b] / 10;
                temp1[2].position = sf::Vector2f((i + i1) * tileSize.x - xoff, (j + j2) * tileSize.y - yoff);
                temp1[2].texCoords = sf::Vector2f(14, 87);
                temp1[3].position = sf::Vector2f(i * tileSize.x + xoff, (j+j1) * tileSize.y + yoff);
                temp1[3].texCoords = sf::Vector2f(0, 87);
                ladderList.push_back(temp1);
            }
            else if (tiles[b] < 0 && b<abs(tiles[b]))
            {
                int t = abs(tiles[b]);  // b start index , abs(tiles[b]) end index
                if (i < (t % 10))
                {
                    xoff = 5; yoff = -5; j1 = 1; j2 = 0; i1 = 1;
                }
                else
                {
                    xoff = 5; yoff = 5;  j1 = 0; j2 = 1; i1 = 1;
                }
                temp2[0].position = sf::Vector2f((i * tileSize.x) + xoff, ((j+j1) * tileSize.y) + yoff);
                temp2[0].texCoords = sf::Vector2f(55, 5);
                temp2[1].position = sf::Vector2f((i+i1) * tileSize.x - yoff, (j+j2) * tileSize.y - xoff);
                temp2[1].texCoords = sf::Vector2f(160, 5);
                i = t % 10; j = t / 10;
                temp2[2].position = sf::Vector2f((i+i1) * tileSize.x-xoff , (j+j2) * tileSize.y-yoff );
                temp2[2].texCoords = sf::Vector2f(160, 451);
                temp2[3].position = sf::Vector2f(i * tileSize.x+yoff , (j+j1) * tileSize.y+xoff);
                temp2[3].texCoords = sf::Vector2f(55, 451);
                snakeList.push_back(temp2);
            }
        }
        return true;
    }             
};



//Start = pos;
// End = pos;
                  // float result = atan2(abs(End.y - Start.y), abs(End.x - Start.x)) * 180 / PI;
                  // tmp1.setSize(sf::Vector2f(30, sqrt((End.x - Start.x) * (End.x - Start.x) + (End.y - Start.y) * (End.y - Start.y))));//(sf::Vector2f(abs(End.x-Start.x, abs(End.y - Start.y)));
                  // tmp1.setOrigin(sf::Vector2f(15, abs(End.y + Start.y) / 2.f));  //(sf::Vector2f(abs(End.x - Start.x)/2.f, abs(End.y - Start.y) / 2.f));
                  // tmp1.setPosition(sf::Vector2f(abs(End.x + Start.x) / 2.f, abs( End.y + Start.y) / 2.f));
                  //// tmp1.setOrigin(sf::Vector2f(15, abs(End.y + Start.y)/2));
                  // tmp1.rotate(90-result);