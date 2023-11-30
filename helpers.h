
#ifndef HELPERS_H
#define HELPERS_H
#pragma once
#include <string>
// Function to create a 2D grid of tiles
std::vector<std::vector<sf::RectangleShape>> createGrid() {
    std::vector<std::vector<sf::RectangleShape>> grid;
    for (int i = 0; i < NUM_ROWS + 1; ++i) {
        std::vector<sf::RectangleShape> row;
        for (int j = 0; j < NUM_COLS + 1; ++j) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(j * TILE_SIZE, i * TILE_SIZE);
            tile.setOutlineThickness(1);
            tile.setOutlineColor(sf::Color(236, 240, 241));
            row.push_back(tile);
        }
        grid.push_back(row);
    }
    return grid;
}

bool isPositionValid(const sf::Vector2f& position, const std::vector<sf::Vector2i>& shelfPositions, sf::Vector2i& currentCashRegisterPosition) {
    // Check if the position is within the bounds of the grid
    if (position.x < 0 || position.x >= NUM_COLS * TILE_SIZE ||
        position.y < 0 || position.y >= NUM_ROWS * TILE_SIZE) {
        return false;
    }

    // Check if the position collides with a shelf
    for (const auto& shelfPos : shelfPositions) {
        if (position.x + CUSTOMER_SIZE >= shelfPos.y * TILE_SIZE &&
            position.x - CUSTOMER_SIZE < (shelfPos.y) * TILE_SIZE &&
            position.y + CUSTOMER_SIZE >= shelfPos.x * TILE_SIZE &&
            position.y - CUSTOMER_SIZE < (shelfPos.x + SHELF_LENGTH) * TILE_SIZE) {
            return false; // Customer is on a shelf
        }
    }

    // Check if the position is within the queue area
    //sf::Vector2f queuePosition((currentCashRegisterPosition.y + 1) * TILE_SIZE, (currentCashRegisterPosition.x-1) * TILE_SIZE);
    //sf::Vector2f queueSize(TILE_SIZE, TILE_SIZE * customersInQueue);
    //if (position.x - CUSTOMER_SIZE >= queuePosition.x &&
    //    position.y + CUSTOMER_SIZE >= queuePosition.y) {
    //    return false; // Customer is in the queue
    //}

    return true;
}


// Function to check if a position is within the cash register area
bool isInCashRegisterArea(const sf::Vector2f& position) {
    int cashRegisterRow = currentCashRegisterPosition.x;
    int cashRegisterCol = currentCashRegisterPosition.y;
    int cashRegisterX = cashRegisterCol * TILE_SIZE;
    int cashRegisterY = cashRegisterRow * TILE_SIZE;

    //cashRegisterX += CUSTOMER_SIZE;
    //cashRegisterY += CUSTOMER_SIZE;

    // Create a rectangular area around the cash register
   int cashRegisterWidth = TILE_SIZE + CUSTOMER_SIZE ;
   int cashRegisterHeight = TILE_SIZE + CUSTOMER_SIZE ;

    return position.x >= cashRegisterX && position.x < cashRegisterX + 1.2*cashRegisterWidth 
        &&
        position.y >= cashRegisterY && position.y < cashRegisterY + 1.2*cashRegisterHeight
        ;
}

// Function to check if two customers are colliding
bool checkCollision(const Customer& customer1, const Customer& customer2) {
    float dx = customer2.shape.getPosition().x - customer1.shape.getPosition().x;
    float dy = customer2.shape.getPosition().y - customer1.shape.getPosition().y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= CUSTOMER_SIZE;
}

sf::Vector2f normalize(const sf::Vector2f& v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y);
    if (length != 0) {
        return sf::Vector2f(v.x / length, v.y / length);
    }
    else {
        return sf::Vector2f(0, 0);
    }
}

