#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <queue>
#include "const.h"
#include "helpers.h"


int simulate() {
    sf::Vector2i currentCashRegisterPosition(NUM_ROWS - 1, NUM_COLS - 1);
    // Create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Grocery Store Simulation");
SFML: window.setFramerateLimit(60);
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("manual.png")) {
        // Failed to load the image
        return -1;
    }

    // Create a sprite and set the texture to the background image
    sf::Sprite backgroundSprite(backgroundTexture);
    // Create the grid
    std::vector<std::vector<sf::RectangleShape>> grid = createGrid();

    sf::Time entranceDelay = sf::seconds(2.0f); // Delay between each customer's entrance
    std::uniform_real_distribution<float> entranceDelayDistribution(0.5f, 5.0f); // Random distribution for entrance delay
    std::uniform_real_distribution<float> durationDistribution(3.0f, 8.0f);//queue
    std::uniform_real_distribution<float> angleDistribution(0.0f, 2 * 3.14159f); // Uniform distribution for angles

    // Randomness
    std::vector<Customer> customers;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    sf::Clock entranceClock;
    

    // Cash register
    sf::RectangleShape cashRegister(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    cashRegister.setPosition(currentCashRegisterPosition.y * TILE_SIZE, currentCashRegisterPosition.x * TILE_SIZE);
    cashRegister.setFillColor(sf::Color(126, 187, 134));
    sf::RectangleShape entrance(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    entrance.setPosition(ENTRANCE_POSITION.y * TILE_SIZE, ENTRANCE_POSITION.x * TILE_SIZE);
    entrance.setFillColor(sf::Color(103, 168, 255));


    // Simulation timer
    sf::Clock timerClock;
    sf::Time elapsedTime;
    bool timerStarted = false;

    // Start button
    sf::RectangleShape startButton(sf::Vector2f(80, 50));
    startButton.setPosition(WINDOW_WIDTH - 130, 30);
    startButton.setFillColor(sf::Color(103, 168, 255)); // Blue color

    sf::Font font;
    font.loadFromFile("arial.ttf");


    sf::Text startText;
    startText.setFont(font);
    startText.setString("Start");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(WINDOW_WIDTH - 120, 40);

    bool simulationStarted = false;

    // Stop button
    sf::RectangleShape stopButton(sf::Vector2f(80, 50));
    stopButton.setPosition(WINDOW_WIDTH - 250, 30);
    stopButton.setFillColor(sf::Color(151, 200, 158)); // Red color

    sf::Text stopText;
    stopText.setFont(font);
    stopText.setString("Stop");
    stopText.setCharacterSize(24);
    stopText.setFillColor(sf::Color::White);
    stopText.setPosition(WINDOW_WIDTH - 235, 40);

    bool simulationStopped = false;

    sf::RectangleShape switchButton1(sf::Vector2f(80, 50));
    switchButton1.setPosition(WINDOW_WIDTH - 250, 270);
    switchButton1.setFillColor(sf::Color(204, 204, 204)); // Orange color

    sf::Text switchText1;
    switchText1.setFont(font);
    switchText1.setString("Store 1");
    switchText1.setCharacterSize(18);
    switchText1.setFillColor(sf::Color::Black);
    switchText1.setPosition(WINDOW_WIDTH - 240, 280);

    sf::RectangleShape switchButton2(sf::Vector2f(80, 50));
    switchButton2.setPosition(WINDOW_WIDTH - 130, 270);
    switchButton2.setFillColor(sf::Color(204, 204, 204)); // Orange color

    sf::Text switchText2;
    switchText2.setFont(font);
    switchText2.setString("Store 2");
    switchText2.setCharacterSize(18);
    switchText2.setFillColor(sf::Color::Black);
    switchText2.setPosition(WINDOW_WIDTH - 120, 280);

    sf::RectangleShape switchButton3(sf::Vector2f(80, 50));
    switchButton3.setPosition(WINDOW_WIDTH - 250, 330);
    switchButton3.setFillColor(sf::Color(204, 204, 204)); // Orange color

    sf::Text switchText3;
    switchText3.setFont(font);
    switchText3.setString("Store 3");
    switchText3.setCharacterSize(18);
    switchText3.setFillColor(sf::Color::Black);
    switchText3.setPosition(WINDOW_WIDTH - 240, 340);

    sf::RectangleShape switchButton4(sf::Vector2f(80, 50));
    switchButton4.setPosition(WINDOW_WIDTH - 130, 330);
    switchButton4.setFillColor(sf::Color(204, 204, 204)); // Orange color

    sf::Text switchText4;
    switchText4.setFont(font);
    switchText4.setString("Store 4");
    switchText4.setCharacterSize(18);
    switchText4.setFillColor(sf::Color::Black);
    switchText4.setPosition(WINDOW_WIDTH - 120, 340);



    // Customer slider
    sf::Text customerSliderCaption;
    customerSliderCaption.setFont(font);
    customerSliderCaption.setString("No. of customers:");
    customerSliderCaption.setCharacterSize(18);
    customerSliderCaption.setFillColor(sf::Color::Black);
    customerSliderCaption.setPosition(WINDOW_WIDTH - 240, 100);

    sf::RectangleShape customerSlider(sf::Vector2f(200, 10));
    customerSlider.setPosition(WINDOW_WIDTH - 240, 140);
    customerSlider.setFillColor(sf::Color(236, 240, 241)); // Light gray color

    float customerSliderButtonPositionX = WINDOW_WIDTH - 240 + (NUM_CUSTOMERS / 50.0f) * 200;
    sf::CircleShape customerSliderButton(10);
    customerSliderButton.setOrigin(10, 10);
    customerSliderButton.setFillColor(sf::Color(44, 62, 80)); // Dark gray color
    customerSliderButton.setPosition(customerSliderButtonPositionX, 145);

    sf::Text customerSliderValueText;
    customerSliderValueText.setFont(font);
    customerSliderValueText.setCharacterSize(18);
    customerSliderValueText.setFillColor(sf::Color::Black);
    customerSliderValueText.setPosition(WINDOW_WIDTH - 80, 100);

    bool isCustomerSliderActive = false;
    bool isCustomerSliderButtonClicked = false;

    // Speed slider
    sf::Text speedSliderCaption;
    speedSliderCaption.setFont(font);
    speedSliderCaption.setString("Speed:");
    speedSliderCaption.setCharacterSize(18);
    speedSliderCaption.setFillColor(sf::Color::Black);
    speedSliderCaption.setPosition(WINDOW_WIDTH - 240, 200);

    sf::RectangleShape speedSlider(sf::Vector2f(200, 10));
    speedSlider.setPosition(WINDOW_WIDTH - 240, 240);
    speedSlider.setFillColor(sf::Color(236, 240, 241)); // Light gray color

    float speedSliderButtonPositionX = WINDOW_WIDTH - 240 + ((SPEED - 0.25f) / 4.75f) * 200;
    sf::CircleShape speedSliderButton(10);
    speedSliderButton.setOrigin(10, 10);
    speedSliderButton.setFillColor(sf::Color(231, 76, 60)); // Red color
    speedSliderButton.setPosition(speedSliderButtonPositionX, 245);

    sf::Text speedSliderValueText;
    speedSliderValueText.setFont(font);
    speedSliderValueText.setCharacterSize(18);
    speedSliderValueText.setFillColor(sf::Color::Black);
    speedSliderValueText.setPosition(WINDOW_WIDTH - 80, 200);

    bool isSpeedSliderActive = false;
    bool isSpeedSliderButtonClicked = false;

    // Variables for tracking customers in store
    int customersInStore = 0;

    // Customer queue at cash register
    std::queue<Customer> customerQueue;
    sf::Clock deletionTimer;
    queueTimer.restart();

    // Main loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Check if the start button is clicked
                    if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        simulationStarted = true;
                        simulationStopped = false;
                        timerStarted = true;
                        timerClock.restart();
                        elapsedTime = sf::Time::Zero;
                        customers.clear(); // Clear the customers vector
                        entranceClock.restart(); // Reset the entrance clock
                        customersInStore = 0; // Reset the number of customers in store

                    }
                    // Check if the stop button is clicked
                    else if (stopButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        simulationStopped = true;
                        simulationStarted = false;
                        timerStarted = false;
                        customers.clear(); // Clear the customers vector
                        entranceClock.restart(); // Reset the entrance clock
                        timerClock.restart(); // Reset the simulation timer
                        customersInStore = 0; // Reset the number of customers in store
                        emptyCustomerQueue(customerQueue);
                        // Stop simulation and timer if no customers are left

                    }
                    // Check if the customer slider button is clicked
                    else if (customerSliderButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        isCustomerSliderButtonClicked = true;
                    }
                    // Check if the speed slider button is clicked
                    else if (speedSliderButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        isSpeedSliderButtonClicked = true;
                    }
                    else {
                        isCustomerSliderButtonClicked = false;
                        isSpeedSliderButtonClicked = false;
                    }

                    // Check if switch button 1 is clicked
                    if (switchButton1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        isShelfPositions1Selected = true; // Toggle the selection
                        isShelfPositions2Selected = false; // Toggle the selection
                        isShelfPositions3Selected = false; // Toggle the selection
                        isShelfPositions4Selected = false; // Toggle the selection
                        if (isShelfPositions1Selected) {
                            shelfPositions = shelfPositions1; // Assign shelf positions 1
                        }

                        grid = createGrid(); // Recreate the grid with the new shelves
                    }

                    if (switchButton2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        isShelfPositions1Selected = false; // Toggle the selection
                        isShelfPositions2Selected = true; // Toggle the selection
                        isShelfPositions3Selected = false; // Toggle the selection
                        isShelfPositions4Selected = false; // Toggle the selection
                        if (isShelfPositions2Selected) {
                            shelfPositions = shelfPositions2; // Assign shelf positions 1
                        }

                        grid = createGrid(); // Recreate the grid with the new shelves
                    }

                    if (switchButton3.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        isShelfPositions1Selected = false; // Toggle the selection
                        isShelfPositions2Selected = false; // Toggle the selection
                        isShelfPositions3Selected = true; // Toggle the selection
                        isShelfPositions4Selected = false; // Toggle the selection
                        if (isShelfPositions3Selected) {
                            shelfPositions = shelfPositions3; // Assign shelf positions 1
                        }

                        grid = createGrid(); // Recreate the grid with the new shelves
                    }

                    if (switchButton4.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        isShelfPositions1Selected = false; // Toggle the selection
                        isShelfPositions2Selected = false; // Toggle the selection
                        isShelfPositions3Selected = false; // Toggle the selection
                        isShelfPositions4Selected = true; // Toggle the selection
                        if (isShelfPositions4Selected) {
                            shelfPositions = shelfPositions4; // Assign shelf positions 1
                        }

                        grid = createGrid(); // Recreate the grid with the new shelves
                    }


                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isCustomerSliderButtonClicked = false;
                    isSpeedSliderButtonClicked = false;
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                // Check if the start button is hovered
                if (startButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    startButton.setFillColor(sf::Color(66, 147, 255));
                }
                else {
                    startButton.setFillColor(sf::Color(103, 168, 255)); // Blue color
                }

                // Check if the stop button is hovered
                if (stopButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    stopButton.setFillColor(sf::Color(151, 200, 158));
                }
                else {
                    stopButton.setFillColor(sf::Color(161, 205, 167)); // Red color
                }

                // Check if the customer slider button is hovered
                if (customerSliderButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y) && !isCustomerSliderButtonClicked) {
                    customerSliderButton.setFillColor(sf::Color(44, 62, 80)); // Dark gray color
                }
                else {
                    customerSliderButton.setFillColor(sf::Color(52, 73, 94)); // Dark gray color
                }

                // Check if the speed slider button is hovered
                if (speedSliderButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y) && !isSpeedSliderButtonClicked) {
                    speedSliderButton.setFillColor(sf::Color(52, 73, 94));
                }
                else {
                    speedSliderButton.setFillColor(sf::Color(52, 73, 94)); // Red color
                }
                if (isCustomerSliderButtonClicked) {
                    float mousePositionX = static_cast<float>(event.mouseMove.x);
                    if (mousePositionX < WINDOW_WIDTH - 240)
                        mousePositionX = WINDOW_WIDTH - 240;
                    else if (mousePositionX > WINDOW_WIDTH - 40)
                        mousePositionX = WINDOW_WIDTH - 40;

                    float sliderValue = (mousePositionX - (WINDOW_WIDTH - 240)) / 200.0f;
                    NUM_CUSTOMERS = static_cast<int>(sliderValue * 50);
                    if (NUM_CUSTOMERS < 0)
                        NUM_CUSTOMERS = 0;
                    else if (NUM_CUSTOMERS > 50)
                        NUM_CUSTOMERS = 50;

                    customerSliderButtonPositionX = mousePositionX;
                    customerSliderButton.setPosition(customerSliderButtonPositionX, 145);
                }
                else if (isSpeedSliderButtonClicked) {
                    float mousePositionX = static_cast<float>(event.mouseMove.x);
                    if (mousePositionX < WINDOW_WIDTH - 240)
                        mousePositionX = WINDOW_WIDTH - 240;
                    else if (mousePositionX > WINDOW_WIDTH - 40)
                        mousePositionX = WINDOW_WIDTH - 40;

                    float sliderValue = (mousePositionX - (WINDOW_WIDTH - 240)) / 200.0f;
                    SPEED = 0.25f + sliderValue * 4.75f;
                    if (SPEED < 0.25f)
                        SPEED = 0.25f;
                    else if (SPEED > 5.0f)
                        SPEED = 5.0f;

                    speedSliderButtonPositionX = mousePositionX;
                    speedSliderButton.setPosition(speedSliderButtonPositionX, 245);
                }
            }
        }

        // Remove customers from the queue
        if (!customerQueue.empty()) {
            sf::Time nextShoppingDuration = sf::seconds(durationDistribution(gen));
            Customer& frontCustomer = customerQueue.front();
            if (deletionTimer.getElapsedTime() >= nextShoppingDuration) {
                customerQueue.pop();
                deletionTimer.restart();
                // Generate a random time between 3 and 8 seconds for the next customer in the queue
                nextShoppingDuration = sf::seconds(durationDistribution(gen));
                frontCustomer.shoppingDuration = nextShoppingDuration;
                donecustomers++; // Increment the number of deleted customers
                currentCashRegisterPosition.y += 1;
            }
        }


        // Update logic
        if (simulationStarted && !simulationStopped && customers.size() < (NUM_CUSTOMERS - customerQueue.size() - donecustomers) && entranceClock.getElapsedTime() > entranceDelay) {
            // Add a new customer
            Customer customer;
            customer.shape.setRadius(10);
            customer.shape.setFillColor(sf::Color(66, 147, 255)); // Blue color
            customer.shape.setOutlineThickness(1);
            customer.shape.setOutlineColor(sf::Color::White);
            customer.shape.setPosition(ENTRANCE_POSITION.y * TILE_SIZE, ENTRANCE_POSITION.x * TILE_SIZE); // Entrance position for all customers

            float angle = angleDistribution(gen);
            float dx = std::sin(angle);
            angle = angleDistribution(gen);
            float dy = std::sin(angle);
            customer.velocity = sf::Vector2f(dx * SPEED, dy * SPEED);

            customer.clock.restart();
            sf::Time targetTime = sf::seconds(300.0f);
            customer.targetTime = targetTime;
            customer.reachedTarget = false;
            customer.deleted = false;
            customer.isShopping = true;
            customer.shoppingClock.restart();
            customer.shoppingDuration = sf::seconds(10.0f);

            customers.push_back(customer);
            entranceDelay = sf::seconds(entranceDelayDistribution(gen)); // Assign a random entrance delay for the next customer
            entranceClock.restart();
            customersInStore++; // Increment the number of customers in store
        }

        // Move customers
        for (auto& customer : customers) {

            // Update customer position
            sf::Vector2f position = customer.shape.getPosition();
            sf::Vector2f newPosition = position + customer.velocity;
            if (isPositionValid(newPosition, shelfPositions, currentCashRegisterPosition)) {
                // Check for collisions with other customers
                for (auto& otherCustomer : customers) {
                    if (&customer != &otherCustomer && checkCollision(customer, otherCustomer)) {

                        // Adjust customer velocities based on distances to other customers
                        sf::Vector2f separationVector = position - otherCustomer.shape.getPosition();

                        // Adjust velocities after collision
                        customer.velocity += separationVector;
                        otherCustomer.velocity -= separationVector;
                        customer.velocity = normalize(customer.velocity) * SPEED;
                        otherCustomer.velocity = normalize(otherCustomer.velocity) * SPEED;

                        break;
                    }
                }
                customer.shape.setPosition(newPosition);
            }
            else {
                // Randomly change the direction of the customer when hitting something
                float angle = angleDistribution(gen);
                float dx = std::sin(angle);
                angle = angleDistribution(gen);
                float dy = std::sin(angle);
                customer.velocity = sf::Vector2f(dx, dy);
                customer.velocity = normalize(customer.velocity) * SPEED;
            }
        }

        // Remove customers who have reached the cash register or touched it
        customers.erase(std::remove_if(customers.begin(), customers.end(), [&](Customer& customer) {
            sf::Vector2f position = customer.shape.getPosition();
            bool inCashRegisterArea = isInCashRegisterArea(position);

            if ((inCashRegisterArea) && !customer.deleted) {
                // Move customer to queue if they are near or at the cash register
                customerQueue.push(customer);
                customer.deleted = true;
                customersInStore--; // Decrement the number of customers in store
                currentCashRegisterPosition.y -= 1;
                customerAddedToQueue = true; // Set the flag to true
                return true;
            }

            return false;
            }), customers.end());




        // Update simulation timer
        if (timerStarted && !simulationStopped) {
            elapsedTime += timerClock.restart();
        }


        // Draw the scene
        window.draw(backgroundSprite);
        // Draw the grid
        for (const auto& row : grid) {
            for (const auto& tile : row) {
                window.draw(tile);
            }
        }

        // Draw the shelves
        for (const auto& shelfPos : shelfPositions) {
            for (int i = 0; i < SHELF_LENGTH; ++i) {
                sf::RectangleShape shelf(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                shelf.setPosition(shelfPos.y * TILE_SIZE, (shelfPos.x + i) * TILE_SIZE);
                shelf.setFillColor(sf::Color(204, 204, 204)); // Gray color
                window.draw(shelf);
            }
        }

        // Draw the cash register
        window.draw(cashRegister);
        window.draw(entrance);

        // Draw the customers
        for (const auto& customer : customers) {
            if (!customer.reachedTarget) {
                window.draw(customer.shape);
            }
        }

        // Draw the customer queue
        std::queue<Customer> tempQueue = customerQueue;
        int i = 0;
        while (!tempQueue.empty()) {
            Customer customer = tempQueue.front();
            customer.shape.setPosition(CASH_REGISTER_POSITION.y * TILE_SIZE - (i) * 20 - CUSTOMER_SIZE, CASH_REGISTER_POSITION.x * TILE_SIZE);
            window.draw(customer.shape);
            tempQueue.pop();
            i++;
        }


        // Draw the simulation timer
        std::stringstream ss;
        int minutes = static_cast<int>(elapsedTime.asSeconds()) / 60;
        int seconds = static_cast<int>(elapsedTime.asSeconds()) % 60;
        ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds;
        std::string timerText = "Time: " + ss.str();
        sf::Text timer(sf::String(timerText), font, 18);
        timer.setPosition(WINDOW_WIDTH - 240, (WINDOW_HEIGHT / 2) + 100);
        timer.setFillColor(sf::Color::Black);
        window.draw(timer);


        // Draw the start button
        window.draw(startButton);
        window.draw(startText);

        // Draw the stop button
        window.draw(stopButton);
        window.draw(stopText);

        // Draw the customer slider
        window.draw(customerSliderCaption);
        window.draw(customerSlider);
        window.draw(customerSliderButton);
        customerSliderValueText.setString(std::to_string(NUM_CUSTOMERS));
        window.draw(customerSliderValueText);
        window.draw(switchButton1);
        window.draw(switchText1);
        window.draw(switchButton2);
        window.draw(switchText2);
        window.draw(switchButton3);
        window.draw(switchText3);
        window.draw(switchButton4);
        window.draw(switchText4);


        // Draw the speed slider
        window.draw(speedSliderCaption);
        window.draw(speedSlider);
        window.draw(speedSliderButton);
        std::stringstream speedSliderValueStream;
        speedSliderValueStream << std::fixed << std::setprecision(2) << std::ceil(SPEED * 100) / 100; // Round up to two decimals
        speedSliderValueText.setString(speedSliderValueStream.str());

        window.draw(speedSliderValueText);

        // Draw the customers in store counter
        sf::Text customersInStoreText;
        customersInStoreText.setFont(font);
        customersInStoreText.setString("Customers in store: " + std::to_string(customersInStore + customerQueue.size()));
        customersInStoreText.setCharacterSize(18);
        customersInStoreText.setFillColor(sf::Color::Black);
        customersInStoreText.setPosition(WINDOW_WIDTH - 240, (WINDOW_HEIGHT / 2) + 130);
        window.draw(customersInStoreText);

        window.display();
    }

    return 0;
}


