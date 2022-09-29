#include "main.hpp"
#include "common.hpp"
#include "manage.hpp"
#include "unit.hpp"
#include "squadrons.hpp"




void PlayerData::file_storage(bool isSaved)
{
std::fstream file;
if(isSaved)
{


    size_t number = 0;
    for(int i = 0; i < this->unit_q.size(); i++) if(this->unit_q.at(i).ID > 0) number+=1;


    file.open("savefile.sav",std::ios::trunc | std::ios::out | std::ios::binary);
    if(file.good())
    {
        //currency section
        file.write(reinterpret_cast<const char *>(&this->currency[0]),5*sizeof(unsigned long long));
        //unit section

        file << number;
        for(int i = 0; i < this->unit_q.size(); i++)
        {
        if(this->unit_q.at(i).ID <= 0) continue;
        file.write(reinterpret_cast<const char *>(&this->unit_q.at(i)),18 * sizeof(short));
        file.write(reinterpret_cast<const char *>(this->unit_q.at(i).weapon), 2 * sizeof(Weapon));
        size_t item_q_size = this->unit_q.at(i).item_q.size();
        file.write(reinterpret_cast<const char *>(&item_q_size),sizeof(size_t));
        file.write(reinterpret_cast<const char *>(&this->unit_q.at(i).item_q.at(0)),item_q_size * sizeof(short));
        }

        //equipment section section
        number = 0;
        for(int i = 0; i < this->item_q.size(); i++) if(this->item_q.at(i).ID > 0) number +=1;
        file << number;
        for(int i = 0; i < this->item_q.size(); i++)
        {
            if(this->item_q.at(i).ID <= 0) continue;
            file.write(reinterpret_cast<const char *>(&this->item_q.at(i)),2*sizeof(short));
            file.write(reinterpret_cast<const char *>(&this->item_q.at(i).type),sizeof(ItemType));
            file.write(reinterpret_cast<const char *>(&this->item_q.at(i).level),sizeof(short));
            size_t item_q_size = this->item_q.at(i).bonus.size();
            file.write(reinterpret_cast<const char *>(&item_q_size),sizeof(size_t));
            for(int q = 0; q < item_q_size; q++)
            {
                short type = std::get<0>(this->item_q.at(i).bonus.at(q));
                bool indicator = std::get<1>(this->item_q.at(i).bonus.at(q));
                float value = std::get<2>(this->item_q.at(i).bonus.at(q));
                file.write(reinterpret_cast<const char *>(&type),sizeof(short));
                file.write(reinterpret_cast<const char *>(&indicator),sizeof(bool));
                file.write(reinterpret_cast<const char *>(&value),sizeof(float));
            }


        }
        file.close();
    }
    
}
else
{
    file.open("savefile.sav",std::ios::in | std::ios::binary);
    if(file.good())
    {
        //currency section

        file.read(reinterpret_cast<char *>(&this->currency[0]),5 * sizeof(unsigned long long));

        //unit section

        size_t elem_num;
        file >> elem_num;
        for(int i =0; i < elem_num; i++)
        {
        UnitInventory buffer;
        file.read(reinterpret_cast<char *>(&buffer),18 * sizeof(short));
        file.read(reinterpret_cast<char *>(buffer.weapon),2 * sizeof(Weapon));

        size_t item_q_num;
        file.read(reinterpret_cast<char *>(&item_q_num),sizeof(size_t));
        buffer.item_q = std::vector<short>(item_q_num);
        file.read(reinterpret_cast<char *>(&(buffer.item_q.front())),item_q_num * sizeof(short));
        this->unit_q.push_back(buffer);
        }

        //equipment section



        file >> elem_num;
        for(int i =0; i < elem_num; i++)
        {
            ItemInst buffer;
            file.read(reinterpret_cast<char *>(&buffer),2*sizeof(short));
            file.read(reinterpret_cast<char *>(&buffer.type),sizeof(ItemType));
            file.read(reinterpret_cast<char *>(&buffer.level),sizeof(short));
            size_t num_of_traits;
            file.read(reinterpret_cast<char *>(&num_of_traits),sizeof(size_t));
            for(int q = 0; q < num_of_traits; q++)
            {
                short type;
                bool indicator;
                float value;
                file.read(reinterpret_cast<char *>(&type),sizeof(short));
                file.read(reinterpret_cast<char *>(&indicator),sizeof(bool));
                file.read(reinterpret_cast<char *>(&value),sizeof(float));
                buffer.bonus.push_back({type,indicator,value});
            }  
        this->item_q.push_back(buffer);
        }



        file.close();
    }
    
}



}


