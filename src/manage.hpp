#ifndef MANAGE_H
#define MANAGE_H
#include "common.hpp"
#include "main.hpp"

enum ManageState{UNIT_SELECT,INFO,ITEM_SELECT};

GameState manage(Asset * asset, PlayerData * data);

class UnitButton : public Button
{
    public:
void spawn(Asset * asset, PlayerData * data, short ID, int list_number);
};


class ItemButton : public Button
{
    public:
void spawn(Asset * asset, std::vector<ItemInst>::iterator object, int list_number);
};




int unitInfo(Asset * asset, PlayerData * data, int ID, bool * quit);
int unitSelect(Asset * asset, PlayerData * data, bool * quit, bool isEmptyEnabled);
int itemSelect(Asset * asset, PlayerData * data, bool * quit);
bool equipmentMount(PlayerData * data, short unit_ID , short slot_ID, short item_ID);
bool equipmentTraitEnforce(PlayerData * data, short unit_ID , short item_ID, bool isEquipping);
#endif