int automode() {
    sf::Vector2i currentCashRegisterPosition(NUM_ROWS - 1, NUM_COLS - 1);
    // Create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Grocery Store Simulation");
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("auto.png")) {return -1;}

    // Create a sprite and set the texture to the background image
    sf::Sprite backgroundSprite(backgroundTexture);
    // Create the grid
    std::vector<std::vector<sf::RectangleShape>> grid = createGrid();

    // Randomness
    std::vector<Customer> customers;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDistribution(0.0f, 2 * 3.14159f); // Uniform distribution for angles

    sf::Clock entranceClock;
    sf::Time entranceDelay = sf::seconds(6.0f); // Delay between each customer's entrance

    // Cash register
    sf::RectangleShape cashRegister(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    cashRegister.setPosition(currentCashRegisterPosition.y * TILE_SIZE, currentCashRegisterPosition.x * TILE_SIZE);
    cashRegister.setFillColor(sf::Color(126, 187, 134));
    sf::RectangleShape entrance(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    entrance.setPosition(ENTRANCE_POSITION.y * TILE_SIZE, ENTRANCE_POSITION.x * TILE_SIZE);
    entrance.setFillColor(sf::Color(103, 168, 255));


    // Simulation timer
    sf::Clock timerClock;
    sf::Time elapsedTime;
    bool timerStarted = false;

    // Start button
    sf::RectangleShape startButton(sf::Vector2f(80, 50));
    startButton.setPosition(WINDOW_WIDTH - 130, 30);
    startButton.setFillColor(sf::Color(103, 168, 255)); // Blue color

    sf::Font font;
    font.loadFromFile("arial.ttf");


    sf::Text startText;
    startText.setFont(font);
    startText.setString("Start");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(WINDOW_WIDTH - 120, 40);

    bool simulationStarted = false;

    // Stop button
    sf::RectangleShape stopButton(sf::Vector2f(80, 50));
    stopButton.setPosition(WINDOW_WIDTH - 250, 30);
    stopButton.setFillColor(sf::Color(151, 200, 158)); // Red color

    sf::Text stopText;
    stopText.setFont(font);
    stopText.setString("Stop");
    stopText.setCharacterSize(24);
    stopText.setFillColor(sf::Color::White);
    stopText.setPosition(WINDOW_WIDTH - 235, 40);

    bool simulationStopped = false;

    sf::RectangleShape switchButton1(sf::Vector2f(80, 50));
    switchButton1.setPosition(WINDOW_WIDTH - 250, 270);
    switchButton1.setFillColor(sf::Color(204, 204, 204)); // Orange color

    sf::Text switchText1;
    switchText1.setFont(font);
    switchText1.setString("Store 1");
    switchText1.setCharacterSize(18);
    switchText1.setFillColor(sf::Color::Black);
    switchText1.setPosition(WINDOW_WIDTH - 240, 280);

    sf::RectangleShape switchButton2(sf::Vector2f(80, 50));
    switchButton2.setPosition(WINDOW_WIDTH - 130, 270);
    switchButton2.setFillColor(sf::Color(204, 204, 204)); // Orange color

    sf::Text switchText2;
    switchText2.setFont(font);
    switchText2.setString("Store 2");
    switchText2.setCharacterSize(18);
    switchText2.setFillColor(sf::Color::Black);
    switchText2.setPosition(WINDOW_WIDTH - 120, 280);

    sf::RectangleShape switchButton3(sf::Vector2f(80, 50));
    switchButton3.setPosition(WINDOW_WIDTH - 250, 330);
    switchButton3.setFillColor(sf::Color(204, 204, 204)); // Orange color

    sf::Text switchText3;
    switchText3.setFont(font);
    switchText3.setString("Store 3");
    switchText3.setCharacterSize(18);
    switchText3.setFillColor(sf::Color::Black);
    switchText3.setPosition(WINDOW_WIDTH - 240, 340);

    sf::RectangleShape switchButton4(sf::Vector2f(80, 50));
    switchButton4.setPosition(WINDOW_WIDTH - 130, 330);
    switchButton4.setFillColor(sf::Color(204, 204, 204)); // Orange color

    sf::Text switchText4;
    switchText4.setFont(font);
    switchText4.setString("Store 4");
    switchText4.setCharacterSize(18);
    switchText4.setFillColor(sf::Color::Black);
    switchText4.setPosition(WINDOW_WIDTH - 120, 340);

    // Speed slider
    sf::Text speedSliderCaption;
    speedSliderCaption.setFont(font);
    speedSliderCaption.setString("Speed:");
    speedSliderCaption.setCharacterSize(18);
    speedSliderCaption.setFillColor(sf::Color::Black);
    speedSliderCaption.setPosition(WINDOW_WIDTH - 240, 200);

    sf::RectangleShape speedSlider(sf::Vector2f(200, 10));
    speedSlider.setPosition(WINDOW_WIDTH - 240, 240);
    speedSlider.setFillColor(sf::Color(236, 240, 241)); // Light gray color

    float speedSliderButtonPositionX = WINDOW_WIDTH - 240 + ((SPEED - 0.25f) / 4.75f) * 200;
    sf::CircleShape speedSliderButton(10);
    speedSliderButton.setOrigin(10, 10);
    speedSliderButton.setFillColor(sf::Color(231, 76, 60)); // Red color
    speedSliderButton.setPosition(speedSliderButtonPositionX, 245);

    sf::Text speedSliderValueText;
    speedSliderValueText.setFont(font);
    speedSliderValueText.setCharacterSize(18);
    speedSliderValueText.setFillColor(sf::Color::Black);
    speedSliderValueText.setPosition(WINDOW_WIDTH - 80, 200);

    bool isSpeedSliderActive = false;
    bool isSpeedSliderButtonClicked = false;

    // Variables for tracking customers in store
    int customersInStore = 0;

    // Customer queue at cash register
    std::queue<Customer> customerQueue;
    sf::Clock deletionTimer;
    queueTimer.restart();

    // Main loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Check if the start button is clicked
                    if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        simulationStarted = true;
                        simulationStopped = false;
                        timerStarted = true;
                        timerClock.restart();
                        elapsedTime = sf::Time::Zero;
                        customers.clear(); // Clear the customers vector
                        entranceClock.restart(); // Reset the entrance clock
                        customersInStore = 0; // Reset the number of customers in store

                    }
                    // Check if the stop button is clicked
                    else if (stopButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        simulationStopped = true;
                        simulationStarted = false;
                        timerStarted = false;
                        customers.clear(); // Clear the customers vector
                        entranceClock.restart(); // Reset the entrance clock
                        timerClock.restart(); // Reset the simulation timer
                        customersInStore = 0; // Reset the number of customers in store
                        emptyCustomerQueue(customerQueue);
                        // Stop simulation and timer if no customers are left

                    }
                    

                    // Check if switch button 1 is clicked
                    if (switchButton1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        isShelfPositions1Selected = true; // Toggle the selection
                        isShelfPositions2Selected = false; // Toggle the selection
                        isShelfPositions3Selected = false; // Toggle the selection
                        isShelfPositions4Selected = false; // Toggle the selection
                        if (isShelfPositions1Selected) {
                            shelfPositions = shelfPositions1; // Assign shelf positions 1
                        }

                        grid = createGrid(); // Recreate the grid with the new shelves
                    }

                    if (switchButton2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        isShelfPositions1Selected = false; // Toggle the selection
                        isShelfPositions2Selected = true; // Toggle the selection
                        isShelfPositions3Selected = false; // Toggle the selection
                        isShelfPositions4Selected = false; // Toggle the selection
                        if (isShelfPositions2Selected) {
                            shelfPositions = shelfPositions2; // Assign shelf positions 1
                        }

                        grid = createGrid(); // Recreate the grid with the new shelves
                    }

                    if (switchButton3.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        isShelfPositions1Selected = false; // Toggle the selection
                        isShelfPositions2Selected = false; // Toggle the selection
                        isShelfPositions3Selected = true; // Toggle the selection
                        isShelfPositions4Selected = false; // Toggle the selection
                        if (isShelfPositions3Selected) {
                            shelfPositions = shelfPositions3; // Assign shelf positions 1
                        }

                        grid = createGrid(); // Recreate the grid with the new shelves
                    }

                    if (switchButton4.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !simulationStarted) {
                        isShelfPositions1Selected = false; // Toggle the selection
                        isShelfPositions2Selected = false; // Toggle the selection
                        isShelfPositions3Selected = false; // Toggle the selection
                        isShelfPositions4Selected = true; // Toggle the selection
                        if (isShelfPositions4Selected) {
                            shelfPositions = shelfPositions4; // Assign shelf positions 1
                        }

                        grid = createGrid(); // Recreate the grid with the new shelves
                    }


                }
            }
           
            else if (event.type == sf::Event::MouseMoved) {
                // Check if the start button is hovered
                if (startButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    startButton.setFillColor(sf::Color(66, 147, 255));
                }
                else {
                    startButton.setFillColor(sf::Color(103, 168, 255)); // Blue color
                }

                // Check if the stop button is hovered
                if (stopButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    stopButton.setFillColor(sf::Color(151, 200, 158));
                }
                else {
                    stopButton.setFillColor(sf::Color(161, 205, 167)); // Red color
                }
            }
        }

        // Remove customers from the queue after spending 10 seconds in the front
        if (!customerQueue.empty()) {
            std::uniform_real_distribution<float> durationDistribution(3.0f, 8.0f);
            sf::Time nextShoppingDuration = sf::seconds(durationDistribution(gen));
            Customer& frontCustomer = customerQueue.front();
            if (deletionTimer.getElapsedTime() >= nextShoppingDuration) {
                customerQueue.pop();
                deletionTimer.restart();
                // Generate a random time between 3 and 10 seconds for the next customer in the queue
                sf::Time nextShoppingDuration = sf::seconds(durationDistribution(gen));
                frontCustomer.shoppingDuration = nextShoppingDuration;
                donecustomers++; // Increment the number of deleted customers
                currentCashRegisterPosition.y += 1;
            }
        }


        // Update logic
        if (simulationStarted && !simulationStopped && customers.size() < (100 - customerQueue.size() - donecustomers) && entranceClock.getElapsedTime() > entranceDelay) {
            // Add a new customer
            Customer customer;
            customer.shape.setRadius(10);
            customer.shape.setFillColor(sf::Color(66, 147, 255)); // Blue color
            customer.shape.setOutlineThickness(1);
            customer.shape.setOutlineColor(sf::Color::White);
            customer.shape.setPosition(ENTRANCE_POSITION.y * TILE_SIZE, ENTRANCE_POSITION.x * TILE_SIZE); // Entrance position for all customers

            float angle = angleDistribution(gen);
            float dx = std::cos(angle);
            float dy = std::sin(angle);
            customer.velocity = sf::Vector2f(dx * SPEED, dy * SPEED); // Adjusted velocity for smoother movement

            customer.clock.restart();
            sf::Time targetTime = sf::seconds(300.0f);
            customer.targetTime = targetTime;
            customer.reachedTarget = false;
            customer.deleted = false;
            customer.isShopping = true;
            customer.shoppingClock.restart();
            customer.shoppingDuration = sf::seconds(10.0f);
            customercounter++;
            customers.push_back(customer);
            entranceDelay = sf::seconds(checkdelay(customercounter)); // Assign a random entrance delay for the next customer
            entranceClock.restart();
            customersInStore++; // Increment the number of customers in store
        }

        // Move customers
        for (auto& customer : customers) {
            // Check if the customer has finished shopping
            if (customer.isShopping && customer.shoppingClock.getElapsedTime() >= customer.shoppingDuration) {
                customer.isShopping = false;
                customer.reachedTarget = false;
            }

            // Check if the customer has reached the target time and not reached the target position
            if (customer.clock.getElapsedTime() >= customer.targetTime && !customer.reachedTarget && !customer.deleted && !customer.isShopping) {
                sf::Vector2f targetPosition = sf::Vector2f(currentCashRegisterPosition.y * TILE_SIZE - CUSTOMER_SIZE, currentCashRegisterPosition.x * TILE_SIZE); // Target position at the cash register
                customer.velocity = (targetPosition - customer.shape.getPosition()) / 50.0f; // Adjusted velocity for smoother movement
                customer.reachedTarget = true;
            }

            // Update customer position
            sf::Vector2f position = customer.shape.getPosition();
            sf::Vector2f newPosition = position + customer.velocity;
            if (isPositionValid(newPosition, shelfPositions, currentCashRegisterPosition)) {
                // Check for collisions with other customers
                for (auto& otherCustomer : customers) {
                    if (&customer != &otherCustomer && checkCollision(customer, otherCustomer)) {

                        // Adjust customer velocities based on distances to other customers
                        sf::Vector2f separationVector = position - otherCustomer.shape.getPosition();

                        // Adjust velocities after collision
                        customer.velocity += separationVector;
                        otherCustomer.velocity -= separationVector;
                        customer.velocity = normalize(customer.velocity) * SPEED;
                        otherCustomer.velocity = normalize(otherCustomer.velocity) * SPEED;

                        break;
                    }
                }
                customer.shape.setPosition(newPosition);
            }
            else {
                // Randomly change the direction of the customer when hitting a shelf
                float angle = angleDistribution(gen);
                float dx = std::cos(angle);
                float dy = std::sin(angle);
                customer.velocity = sf::Vector2f(dx * SPEED, dy * SPEED);
                customer.velocity = normalize(customer.velocity) * SPEED;
            }
        }

        // Remove customers who have reached the cash register or touched it
        customers.erase(std::remove_if(customers.begin(), customers.end(), [&](Customer& customer) {
            sf::Vector2f position = customer.shape.getPosition();
            bool inCashRegisterArea = isInCashRegisterArea(position);

            if ((inCashRegisterArea) && !customer.deleted) {
                // Move customer to queue if they are near or at the cash register
                customerQueue.push(customer);
                customer.deleted = true;
                customersInStore--; // Decrement the number of customers in store
                currentCashRegisterPosition.y -= 1;
                customerAddedToQueue = true; // Set the flag to true
                return true;
            }

            return false;
            }), customers.end());

        // Update simulation timer
        if (timerStarted && !simulationStopped) {
            elapsedTime += timerClock.restart();
        }


        // Draw the scene
        window.draw(backgroundSprite);
        // Draw the grid
        for (const auto& row : grid) {
            for (const auto& tile : row) {
                window.draw(tile);
            }
        }

        // Draw the shelves
        for (const auto& shelfPos : shelfPositions) {
            for (int i = 0; i < SHELF_LENGTH; ++i) {
                sf::RectangleShape shelf(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                shelf.setPosition(shelfPos.y * TILE_SIZE, (shelfPos.x + i) * TILE_SIZE);
                shelf.setFillColor(sf::Color(204, 204, 204)); // Gray color
                window.draw(shelf);
            }
        }

        // Draw the cash register
        window.draw(cashRegister);
        window.draw(entrance);

        // Draw the customers
        for (const auto& customer : customers) {
            if (!customer.reachedTarget) {
                window.draw(customer.shape);
            }
        }

        // Draw the customer queue
        std::queue<Customer> tempQueue = customerQueue;
        int i = 0;
        while (!tempQueue.empty()) {
            Customer customer = tempQueue.front();
            customer.shape.setPosition(CASH_REGISTER_POSITION.y * TILE_SIZE - i * 20 - CUSTOMER_SIZE, CASH_REGISTER_POSITION.x * TILE_SIZE);
            window.draw(customer.shape);
            tempQueue.pop();
            i++;
        }


        // Draw the simulation timer
        std::string timerText = "Time: " + nowtime;
        sf::Text timer(sf::String(timerText), font, 18);
        timer.setPosition(WINDOW_WIDTH - 240, (WINDOW_HEIGHT / 2) + 100);
        timer.setFillColor(sf::Color::Black);
        window.draw(timer);


        // Draw the start button
        window.draw(startButton);
        window.draw(startText);

        // Draw the stop button
        window.draw(stopButton);
        window.draw(stopText);

        // Draw the customer slider
        
        window.draw(switchButton1);
        window.draw(switchText1);
        window.draw(switchButton2);
        window.draw(switchText2);
        window.draw(switchButton3);
        window.draw(switchText3);
        window.draw(switchButton4);
        window.draw(switchText4);

        // Draw the customers in store counter
        sf::Text customersInStoreText;
        customersInStoreText.setFont(font);
        customersInStoreText.setString("Customers in store: " + std::to_string(customersInStore + customerQueue.size()));
        customersInStoreText.setCharacterSize(18);
        customersInStoreText.setFillColor(sf::Color::Black);
        customersInStoreText.setPosition(WINDOW_WIDTH - 240, (WINDOW_HEIGHT / 2) + 130);
        window.draw(customersInStoreText);

        window.display();
    }

    return 0;
}


