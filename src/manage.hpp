#ifndef MANAGE_H
#define MANAGE_H
#include "common.hpp"
#include "main.hpp"

GameState manage(Asset * asset, PlayerData * data);

class UnitButton : public Button
{
    public:
sf::Text health;
sf::RectangleShape health_bar;

void draw(sf::RenderTarget& target);
void spawn(Asset * asset, std::vector<UnitInventory>::iterator object, int list_number);
};


class ItemButton : public Button
{
    public:
    sf::Text traits;
    sf::Text prompt;
    void draw(sf::RenderTarget& target);
    void spawn(Asset * asset, std::vector<ItemInst>::iterator object, int list_number);
};




int unitInfo(Asset * asset, PlayerData * data, int ID, bool * quit);
int unitSelect(Asset * asset, PlayerData * data);
int itemSelect(Asset * asset, PlayerData * data, bool * quit);
#endif