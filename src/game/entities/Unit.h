#pragma once
#include "engine/graphics/Sprite.h"
#include "engine/graphics/AnimationController.h"
#include "raylib-cpp.hpp"
#include <vector>

enum class Direction {  // Направление движения юнита (8 направлений)
    Up,         // Вверх (спиной к экрану)
    UpRight,    // Вверх-вправо по диагонали
    Right,      // Вправо
    DownRight,  // Вниз-вправо по диагонали
    Down,       // Вниз (лицом к экрану)
    DownLeft,   // Вниз-влево по диагонали
    Left,       // Влево
    UpLeft      // Вверх-влево по диагонали
};

class Unit {
public:
    Unit(raylib::Vector2 position);  // Конструктор: создать юнита в позиции
    ~Unit();                         // Деструктор: очистить ресурсы

    void SetPath(const std::vector<raylib::Vector2>& path);  // Задать путь для движения (от A*)
    void Update(float deltaTime);                            // Обновить состояние (движение, анимация)
    void Draw();                                             // Отрисовать юнита

    raylib::Vector2 GetPosition() const { return position; }     // Получить текущую позицию
    bool IsMoving() const { return !path.empty(); }              // Проверка: юнит движется?

private:
    void LoadAnimations();       // Загрузить все анимации из spritesheet
    void UpdateMovement(float deltaTime);  // Обновить движение по пути
    Direction GetDirectionToTarget(raylib::Vector2 target);  // Определить направление к цели
    void SetDirection(Direction dir);  // Установить направление (выбрать анимацию)

    raylib::Vector2 position;         // Текущая позиция юнита в мире (пиксели)
    std::vector<raylib::Vector2> path;  // Путь для движения (список точек)
    int currentPathIndex;              // Текущая точка в пути (индекс)
    float speed;                       // Скорость движения (пикселей/секунду)

    Engine::Sprite* sprite;                       // Спрайт юнита (spritesheet)
    Engine::AnimationController* animController;  // Контроллер анимаций
    Direction currentDirection;                   // Текущее направление взгляда юнита
};
