#include "ClockFace.hpp"
#include <cstdio>
#include <cmath>
#include <ctime>

namespace
{

    sf::Vector2f getPoint(std::size_t index, std::size_t count, sf::Vector2f radius)
    {
        static float pi = 3.141592654f;

        float angle = index * 2 * pi / count - pi / 2;
        float x = std::cos(angle) * radius.x;
        float y = std::sin(angle) * radius.y;

        return sf::Vector2f(radius.x + x, radius.y + y);
    }

    sf::Vector2f computeNormal(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        sf::Vector2f normal(p1.y - p2.y, p2.x - p1.x);
        float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
        if (length != 0.f)
            normal /= length;
        return normal;
    }

    float dotProduct(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        return p1.x * p2.x + p1.y * p2.y;
    }
}

namespace BLAK
{

ClockFace::~ClockFace()
{
    m_secondhand.clear();
    m_minutehand.clear();
    m_hourhand.clear();
}

void ClockFace::setBorderColor(sf::Color color)
{
    m_outlineColor = color;
    m_outlineHighlightColor = color - sf::Color(24,24,24);
    updateOutlineColors();
}

void ClockFace::setBorderHighlightColor(sf::Color color)
{
    m_outlineHighlightColor = color;
    updateOutlineColors();
}

void ClockFace::setDigitalBoxColor(sf::Color color)
{
    m_digitalBoxColor = color;
    m_digitalBox.setFillColor(color);
}

void ClockFace::setDigitalColor(sf::Color color)
{
    m_textColor = color;
    m_timetext.setFillColor(color);
}

void ClockFace::setDigitalVisible(bool visible)
{
    digitalVisible = visible;
}

void ClockFace::setFaceColor(sf::Color color)
{
    m_faceColor = color;
    m_faceHighlightColor = color;
    updateColors();
}

void ClockFace::setHourHandColor(sf::Color color)
{
    m_hourhandColor = color;
    m_hourhandHighlightColor = color + sf::Color(32,32,32);
    updateHour();
}

void ClockFace::setMinuteHandColor(sf::Color color)
{
    m_minutehandColor = color;
    m_minutehandHighlightColor = color + sf::Color(32,32,32);
    updateMinute();
}

void ClockFace::setNumberColor(sf::Color color)
{
    m_numberColor = color;
    for(auto &number : m_numbers)
        number.second.setFillColor(color);
}

void ClockFace::setSecondHandColor(sf::Color color)
{
    m_secondhandColor = color;
    m_secondhandHighlightColor = color + sf::Color(32,32,32);
    updateSecond();
    updateColors();
}

void ClockFace::setSize(float size)
{
    m_size = sf::Vector2f(size,size);
    m_outlineThickness = m_size.y * 0.25f;
    update();

    return;
}

void ClockFace::showTime()
{
    time_t rawtime;
    time(&rawtime);
    std::string txt = ctime(&rawtime);
    std::string em;
    sscanf(txt.c_str(), "%s %s %i %d:%d:%d", &day, &em, &days, &hours, &minutes, &seconds);
 
    sf::Transform rots;
    rots.rotate((seconds-1)*6);
    sf::Transform transform = rots;
    for(std::size_t i = 0; i < m_secondhand.getVertexCount(); ++i)
        m_secondhand[i].position = transform.transformPoint(m_secondhand[i].position);
    sf::Transform transs;
    transs.translate(m_face[0].position - m_secondhand[0].position);
    transform = transs;
    for(std::size_t i = 0; i < m_secondhand.getVertexCount(); ++i)
        m_secondhand[i].position = transform.transformPoint(m_secondhand[i].position);
    sf::Transform rotm;
    rotm.rotate(minutes*6);
    transform = rotm;
    for(std::size_t i = 0; i < m_minutehand.getVertexCount(); ++i)
        m_minutehand[i].position = transform.transformPoint(m_minutehand[i].position);
    sf::Transform transm;
    transm.translate(m_face[0].position - m_minutehand[0].position);
    transform = transm;
    for(std::size_t i = 0; i < m_minutehand.getVertexCount(); ++i)
        m_minutehand[i].position = transform.transformPoint(m_minutehand[i].position);

    sf::Transform rot;
    rot.rotate((hours*30)+(minutes*0.5));
    transform = rot;
    for(std::size_t i = 0; i < m_hourhand.getVertexCount(); ++i)
        m_hourhand[i].position = transform.transformPoint(m_hourhand[i].position);
    sf::Transform trans;
    trans.translate(m_face[0].position - m_hourhand[0].position);
    transform = trans;
    for(std::size_t i = 0; i < m_hourhand.getVertexCount(); ++i)
        m_hourhand[i].position = transform.transformPoint(m_hourhand[i].position);             

    std::string newtext = (hours < 10) ? "0" + std::to_string(hours) : std::to_string(hours);
    newtext += ":";
    newtext += (minutes < 10) ? "0" + std::to_string(minutes) : std::to_string(minutes);
    newtext += ":";
    newtext += (seconds < 10) ? "0" + std::to_string(seconds) : std::to_string(seconds);

    m_timetext.setString(newtext);

}

void ClockFace::tick()
{
    if(seconds == 59)
    {
        seconds = 0;
        ++minutes;
        sf::Transform rotm;
        rotm.rotate(6);
        sf::Transform transform = rotm;
        for(std::size_t i = 0; i < m_minutehand.getVertexCount(); ++i)
            m_minutehand[i].position = transform.transformPoint(m_minutehand[i].position);
        sf::Transform transm;
        transm.translate(m_face[0].position - m_minutehand[0].position);
        transform = transm;
        for(std::size_t i = 0; i < m_minutehand.getVertexCount(); ++i)
            m_minutehand[i].position = transform.transformPoint(m_minutehand[i].position);

        sf::Transform rot;
        rot.rotate(0.5f);
        transform = rot;
        for(std::size_t i = 0; i < m_hourhand.getVertexCount(); ++i)
            m_hourhand[i].position = transform.transformPoint(m_hourhand[i].position);
        sf::Transform trans;
        trans.translate(m_face[0].position - m_hourhand[0].position);
        transform = trans;
        for(std::size_t i = 0; i < m_hourhand.getVertexCount(); ++i)
            m_hourhand[i].position = transform.transformPoint(m_hourhand[i].position);             

        if(minutes == 60)
        {
            minutes = 0;
            ++hours;
            if(hours == 24)
            {
                hours = 0;
            }
        }
    }
    else
    {
        ++seconds;
    }

    std::string newtext = (hours <= 9) ? "0" + std::to_string(hours) : std::to_string(hours);
    newtext += ":";
    newtext += (minutes <= 9) ? "0" + std::to_string(minutes) : std::to_string(minutes);
    newtext += ":";
    newtext += (seconds < 10) ? "0" + std::to_string(seconds) : std::to_string(seconds);

    m_timetext.setString(newtext);

    sf::Transform rots;
    rots.rotate(6);
    sf::Transform transform = rots;
    for(std::size_t i = 0; i < m_secondhand.getVertexCount(); ++i)
        m_secondhand[i].position = transform.transformPoint(m_secondhand[i].position);
    sf::Transform transs;
    transs.translate(m_face[0].position - m_secondhand[0].position);
    transform = transs;
    for(std::size_t i = 0; i < m_secondhand.getVertexCount(); ++i)
        m_secondhand[i].position = transform.transformPoint(m_secondhand[i].position);

}

ClockFace::ClockFace() :
m_secondhand(sf::TriangleStrip),
m_minutehand(sf::TriangleStrip),
m_hourhand(sf::TriangleStrip),
m_outline(sf::TriangleStrip),
m_glass(sf::TriangleFan),
m_face(sf::TriangleFan),
m_pin(sf::TriangleFan),
seconds(0),
minutes(0),
hours(0),
m_size(400,400),
m_outlineThickness(100),
m_textColor(0,0,0,192),
m_numberColor(0,0,0),
m_secondhandColor(128,128,128),
m_secondhandHighlightColor(192,192,192),
m_secondhandShadeColor(64,64,64),
m_minutehandColor(128,128,128),
m_minutehandHighlightColor(192,192,192),
m_minutehandShadeColor(64,64,64),
m_hourhandColor(128,128,128),
m_hourhandHighlightColor(192,192,192),
m_hourhandShadeColor(64,64,64),
m_outlineColor(0,178,225),
m_outlineHighlightColor(0,128,192),
m_glassColor(155,155,155,75),
m_glassHighlightColor(255,255,255,75),
m_glassShadeColor(200,200,200,75),
m_faceColor(0,192,255),
m_faceHighlightColor(0,192,255),
m_faceShadeColor(0,128,255),
digitalVisible(false)
{
    update();
}

void ClockFace::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;

