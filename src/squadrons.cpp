#include "squadrons.hpp"
#include "manage.hpp"
#include "main.hpp"
#include "common.hpp"


//squad_ID assumed to be correct
void squadMount(PlayerData * data, short squad_ID , short slot_ID, short unit_ID)
{
if(squad_ID < 1) return;
if(slot_ID < 0 || slot_ID >= 5) return; 
auto found_squad = data->squad_q.begin()+squad_ID-1;
auto found_unit = data->find_unit(unit_ID);



if(found_unit < data->unit_q.end() || unit_ID == 0) //is a valid unit
{
    short target_unit_ID = found_squad->at(slot_ID); //ID of unit at slot_ID


    if(unit_ID > 0)
    {
        short origin_slot_ID = -1; //slot of unit_ID
        short origin_unit_ID = found_unit->Owner_ID; //can be zero
        if(origin_unit_ID > 0) //information gathering section
        {
            auto origin_squad = data->squad_q.begin()+origin_unit_ID-1;

            for(auto i = 0; i < origin_squad->size(); i++)
            {
                if(origin_squad->at(i) == unit_ID) 
                {
                    origin_slot_ID = i;
                    break;
                }

            }

            origin_squad->at(origin_slot_ID) = (target_unit_ID > 0 ? target_unit_ID : 0 );
        }
    }
    /*
    squad_ID assumed valid
    slot_ID assumed valid
    target_unit_ID not assumed valid

    origin_unit_ID not assumed valid
    origin_slot_ID not assumed valid
    unit_ID assumed valid
    */

    if(target_unit_ID > 0)
    {
        auto target = data->find_unit(target_unit_ID);
        if(target < data->unit_q.end()) //target_unit_ID assumed valid
        {
            target->Owner_ID = found_unit->Owner_ID;
        }
    }
    if(unit_ID > 0) found_unit->Owner_ID = squad_ID;
    found_squad->at(slot_ID) = unit_ID;
     

}



}



SquadButton::SquadButton(Asset * asset, int list_number)
{
    this->box.push_back(sf::RectangleShape( sf::Vector2f(170,200)));
    this->box.front().setFillColor(sf::Color(17,27,17,255));

    std::string text = "Squad " + std::to_string(list_number+1);

    this->text.push_back(sf_spawn_text(asset->font,12,text));
    this->setTextPosition(0,0.5f,0.5f);


    this->centerOrigin();
    this->setPosition(sf::Vector2f( this->getSize().x/2 + 10, 110 + list_number * 210));
}

void  UnitSquadButton::spawn(Asset * asset, PlayerData * data, short ID, int list_number)
{
    this->box.clear();
    this->text.clear();

auto object = data->find_unit(ID);

    this->box.push_back(sf::RectangleShape( sf::Vector2f(150,400)));
    this->box.front().setFillColor(sf::Color(27,27,0,255));

    this->text.push_back(sf_spawn_text(asset->font,12,(  object < data->unit_q.end()  ?  object->get_name() : "EMPTY" )));
    this->setTextPosition(0,0.5f,0.f);


if(ID > 0)
{
    
//HP section
    std::string HP = std::to_string(object->HP[1]) + "/" + std::to_string(object->HP[0]);
    this->text.push_back(sf_spawn_text(asset->font,10,HP)); //1st text
    this->setTextPosition(1,0.5f,0.6f);






    float HP_percentage = (float) object->HP[1] / object->HP[0];

//HP bar
    this->box.push_back(sf::RectangleShape(sf::Vector2f(HP_percentage * (this->box.front().getSize().x * 0.8f),8)));
    this->box.back().setPosition(
        this->text[1].getPosition() - sf::Vector2f(this->box.front().getSize().x * 2 /5,-10)
    );
    this->box.back().setFillColor(sf::Color( (1-HP_percentage) * 255, HP_percentage * 255, 0, 255));

//info square


    this->box.push_back(sf::RectangleShape( sf::Vector2f(40,40)));
    this->box.back().setFillColor(sf::Color(120,120,120,255));
    this->box.back().setOrigin(this->box.back().getSize()/2.f);
    this->setBoxPosition(2,this->getSize() - sf::Vector2f(25,25));

    this->text.push_back(sf_spawn_text(asset->font,16,"(i)"));
    this->text.back().setPosition(this->box.back().getPosition());

}
    this->centerOrigin();
    this->setPosition(sf::Vector2f( list_number * 160 + 275, 250));


}