GameState menu(Asset * asset);

int main()
{
PlayerData * data = new PlayerData;
std::random_device seed_generator;
Asset * asset = new Asset(1000,500,seed_generator());


data->item_q.push_back(
ItemInst{
            .OwnerID = 0,
            .ID = 0,
            .type = EMPTY,
            .level = -1,
            .bonus = std::vector<std::tuple<short,bool,float>>()
        }
); //empty piece
//data->file_storage(0);




while(data->unit_q.size() < 5)
{
std::uniform_int_distribution<std::mt19937::result_type> dist4(1,4);
UnitInventory buffer;
buffer.spawn(dist4(asset->generator),data->get_unit_ID());
data->unit_q.push_back(buffer);
}

while(data->squad_q.size() < 3)
{
    data->squad_q.push_back({{0,0,0,0,0}});
}



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
        case GameState::MENU: state = menu(asset); break;
        case GameState::UNIT_MANAGE: state = manage(asset,data); break;
        case GameState::SQUAD_MANAGE: state = squadrons(asset,data); break;
        default: quit = true; break;
    }
}



asset->display.close();
//data->file_storage(1);
delete asset;
}





void render(sf::RenderWindow & display,std::array<Button,4> & button)
{
for(int i = 0; i < button.size(); i++) 
{
    button[i].draw(display);
}

display.display();
display.clear(sf::Color(0,27,27,255));
}


void update_button_pos(sf::RenderWindow & display,std::array<Button,4> & button)
{
button[0].setPosition(sf::Vector2f(display.getSize().x - button[0].box.front().getSize().x,display.getSize().y * 2 / 6));
button[1].setPosition(button[0].getPosition() + sf::Vector2f(-button[0].box.front().getSize().x/4 -5,button[0].box.front().getSize().y+10));
button[2].setPosition(button[0].getPosition() + sf::Vector2f(button[0].box.front().getSize().x/4 + 5,button[0].box.front().getSize().y+10));
button[3].setPosition(sf::Vector2f(button[0].getPosition().x, button[1].getPosition().y + button[1].box.front().getSize().y+10) );
}




GameState menu(Asset * asset)
{

sf::View camera(sf::FloatRect(0,0,asset->display.getSize().x,asset->display.getSize().y));
asset->display.setView(camera);


std::array<Button,4> button = {{
    {
        .box = std::vector<sf::RectangleShape>(1),
        .text = std::vector<sf::Text>(1)
    },
    {
        .box = std::vector<sf::RectangleShape>(1),
        .text = std::vector<sf::Text>(1)
    },
    {
        .box = std::vector<sf::RectangleShape>(1),
        .text = std::vector<sf::Text>(1)
    },
    {
        .box = std::vector<sf::RectangleShape>(1),
        .text = std::vector<sf::Text>(1)
    }
}};

button[0].text[0] = sf_spawn_text(asset->font,12,"Sortie");
button[1].text[0] = sf_spawn_text(asset->font,12,"Squadrons");
button[2].text[0] = sf_spawn_text(asset->font,12,"Units");
button[3].text[0] = sf_spawn_text(asset->font,12,"Base");

button[0].box[0] = sf::RectangleShape(sf::Vector2f(280,120));
button[1].box[0] = sf::RectangleShape(sf::Vector2f(135,100));
button[2].box[0] = sf::RectangleShape(sf::Vector2f(135,100));
button[3].box[0] = sf::RectangleShape(sf::Vector2f(280,120));

for(int i = 0; i < button.size(); i++) 
{
button[i].centerOrigin();
button[i].setTextPosition(0,0.5f,0.5f);
button[i].box.front().setFillColor(sf::Color(27,27,0,255));
}

update_button_pos(asset->display,button);




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
                        if(button[i].box.front().getGlobalBounds().contains(translated_pos.x,translated_pos.y))
                        {
                            pressed = i;
                            break;
                        }
                    }
                    switch(pressed)
                    {
                        case 0: return SORTIE; break;
                        case 1: return SQUAD_MANAGE; break;
                        case 2: return UNIT_MANAGE; break;
                        case 3: return BASE; break;
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




short PlayerData::get_unit_ID()
{
std::vector<short> buffer;
for(int i = 0; i < this->unit_q.size(); i++) buffer.push_back(this->unit_q.at(i).ID);
std::sort(buffer.begin(),buffer.end());
short i = 0;
for(; i < buffer.size(); i++)
{
    if(buffer.at(i) != i+1) break;
}
return i+1;
}