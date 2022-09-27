#include "common.hpp"











sf::Text sf_spawn_text(sf::Font & font, int size, const char * str)
{
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color(240,240,240,255));
return text;
}


sf::Text sf_spawn_text(sf::Font & font, int size, std::string str)
{
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color(240,240,240,255));
return text;
}




sf::Vector2f Button::getPosition()
{
    return box.getPosition();
}

void Button::setPosition(sf::Vector2f vector, short pos)
{
    this->box.setPosition(vector);
    this->text.setPosition(vector);

    if(pos == 0 || pos == 2) this->text.move(sf::Vector2f(0,  (this->box.getSize().y - this->text.getCharacterSize() - 10)/2 * (pos == 0 ? -1 : 1)   ));
}


void Button::move(sf::Vector2f vector)
{
    this->box.move(vector);
    this->text.move(vector);
}

sf::Vector2f operator/(const sf::Vector2f & vector, const float &num )
{
    return sf::Vector2f(vector.x/num,vector.y/num);
}

sf::Vector2f operator/(const sf::Vector2f & vector, const int &num )
{
    return sf::Vector2f(vector.x/num,vector.y/num);
}


sf::Vector2f operator*(const sf::Vector2f & vector, const float &num )
{
    return sf::Vector2f(vector.x*num,vector.y*num);
}

sf::Vector2f operator*(const sf::Vector2f & vector, const int & num )
{
    return sf::Vector2f(vector.x*num,vector.y*num);
}




void Button::draw(sf::RenderTarget& target)
{
    target.draw(this->box);
    target.draw(this->text);
}

void Button::centerOrigin()
{
box.setOrigin(box.getSize()/2);
text.setOrigin(text.getLocalBounds().width/2,text.getLocalBounds().height/2);
}