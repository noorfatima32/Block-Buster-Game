#include <graphics.h> 
#include <conio.h>
#include <iostream>
#include <vector>
#include <cstdlib>  
#include <ctime>  
#include <sstream>  

using namespace std;

#define WIDTH 800
#define HEIGHT 600
#define BLOCK_COLOR GREEN  
#define MOVE_INTERVAL 30  
#define GAME_TIME 60  // Game time in seconds

// Bullet structure
struct Bullet {
    int x, y;
    bool active;
    Bullet(int _x, int _y) {
        x = _x;
        y = _y;
        active = true;
    }
};

// Paddle (Gun) structure
struct Paddle {
    int x, y, width, height, speed;
    Paddle() {
        x = WIDTH / 2 - 50;
        y = HEIGHT - 30;
        width = 100;
        height = 10;
        speed = 15;
    }
    void moveLeft() {
        if (x > 0) x -= speed;
    }
    void moveRight() {
        if (x < WIDTH - width) x += speed;
    }
};

// Block structure (with random movement)
struct Block {
    int x, y, width, height;
    bool destroyed;
    int direction;
    int moveCounter;
    int color;
    Block(int _x, int _y, int _color) {
        x = _x;
        y = _y;
        width = 80;
        height = 30;
        destroyed = false;
        direction = rand() % 4;
        moveCounter = MOVE_INTERVAL;
        color = _color;
    }
    void move() {
        if (moveCounter > 0) {
            moveCounter--;  
        } else {
            direction = rand() % 4;
            moveCounter = MOVE_INTERVAL;  
        }
        if (direction == 0 && x + width < WIDTH) x += 2;
        else if (direction == 1 && x > 0) x -= 2;
        else if (direction == 2 && y > 50) y -= 2;
        else if (direction == 3 && y + height < HEIGHT / 2) y += 2;
    }
};

// Game variables
Paddle paddle;
vector<Block> blocks;
vector<Bullet> bullets;
bool isGameOver = false;
int score = 0;
time_t startTime;
int level = 1;

void initGame() {
    paddle = Paddle();
    blocks.clear();
    bullets.clear();
    srand(time(0));
    for (int i = 0; i < 6 + level; i++) {
        for (int j = 0; j < 3 + level / 2; j++) {
            int blockColor = (level == 2) ? ((i + j) % 2 == 0 ? RED : WHITE) : GREEN;
            blocks.push_back(Block(50 + i * 90, 50 + j * 40, blockColor));
        }
    }
    startTime = time(0);
}

void drawPaddle() {
    setfillstyle(SOLID_FILL, BLUE);
    bar(paddle.x, paddle.y, paddle.x + paddle.width, paddle.y + paddle.height);
}

void drawBullets() {
    setfillstyle(SOLID_FILL, RED);
    for (size_t i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            fillellipse(bullets[i].x, bullets[i].y, 5, 5);
        }
    }
}

void drawBlocks() {
    for (size_t i = 0; i < blocks.size(); i++) {
        if (!blocks[i].destroyed) {
            setfillstyle(SOLID_FILL, blocks[i].color);
            bar(blocks[i].x, blocks[i].y, blocks[i].x + blocks[i].width, blocks[i].y + blocks[i].height);
            blocks[i].move();
        }
    }
}

void checkCollisions() {
    for (size_t i = 0; i < bullets.size(); i++) {
        if (!bullets[i].active) continue;
        for (size_t j = 0; j < blocks.size(); j++) {
            if (!blocks[j].destroyed &&
                bullets[i].x >= blocks[j].x && bullets[i].x <= blocks[j].x + blocks[j].width &&
                bullets[i].y >= blocks[j].y && bullets[i].y <= blocks[j].y + blocks[j].height) {
                blocks[j].destroyed = true;
                bullets[i].active = false;
                score += 10;
            }
        }
    }
}

void moveBullets() {
    for (size_t i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            bullets[i].y -= 8;
            if (bullets[i].y < 0) {
                bullets[i].active = false;
            }
        }
    }
}

void handleInput() {
    if (kbhit()) {
        char ch = getch();
        if (ch == 'a') paddle.moveLeft();
        if (ch == 'd') paddle.moveRight();
        if (ch == ' ') {
            bullets.push_back(Bullet(paddle.x + paddle.width / 2, paddle.y - 10));
        }
    }
}

void drawHUD() {
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    stringstream ss;
    ss << "Score: " << score;
    outtextxy(20, 10, const_cast<char*>(ss.str().c_str()));
    
    int elapsedTime = time(0) - startTime;
    int remainingTime = GAME_TIME - elapsedTime;
    
    stringstream timeStream;
    timeStream << "Time: " << remainingTime << "s";
    outtextxy(WIDTH - 150, 10, const_cast<char*>(timeStream.str().c_str()));
    
    stringstream levelStream;
    levelStream << "Level: " << level;
    outtextxy(WIDTH / 2 - 50, 10, const_cast<char*>(levelStream.str().c_str()));
}

void checkGameOver() {
    int elapsedTime = time(0) - startTime;
    if (elapsedTime >= GAME_TIME) {
        isGameOver = true;
    }
}

int main() {
    initwindow(WIDTH, HEIGHT, "Shooter Block Breaker");
    initGame();
    
    while (!isGameOver) {
        cleardevice();
        handleInput();
        moveBullets();
        checkCollisions();
        drawPaddle();
        drawBullets();
        drawBlocks();
        drawHUD();
        checkGameOver();
        delay(20);
        
        bool allBlocksDestroyed = true;
        for (size_t i = 0; i < blocks.size(); i++) {
            if (!blocks[i].destroyed) {
                allBlocksDestroyed = false;
                break;
            }
        }
        if (allBlocksDestroyed) {
            level++;
            initGame();
        }
    }
    
    closegraph();
    return 0;
}

    
    
