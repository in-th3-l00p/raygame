//
// Created by intheloop on 11.01.2024.
//

#include "Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace engine {
    Player::Player(
            const std::set<std::unique_ptr<Entity>>& container,
            const std::map<std::string, Entity*>& labels,
            engine::math::Vec2<float> position,
            engine::math::Vec2<float> direction,
            float radius,
            float speed,
            float rotateSpeed
    ):
        Entity(container, labels),
        position(position),
        direction(direction),
        radius(radius),
        speed(speed),
        rotateSpeed(rotateSpeed),
        stamina(80),
        maxStamina(100),
        staminaRegen(20),
        isRunning(false),
        map(dynamic_cast<Map&>(*labels.at("map")))
        {
        setZIndex(1000);
    }

    void Player::render(sf::RenderWindow &window) {
        sf::CircleShape circle(radius);
        circle.setPosition(position.x - radius, position.y - radius);
        circle.setFillColor(sf::Color::Red);
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(position.x, position.y);
        line[1].position = sf::Vector2f(position.x + direction.x * 30, position.y + direction.y * 30);
        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
            window.draw(circle);
            window.draw(line);
        }
    }

    void Player::renderStaminaBar(sf::RenderWindow &window) {
        sf::RectangleShape staminaBar(sf::Vector2f(stamina, 10));
        staminaBar.setPosition(position.x - radius, position.y + radius + 5);
        staminaBar.setFillColor(sf::Color::White);
        staminaBar.setOutlineColor(sf::Color::Black);
        staminaBar.setOutlineThickness(1.0f);
        staminaBar.setOrigin(0, staminaBar.getSize().y / 2.0f);
        staminaBar.move(0, -radius - 5);
        window.draw(staminaBar);
    }

    void Player::update(float delta) {
        float currentSpeed = speed;

        if (sf::Keyboard::isKeyPressed((sf::Keyboard::LShift))) {
            if (stamina > 0) {
                isRunning = true;
                currentSpeed = speed * 2;
                stamina -= delta * 30;
            }
        } else {
            isRunning = false;
            currentSpeed = speed;
            if (stamina < maxStamina) {
                stamina += delta * staminaRegen;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            auto newPosition = position + direction * currentSpeed * delta;
            auto tileX = static_cast<int>((newPosition.x + direction.x * radius) / map.getTileSize());
            auto tileY = static_cast<int>((newPosition.y + direction.y * radius) / map.getTileSize());

            if (map.getTile(tileY, tileX).empty)
            {
                position = newPosition;
            }
            else if (map.getTile(tileY+1, tileX).empty)
            {
                position.x = newPosition.x;
            }
            else if (map.getTile(tileY, tileX+1).empty)
            {
                position.y = newPosition.y;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            auto newPosition = position - direction * currentSpeed * delta;
            auto tileX = static_cast<int>((newPosition.x - direction.x * radius) / map.getTileSize());
            auto tileY = static_cast<int>((newPosition.y - direction.y * radius) / map.getTileSize());

            if (map.getTile(tileY, tileX).empty)
            {
                position = newPosition;
            }
            else if (map.getTile(tileY-1, tileX).empty)
            {
                position.x = newPosition.x;
            }
            else if (map.getTile(tileY, tileX-1).empty)
            {
                position.y = newPosition.y;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            engine::math::Mat2<float> rotMat = engine::math::getRotationMatrix(rotateSpeed * delta);
            direction = direction * rotMat;
            cameraPlane = cameraPlane * rotMat;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            engine::math::Mat2<float> rotMat = engine::math::getRotationMatrix(-rotateSpeed * delta);
            direction = direction * rotMat;
            cameraPlane = cameraPlane * rotMat;
        }
    }

    const engine::math::Vec2<float> &Player::getPosition() const {
        return position;
    }

    const engine::math::Vec2<float> &Player::getDirection() const {
        return direction;
    }

    float Player::getRadius() const {
        return radius;
    }

    float Player::getSpeed() const {
        return speed;
    }

    float Player::getRotateSpeed() const {
        return rotateSpeed;
    }

    const engine::math::Vec2<float> &Player::getCameraPlane() const {
        return cameraPlane;
    }

    float Player::getStamina() const {
        return stamina;
    }
} // engine