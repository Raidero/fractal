#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#define BUFFER 1000

using namespace std;

extern "C" void _fractal(double originR, double originI, double cR, double cI, double scale, double buffer, int maxIt, int *output);

int main()
{

    bool drawAgain = true;
    double origin[2] = {0, 0};
    double scale = 2.0;
    double c[2] = {-0.11, 0.66};
    int iterations = 10000;
    sf::Vector2i mouse_coordinates;
    int *input;
    input = new int [BUFFER * BUFFER];

    sf::RenderWindow window(sf::VideoMode(BUFFER, BUFFER), "Fractal");
    window.setFramerateLimit(60);

    sf::Image point_image;
    point_image.create(1,1, sf::Color(255,255,255));
    sf::Texture point_texture;
    sf::Sprite point;
    point_texture.loadFromImage(point_image);
    point.setTexture(point_texture);

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                mouse_coordinates = sf::Mouse::getPosition(window);
                if(mouse_coordinates.x >= 0 && mouse_coordinates.x <= BUFFER &&
                    mouse_coordinates.y >= 0 && mouse_coordinates.y <= BUFFER)
                    {
                        //std::cout<< mouse_coordinates.x<< "  "<< mouse_coordinates.y<< '\n';
                        drawAgain = true;
                        origin[0] = -scale + origin[0] + (double)mouse_coordinates.x / ((double)BUFFER/(2*scale));
                        origin[1] = scale + origin[1] - (double)mouse_coordinates.y / ((double)BUFFER/(2*scale));
                        //std::cout<< origin[0]<< "  "<< origin[1]<< '\n';
                        scale = scale / 2.0;
                    }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                c[1] += 0.01;
                drawAgain = true;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                c[1] -= 0.01;
                drawAgain = true;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                c[0] -= 0.01;
                drawAgain = true;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                c[0] += 0.01;
                drawAgain = true;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                origin[0] = 0;
                origin[1] = 0;
                scale = 2;
                drawAgain = true;
            }
        }
        if(drawAgain)
        {
            std::cout<<"cR = "<< c[0]<< ", cI = " << c[1] <<'\n';
            window.clear();
            _fractal(origin[0], origin[1], c[0], c[1], scale, BUFFER, iterations, input);
            for(int i = 0; i < BUFFER; ++i)
            {
                for(int j = 0; j < BUFFER; ++j)
                {
                    point.setPosition(i,j);
                    if(input[i*BUFFER + j]*16%512 > 255)
                        point.setColor(sf::Color(96 - input[i*BUFFER + j]*6%96, 160 - input[i*BUFFER + j]*10%160, 255 - input[i*BUFFER + j]*16%256 ));
                        //point.setColor(sf::Color(255 - colors[i][j]*3%96, 255 - colors[i][j]*5%160, 255 - colors[i][j]*8%256 ));
                    else
                        point.setColor(sf::Color(input[i*BUFFER + j]*6%96, input[i*BUFFER + j]*10%160, input[i*BUFFER + j]*16%256 ));
                    window.draw(point);
                }
            }
            window.display();
            drawAgain = false;
        }
    }
    delete [] input;
    return 0;
}
