#include <SFML/Graphics.hpp>
#include "ClockFace.hpp"

int main()
{
    sf::VideoMode scrn(sf::VideoMode::getDesktopMode());
    sf::RenderWindow window(scrn, "Clock");
    window.setFramerateLimit(30);

    sf::Vector2f cntr(scrn.width/2, scrn.height/2);

    sf::View view;
    view.setSize(sf::Vector2f(scrn.width,scrn.height));
    view.setCenter(cntr);

    BLAK::ClockFace clock;
    clock.setBorderColor(sf::Color(32,32,32));
    clock.setBorderHighlightColor(sf::Color(0,0,0));
    clock.setFaceColor(sf::Color(16,16,16));
    clock.setDigitalBoxColor(sf::Color(16,16,16));
    clock.setDigitalColor(sf::Color(192,192,192));
    clock.setNumberColor(sf::Color(225,225,255));
    clock.setDigitalVisible(true);
//    clock.setSize(200);
    clock.setPosition(cntr);

    sf::Clock timer;
    sf::Time secondspassed;
    secondspassed = timer.getElapsedTime();

    clock.showTime();
    
    bool isActive = false;

    while (window.isOpen())
    {
        sf::Event ev;
        while (isActive ? window.pollEvent(ev) : window.waitEvent(ev))
        {
            switch (ev.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    view.setSize(sf::Vector2f(ev.size.width,ev.size.height));
                    view.setCenter(sf::Vector2f(ev.size.width/2,ev.size.height/2));
                    clock.setPosition(sf::Vector2f(ev.size.width/2,ev.size.height/2));
                    break;
                case sf::Event::LostFocus:
                    isActive = false;
                    break;
                case sf::Event::GainedFocus:
                    view.setSize(sf::Vector2f(ev.size.width,ev.size.height));
                    view.setCenter(sf::Vector2f(ev.size.width/2,ev.size.height/2));
                    clock.setPosition(sf::Vector2f(ev.size.width/2,ev.size.height/2));
                    isActive = true;
                    break;
            }
        }
        secondspassed = timer.getElapsedTime();
        if(secondspassed.asSeconds() >= 0.99f) // allows for lag
        {
            timer.restart();
            clock.tick();
        }

        if(isActive)
        {
            window.clear(sf::Color(64,64,64));
 
            window.setView(view);
            window.draw(clock);

            window.display();
        }
        else {
            sf::sleep(sf::milliseconds(100));
        }
    }

    return 0;
}