#include "unit.hpp"
#include "main.hpp"
#include "manage.hpp"

const char * ItemInst::get_name()
{

    switch(this->type)
    {
        case ItemType::EMPTY: return "Empty"; break;
        case ItemType::PLATE: return "Steel Plates"; break;
        case ItemType::ERA: 
        if(this->level < 4) return "Light ERA"; else return "Heavy ERA";break;
        case ItemType::NERA: return "NERA"; break;
        case ItemType::APS: return "Active Protection System"; break;
        case ItemType::NVG: return "Night Vision"; break;
        case ItemType::IRT: return "Thermal Vision"; break;
        case ItemType::LWR: return "Laser Warning Receiver"; break;
        case ItemType::FLT: return "Fuel Tanks"; break;
        default: return "NAME?"; break;
    }
}


const char * Weapon::get_name()
{
    switch(this->type)
    {
        case WeaponType::CANNON: return "Cannon"; break;
        case WeaponType::AUTOCANNON: return "Autocannon"; break;
        case WeaponType::MG: return "MG"; break;
        case WeaponType::ATGM: return "ATGM"; break;
        default: return "NAME?"; break;
    }
}


const char * UnitInventory::get_name()
{


    switch(this->type)
    {
        case EMPTY: return "EMPTY"; break;
        case UnitType::T55: return "T-55"; break;
        case UnitType::M48: return "M48"; break;
        case UnitType::BMP1: return "BMP-1"; break;
        case UnitType::LAV25: return "LAV-25"; break;
        default: return "NAME?"; break;
    }
}


std::string ItemInst::extract_traits()
{
    std::string text;
if(!this) return text;
for(int i = 0; i < this->bonus.size(); i++)
{
if( std::get<1>(this->bonus.at(i))) //multiplicative
{
    int value = 100*std::get<2>(this->bonus.at(i));
    text +=  std::to_string(value) + "% ";

    switch (std::get<0>(this->bonus.at(i)))
    {
        case 101: text += "Turret front HEAT protection"; break;
        case 102: text += "Turret side HEAT protection"; break;
        case 103: text += "Hull front HEAT protection"; break;
        case 104: text += "Hull side HEAT protection"; break;
        case 105: text += "Turret front AP protection"; break;
        case 106: text += "Turret side AP protection"; break;
        case 107: text += "Hull front AP protection"; break;
        case 108: text += "Hull side AP protection"; break;
        case 109: text += "APS effectivness"; break;
    }
}
else
{ //additive
if(std::get<2>(this->bonus.at(i)) >= 0.f) text += "+";
int value = std::get<2>(this->bonus.at(i));
text +=  std::to_string(value) + " ";

    switch (std::get<0>(this->bonus.at(i)))
    {
        case 0: text += "max health"; break;
        case 1: text += "front turret armor"; break;
        case 2: text += "side turret armor"; break;
        case 3: text += "front hull armor"; break;
        case 4: text += "side hull armor"; break;
        case 5: text += "accuracy"; break;
        case 6: text += "max fuel"; break;
        case 7: text += "mobility"; break;
        case 8: text += "vision"; break;
        case 9: text += "evansion"; break;
        case 10: text += "night vision"; break;
        case 110: text += "dodge"; break;
        default: text += "NAME?"; break;
    }





}

text+= "\n";

}
return text;
}

