#include <iostream>
#include <vector>
#include <time.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define CLEARSCREEN system("cls")
    #define KEYPRESS _kbhit()
    #define GETCHAR _getch()
    #define SLEEP(x) Sleep(x)
#elif __linux__
    #include <conio.h>
    #include <unistd.h>
    #define CLEARSCREEN system("clear")
    #define KEYPRESS kbhit()
    #define GETCHAR getch()
    #define SLEEP(x) usleep(x * 1000)
#endif

bool running = 1;
const int width = 20, height = 20;
int score = 0;

struct Node {
   int x, y;
};

enum Move {up, down, left, right, none};

std::vector<Node> snake;
Node food;
Move move = Move::none;
bool hasEaten = false;

bool isSnakeBody(Node cell) {
    for (int i = 1; i < snake.size(); ++i)
        if (cell.x == snake[i].x && cell.y == snake[i].y) return true;
    return false;
}

void placeFood() {
    food = {1 + rand() % (width - 2), 1 + rand() % (height - 2)};
    if (isSnakeBody(food)) placeFood();
}

void init(){
    srand(time(0));

    snake.push_back({1 + rand() % (width - 2), 1 + rand() % (height - 2)});
    placeFood();
}

void getMove(){

    if (KEYPRESS){
        switch (GETCHAR)
        {
            case 'w':
            case 'W':
                if(move != Move::down) move = Move::up;
                break;
            case 'a':
            case 'A':
                if(move != Move::right) move = Move::left;
                break;
            case 's':
            case 'S':
                if(move != Move::up) move = Move::down;
                break;
            case 'd':
            case 'D':
                if(move != Move::left) move = Move::right;
        }
    }
}

void moveSnakeBody(){

    Node lastNode = snake.back();

    for(int i = snake.size() - 1; i > 0; --i)
        snake[i] = snake[i - 1];

    if (hasEaten) {
        snake.push_back(lastNode);
        hasEaten = false;
    }
}

void moveSnakeHead(){

    switch(move){
        case Move::up:
            --snake[0].y;
            break;
        case Move::left:
            --snake[0].x;
            break;
        case Move::down:
            ++snake[0].y;
            break;
        case Move::right:
            ++snake[0].x;
            break;
    }
}

bool wallHit() {
    if (snake[0].x == 0 || snake[0].x == width - 1 || snake[0].y == 0 || snake[0].y == height - 1)
        return true;
    else return false;
}

bool foodHit() {
    if (snake[0].x == food.x && snake[0].y == food.y) return true;
    else return false;
}

bool bodyHit() {
    for (int i = 1; i < snake.size(); ++i)
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) return true;
    return false;
}

void evaluateCases(){
    if (bodyHit() || wallHit()) running = 0;
    if (foodHit()) {
        ++score;
        hasEaten = true;
        placeFood();
    }
}

void update(){

    moveSnakeBody();
    moveSnakeHead();
    evaluateCases();
}


void draw(){

    SLEEP(200);
    CLEARSCREEN;

    for (int j = 0; j < width; ++j) std::cout << "\033[36;46m##\033[0m";
    std::cout << "\n";
    for (int i = 1; i < height - 1; ++i) {
        std::cout << "\033[36;46m##\033[0m";
        for (int j = 1; j < width - 1; ++j) {

            if(snake[0].x == j && snake[0].y == i) std::cout << "\033[32;42mSS\033[0m";
            else  if(food.x == j && food.y == i) std::cout << "\033[31;41m@@\033[0m";
            else if (isSnakeBody({j, i})) std::cout << "\033[32;42mBB\033[0m"; 
            else std::cout << "\033[37;47m  \033[0m";
        }
        std::cout << "\033[36;46m##\033[0m\n";
    }
    for (int j = 0; j < width; ++j) std::cout << "\033[36;46m##\033[0m";
    printf("\n\033[1;33mScore: %d\033[0m\n", score);
}

void gameLoop(){

    while(running){
        getMove();
        update();
        draw();
    }
    std::cout << "\033[1;35mGAME OVER\033[0m\n";
}

int main(){

    init();
    gameLoop();
}
