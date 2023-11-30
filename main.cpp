#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <queue>
#include "simulation.h"

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 620;

// Function to run manual mode
void runManualMode() {
    std::cout << "Manual Mode Selected" << std::endl;
    simulate();
}

// Function to run auto mode
void runAutoMode() {
    std::cout << "Auto Mode Selected" << std::endl;
    automode();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Grocery Store Simulation");
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.png")) {
        // Failed to load the image
        return -1;
    }

    // Create a sprite and set the texture to the background image
    sf::Sprite backgroundSprite(backgroundTexture);


    // Create title text
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Replace "arial.ttf" with the path to your font file
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    // Create manual mode button
    sf::RectangleShape manualButton(sf::Vector2f(200, 50));
    manualButton.setFillColor(sf::Color(66, 147, 255));
    manualButton.setPosition((WINDOW_WIDTH / 2) - (manualButton.getSize().x / 2) , (WINDOW_HEIGHT / 2)+50);

    sf::Text manualButtonText("Manual Mode", font, 20);
    sf::FloatRect manualButtonBounds = manualButton.getLocalBounds();
    manualButtonText.setOrigin(manualButtonBounds.left + (manualButtonBounds.width / 3.2f),
        manualButtonBounds.top + manualButtonBounds.height / 4.0f);
    manualButtonText.setPosition(manualButton.getPosition() + sf::Vector2f(manualButton.getSize().x / 2,
        manualButton.getSize().y / 2));
    manualButtonText.setFillColor(sf::Color::White);

    // Create auto mode button
    sf::RectangleShape autoButton(sf::Vector2f(200, 50));
    autoButton.setFillColor(sf::Color(151, 200, 158));
    autoButton.setPosition((WINDOW_WIDTH / 2) -(autoButton.getSize().x / 2), (WINDOW_HEIGHT / 2) - 25);

    sf::Text autoButtonText("Auto Mode", font, 20);
    sf::FloatRect autoButtonBounds = autoButton.getLocalBounds();
    autoButtonText.setOrigin(autoButtonBounds.left + autoButtonBounds.width / 3.5f,
        autoButtonBounds.top + autoButtonBounds.height / 4.0f);
    autoButtonText.setPosition(autoButton.getPosition() + sf::Vector2f(autoButton.getSize().x / 2,
        autoButton.getSize().y / 2));
    autoButtonText.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                    if (manualButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        runManualMode();
                    }
                    else if (autoButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        runAutoMode();
                    }
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                // Check if the mouse is hovering over the manual button
                if (manualButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                    manualButton.setFillColor(sf::Color(66, 147, 255)); // Dark blue
                }
                else {
                    manualButton.setFillColor(sf::Color(103, 168, 255)); // Restore original color
                }

                // Check if the mouse is hovering over the auto button
                if (autoButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                    autoButton.setFillColor(sf::Color(126, 187, 134)); // Dark red
                }
                else {
                    autoButton.setFillColor(sf::Color(151, 200, 158)); // Restore original color
                }
            }
        }

        window.draw(backgroundSprite);
        window.draw(manualButton);
        window.draw(manualButtonText);
        window.draw(autoButton);
        window.draw(autoButtonText);
        window.display();
    }

    return 0;
}



