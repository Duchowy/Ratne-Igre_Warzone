#ifndef SQUADRONS_H
#define SQUADRONS_H
#include "common.hpp"
#include "main.hpp"

struct UnitSquadButton : public Button
{

void spawn(Asset * asset, PlayerData * data, short ID, int list_number);
};


struct SquadButton : public Button
{

SquadButton(Asset * asset, int list_number);
};

GameState squadrons(Asset * asset, PlayerData * data);
void squadMount(PlayerData * data, short squad_ID , short slot_ID, short unit_ID);
#endif