void ItemInst::spawn(short ID, ItemType type, short level)
{
    *this = ItemInst{
            .OwnerID = 0,
            .ID = ID,
            .type = type,
            .level = level,
            .bonus = std::vector<std::tuple<short,bool,float>>()
        };

    switch(type)
    {
        case PLATE:
        switch(level)
        {
            case 0: //front armor boosted
            this->bonus.push_back({3,false,10.f});
            break;
            case 1: //front and side armor boosted, mobility suffers
            this->bonus.push_back({3,false,20.f});
            this->bonus.push_back({4,false,10.f});
            this->bonus.push_back({7,false,-1.f});
            break;
            case 2: //front and side hull armor boosted,front turret armor boosted, mobility suffers
            this->bonus.push_back({1,false,20.f});
            this->bonus.push_back({3,false,20.f});
            this->bonus.push_back({4,false,10.f});
            this->bonus.push_back({7,false,-2.f});
            break;
        }
        break;
        case ERA:
        {
        switch(level)
        {
            case 0:
            this->bonus.push_back({101,true,0.6f}); //60% HEAT reduction turret front
            this->bonus.push_back({103,true,0.6f}); //60% HEAT reduction hull front
            this->bonus.push_back({105,true,0.05f});
            this->bonus.push_back({107,true,0.05f});
            break;
            case 1:
            this->bonus.push_back({101,true,0.6f});
            this->bonus.push_back({103,true,0.6f});
            this->bonus.push_back({104,true,0.6f});
            this->bonus.push_back({105,true,0.05f});
            this->bonus.push_back({107,true,0.05f});
            this->bonus.push_back({108,true,0.05f});
            this->bonus.push_back({7,false,-1.f});
            break;
            case 2:
            this->bonus.push_back({101,true,0.8f});
            this->bonus.push_back({103,true,0.8f});
            this->bonus.push_back({105,true,0.08f});
            this->bonus.push_back({107,true,0.08f});
            break;
            case 3:
            this->bonus.push_back({101,true,0.8f});
            this->bonus.push_back({103,true,0.8f});
            this->bonus.push_back({104,true,0.8f});
            this->bonus.push_back({105,true,0.08f});
            this->bonus.push_back({107,true,0.08f});
            this->bonus.push_back({108,true,0.08f});
            this->bonus.push_back({7,false,-2.f});
            break;
            case 4:
            this->bonus.push_back({101,true,0.8f}); //60% HEAT reduction turret front
            this->bonus.push_back({103,true,0.8f}); //60% HEAT reduction hull front
            this->bonus.push_back({105,true,0.35f});
            this->bonus.push_back({107,true,0.35f});
            break;
            case 5:
            this->bonus.push_back({101,true,0.8f});
            this->bonus.push_back({103,true,0.8f});
            this->bonus.push_back({104,true,0.6f});
            this->bonus.push_back({105,true,0.35f});
            this->bonus.push_back({107,true,0.35f});
            this->bonus.push_back({108,true,0.05f});
            this->bonus.push_back({7,false,-1.f});
            break;
            case 6:
            this->bonus.push_back({101,true,0.9f});
            this->bonus.push_back({103,true,0.9f});
            this->bonus.push_back({105,true,0.45f});
            this->bonus.push_back({107,true,0.45f});
            break;
            case 7:
            this->bonus.push_back({101,true,0.9f});
            this->bonus.push_back({103,true,0.9f});
            this->bonus.push_back({104,true,0.6f});
            this->bonus.push_back({105,true,0.45f});
            this->bonus.push_back({107,true,0.45f});
            this->bonus.push_back({108,true,0.05f});
            this->bonus.push_back({7,false,-2.f});
            break;
        }
        }
        break;
        case NERA:
        switch(level)
        {
            case 0:
            this->bonus.push_back({101,true,0.6f}); //60% HEAT reduction turret front
            this->bonus.push_back({103,true,0.6f}); //60% HEAT reduction hull front
            break;
            case 1:
            this->bonus.push_back({101,true,0.6f});
            this->bonus.push_back({103,true,0.6f});
            this->bonus.push_back({104,true,0.6f});
            break;
            case 2:
            this->bonus.push_back({101,true,0.8f});
            this->bonus.push_back({103,true,0.8f});
            break;
            case 3:
            this->bonus.push_back({101,true,0.8f});
            this->bonus.push_back({103,true,0.8f});
            this->bonus.push_back({104,true,0.8f});
            this->bonus.push_back({7,false,-1.f});
            break;
        }
        break;
        case APS:
        switch(level)
        {
            case 0:
            this->bonus.push_back({109,true,0.2f});
            this->bonus.push_back({9,false,-1.f});
            break;
            case 1:
            this->bonus.push_back({109,true,0.6f});
            this->bonus.push_back({9,false,-2.f});
            break;
            case 2:
            this->bonus.push_back({109,true,0.8f});
            this->bonus.push_back({9,false,-1.f});
            break;
        }
        break;
        case LWR:
        switch(level)
        {
            case 0:
            this->bonus.push_back({9,false,1.f});
            break;
            case 1:
            this->bonus.push_back({9,false,2.f});
            break;
            case 2:
            this->bonus.push_back({9,false,2.f});
            this->bonus.push_back({110,false,1.f});
            break;
        }
        break;
        case NVG:
        switch(level)
        {
            case 0:
            this->bonus.push_back({10,false,1.f});
            this->bonus.push_back({9,false,-1.f});
            break;
            case 1:
            this->bonus.push_back({10,false,2.f});
            break;
            case 2:
            this->bonus.push_back({10,false,4.f});
            break;
        }
        break;
        case IRT:
        switch(level)
        {
            case 0:
            this->bonus.push_back({8,false,1.f});
            this->bonus.push_back({10,false,1.f});
            break;
            case 1:
            this->bonus.push_back({8,false,2.f});
            this->bonus.push_back({10,false,2.f});
            break;
            case 2:
            this->bonus.push_back({8,false,2.f});
            this->bonus.push_back({10,false,4.f});
            break;
        }
        break;
        case FLT:
        switch(level)
        {
            case 0:
            this->bonus.push_back({6,false,1.f});
            break;
            case 1:
            this->bonus.push_back({6,false,2.f});
            break;
            case 2:
            this->bonus.push_back({6,false,3.f});
            this->bonus.push_back({7,false,-1.f});
            break;
        }
        break;
        default: break;








    }
}



