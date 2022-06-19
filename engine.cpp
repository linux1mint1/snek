
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <time.h>
#include <math.h>

#define CELL_SIZE 32 //[px]
#define MAP_WIDTH 21 //[cells]
#define MAP_HIGHT 15 //[cells]

class Cell{
private:
    std::string tempState;

    std::vector <Cell*> neighbors;
    // 4 neighbors
    // it's pointer to specific Cell
    // if nullptr neightbor is a wall
    // 0 - up
    // 1 - right
    // 2 - down
    // 3 - left 

    int size; //size in px

    sf::Vector2i pos; // row,column


public:
    Cell(){
        this->tempState = "dead";
        this->size = CELL_SIZE;
        this->pos = {0,0};
        this->neighbors = {nullptr,nullptr,nullptr,nullptr};
    }

    Cell(sf::Vector2i pos){
        this->tempState = "dead";
        this->size = CELL_SIZE;
        this->pos = pos;
        this->neighbors = {nullptr,nullptr,nullptr,nullptr};
    }

    void setNeighbors(std::vector <Cell*> neighbors){
        this->neighbors = neighbors;
    }

    std::vector <Cell*> getNeighbors(){
        return this->neighbors;
    }

    Cell* getNeighborByID(int id){
        return this->neighbors[id];
    }

    std::string getState(){
        return this->tempState;
    }

    void setState(std::string state){
        this->tempState = state;
    }

    sf::Vector2i getPos (){
        return this->pos;
    }

    void draw(sf::RenderWindow &window){

        //this->tempState = "dead";
        sf::RectangleShape body;
        body.setSize(sf::Vector2f(CELL_SIZE,CELL_SIZE));
        body.setPosition(pos.y*CELL_SIZE,pos.x*CELL_SIZE);
        if(this->tempState == "dead")
            body.setFillColor(sf::Color::Black);
        else if(this->tempState == "fruit") 
            body.setFillColor(sf::Color::Magenta);
        else if(this->tempState == "head")
            body.setFillColor(sf::Color(0x225500ff));
        else if(this->tempState == "tail")
            body.setFillColor(sf::Color(0x229922ff));
        else 
            body.setFillColor(sf::Color::Red);

        body.setOutlineColor(sf::Color(0xffffff22));
        body.setOutlineThickness(-1);

        window.draw(body);

        /*sf::Text text;
        sf::Font font;
        font.loadFromFile("arial.ttf");
        text.setPosition(pos.y*CELL_SIZE,pos.x*CELL_SIZE);
        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setString(tempState);
        text.setCharacterSize(18);
        window.draw(text);*/
        

    }

};

class Engine{ //snake
private:
    std::vector <std::vector <Cell>> map;
    Cell* head; // opinter to head
    Cell* fruit; // pointer to fruit
    std::vector <Cell*> tail; // pointer to tail
    int len; // snake length
    int dir; // direction
    int score;
    int moves;
    bool alive;

    void gameOver(){
        this->alive = false;
        std::cout << "Game over!\n";
    }

public:

    std::vector <int> getInfo(){
        //odleglosc od gurnej granicy mapy
        //odleglosc od prawej granicy mapy
        //odleclosc od dolnej granicy mapy
        //odleglosc od lewej  granicy mapy

        //odleglosc pionowa od owocu do gory
        //odleglosc pozioma od owocu po prawej stronie
        //odleglosc pionowa od owocu do dolu
        //odleglosc pozioma od owocu po lewej  stronie

        //odleglosc od ogona w gore
        //odleglosc od ogona w prawo
        //odleglosc od ogona w dol
        //odleglosc od ogona w lewo

        std::vector <int> out;

        out.push_back(head->getPos().x);
        out.push_back(MAP_WIDTH-head->getPos().y-1);
        out.push_back(MAP_HIGHT-head->getPos().x-1);
        out.push_back(head->getPos().y);

        out.push_back(head->getPos().x - fruit->getPos().x);
        out.push_back(fruit->getPos().y - head->getPos().y);
        out.push_back(fruit->getPos().x - head->getPos().x);
        out.push_back(head->getPos().y - fruit->getPos().y);

        out.push_back(score);
        out.push_back(moves);
        out.push_back(dir);
        out.push_back(alive);
        
        

        return out;
    }

