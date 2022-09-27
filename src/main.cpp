#include "main.hpp"
#include "common.hpp"
#include "manage.hpp"
#include "unit.hpp"




GameState menu(Asset * asset);

int main()
{

PlayerData * data = new PlayerData;
Asset * asset = new Asset(1000,500);

for(int i =0; i < 7; i++) 
{
UnitInventory buffer;
buffer.spawn(i%4,i+1);
buffer.HP[1] *= (float)(i+1) / 7;
data->unit_q.push_back(buffer);
}

data->item_q.push_back(
ItemInst{
            .OwnerID = 0,
            .ID = 0,
            .type = EMPTY,
            .level = -1,
            .bonus = std::vector<std::tuple<short,bool,float>>()
        }
); //empty piece


for(int i = 0; i < 12; i++)
{
    ItemInst buffer;
    buffer.spawn(i+1,static_cast<ItemType>(1 + i%8),i%3);
    data->item_q.push_back(buffer);
}



asset->display.setFramerateLimit(60);
asset->font.loadFromFile("font.ttf");

GameState state = GameState::MENU;
bool quit = false;

while(!quit)
{
    switch(state)
    {
        case MENU: state = menu(asset); break;
        case MANAGE: state = manage(asset,data); break;
        default: quit = true; break;
    }
}



asset->display.close();
delete asset;
}





void render(sf::RenderWindow & display,std::array<Button,3> & button)
{
for(int i = 0; i < button.size(); i++) 
{
    button[i].draw(display);
}

display.display();
display.clear(sf::Color(0,27,27,255));
}


void update_button_pos(sf::RenderWindow & display,std::array<Button,3> & button)
{
button[0].setPosition(sf::Vector2f(display.getSize().x - button[0].box.getSize().x,display.getSize().y * 2 / 5),1);
button[1].setPosition(button[0].getPosition() + sf::Vector2f(-button[0].box.getSize().x/4 -5,button[0].box.getSize().y+10),1);
button[2].setPosition(button[0].getPosition() + sf::Vector2f(button[0].box.getSize().x/4 + 5,button[0].box.getSize().y+10),1);
}




GameState menu(Asset * asset)
{

sf::View camera(sf::FloatRect(0,0,asset->display.getSize().x,asset->display.getSize().y));
asset->display.setView(camera);


std::array<Button,3> button = {{
    {
        .box = sf::RectangleShape(sf::Vector2f(280,120)),
        .text = sf_spawn_text(asset->font,12,"Sortie")
    },
    {
        .box = sf::RectangleShape(sf::Vector2f(135,100)),
        .text = sf_spawn_text(asset->font,12,"Inventory")
    },
    {
        .box = sf::RectangleShape(sf::Vector2f(135,100)),
        .text = sf_spawn_text(asset->font,12,"Squadrons")
    }
}};
update_button_pos(asset->display,button);
for(int i = 0; i < button.size(); i++) 
{
button[i].centerOrigin();
button[i].box.setFillColor(sf::Color(27,27,0,255));
}






bool quit = false;
sf::Event event;

while(!quit)
{
    while (asset->display.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed: 
            quit = true; 
            break;
            case sf::Event::Resized:
            update_button_pos(asset->display,button);
            break;
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    auto mouse_pos = sf::Mouse::getPosition(asset->display);
                    auto translated_pos = asset->display.mapPixelToCoords(mouse_pos); //map clicked position to display world

                    int pressed = -1;
                    for(int i = 0; i < button.size(); i++) 
                    {
                        if(button[i].box.getGlobalBounds().contains(translated_pos.x,translated_pos.y))
                        {
                            pressed = i;
                            break;
                        }
                    }
                    switch(pressed)
                    {
                        case 0: return SORTIE; break;
                        case 1: return EQUIPMENT; break;
                        case 2: return MANAGE; break;
                    }
                }
            break;
            default: break;
        }
    }

render(asset->display,button);

}



return QUIT;
}



std::vector<ItemInst>::iterator PlayerData::find_item(short ID)
{
    for(int i = 0; i < this->item_q.size(); i++)
    {
        if(this->item_q.at(i).ID == ID) return this->item_q.begin()+i;
    }
    return this->item_q.end();
}

std::vector<UnitInventory>::iterator PlayerData::find_unit(short ID)
{
    for(int i = 0; i < this->unit_q.size(); i++)
    {
        if(this->unit_q.at(i).ID == ID) return this->unit_q.begin()+i;
    }
    return this->unit_q.end();
}