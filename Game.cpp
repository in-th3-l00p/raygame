//
// Created by intheloop on 09.01.2024.
//

#include "Game.h"
#include "config.h"

Game::Game():
    window(
    sf::VideoMode(config::DEFAULT_WIDTH, config::DEFAULT_HEIGHT),
    "Do you miss raving?"
    ) {
    scene = std::unique_ptr<engine::Scene>(new engine::TestScene(window));
}

void Game::run() {
    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        sf::Time dt = deltaClock.restart();
        window.clear();
        scene->update(dt.asSeconds());
        window.display(); // update
    }
}
