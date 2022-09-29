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
#include<fstream>
#include<random>

struct Button
{

std::vector<sf::RectangleShape> box; //the first one is always theme
std::vector<sf::Text> text; //the first one is always a name



void setBoxPosition(sf::Vector2f vector);
void setBoxPosition(size_t box_id,sf::Vector2f vector);
void setBoxPosition(size_t box_id,float x_pos,float y_pos);

void setTextPosition(size_t text_id,float x_pos,float y_pos);
void setTextPosition(size_t text_id,sf::Vector2f vector);
void setPosition(sf::Vector2f vector);
void move(sf::Vector2f vector);
sf::Vector2f getPosition();
sf::Vector2f getSize();
void centerOrigin();
void draw(sf::RenderTarget& target);
};

sf::Vector2f resizeVector(sf::Vector2f vector, float x_mult, float y_mult);

sf::FloatRect RatioRect_LenRect(sf::FloatRect rectangle, sf::Vector2f base);
sf::FloatRect RatioRect_LenRect(sf::FloatRect rectangle, sf::Vector2u base);

sf::Text sf_spawn_text(sf::Font & font, int size, const char * str);
sf::Text sf_spawn_text(sf::Font & font, int size, std::string str);



#endif