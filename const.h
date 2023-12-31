
#ifndef CONST_H
#define CONST_H
#pragma once
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 620;
const int TILE_SIZE = 20;
const int NUM_ROWS = 30;
const int NUM_COLS = 30;
int NUM_CUSTOMERS = 10; // Default value set to 10
float SPEED = 2.0f;      // Default value set to 2.0
const int CUSTOMER_SIZE = 20;
const int SHELF_LENGTH = 1;
const sf::Vector2i ENTRANCE_POSITION(NUM_ROWS - 1, 1); // Entrance position (row, col)
const sf::Vector2i CASH_REGISTER_POSITION(NUM_ROWS - 1, NUM_COLS - 1); // Cash register position
int customersInQueue = 0;
sf::Vector2i currentCashRegisterPosition(NUM_ROWS - 1, NUM_COLS - 1);
float queueTop = CASH_REGISTER_POSITION.y * TILE_SIZE - CUSTOMER_SIZE;
sf::Clock queueTimer;
sf::Time queueDuration = sf::seconds(10.0f); // Duration for the customer in the queue
bool customerAddedToQueue = false;
sf::Vector2i nextCashRegisterPosition = CASH_REGISTER_POSITION;
int donecustomers = 0;
bool isShelfPositions1Selected = true;
bool isShelfPositions2Selected = false;
bool isShelfPositions3Selected = false;
bool isShelfPositions4Selected = false;
std::string nowtime = "6:00";
int customercounter=0;
sf::Clock deletionTimer;
struct Customer {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    sf::Clock clock;
    sf::Time targetTime;
    bool reachedTarget;
    bool deleted;
    bool isShopping;
    sf::Clock shoppingClock;
    sf::Time shoppingDuration;
    sf::Time deletionTime;
};


int tab[16] = { 2,4,5,6,7,8,6,7,8,8,10,9,8,6,5,1 };

std::vector<sf::Vector2i> shelfPositions{
    {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5}, {8, 5}, {9, 5}, {10, 5}, {14, 5}, {15, 5}, {16, 5}, {17, 5}, {18, 5}, {19, 5}, {20, 5}, {21, 5}, {22, 5}, {23, 5},
    {2, 10}, {3, 10}, {4, 10}, {5, 10}, {6, 10}, {7, 10}, {8, 10}, {9, 10}, {10, 10}, {14, 10}, {15, 10}, {16, 10}, {17, 10}, {18, 10}, {19, 10}, {20, 10}, {21, 10}, {22, 10}, {23, 10},
    {2, 15}, {3, 15}, {4, 15}, {5, 15}, {6, 15}, {7, 15}, {8, 15}, {9, 15}, {10, 15}, {14, 15}, {15, 15}, {16, 15}, {17, 15}, {18, 15}, {19, 15}, {20, 15}, {21, 15}, {22, 15}, {23, 15},
    {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 20}, {7, 20}, {8, 20}, {9, 20}, {10, 20}, {14, 20}, {15, 20}, {16, 20}, {17, 20}, {18, 20}, {19, 20}, {20, 20}, {21, 20}, {22, 20}, {23, 20},
    {2, 25}, {3, 25}, {4, 25}, {5, 25}, {6, 25}, {7, 25}, {8, 25}, {9, 25}, {10, 25}, {14, 25}, {15, 25}, {16, 25}, {17, 25}, {18, 25}, {19, 25}, {20, 25}, {21, 25}, {22, 25}, {23, 25}
};


std::vector<sf::Vector2i> shelfPositions1{
    {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5}, {8, 5}, {9, 5}, {10, 5}, {14, 5}, {15, 5}, {16, 5}, {17, 5}, {18, 5}, {19, 5}, {20, 5}, {21, 5}, {22, 5}, {23, 5},
    {2, 10}, {3, 10}, {4, 10}, {5, 10}, {6, 10}, {7, 10}, {8, 10}, {9, 10}, {10, 10}, {14, 10}, {15, 10}, {16, 10}, {17, 10}, {18, 10}, {19, 10}, {20, 10}, {21, 10}, {22, 10}, {23, 10},
    {2, 15}, {3, 15}, {4, 15}, {5, 15}, {6, 15}, {7, 15}, {8, 15}, {9, 15}, {10, 15}, {14, 15}, {15, 15}, {16, 15}, {17, 15}, {18, 15}, {19, 15}, {20, 15}, {21, 15}, {22, 15}, {23, 15},
    {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 20}, {7, 20}, {8, 20}, {9, 20}, {10, 20}, {14, 20}, {15, 20}, {16, 20}, {17, 20}, {18, 20}, {19, 20}, {20, 20}, {21, 20}, {22, 20}, {23, 20},
    {2, 25}, {3, 25}, {4, 25}, {5, 25}, {6, 25}, {7, 25}, {8, 25}, {9, 25}, {10, 25}, {14, 25}, {15, 25}, {16, 25}, {17, 25}, {18, 25}, {19, 25}, {20, 25}, {21, 25}, {22, 25}, {23, 25}
};