GameState squadrons(Asset * asset, PlayerData * data)
{
//0 for theme, 1 for HP bar, 2 for info square
//0 for name, 1 for HP

size_t selected_squad = 1;

std::array<UnitSquadButton,5> ubutton;

for(size_t i = 0; i < ubutton.size(); i++) 
{
short ID = data->squad_q[selected_squad-1].at(i);
ubutton[i].spawn(asset,data,ID,i);
}



std::vector<SquadButton> sbutton;

for(size_t i = 0; i < data->squad_q.size(); i++)  sbutton.push_back(SquadButton(asset,i));


sf::View prev_cam = asset->display.getView();
sf::View scroll_cam(sf::FloatRect(0,0,190,asset->display.getSize().y));
sf::View unit_cam(sf::FloatRect(190,0,asset->display.getSize().x - 190,asset->display.getSize().y));
scroll_cam.setViewport(sf::FloatRect(0.f,0.f,0.190f,1.f));
unit_cam.setViewport(sf::FloatRect(0.190f,0.f,0.810f,1.f));


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
            break;
            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Escape) 
                {
                    asset->display.setView(prev_cam);
                    return MENU;
                }
                
            break;
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    auto mouse_pos = sf::Mouse::getPosition(asset->display);
                    short activeBox = -1;

                    if(RatioRect_LenRect(scroll_cam.getViewport(),asset->display.getSize()).contains(mouse_pos.x,mouse_pos.y)) //is in scrollable section
                    {
                        activeBox = 0;
                        asset->display.setView(scroll_cam);
                    }
                    else //is in clickable section
                    {
                        activeBox = 1;
                        asset->display.setView(unit_cam);
                    }


                    auto translated_pos = asset->display.mapPixelToCoords(mouse_pos); //map clicked position to display world

                    int pressed = -1;

                    switch(activeBox)
                    {
                        case 0: //squad section
                            for(int i = 0; i < sbutton.size(); i++) //check for squad selections clicks
                            {
                                if(sbutton[i].box.front().getGlobalBounds().contains(translated_pos.x,translated_pos.y))
                                {
                                    pressed = i;
                                    break;
                                }
                            }
                            if(pressed + 1 > 0) selected_squad = pressed + 1;
                            if(selected_squad != pressed) //generate new squad buttons
                            {
                                for(size_t i = 0; i < ubutton.size(); i++) 
                                {
                                short ID = data->squad_q[selected_squad-1].at(i);
                                ubutton[i].spawn(asset,data,ID,i);
                                }
                            }
                        break;
                        case 1: //unit ssection
                            bool isInfoPressed = false;

                            for(int i = 0; i < ubutton.size(); i++) //check for unit selections clicks
                            {
                                if(ubutton[i].box.front().getGlobalBounds().contains(translated_pos))
                                {
                                    pressed = i;
                                    if(ubutton[i].box.size() >= 3)
                                    {
                                        if(ubutton[i].box.at(2).getGlobalBounds().contains(translated_pos)) isInfoPressed = true;
                                    }
                                    break;
                                }
                            }
                            if(pressed > -1 && pressed < 5)
                            {
                                if(isInfoPressed)
                                {
                                    short selected_slot = -1;
                                    short selected_item = -1;
                                    ManageState state = ManageState::INFO;
                                    do
                                    {
                                        switch(state)
                                        {
                                            case ManageState::INFO:
                                            selected_slot = unitInfo(asset,data,data->squad_q[selected_squad-1].at(pressed),&quit);
                                            if(selected_slot != -1) state = ManageState::ITEM_SELECT;
                                            break;
                                            case ManageState::ITEM_SELECT:
                                            selected_item = itemSelect(asset,data,&quit);
                                            if(selected_item >= 0)
                                            {
                                                equipmentMount(data,data->squad_q[selected_squad-1].at(pressed),selected_slot,selected_item);
                                            }
                                            state = ManageState::INFO;
                                            break;
                                        }
                                    } while (selected_slot != -1);
                                    
                                }
                                else
                                {
                                    short select_unit = unitSelect(asset,data,&quit,1);
                                    squadMount(data,selected_squad,pressed,select_unit);

                                    for(size_t i = 0; i < ubutton.size(); i++) //generate new squad buttons
                                    {
                                    short ID = data->squad_q[selected_squad-1].at(i);
                                    ubutton[i].spawn(asset,data,ID,i);
                                    }
                                }


                                

                            }


                        break;







                    }



                    



                }
            break;
            case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                auto mouse_pos = sf::Mouse::getPosition(asset->display);

                
                if( RatioRect_LenRect(scroll_cam.getViewport(),asset->display.getSize()).contains(mouse_pos.x,mouse_pos.y)) //is in scrollable section
                {
                    bool direction = (event.mouseWheelScroll.delta > 0);
                    float pos_delta = 40;

                    sf::Vector2f cam_pos = scroll_cam.getCenter();

                    float upper_bound = (sbutton.size() + 1) * 210;
                    if(scroll_cam.getSize().y/2 < upper_bound)
                    {
                        if(direction)
                        {
                            cam_pos.y = ( cam_pos.y - pos_delta < scroll_cam.getSize().y/2 ?  scroll_cam.getSize().y/2 :  cam_pos.y - pos_delta);
                        }
                        else
                        {
                            
                            cam_pos.y = ( cam_pos.y + pos_delta > upper_bound - scroll_cam.getSize().y/2 ?  upper_bound - scroll_cam.getSize().y/2 :  cam_pos.y + pos_delta);
                        }
                        scroll_cam.setCenter(cam_pos);
                    }

                    
                }

                
            }

            break;
            default: break;
        }
    }


    //render
    asset->display.setView(scroll_cam);
    for(auto & obj : sbutton) obj.draw(asset->display);
    asset->display.setView(unit_cam);
    for(auto & obj : ubutton) obj.draw(asset->display);
    asset->display.display();
    asset->display.clear(sf::Color(0,27,27,255));

}




    asset->display.setView(prev_cam);
    return QUIT;
}