    target.draw(m_face, states);
    target.draw(m_outline, states);
    for(auto number : m_numbers)
        target.draw(number.second, states);
    if(digitalVisible)
    {
        target.draw(m_digitalBox, states);
        target.draw(m_timetext, states);
    }
    target.draw(m_hourhand, states);
    target.draw(m_minutehand, states);
    target.draw(m_secondhand, states);
    target.draw(m_pin, states);
    target.draw(m_glass, states);

    return;
}

void ClockFace::update()
{
    std::size_t count = 30;
    m_pin.resize(count+2);

    for(std::size_t i = 0; i < count; ++i)
        m_pin[i+1].position = getPoint(i, count, m_size*0.05f);
    m_pin[count+1].position = m_pin[1].position;

    m_pin[0] = m_pin[1];
    m_insideBounds = m_pin.getBounds();
 
    m_pin[0].position.x = m_insideBounds.left + m_insideBounds.width / 2;
    m_pin[0].position.y = m_insideBounds.top + m_insideBounds.height / 2;

    count = 60;
    m_glass.resize(count+2);

    for(std::size_t i = 0; i < count; ++i)
        m_glass[i+1].position = getPoint(i, count, m_size);
    m_glass[count+1].position = m_glass[1].position;

    m_glass[0] = m_glass[1];
    m_insideBounds = m_glass.getBounds();
 
    m_glass[0].position.x = m_insideBounds.left + m_insideBounds.width / 2;
    m_glass[0].position.y = m_insideBounds.top + m_insideBounds.height / 2;

    m_face.resize(count+2);

    for(std::size_t i = 0; i < count; ++i)
        m_face[i+1].position = getPoint(i, count, m_size);
    m_face[count+1].position = m_face[1].position;

    m_face[0] = m_face[1];
    m_insideBounds = m_face.getBounds();
 
    m_face[0].position.x = m_insideBounds.left + m_insideBounds.width / 2;
    m_face[0].position.y = m_insideBounds.top + m_insideBounds.height / 2;

    updateColors();
    updateOutline();
    updateText();
    updateHour();
    updateMinute();
    updateSecond();
    updatePin();

    setOrigin(m_face[0].position);

    tick();
}

