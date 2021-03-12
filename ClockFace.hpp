/*****************************************************************************************/
/*  ClockFace using 
/*  SFML - Simple and Fast Multimedia Library
/*  Copyright (C) 2007-2020 Laurent Gomila (laurent@sfml-dev.org)
/*
/*  This software is provided 'as-is', without any express or implied warranty.
/*  In no event will the authors be held liable for any damages arising from the use of this software.
/*
/*  Permission is granted to anyone to use this software for any purpose,
/*  including commercial applications, and to alter it and redistribute it freely,
/*  subject to the following restrictions:
/*
/*  1. The origin of this software must not be misrepresented;
/*     you must not claim that you wrote the original software.
/*     If you use this software in a product, an acknowledgment
/*     in the product documentation would be appreciated but is not required.
/*
/*  2. Altered source versions must be plainly marked as such,
/*     and must not be misrepresented as being the original software.
/* 
/*  3. This notice may not be removed or altered from any source distribution.
/*
/*  created by Blakrunner (blakrunner66@gmail.com)
/*****************************************************************************************/

#ifndef __BLAK_CLOCK_FACE_HPP__
#define __BLAK_CLOCK_FACE_HPP__

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <cstring>

namespace BLAK
{

class ClockFace : public sf::Drawable , public sf::Transformable 
{
public:

    ClockFace();
    virtual ~ClockFace();

    void setBorderColor(sf::Color color);
    void setBorderHighlightColor(sf::Color color);
    void setDigitalBoxColor(sf::Color color);
    void setDigitalColor(sf::Color color);
    void setDigitalVisible(bool visible);
    void setFaceColor(sf::Color color);
    void setHourHandColor(sf::Color color);
    void setMinuteHandColor(sf::Color color);
    void setNumberColor(sf::Color color);
    void setSecondHandColor(sf::Color color);

    void setSize(float size);
    void showTime();
    void tick();

protected:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void update();
    void updateColors();
    void updateHour();
    void updateMinute();
    void updateOutline();
    void updateOutlineColors();
    void updatePin();
    void updateSecond();
    void updateText();

private:

    bool digitalVisible;

    float m_outlineThickness;

    sf::Font m_font;
    sf::Font m_fontDigit;

    int seconds;
    int minutes;
    int hours;
    int days;

    std::string day;

    sf::FloatRect m_bounds;
    sf::FloatRect m_insideBounds;

    sf::Vector2f m_size;

    sf::Color m_digitalBoxColor;
    sf::Color m_numberColor;
    sf::Color m_textColor;
    sf::Color m_secondhandColor;
    sf::Color m_secondhandHighlightColor;
    sf::Color m_secondhandShadeColor;
    sf::Color m_minutehandColor;
    sf::Color m_minutehandHighlightColor;
    sf::Color m_minutehandShadeColor;
    sf::Color m_hourhandColor;
    sf::Color m_hourhandHighlightColor;
    sf::Color m_hourhandShadeColor;
    sf::Color m_outlineColor;
    sf::Color m_outlineHighlightColor;
    sf::Color m_glassColor;
    sf::Color m_glassHighlightColor;
    sf::Color m_glassShadeColor;
    sf::Color m_faceColor;
    sf::Color m_faceHighlightColor;
    sf::Color m_faceShadeColor;

    sf::RectangleShape m_digitalBox;

    sf::Text m_timetext;

    sf::VertexArray m_secondhand;
    sf::VertexArray m_pin;
    sf::VertexArray m_minutehand;
    sf::VertexArray m_hourhand;
    sf::VertexArray m_outline;
    sf::VertexArray m_glass;
    sf::VertexArray m_face;

    std::unordered_map<std::string, sf::Text> m_numbers;

};

} // end namespace BLAK

#endif // __BLAK_CLOCK_FACE_HPP__