void UnitInventory::spawn( short type, short ID)
{
switch(type)
{
    case T55:
    {
        *this = UnitInventory{
            .level = 0,
            .type = type,
            .Owner_ID = 0,
            .ID = ID,
            .HP = {120,120},
            .armor = {{200,120,60},{180,80,50}},
            .accuracy = 2,
            .fuel = {5,5},
            .mobility = 1,
            .vision = 2,
            .evansion = 1,
            .weapon = { 
                {.type = WeaponType::CANNON,
                .range = 5,
                .damage = 150,
                .penetration = 200, //390?
                .energyCoef = 0.8,
                .isATGM = false
                },
                {.type = WeaponType::MG,
                .range = 2,
                .damage = 20,
                .penetration = 5,
                .energyCoef = 0.8,
                .isATGM = false
                }
            },
            .item_q = std::vector<short>(3)
        };
        
    }
    break;
    case M48:
    {
        *this = UnitInventory{
            .level = 0,
            .type = type,
            .Owner_ID = 0,
            .ID = ID,
            .HP = {110,110},
            .armor = {{200,100,60},{180,76,50}},
            .accuracy = 2,
            .fuel = {7,7},
            .mobility = 2,
            .vision = 2,
            .evansion = 1,
            .weapon = { 
                {.type = WeaponType::CANNON,
                .range = 5,
                .damage = 140,
                .penetration = 200, //more?
                .energyCoef = 0.8,
                .isATGM = false
                },
                {.type = WeaponType::MG,
                .range = 2,
                .damage = 20,
                .penetration = 5,
                .energyCoef = 0.8,
                .isATGM = false
                }
            },
            .item_q = std::vector<short>(3)
        };
    }
    break;
    case UnitType::BMP1:
    {
        *this = UnitInventory{
            .level = 0,
            .type = type,
            .Owner_ID = 0,
            .ID = ID,
            .HP = {80,80},
            .armor = {{25,20,15},{30,20,15}},
            .accuracy = 2,
            .fuel = {7,7},
            .mobility = 3,
            .vision = 3,
            .evansion = 3,
            .weapon = { 
                {.type = WeaponType::CANNON,
                .range = 5,
                .damage = 80,
                .penetration = 300,
                .energyCoef = 1.0,
                .isATGM = false
                },
                {.type = WeaponType::MG,
                .range = 2,
                .damage = 20,
                .penetration = 5,
                .energyCoef = 0.8,
                .isATGM = false
                }
            },
            .item_q = std::vector<short>(3)
        };
    }
    break;
    case UnitType::LAV25:
    {
        *this = UnitInventory{
            .level = 0,
            .type = type,
            .Owner_ID = 0,
            .ID = ID,
            .HP = {110,110},
            .armor = {{15,10,10},{15,10,10}},
            .accuracy = 2,
            .fuel = {7,7},
            .mobility = 3,
            .vision = 4,
            .evansion = 3,
            .weapon = { 
                {.type = WeaponType::AUTOCANNON,
                .range = 5,
                .damage = 30,
                .penetration = 60, //more?
                .energyCoef = 0.8,
                .isATGM = false
                },
                {.type = WeaponType::MG,
                .range = 2,
                .damage = 20,
                .penetration = 5,
                .energyCoef = 0.8,
                .isATGM = false
                }
            },
            .item_q = std::vector<short>(3)
        };
    }
    break;


}


}

short UnitInventory::get_item_slot(short ID)
{
    for(int i = 0; i < this->item_q.size(); i++)
    {
        if(item_q.at(i) == ID) return i;
    }
return -1;
}