void ClockFace::updateColors()
{
    for(std::size_t i = 0; i < m_glass.getVertexCount(); ++i)
    {
        if(i == 8) m_glass[i].color = m_glassHighlightColor;
        else m_glass[i].color = m_glassColor;
    }
    for(std::size_t i = 0; i < m_face.getVertexCount(); ++i)
    {
        if((i % 2) == 0) m_face[i].color = m_faceColor;
        else m_face[i].color = m_faceHighlightColor;
    }
    m_pin[0].color = m_secondhandHighlightColor;
    for(std::size_t i = 1; i < m_pin.getVertexCount(); ++i)
        m_pin[i].color = m_secondhandColor;
}

void ClockFace::updateHour()
{
    sf::Vector2f size = m_size * 0.45f;
    m_hourhand.clear();

    m_hourhand.append(sf::Vertex(m_face[0].position, m_hourhandShadeColor));
    m_hourhand.append(sf::Vertex(m_face[0].position+sf::Vector2f(-size.x*0.075f,-size.y*0.25f), m_hourhandHighlightColor));
    m_hourhand.append(sf::Vertex(m_face[0].position+sf::Vector2f(size.x*0.075f,-size.y*0.25f), m_hourhandColor));
    m_hourhand.append(sf::Vertex(m_face[1].position+sf::Vector2f(0,size.y), m_hourhandColor));
}

void ClockFace::updateMinute()
{
    sf::Vector2f size = m_size * 0.25f;
    m_minutehand.clear();

    m_minutehand.append(sf::Vertex(m_face[0].position, m_minutehandShadeColor));
    m_minutehand.append(sf::Vertex(m_face[0].position+sf::Vector2f(-size.x*0.1f, -size.y*0.5f), m_minutehandHighlightColor));
    m_minutehand.append(sf::Vertex(m_face[0].position+sf::Vector2f(size.x*0.1f, -size.y*0.5f), m_minutehandColor));
    m_minutehand.append(sf::Vertex(m_face[1].position+sf::Vector2f(0,size.y), m_minutehandColor));
}

