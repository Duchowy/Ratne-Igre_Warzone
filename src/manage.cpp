#include "manage.hpp"




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







void UnitButton::spawn(Asset * asset, PlayerData * data, short ID, int list_number)
{
    auto object = data->find_unit(ID);

    this->box.push_back(sf::RectangleShape(sf::Vector2f(300,200)));
    this->box.front().setFillColor(sf::Color(27,27,0,255));


    //Main body section

    this->text.push_back(sf_spawn_text(asset->font,12,(  object < data->unit_q.end()  ?  object->get_name() : "EMPTY" ))); //0th text
    this->setTextPosition(0,0.5f,0.f);

    if(ID > 0)
    {

        //HP section
        std::string HP = std::to_string(object->HP[1]) + "/" + std::to_string(object->HP[0]);
        sf::Text text = sf_spawn_text(asset->font,10,HP);
        this->text.push_back(text); //1st text
        this->setTextPosition(1,0.5f,0.8f);


        float HP_percentage = (float) object->HP[1] / object->HP[0];

        this->box.push_back(sf::RectangleShape(sf::Vector2f(HP_percentage * (this->box.front().getSize().x * 0.8f),8)));
        this->box.back().setPosition(
            this->text[1].getPosition() - sf::Vector2f(this->box.front().getSize().x * 2 /5,-10)
        );
        this->box.back().setFillColor(sf::Color( (1-HP_percentage) * 255, HP_percentage * 255, 0, 255));

        if(object->Owner_ID > 0)
        {
            std::string equip_indicator;
            equip_indicator += " (squad " + std::to_string(object->Owner_ID) + ")\n";
            sf::Text text = sf_spawn_text(asset->font,12,equip_indicator);
            text.setFillColor(sf::Color(120,120,0,255));

            this->text.push_back(text);
            this->setTextPosition(this->text.size()-1,sf::Vector2f(this->getSize().x/2,this->getSize().y - text.getCharacterSize()));
        }
    }

    this->centerOrigin();
    this->setPosition(sf::Vector2f(
        (list_number%3) * 333 + 333/2, 225/2 + (list_number/3) * 225  
    ));

}




