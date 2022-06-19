#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine.cpp"
#include "network.cpp"


int main(int argc, char** argv){
    using std::cout, std::string, std::vector;

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE*MAP_WIDTH,CELL_SIZE*MAP_HIGHT), "Snake rendered");
    window.setFramerateLimit(60);
    Engine snek;

    //main game loop
    while(window.isOpen()){

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
                switch (event.key.code)
                {
                case sf::Keyboard::Up:
                    snek.eval(0);
                    break;
                case sf::Keyboard::Right:
                    snek.eval(1);
                    break;
                case sf::Keyboard::Down:
                    snek.eval(2);
                    break;
                case sf::Keyboard::Left:
                    snek.eval(3);
                    break;
                
                default:
                    break;
                }
        }

        system("clear");
        std::vector <int> vec = snek.getInfo();
        cout << "gurna granica " << vec[0] << '\n';
        cout << "prawa granica " << vec[1] << '\n';
        cout << "dolna granica " << vec[2] << '\n';
        cout << "lewa  granica " << vec[3] << '\n';

        cout << "gurna do owocu " << vec[4] << '\n';
        cout << "prawa do owocu " << vec[5] << '\n';
        cout << "dolna do owocu " << vec[6] << '\n';
        cout << "lewa  do owocu " << vec[7] << '\n';

        /////////kontrolne
        /*cout << "wynik " << vec[8] << '\n';
        cout << "ruchy " << vec[9] << '\n';
        cout << "kier " << vec[10] << '\n';
        cout << "zyje " << vec[11] << '\n';*/

        window.clear();
        snek.draw(window);
        window.display();
    }

    return 0;
}
