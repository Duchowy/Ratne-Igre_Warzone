#ifndef MAIN_H
#define MAIN_H
#include <libconfig.h++>
#include "common.hpp"
#include "unit.hpp"

enum GameState{MENU,SQUAD_MANAGE,UNIT_MANAGE,BASE,SORTIE,QUIT};

struct PlayerData{
std::array<unsigned long long, 5> currency; //money, 50-70s parts, 70-90s parts, 90s+ parts
std::vector<std::array<short, 5>> squad_q;
std::vector<UnitInventory> unit_q;
std::vector<ItemInst> item_q;

std::vector<ItemInst>::iterator find_item(short ID);
std::vector<UnitInventory>::iterator find_unit(short ID);
short get_unit_ID();
void file_storage(bool isSaved);
};



class Asset{
    public:
    sf::RenderWindow display;
    sf::Font font;
    std::mt19937 generator;

    Asset(int w, int h, unsigned int seed) : display(sf::VideoMode(w,h), "Ratne Igre: Warzone"), font(), generator(seed) {}
};



#endif