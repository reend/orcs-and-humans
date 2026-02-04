#pragma once  // Предотвращает множественное включение этого заголовочного файла
#include "raylib-cpp.hpp"  // Подключаем raylib для Vector2
#include <vector>  // Для std::vector (динамические массивы)
#include <functional>  // Для std::function (функциональные объекты)

namespace Engine {  // Все классы движка находятся в пространстве имен Engine

// PathNode - узел для алгоритма A* (представляет одну клетку на карте)
struct PathNode {
    int x;  // X координата тайла
    int y;  // Y координата тайла
    float g;  // Стоимость пути от стартовой точки до этого узла (пройденное расстояние)
    float h;  // Эвристическая оценка расстояния от этого узла до цели (примерное оставшееся расстояние)
    float f;  // Общая стоимость узла: f = g + h (используется для выбора следующего узла)
    PathNode* parent;  // Указатель на родительский узел (откуда мы пришли), для восстановления пути

    // Конструктор: создает узел с заданными координатами
    PathNode(int x, int y)
        : x(x), y(y)  // Инициализируем координаты
        , g(0)  // Изначально стоимость пути = 0
        , h(0)  // Эвристика = 0 (будет вычислена позже)
        , f(0)  // Общая стоимость = 0
        , parent(nullptr) {}  // Изначально нет родителя
};

// Pathfinder - класс для поиска пути с использованием алгоритма A*
class Pathfinder {
public:
    // Тип функции для проверки проходимости тайла (принимает x, y и возвращает true если проходим)
    using IsPassableFunc = std::function<bool(int, int)>;

    // FindPath - основной метод: находит путь от start до goal
    // Возвращает список точек (Vector2) или пустой вектор, если путь не найден
    static std::vector<raylib::Vector2> FindPath(
        raylib::Vector2 start,  // Стартовая позиция (в тайловых координатах)
        raylib::Vector2 goal,   // Целевая позиция (в тайловых координатах)
        IsPassableFunc isPassable  // Функция проверки проходимости тайла
    );

private:
    // Heuristic - вычисляет эвристическую оценку расстояния между двумя точками (Manhattan distance)
    static float Heuristic(int x1, int y1, int x2, int y2);

    // GetNeighbors - возвращает список соседних узлов для заданного узла (8 направлений)
    static std::vector<PathNode*> GetNeighbors(PathNode* node, int goalX, int goalY, IsPassableFunc isPassable);

    // ReconstructPath - восстанавливает путь от конечного узла до начального, идя по цепочке parent
    static std::vector<raylib::Vector2> ReconstructPath(PathNode* endNode);
};

}  // namespace Engine