    Engine(){
        //create map
        this->map.reserve(MAP_HIGHT);
        for(int i = 0; i<MAP_HIGHT; i++){
            std::vector <Cell> row;
            row.reserve(MAP_WIDTH);
            for(int j = 0; j<MAP_WIDTH; j++){
                row.emplace_back(sf::Vector2i(i,j));

            }
            this->map.push_back(row);
        }

        //set correct neightbors
        for(int i = 0; i<MAP_HIGHT; i++){
            for(int j = 0; j<MAP_WIDTH; j++){
                std::vector <Cell*> neighbors;
                neighbors.reserve(4);
                if(i>0) neighbors.emplace_back(&map[i-1][j]);
                else neighbors.emplace_back(nullptr);
                if(j<MAP_WIDTH-1) neighbors.emplace_back(&map[i][j+1]);
                else neighbors.emplace_back(nullptr);
                if(i<MAP_HIGHT-1) neighbors.emplace_back(&map[i+1][j]);
                else neighbors.emplace_back(nullptr);
                if(j>0) neighbors.emplace_back(&map[i][j-1]);
                else neighbors.emplace_back(nullptr);

                map[i][j].setNeighbors(neighbors);
            }
        }


        srand(time(NULL));

        this->score = 0;
        this->dir = 999;
        this->head = &map[MAP_HIGHT/2][MAP_WIDTH/2];
        this->alive = true;
        this->moves = 100;
        
        do{
           this->fruit = &map[rand()%MAP_HIGHT][rand()%MAP_WIDTH];
        }while(this->head == this->fruit);

        this->fruit->setState("fruit");
        this->head->setState("head");
        
    }

    bool eval(int direction){
        if(!alive) return false;
        if(direction < 0 || direction > 3) direction = 0;
        // 0 - up
        // 1 - right
        // 2 - down 
        // 3 - left
        // else - up
        if((direction+2)%4 != this->dir)
            // if given direction is not opposite to current direction. the current direction will be set to given direction
            this->dir = direction;

        Cell* expected = head->getNeighborByID(this->dir);
        if(expected == nullptr) {
                gameOver();
                return false;
            }
        
        if(tail.size()>0)
            for(int i = 0; i<tail.size()-1; i++){
                if(tail[i] == expected) {
                    gameOver();
                    return false;
                }
            }

        if(expected == fruit){
            score++;
            moves+=(MAP_WIDTH+MAP_HIGHT)*0.65;
            if(score > 129) gameOver();
            bool temp = false;
            do{
                temp = false;
            this->fruit = &map[rand()%MAP_HIGHT][rand()%MAP_WIDTH];
            if(expected == this->fruit) temp = true;
            if(this->head == this->fruit) temp = true;
            for(Cell * cell : this->tail){
                if(cell == this->fruit) temp = true;
            }
            }while(temp);
        }
        
        head->setState("tail");
        tail.insert(tail.begin(),head);
        while(tail.size()>score+3){
            tail[tail.size()-1]->setState("dead");
            tail.pop_back();
        }
        head = expected;
        moves--;
        if(moves < 1) gameOver();
        
        head->setState("head");
        
        fruit->setState("fruit");


        return true;
    }
    // will return false if game is over and true if game is going. will evaluate state of game;

    void draw(sf::RenderWindow &window){
        if(!alive){
            sf::Text text;
            sf::Font font;
            font.loadFromFile("arial.ttf");
            text.setPosition(0,0);
            text.setFont(font);
            text.setFillColor(sf::Color(0x789922ff));
            text.setString(">be me\n>make crappy snake remake\n>your remake is suitable for neural networks\n>debug the living shit out of it\n>it doesnt work\n>face.jpg");
            text.setCharacterSize(18);

            sf::RectangleShape bg;
            bg.setFillColor(sf::Color(0xffffeeff));
            bg.setSize(sf::Vector2f(MAP_WIDTH*CELL_SIZE,MAP_HIGHT*CELL_SIZE));
            bg.setPosition(0,0);

            window.setTitle("Game Over!");
            window.draw(bg);
            window.draw(text);
            return;
        }
        for(std::vector <Cell> &row : map){
            for(Cell &cell : row){
                cell.draw(window);
            }
        }
    }



};