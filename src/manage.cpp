#include "manage.hpp"

enum ManageState{UNIT_SELECT,INFO,ITEM_SELECT};


class UnitInfo{
    public:
    sf::Text name;
    sf::Text health;
    sf::RectangleShape health_bar;
    sf::Text l_text; //mobility, vision etc. related
    sf::Text r_text; //armor & firepower

    void spawn(Asset * asset,std::vector<UnitInventory>::iterator unit);
    void draw(sf::RenderTarget& target);

};

void UnitInfo::draw(sf::RenderTarget& target)
{
target.draw(this->name);
target.draw(this->health);
target.draw(this->health_bar);
target.draw(this->l_text);
target.draw(this->r_text);
}

void UnitInfo::spawn(Asset * asset, std::vector<UnitInventory>::iterator unit)
{
//name section
this->name = sf_spawn_text(asset->font,16,unit->get_name());
this->name.setOrigin(this->name.getGlobalBounds().width/2,0);
this->name.setPosition(sf::Vector2f(250,250));

//hp section

std::string HP = std::to_string(unit->HP[1]) + "/" + std::to_string(unit->HP[0]);
this->health = sf_spawn_text(asset->font,12,HP);
this->health.setOrigin(this->health.getGlobalBounds().width/2, 0);
this->health.setPosition(this->name.getPosition() + sf::Vector2f(0,16));



float HP_percentage = (float) unit->HP[1] / unit->HP[0];

this->health_bar.setSize(sf::Vector2f(HP_percentage * 200,10));
this->health_bar.setPosition(this->health.getPosition() + sf::Vector2f(-100,12));
this->health_bar.setFillColor(sf::Color( (1-HP_percentage) * 255, HP_percentage * 240, 0, 255));


std::string left_bank;
left_bank += "Fuel: " + std::to_string(unit->fuel[1]) + "/" + std::to_string(unit->fuel[0]) += "\n";
left_bank += "Mobility: " + std::to_string(unit->mobility) + "\n";
left_bank += "Vision: " + std::to_string(unit->vision) + "\n";
this->l_text = sf_spawn_text(asset->font,16,left_bank);
this->l_text.setPosition(this->health_bar.getPosition() + sf::Vector2f(0,12));


std::string right_bank;
right_bank += "Turret protection: " + std::to_string(unit->armor[0][0]) + "/" + std::to_string(unit->armor[0][1]) + "/" + std::to_string(unit->armor[0][2]) + "\n";
right_bank += "Hull protection: " + std::to_string(unit->armor[1][0]) + "/" + std::to_string(unit->armor[1][1]) + "/" + std::to_string(unit->armor[1][2]) + "\n";
right_bank += "Primary armament: " + std::string(unit->weapon[0].get_name()) + "\n";
right_bank += "  Range: " + std::to_string(unit->weapon[0].range) + "\n";
right_bank += "  Damage: " + std::to_string(unit->weapon[0].damage) + "\n";
right_bank += "  Penetration: " + std::to_string(unit->weapon[0].penetration) + "\n";
right_bank += "Secondary armament: " + std::string(unit->weapon[1].get_name()) + "\n";
right_bank += "  Range: " + std::to_string(unit->weapon[1].range) + "\n";
right_bank += "  Damage: " + std::to_string(unit->weapon[1].damage) + "\n";
right_bank += "  Penetration: " + std::to_string(unit->weapon[1].penetration) + "\n";

this->r_text = sf_spawn_text(asset->font,16,right_bank);
this->r_text.setPosition(500,this->l_text.getPosition().y);
}




void UnitButton::draw(sf::RenderTarget& target)
{
    target.draw(this->box);
    target.draw(this->text);
    target.draw(this->health);
    target.draw(this->health_bar);
}


