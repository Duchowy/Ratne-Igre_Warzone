#ifndef UNIT_H
#define UNIT_H
#include "common.hpp"

enum WeaponType{CANNON,AUTOCANNON,MG,ATGM};

enum UnitType{T55 = 1,M48,BMP1,LAV25,GVOZDIKA,M109};



struct Weapon{
    enum WeaponType type;
    short range;
    short damage;
    short penetration;
    float energyCoef; //1.f for HEAT warhead
    bool isATGM; //may be removed in the future
    const char * get_name();
};





/*
NVG for night vision, IRT for thermal vision, LWR for Laser Warning Receiver, APS for Active Protection System
FL for fuel tanks
*/
enum ItemType{EMPTY,PLATE,ERA,NERA,APS,LWR,NVG,IRT,FLT};



struct ItemInst
{
short OwnerID;
short ID; //only >0 are valid
enum ItemType type;
short level;
std::vector<std::tuple<short,bool,float>> bonus; //short for bonus ID, bool for additive or multiplicative, float for value

void spawn(short ID, ItemType type, short lvl);
const char * get_name();
std::string extract_traits();
};

//bool to true if multiplicative
/*############################
ITEM BONUS PARAMETER REFERENCE
############################*/
/*
Passive:

0 - HP bonus
1 - front armor turret
2 - side armor turret
3 - front armor hull
4 - side armor hull
5 - passive accuracy bonus
6 - fuel bonus
7 - mobility bonus
8 - vision bonus
9 - passive evansion bonus
10 - night vision bonus

Active:

101 - front armor pen reduction turret (ERA) (mult) (HEAT)
102 - side armor pen reduction turret (ERA) (mult) (HEAT)
103 - front armor pen reduction hull (ERA) (mult) (HEAT)
104 - side armor pen reduction hull (ERA) (mult) (HEAT)
105 - front armor pen reduction turret (ERA) (mult) (AP)
106 - side armor pen reduction turret (ERA) (mult) (AP)
107 - front armor pen reduction hull (ERA) (mult) (AP)
108 - side armor pen reduction hull (ERA) (mult) (AP)
109 - APS

110 - active evansion bonus
*/





class UnitInventory{
    public:
        short level;
        short type;
        short Owner_ID;
        short ID; // only >0 are valid
        short HP[2]; //0 for base, 1 for alter
        short armor[2][3]; //0 for turret, 1 for hull
        short accuracy;
        short fuel[2]; //fuel
        short mobility;
        short vision; //max vision range
        short evansion;

        struct Weapon weapon[2]; //primary and secondary
        std::vector<short> item_q; //stores item IDs

        void spawn(short type,short ID);
        const char * get_name();
        short get_item_slot(short ID);
};

/*###########
# INSTANCES #
###########*/



enum Rotation{UP,DOWN,LEFT,RIGHT};

struct Position
{
    short x;
    short y;
    enum Rotation direction;
};

struct UnitInst{
short type;
short ID;
short HP;
short target_ID;
struct Position pos; //max fuel
short fuel[2]; //current fuel
short mobility; //current tile movement that's left
short vision;
};







#endif