void ItemButton::spawn(Asset * asset, std::vector<ItemInst>::iterator object, int list_number)
{
    this->box.push_back( sf::RectangleShape(sf::Vector2f(225,200)));
    this->box.front().setFillColor(sf::Color(27,27,0,255));


    //Main body section

    this->text.push_back( sf_spawn_text(asset->font,16,object->get_name())); //name text
    this->setTextPosition(0,0.5f,0.f);

    //trait section
    this->text.push_back(sf_spawn_text(asset->font,12,object->extract_traits())); //traits
    this->text[1].setPosition(this->text[0].getPosition() + sf::Vector2f(0,this->text[0].getCharacterSize()));


    this->centerOrigin();
    this->setPosition(sf::Vector2f(
        (list_number%4) * 250 + 250/2, 225/2 + (list_number/4) * 225  
    ));
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
                unit_selected = unitSelect(asset,data,&quit,0);
                if(quit) return QUIT;
                if(unit_selected == -1) return MENU;
                else if(unit_selected > 0) state = ManageState::INFO;
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


//this function may be problematic, as it doesn't handle errors well
bool equipmentMount(PlayerData * data, short unit_ID , short slot_ID, short item_ID)
{//to be rewritten to simpler swap-based system (intraunit swaps not allowed)
auto found_item = data->find_item(item_ID);
auto found_unit = data->find_unit(unit_ID);

if(found_unit < data->unit_q.end() && found_item < data->item_q.end()) //both unit and item exists
{
   

    //item swapping mechanic
    if(found_item->OwnerID > 0 && found_item->OwnerID == unit_ID && //item already equipped on this exact unit
    found_unit->item_q[slot_ID] > 0 && found_unit->item_q[slot_ID] != item_ID) //there is a different item in target slot
    {
        short slot_source = found_unit->get_item_slot(item_ID);

        if(slot_source >= 0)
        {
            short buffer = found_unit->item_q[slot_ID];
            found_unit->item_q[slot_ID] = found_unit->item_q[slot_source];
            found_unit->item_q[slot_source] = buffer;
        }
        
    }
    else
    {
         //de-equip item of the same type
        if(item_ID > 0)
        {
            for(int i = 0; i < found_unit->item_q.size(); i++)
            {
                auto probed_item = data->find_item(found_unit->item_q.at(i));
                if(probed_item->type == found_item->type)
                {
                    equipmentTraitEnforce(data,found_unit->ID,probed_item->ID,false);
                    probed_item->OwnerID = 0;
                    found_unit->item_q.at(i) = 0;
                }
            }
        }

        if(found_item->OwnerID > 0) //item that belongs to some unit is equipped on target unit
        {
            auto item_owner = data->find_unit(found_item->OwnerID);
            if(item_owner < data->unit_q.end()) //item owner is valid
            {
                item_owner->item_q[item_owner->get_item_slot(item_ID)] = 0;
                equipmentTraitEnforce(data,item_owner->ID,item_ID,false);
                found_item->OwnerID = 0;
            }
        }

        if(found_unit->item_q[slot_ID] > 0) //slot is occupied
        {
            auto occupying_item = data->find_item(found_unit->item_q[slot_ID]);

            if(occupying_item < data->item_q.end())
            {
                equipmentTraitEnforce(data,unit_ID,occupying_item->ID,0);
                occupying_item->OwnerID = 0;
            }
            
            found_unit->item_q[slot_ID] = 0; //this piece repairs not existing equipped items
        }



        if(item_ID > 0) //equipping
        {
            found_unit->item_q[slot_ID] = found_item->ID;
            equipmentTraitEnforce(data,unit_ID,item_ID,true);
            found_item->OwnerID = found_unit->ID;
        }

    }


}



}


//unit selectiion module
int unitSelect(Asset * asset, PlayerData * data, bool * quit, bool isEmptyEnabled)
{



std::vector<UnitButton> unit_button_q;

if(isEmptyEnabled)
{
UnitButton buffer;
buffer.spawn(asset,data,0,unit_button_q.size());
unit_button_q.push_back(buffer);
}

for(int i = 0; i < data->unit_q.size(); i++)
{
    UnitButton buffer;
    buffer.spawn(asset,data,(data->unit_q.begin()+i)->ID,unit_button_q.size());
    unit_button_q.push_back(buffer);
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
                        if(unit_button_q[i].box.front().getGlobalBounds().contains(translated_pos.x,translated_pos.y))
                        {
                            asset->display.setView(prev_cam);
                            
                            if(isEmptyEnabled)
                            {
                                if(i==0) return 0;
                                else return data->unit_q.at(i-1).ID;
                            }
                            else
                            {
                                return data->unit_q.at(i).ID;
                            }
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


//unit info module, returns selected item slot
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
                        if(item_button_q[i].box.front().getGlobalBounds().contains(translated_pos.x,translated_pos.y))
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
for(auto i = 0; i < item_button_q.size(); i++)
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
for(auto i = 0; i < item_button_q.size(); i++)
{
    item_button_q[i].spawn(asset,data->item_q.begin()+i,i);

    if(data->item_q.at(i).OwnerID > 0)
    {
        auto owner = data->find_unit(data->item_q.at(i).OwnerID);
        std::string equip_indicator;
        equip_indicator += "Equipped on " + std::string(owner->get_name());
        if(owner->Owner_ID) equip_indicator += " (squad " + std::to_string(owner->Owner_ID) + ")\n";
        
        item_button_q[i].text.push_back(sf_spawn_text(asset->font,10,equip_indicator));
        item_button_q[i].text.back().setFillColor(sf::Color(120,120,0,255));
        item_button_q[i].setTextPosition(2,sf::Vector2f(item_button_q[i].getSize().x/2,item_button_q[i].getSize().y - item_button_q[i].text.back().getCharacterSize()));
        item_button_q[i].centerOrigin();
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

                    for(auto i = 0; i < item_button_q.size(); i++)
                    {
                        if(item_button_q[i].box.front().getGlobalBounds().contains(translated_pos.x,translated_pos.y))
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
for(auto i = 0; i < item_button_q.size(); i++)
{
    item_button_q[i].draw(asset->display);
}
asset->display.display();
asset->display.clear(sf::Color(0,27,27,255));



}



return -1;
}




bool equipmentTraitEnforce(PlayerData * data, short unit_ID , short item_ID, bool isEquipping)
{
    auto found_unit = data->find_unit(unit_ID);
    auto found_item = data->find_item(item_ID);


float mult = (isEquipping ? 1.f : -1.f );


for(auto & obj : found_item->bonus)
{
    if(std::get<1>(obj) == false) //for additive bonuses
    {
        switch(std::get<0>(obj))
        {
            case 0:
            found_unit->HP[0] += mult * std::get<2>(obj);
            found_unit->HP[1] += mult * std::get<2>(obj);
            break;
            case 1:
            found_unit->armor[0][0] += mult * std::get<2>(obj);
            break;
            case 2:
            found_unit->armor[0][1] += mult * std::get<2>(obj);
            break;
            case 3:
            found_unit->armor[1][0] += mult * std::get<2>(obj);
            break;
            case 4:
            found_unit->armor[1][1] += mult * std::get<2>(obj);
            break;
            case 5:
            found_unit->accuracy += mult * std::get<2>(obj);
            break;
            case 6:
            found_unit->fuel[0] += mult * std::get<2>(obj);
            found_unit->fuel[1] += mult * std::get<2>(obj);
            break;
            case 7:
            found_unit->mobility += mult * std::get<2>(obj);
            break;
            case 8:
            found_unit->vision += mult * std::get<2>(obj);
            break;
            case 9:
            found_unit->evansion += mult * std::get<2>(obj);
            break;
        }

    }







}






}