void UnitButton::spawn(Asset * asset, std::vector<UnitInventory>::iterator object, int list_number)
{
    this->box.setSize(sf::Vector2f(300,200));
    this->box.setFillColor(sf::Color(27,27,0,255));


    //Main body section

    this->text = sf_spawn_text(asset->font,12,object->get_name());
    this->centerOrigin();
    this->setPosition(sf::Vector2f(
        (list_number%3) * 333 + 333/2, 225/2 + (list_number/3) * 225  
    ),2);

    //HP section
    std::string HP = std::to_string(object->HP[1]) + "/" + std::to_string(object->HP[0]);
    this->health = sf_spawn_text(asset->font,10,HP);
    this->health.setOrigin(this->health.getGlobalBounds().width/2, this->health.getGlobalBounds().height/2);
    this->health.setPosition(this->text.getPosition() - sf::Vector2f(0,24));

    float HP_percentage = (float) object->HP[1] / object->HP[0];

    this->health_bar.setSize(sf::Vector2f(HP_percentage * (this->box.getSize().x * 0.8f),8));
    this->health_bar.setPosition(
        this->health.getPosition() - sf::Vector2f(this->box.getSize().x * 2 /5,-10)
    );
    this->health_bar.setFillColor(sf::Color( (1-HP_percentage) * 255, HP_percentage * 255, 0, 255));


}




void ItemButton::spawn(Asset * asset, std::vector<ItemInst>::iterator object, int list_number)
{
    this->box.setSize(sf::Vector2f(225,200));
    this->box.setFillColor(sf::Color(27,27,0,255));


    //Main body section

    this->text = sf_spawn_text(asset->font,16,object->get_name());
    this->centerOrigin();
    this->setPosition(sf::Vector2f(
        (list_number%4) * 250 + 250/2, 225/2 + (list_number/4) * 225  
    ),0);

    //trait section
    this->traits = sf_spawn_text(asset->font,12,object->extract_traits());
    this->traits.setPosition(this->text.getPosition() + sf::Vector2f(0,12));
    this->traits.setOrigin(this->traits.getGlobalBounds().width/2,0);



}

void ItemButton::draw(sf::RenderTarget& target)
{
    target.draw(this->box);
    target.draw(this->text);
    target.draw(this->traits);
    target.draw(this->prompt);
}










//unit selectiion module
int unitSelect(Asset * asset, PlayerData * data, bool * quit)
{

std::vector<UnitButton> unit_button_q(data->unit_q.size());
for(int i = 0; i < unit_button_q.size(); i++)
{
    unit_button_q[i].spawn(asset,data->unit_q.begin()+i,i);


}




sf::Event event;

sf::View prev_cam = asset->display.getView();
sf::View camera(sf::FloatRect(0,0,asset->display.getSize().x,asset->display.getSize().y));
asset->display.setView(camera);



while(!(*quit))
{
    while (asset->display.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed: 
            *quit = true; 
            break;
            case sf::Event::Resized:
            break;
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    auto mouse_pos = sf::Mouse::getPosition(asset->display);
                    auto translated_pos = asset->display.mapPixelToCoords(mouse_pos); //map clicked position to display world

                    for(int i = 0; i < unit_button_q.size(); i++)
                    {
                        if(unit_button_q[i].box.getGlobalBounds().contains(translated_pos.x,translated_pos.y))
                        {
                            asset->display.setView(prev_cam);
                            return data->unit_q.at(i).ID;
                            break;
                        }
                    }
                }
            break;
            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Escape) return -1;
            break;
            case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                bool direction = (event.mouseWheelScroll.delta > 0);
                float pos_delta = 40;

                camera = asset->display.getView();
                sf::Vector2f cam_pos = camera.getCenter();
                if(direction)
                {
                    cam_pos.y = ( cam_pos.y - pos_delta < camera.getSize().y/2 ?  camera.getSize().y/2 :  cam_pos.y - pos_delta);
                }
                else
                {
                    float upper_bound = (unit_button_q.size()/3 + 1) * 225;
                    cam_pos.y = ( cam_pos.y + pos_delta > upper_bound - camera.getSize().y/2 ?  upper_bound - camera.getSize().y/2 :  cam_pos.y + pos_delta);
                }
                camera.setCenter(cam_pos);


                asset->display.setView(camera);
            }

            break;


            default: break;
        }
    }
//drawing routine
for(int i = 0; i < unit_button_q.size(); i++)
{
    unit_button_q[i].draw(asset->display);
}
asset->display.display();
asset->display.clear(sf::Color(0,27,27,255));



}



return -1;
}

