#ifndef COMMON_H
#define COMMON_H

#include <SFML/Graphics.hpp>
#include<array>
#include<vector>
#include<tuple>
#include<omp.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<time.h>
#include<algorithm>
#include<random>

class Button
{
    public:
sf::RectangleShape box;
sf::Text text;



void setPosition(sf::Vector2f vector, short pos);
void move(sf::Vector2f vector);
sf::Vector2f getPosition();
void centerOrigin();
void draw(sf::RenderTarget& target);
};







sf::Text sf_spawn_text(sf::Font & font, int size, const char * str);
sf::Text sf_spawn_text(sf::Font & font, int size, std::string str);

#endif