void ClockFace::updateSecond()
{
    sf::Vector2f size = m_size * 0.1f;
    m_secondhand.clear();

    m_secondhand.append(sf::Vertex(m_face[0].position, m_secondhandShadeColor));
    m_secondhand.append(sf::Vertex(m_face[0].position+sf::Vector2f(-size.x*0.2f,-size.y*2.0f), m_secondhandHighlightColor));
    m_secondhand.append(sf::Vertex(m_face[0].position+sf::Vector2f(size.x*0.2f,-size.y*2.0f), m_secondhandColor));
    m_secondhand.append(sf::Vertex(m_face[1].position+sf::Vector2f(0,size.y), m_secondhandColor));
}

void ClockFace::updateOutline()
{
    std::size_t count = m_face.getVertexCount() - 2;
    m_outline.resize((count + 1) * 2);

    for (std::size_t i = 0; i < count; ++i)
    {
        std::size_t index = i + 1;

        sf::Vector2f p0 = (i == 0) ? m_face[count].position : m_face[index - 1].position;
        sf::Vector2f p1 = m_face[index].position;
        sf::Vector2f p2 = m_face[index + 1].position;

        sf::Vector2f n1 = computeNormal(p0, p1);
        sf::Vector2f n2 = computeNormal(p1, p2);

        if (dotProduct(n1, m_face[0].position - p1) > 0)
            n1 = -n1;
        if (dotProduct(n2, m_face[0].position - p1) > 0)
            n2 = -n2;

        float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
        sf::Vector2f normal = (n1 + n2) / factor;

        m_outline[i * 2 + 0].position = p1;
        m_outline[i * 2 + 1].position = p1 + normal * m_outlineThickness;
    }

    m_outline[count * 2 + 0].position = m_outline[0].position;
    m_outline[count * 2 + 1].position = m_outline[1].position;

    updateOutlineColors();

    m_bounds = m_outline.getBounds();
}

void ClockFace::updateOutlineColors()
{
    for(std::size_t i = 0; i < m_outline.getVertexCount(); ++ i)
    {
        if((i % 2) == 0)
            m_outline[i].color = m_outlineColor;
        else
            m_outline[i].color = m_outlineHighlightColor;
    }
}

void ClockFace::updatePin()
{
    sf::Transform translate;
    translate.translate(m_face[0].position - m_pin[0].position);
    sf::Transform transform = translate;
    for(std::size_t i = 0; i < m_pin.getVertexCount(); ++i)
        m_pin[i].position = transform.transformPoint(m_pin[i].position);
}

void ClockFace::updateText()
{
    bool loaded = m_font.loadFromFile("Roboto-Bold.ttf");
    if(!loaded) return;

    loaded = m_fontDigit.loadFromFile("clock.ttf");
    if(!loaded) return;

    int fontsize = m_size.y*0.2f;
    sf::Vector2f size = m_size*0.9f;
    sf::Vector2f offset(m_size.x*0.1f,m_size.y*0.05f);

    m_timetext.setFont(m_fontDigit);
    m_timetext.setCharacterSize(fontsize);
    m_timetext.setFillColor(m_textColor);
    m_timetext.setString("00:00:00");
    sf::FloatRect bnds = m_timetext.getGlobalBounds();
    m_timetext.setOrigin(sf::Vector2f((bnds.width/2)-(bnds.width*0.001f), bnds.height+(bnds.height*0.1f)));
    m_timetext.setPosition(m_face[0].position+sf::Vector2f(0,m_size.y*0.25f));

    m_digitalBox.setFillColor(m_faceColor);
    m_digitalBox.setSize(sf::Vector2f(bnds.width, bnds.height+(bnds.height*0.25f)));
    sf::FloatRect box = m_digitalBox.getGlobalBounds();
    m_digitalBox.setOrigin(sf::Vector2f(box.width/2, box.height/2));
    m_digitalBox.setPosition(m_timetext.getPosition());

    for(int i = 0; i < 12; ++i)
    {
        sf::Text text;
        text.setFont(m_font);
        text.setFillColor(m_numberColor);
        text.setCharacterSize(fontsize);
        if(i == 0) text.setString("12");
        else text.setString(std::to_string(i));
        sf::FloatRect bounds = text.getGlobalBounds();
        text.setOrigin(sf::Vector2f(bounds.width/2, bounds.height/2));
        sf::Vector2f position = getPoint(i, 12, size);
        text.setPosition(position+offset);
        m_numbers[std::to_string(i)] = text;
    }
}

} // end namespace BLAK