//this function may be problematic, as it doesn't handle errors well
bool equipmentMount(PlayerData * data, short unit_ID , short slot_ID, short item_ID)
{
auto found_item = data->find_item(item_ID);
auto found_unit = data->find_unit(unit_ID);

if(found_unit < data->unit_q.end() && found_item < data->item_q.end()) //both unit and item exists
{
    //de-equip item of the same type
    for(int i = 0; i < found_unit->item_q.size(); i++)
    {
        auto probed_item = data->find_item(found_unit->item_q.at(i));
        if(probed_item->type == found_item->type)
        {
            probed_item->OwnerID = 0;
            found_unit->item_q.at(i) = 0;
        }
    }


    if(found_unit->item_q[slot_ID] == 0 && found_item->OwnerID == 0) //equipping item
    {
        found_unit->item_q[slot_ID] = found_item->ID;
        if(item_ID) found_item->OwnerID = found_unit->ID; //checks if item ID is different from EMPTY placeholder
    }
    else if(found_unit->item_q[slot_ID] == found_item->OwnerID || item_ID == 0) //de-equipping item
    {
    found_unit->item_q[slot_ID] = 0;
    found_item->OwnerID = 0;
    }
    else //item that belongs to another unit is equipped on target unit
    {
        auto item_owner = data->find_unit(found_item->OwnerID);
        if(item_owner < data->unit_q.end()) item_owner->item_q[item_owner->get_item_slot(item_ID)] = 0;

        found_unit->item_q[slot_ID] = found_item->ID;
        if(item_ID) found_item->OwnerID = found_unit->ID;
        
    }
}







}



GameState manage(Asset * asset, PlayerData * data)
{
    bool quit = false;
    short unit_selected; 
    short item_slot_selected;
    short item_selected;

    ManageState state = ManageState::UNIT_SELECT;
    while(!quit)
    {
        switch(state)
        {
            case ManageState::UNIT_SELECT:
            {
                unit_selected = unitSelect(asset,data,&quit);
                if(quit) return QUIT;
                if(unit_selected > 0) state = ManageState::INFO;
                else return MENU;
            }
            break;
            case ManageState::INFO:
            {
                item_slot_selected = unitInfo(asset,data,unit_selected,&quit);
                if(quit) return QUIT;
                if(item_slot_selected > -1) state = ManageState::ITEM_SELECT;
                else state = ManageState::UNIT_SELECT;
            }
            break;
            case ManageState::ITEM_SELECT:
            {
                item_selected = itemSelect(asset,data,&quit);
                if(quit) return QUIT;
                if(item_selected >= 0)
                {
                    equipmentMount(data,unit_selected,item_slot_selected,item_selected);
                }
                state = ManageState::INFO;
            }
            break;
        }
    }


}


//unit info module
int unitInfo(Asset * asset, PlayerData * data, int ID, bool * quit)
{

sf::Event event;

sf::View prev_cam = asset->display.getView();
sf::View camera(sf::FloatRect(0,0,asset->display.getSize().x,asset->display.getSize().y));
asset->display.setView(camera);




std::vector<ItemButton> item_button_q;


UnitInfo current;
if(ID > -1) 
{
    auto selected = data->find_unit(ID);


    current.spawn(asset,selected);


    for(int i = 0; i < selected->item_q.size(); i++)
    {
    ItemButton button;
    button.spawn(asset, data->find_item(selected->item_q.at(i)),i); 
    button.move(sf::Vector2f(0,500));
    button.traits.move(sf::Vector2f(0,500));
    item_button_q.push_back(button);
    }
}





while(!(*quit))
{
    while (asset->display.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed: 
            *quit = true; 
            break;
            case sf::Event::Resized:
            break;
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    auto mouse_pos = sf::Mouse::getPosition(asset->display);
                    auto translated_pos = asset->display.mapPixelToCoords(mouse_pos); //map clicked position to display world

                    for(int i = 0; i < item_button_q.size(); i++)
                    {
                        if(item_button_q[i].box.getGlobalBounds().contains(translated_pos.x,translated_pos.y))
                        {
                            asset->display.setView(prev_cam);
                            return i;
                            break;
                        }
                    }
                }
            break;
            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Escape)
                {
                    asset->display.setView(prev_cam);
                    return -1;
                }
            break;
            case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                bool direction = (event.mouseWheelScroll.delta > 0);
                float pos_delta = 20;

                camera = asset->display.getView();
                sf::Vector2f cam_pos = camera.getCenter();
                if(direction)
                {
                    cam_pos.y = ( cam_pos.y - pos_delta < camera.getSize().y/2 ?  camera.getSize().y/2 :  cam_pos.y - pos_delta);
                }
                else
                {
                    float upper_bound = 500 + (item_button_q.size()/4 + 1) * 225;
                    cam_pos.y = ( cam_pos.y + pos_delta > upper_bound - camera.getSize().y/2 ?  upper_bound - camera.getSize().y/2 :  cam_pos.y + pos_delta);
                }
                camera.setCenter(cam_pos);


                asset->display.setView(camera);
            }
            break;


            default: break;
        }
    }
