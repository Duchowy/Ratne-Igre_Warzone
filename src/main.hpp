#ifndef MAIN_H
#define MAIN_H
#include <libconfig.h++>
#include "common.hpp"
#include "unit.hpp"

enum GameState{MENU,EQUIPMENT,MANAGE,SORTIE,QUIT};

struct PlayerData{
std::vector<UnitInventory> unit_q;
std::vector<ItemInst> item_q;

std::vector<ItemInst>::iterator find_item(short ID);
std::vector<UnitInventory>::iterator find_unit(short ID);
};



class Asset{
    public:
    sf::RenderWindow display;
    sf::Font font;

    Asset(int w, int h) : display(sf::VideoMode(w,h), "Ratne Igre: Warzone"), font() {}
};



#endif