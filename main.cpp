#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#define BUFFER 700

using namespace std;
int n = 0;
std::mutex m;
bool isOn = true;
void fractal(long double originR, long double originI, long double cR, long double cI, long double scale, long double buffer, int maxIt, int *output);
void mandlebrot(double originR, double originI, double scale, double buffer, int maxIt, int *output);
int main(int argc, char *argv[])
{
    bool drawAgain = true;
    bool frac_or_brot = true;
    long double origin[2] = {0, 0};
    long double scale = 2.0;
    long double c[2] = {-0.14, 0.7};
    int iterations = 1000;
    sf::Vector2i mouse_coordinates;
    int *colors;
    colors = new int [BUFFER * BUFFER];

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
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            mouse_coordinates = sf::Mouse::getPosition(window);
            if(mouse_coordinates.x >= 0 && mouse_coordinates.x <= BUFFER &&
                mouse_coordinates.y >= 0 && mouse_coordinates.y <= BUFFER)
                {
                    std::cout<< mouse_coordinates.x<< "  "<< mouse_coordinates.y<< '\n';
                    drawAgain = true;
                    origin[0] = -scale + origin[0] + (long double)mouse_coordinates.x / ((long double)BUFFER/(2*scale));
                    origin[1] = scale + origin[1] - (long double)mouse_coordinates.y / ((long double)BUFFER/(2*scale));
                    std::cout<< origin[0]<< "  "<< origin[1]<< '\n';
                    scale = scale / 2.0;
                }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            frac_or_brot = !frac_or_brot;
            origin[0] = 0;
            origin[1] = 0;
            scale = 2;
            drawAgain = true;
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
        if(drawAgain)
        {
            window.clear();
            if(frac_or_brot)
            {
                std::thread first (fractal, origin[0], origin[1], c[0], c[1], scale, BUFFER, iterations, colors);
                std::thread second (fractal, origin[0], origin[1], c[0], c[1], scale, BUFFER, iterations, colors);
                std::thread third (fractal, origin[0], origin[1], c[0], c[1], scale, BUFFER, iterations, colors);
                std::thread fourth (fractal, origin[0], origin[1], c[0], c[1], scale, BUFFER, iterations, colors);

                first.join();
                second.join();
                third.join();
                fourth.join();
            }
            else
            {
                mandlebrot(origin[0], origin[1], scale, BUFFER, iterations, colors);
            }


            cout<< clock();
            for(int i = 0; i < BUFFER; ++i)
            {
                for(int j = 0; j < BUFFER; ++j)
                {
                    point.setPosition(i,j);
                    //if(colors[i*BUFFER + j]*16%512 > 255)
                    //    point.setColor(sf::Color(colors[i*BUFFER + j]*12, 160 - colors[i*BUFFER + j]*10, 255 - colors[i*BUFFER + j]*16 ));
                    if(colors[i*BUFFER + j]*12%1536 < 256)
                        point.setColor(sf::Color(colors[i*BUFFER + j]*12%256, 0, 0));
                    else if(colors[i*BUFFER + j]*12%1536 < 512)
                        point.setColor(sf::Color(255, colors[i*BUFFER + j]*12%256, 0));
                    else if(colors[i*BUFFER + j]*12%1536 < 768)
                        point.setColor(sf::Color(255, 255, colors[i*BUFFER + j]*12%256));
                    else if(colors[i*BUFFER + j]*12%1536 < 1024)
                        point.setColor(sf::Color(255 - colors[i*BUFFER + j]*12%256, 255, 255));
                    else if(colors[i*BUFFER + j]*12%1536 < 1280)
                        point.setColor(sf::Color(0, 255 - colors[i*BUFFER + j]*12%256, 255));
                    else if(colors[i*BUFFER + j]*12%1536 < 1536)
                        point.setColor(sf::Color(0, 0, 255 - colors[i*BUFFER + j]*12%256));
                        //point.setColor(sf::Color(255 - colors[i][j]*3%96, 255 - colors[i][j]*5%160, 255 - colors[i][j]*8%256 ));
                    else
                        point.setColor(sf::Color(colors[i*BUFFER + j]*6, colors[i*BUFFER + j]*10, colors[i*BUFFER + j]*16 ));
                    window.draw(point);
                }
            }
            window.display();
            drawAgain = false;
        }
    }
    return 0;
}

/*void fractal(double originR, double originI, double cR, double cI, double scale, double buffer, int maxIt, int *output)
{
    register int it = 0;
    register double z[2];
    register double temp;
    register double realScale = buffer/(2.0*scale);
    for(int i = 0; i < buffer; ++i)
    {
        for(int j = 0; j < buffer; ++j)
        {
            z[0] = -scale + originR + (double)i/realScale;
            z[1] = scale + originI - (double)j/realScale;
            it = 0;
            while(it < maxIt)
            {
                temp = z[0];
                z[0] = (z[0]*z[0]) - (z[1]*z[1]) + cR;
                z[1] = 2*z[1]*temp + cI; // temp <=> z[0]
                if((z[0]*z[0]) + (z[1]*z[1]) > 4)
                    break;
                it++;
            }
            output[i*(int)buffer + j] = it;
        }
    }
}*/
void fractal(long double originR, long double originI, long double cR, long double cI, long double scale, long double buffer, int maxIt, int *output)
{
    register int i = 0;
    register int j = 0;
    register int it = 0;
    register long double z[2];
    register long double temp;
    register long double realScale = buffer/(2.0*scale);
    ::n = 0;
    while(true)
    {
        m.lock();
        if( n >= BUFFER*BUFFER)
        {
            m.unlock();
            break;
        }
        i = ::n / BUFFER;
        j = ::n % BUFFER;
        ::n++;
        m.unlock();
        z[0] = -scale + originR + (long double)i/realScale;
        z[1] = scale + originI - (long double)j/realScale;
        it = 0;
        while(it < maxIt)
        {
            temp = z[0];
            z[0] = (z[0]*z[0]) - (z[1]*z[1]) + cR;
            z[1] = 2*z[1]*temp + cI; // temp <=> z[0]
            if((z[0]*z[0]) + (z[1]*z[1]) > 4)
                break;
            it++;
        }
        output[i*(int)buffer + j] = it;
    }
}

void mandlebrot(double originR, double originI, double scale, double buffer, int maxIt, int *output)
{
    int it = 0;
    register double p[2];
    register double z[2] = {0, 0};
    register double temp = 0;
    register double realScale = buffer/(2.0*scale);
    for(int i = 0; i < buffer; ++i)
    {
        for(int j = 0; j < buffer; ++j)
        {
            p[0] = -scale + originR + (double)i/realScale;
            p[1] = scale + originI - (double)j/realScale;
            it = 0;
            z[0] = 0;
            z[1] = 0;
            while(it < maxIt)
            {
                temp = z[0];
                z[0] = (z[0]*z[0]) - (z[1]*z[1]) + p[0];
                z[1] = 2*z[1]*temp + p[1]; // temp <=> z[0]
                if(((z[0]*z[0]) + (z[1]*z[1])) > 4.0)
                {
                    break;
                }
                it++;
            }
            output[i*(int)buffer + j] = it;
        }
    }

}