sf::Vector2f calculateReflection(const sf::Vector2f& velocity, sf::Vector2f collisionNormal, bool inQueue) { //obsolete
    // Normalize the velocity vector
    sf::Vector2f normalizedVelocity = velocity;
    float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (length != 0) {
        normalizedVelocity /= length;
    }

    // Calculate the reflection direction using the formula: reflection = velocity - 2 * dot(velocity, normal) * normal
    sf::Vector2f reflection = normalizedVelocity - 2.0f * (normalizedVelocity.x * collisionNormal.x + normalizedVelocity.y * collisionNormal.y) * collisionNormal;

    // Scale the reflection vector to the simulation speed
    reflection *= SPEED;

    // Prevent crossing the top border of the queue
    float queueTopPosition = queueTop - CUSTOMER_SIZE;
    if (collisionNormal.y < 0 && reflection.y > 0 && queueTopPosition < 0 && inQueue) {
        reflection.y = -reflection.y; // Reverse the y-component to bounce back
    }

    return reflection;
}

// Function to check if a position is at the cash register //obsolete
bool isAtCashRegister(const sf::Vector2f& position) {
    int cashRegisterRow = currentCashRegisterPosition.x;
    int cashRegisterCol = currentCashRegisterPosition.y;
    int cashRegisterX = cashRegisterCol * TILE_SIZE;
    int cashRegisterY = cashRegisterRow * TILE_SIZE;

    // Adjust the cash register position to include the customer's radius
    cashRegisterX += 1.5 * CUSTOMER_SIZE;
    cashRegisterY += 1.5 * CUSTOMER_SIZE;

    return position.x >= 1.5 * cashRegisterX && position.x < cashRegisterX + 1.5 * TILE_SIZE &&
        position.y >= 1.5 * cashRegisterY && position.y < cashRegisterY + 1.5 * TILE_SIZE;
}



void emptyCustomerQueue(std::queue<Customer>& customerQueue) {
    while (!customerQueue.empty()) {
        customerQueue.pop();
    }
}

sf::Vector2f calculateAvoidance(const Customer& customer, const std::vector<Customer>& otherCustomers) { //obsolete
    sf::Vector2f avoidance(0.0f, 0.0f);
    for (const auto& otherCustomer : otherCustomers) {
        if (&customer != &otherCustomer && checkCollision(customer, otherCustomer)) {
            // Calculate the direction away from the colliding customer
            sf::Vector2f direction = customer.shape.getPosition() - otherCustomer.shape.getPosition();
            // Normalize the direction vector
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length != 0) {
                direction /= length;
            }
            // Accumulate the avoidance vector
            avoidance += direction;
        }
    }

    // Check if the customer is crossing the top border of the queue
    float queueTopPosition = queueTop - CUSTOMER_SIZE;
    if (customer.shape.getPosition().y < queueTopPosition) {
        sf::Vector2f direction(0.0f, 1.0f); // Move away from the top border of the queue
        avoidance += direction;
    }

    return avoidance;
}

int sum(int x) {
    int sum = 0;
    for (int i = 0; i <= x; i++) {
        sum += tab[i];
    }
    return sum;
}

float checkdelay(int count) {
    float h;
    if (count <= 2) { h = 10.0; nowtime = "6:00";}
    else if (count <= sum(1)) { h = 20.0 / tab[1]; nowtime = "7:00"; }
    else if (count <= sum(2)) { h = 20.0 / tab[2]; nowtime = "8:00";}
    else if (count <= sum(3)) { h = 20.0 / tab[3]; nowtime = "9:00"; }
    else if (count <= sum(4)) { h = 20.0 / tab[4]; nowtime = "10:00";}
    else if (count <= sum(5)) { h = 20.0 / tab[5];  nowtime = "11:00"; }
    else if (count <= sum(6)) { h = 20.0 / tab[6];  nowtime = "12:00"; }
    else if (count <= sum(7)) { h = 20.0 / tab[7];  nowtime = "13:00"; }
    else if (count <= sum(8)) { h = 20.0 / tab[8];  nowtime = "14:00"; }
    else if (count <= sum(9)) { h = 20.0 / tab[9];  nowtime = "15:00"; }
    else if (count <= sum(10)) { h = 20.0 / tab[10];  nowtime = "16:00"; }
    else if (count <= sum(11)) { h = 20.0 / tab[11];  nowtime = "17:00"; }
    else if (count <= sum(12)) { h = 20.0 / tab[12]; nowtime = "18:00"; }
    else if (count <= sum(13)) { h = 20.0 / tab[13];  nowtime = "19:00";}
    else if (count <= sum(14)) { h = 20.0 / tab[14];  nowtime = "20:00"; }
    else { h = 20.0 / tab[15];  nowtime = "21:00";}

    return h;
}



#endif // HEADER_H