std::vector<sf::Vector2i> shelfPositions2{
    {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {4, 9}, {4, 10}, {4, 11}, {4, 12}, {4, 17}, {4, 18}, {4, 19}, {4, 20}, {4, 21}, {4, 22}, {4, 23}, {4, 24}, {4, 25},
    {9, 4}, {9, 5}, {9, 6}, {9, 7}, {9, 8}, {9, 9}, {9, 10}, {9, 11}, {9, 12}, {9, 17}, {9, 18}, {9, 19}, {9, 20}, {9, 21}, {9, 22}, {9, 23}, {9, 24}, {9, 25},
    {14, 4}, {14, 5}, {14, 6}, {14, 7}, {14, 8}, {14, 9}, {14, 10}, {14, 11}, {14, 12}, {14, 17}, {14, 18}, {14, 19}, {14, 20}, {14, 21}, {14, 22}, {14, 23}, {14, 24}, {14, 25},
    {19, 4}, {19, 5}, {19, 6}, {19, 7}, {19, 8}, {19, 9}, {19, 10}, {19, 11}, {19, 12}, {19, 17}, {19, 18}, {19, 19}, {19, 20}, {19, 21}, {19, 22}, {19, 23}, {19, 24}, {19, 25},
    {24, 4}, {24, 5}, {24, 6}, {24, 7}, {24, 8}, {24, 9}, {24, 10}, {24, 11}, {24, 12}, {24, 17}, {24, 18}, {24, 19}, {24, 20}, {24, 21}, {24, 22}, {24, 23}, {24, 24}, {24, 25}
};


std::vector<sf::Vector2i> shelfPositions3{
    // cashiers
    {0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,8},{0,9},{0,10},{0,11},{0,12},{0,13},{0,14},{0,15},{0,16},{0,17},{0,18},{0,19},{0,20},{0,21},{0,22},{0,23},{0,24},{0,25},{0,26},{0,27},{0,28},
    // internal shelves
    {4,5},{4,6},{4,7},{4,8},{4,9},   // shelf 1
    {7,12},{7,13},{7,14},{7,15},{7,16},{7,17},{7,18}, // shelf 2
    {11,21},{11,22},{11,23},{11,24},{11,25}, // shelf 3
    {15,5},{15,6},{15,7},{15,8},{15,9},{15,10},{15,11},{15,12},{15,13},{15,14}, // shelf 4
    {19,16},{19,17},{19,18},{19,19},{19,20},{19,21},{19,22},{19,23},{19,24}, // shelf 5
    {23,5},{23,6},{23,7},{23,8},{23,9},{23,10}, // shelf 6
    {26,14},{26,15},{26,16},{26,17},{26,18},{26,19},{26,20},{26,21}, // shelf 7
};

std::vector<sf::Vector2i> shelfPositions4{
    {0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,8},{0,9},{0,10},{0,11},{0,12},{0,13},{0,14},{0,15},{0,16},{0,17},{0,18},{0,19},{0,20},{0,21},{0,22},{0,23},{0,24},{0,25},{0,26},{0,27},{0,28},{0,29},
    {0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{13,0},{14,0},{15,0},{16,0},{17,0},{18,0},{19,0},{20,0},{21,0},{22,0},{23,0},{24,0},{25,0},{26,0},
    {0,30},{1,30},{2,30},{3,30},{4,30},{5,30},{6,30},{7,30},{8,30},{9,30},{10,30},{11,30},{12,30},{13,30},{14,30},{15,30},{16,30},{17,30},{18,30},{19,30},{20,30},{21,30},{22,30},{23,30},{24,30},{25,30},{26,30},
    {4,4},{5,4},{6,4},{7,4},{8,4},{9,4},{10,4},{11,4},{12,4},{13,4},{14,4},{15,4},{16,4},{17,4},{18,4}, {8,8},{9,8},{10,8},{11,8},{12,8},{13,8},{14,8},{15,8},{16,8},{17,8},{18,8},{19,8},{20,8},{21,8},{22,8},
    {8,26},{9,26},{10,26},{11,26},{12,26},{13,26},{14,26},{15,26},{16,26},{17,26},{18,26},{19,26},{20,26},{21,26},{22,26}, {12,22},{13,22},{14,22},{15,22}, 
    {4,8},{4,9},{4,10},{4,11},{4,12},{4,13},{4,14},{4,15},{4,16},{4,17},{4,18},{4,19},{4,20},{4,21},{4,22},{4,23},{4,24},{4,25},{4,26},
    {22,8},{22,9},{22,10},{22,11},{22,12},{22,13},{22,14},{22,15},{22,16},{22,17},{22,18},{22,19},{22,20},
    {8,8},{8,9},{8,10},{8,11},{8,12},{8,13},{8,14},{8,15},{8,16},{8,17},{8,18},{8,19},{8,20},{8,21},{8,22}, {15,12},{15,13},{15,14},{15,15},{15,16},{15,17},{15,18},{15,19},{15,20},{15,21}
};

#endif // HEADER_H