//drawing routine



current.draw(asset->display);
for(int i = 0; i < item_button_q.size(); i++)
{
    item_button_q.at(i).draw(asset->display);
}

asset->display.display();
asset->display.clear(sf::Color(0,27,27,255));



}


asset->display.setView(prev_cam);
return -1;
}





//item selection module
int itemSelect(Asset * asset, PlayerData * data, bool * quit)
{

std::vector<ItemButton> item_button_q(data->item_q.size());
for(int i = 0; i < item_button_q.size(); i++)
{
    item_button_q[i].spawn(asset,data->item_q.begin()+i,i);

    if(data->item_q.at(i).OwnerID > 0)
    {
        auto owner = data->find_unit(data->item_q.at(i).OwnerID);
        std::string equip_indicator;
        equip_indicator += "Equipped on " + std::string(owner->get_name()) + " (" + std::to_string(owner->level) + ")\n";
        item_button_q[i].prompt = sf_spawn_text(asset->font,10,equip_indicator);
        item_button_q[i].prompt.setFillColor(sf::Color(120,120,0,255));
        item_button_q[i].prompt.setOrigin(item_button_q[i].prompt.getGlobalBounds().width/2,0);
        item_button_q[i].prompt.setPosition(item_button_q[i].getPosition() + sf::Vector2f(0,item_button_q[i].box.getSize().y/2 - item_button_q[i].prompt.getCharacterSize()));
    }

    
}




sf::Event event;

sf::View prev_cam = asset->display.getView();
sf::View camera(sf::FloatRect(0,0,asset->display.getSize().x,asset->display.getSize().y));
asset->display.setView(camera);



while(!(*quit))
{
    while (asset->display.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed: 
            *quit = true; 
            break;
            case sf::Event::Resized:
            break;
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    auto mouse_pos = sf::Mouse::getPosition(asset->display);
                    auto translated_pos = asset->display.mapPixelToCoords(mouse_pos); //map clicked position to display world

                    for(int i = 0; i < item_button_q.size(); i++)
                    {
                        if(item_button_q[i].box.getGlobalBounds().contains(translated_pos.x,translated_pos.y))
                        {
                            asset->display.setView(prev_cam);
                            return data->item_q.at(i).ID;
                            break;
                        }
                    }
                }
            break;
            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Escape) 
                {
                    asset->display.setView(prev_cam);
                    return -1;
                }
            break;
            case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                bool direction = (event.mouseWheelScroll.delta > 0);
                float pos_delta = 40;

                camera = asset->display.getView();
                sf::Vector2f cam_pos = camera.getCenter();
                if(direction)
                {
                    cam_pos.y = ( cam_pos.y - pos_delta < camera.getSize().y/2 ?  camera.getSize().y/2 :  cam_pos.y - pos_delta);
                }
                else
                {
                    float upper_bound = (item_button_q.size()/3 + 1) * 225;
                    cam_pos.y = ( cam_pos.y + pos_delta > upper_bound - camera.getSize().y/2 ?  upper_bound - camera.getSize().y/2 :  cam_pos.y + pos_delta);
                }
                camera.setCenter(cam_pos);


                asset->display.setView(camera);
            }

            break;


            default: break;
        }
    }
//drawing routine
for(int i = 0; i < item_button_q.size(); i++)
{
    item_button_q[i].draw(asset->display);
}
asset->display.display();
asset->display.clear(sf::Color(0,27,27,255));



}



return -1;
}