#include "Pathfinder.h"  // Подключаем заголовочный файл с объявлениями
#include "engine/logging/Logger.h"  // Для логирования (LOG_INFO, LOG_WARN)
#include <cmath>  // Для математических функций (std::abs)
#include <algorithm>  // Для std::min_element, std::find_if, std::reverse

namespace Engine {  // Все реализации находятся в пространстве имен Engine

// FindPath - основной метод A* алгоритма
// Принимает: start (стартовая позиция), goal (целевая позиция), isPassable (функция проверки проходимости)
// Возвращает: список точек Vector2 (путь) или пустой вектор, если путь не найден
std::vector<raylib::Vector2> Pathfinder::FindPath(
    raylib::Vector2 start,  // Стартовая точка (float координаты)
    raylib::Vector2 goal,   // Целевая точка (float координаты)
    IsPassableFunc isPassable  // Функция для проверки: можно ли пройти через тайл (x, y)
) {
    // Преобразуем float координаты в целые числа (тайловые координаты)
    int startX = static_cast<int>(start.x);  // X координата старта
    int startY = static_cast<int>(start.y);  // Y координата старта
    int goalX = static_cast<int>(goal.x);    // X координата цели
    int goalY = static_cast<int>(goal.y);    // Y координата цели

    // Проверяем, что стартовая и целевая точки проходимы (не стены/препятствия)
    if (!isPassable(startX, startY) || !isPassable(goalX, goalY)) {
        LOG_WARN("Pathfinding: start or goal is not passable");  // Логируем предупреждение
        return {};  // Возвращаем пустой путь (невозможно найти)
    }

    // Если старт и цель совпадают, путь не нужен
    if (startX == goalX && startY == goalY) {
        return {start};  // Возвращаем путь из одной точки (уже на месте)
    }
    
    // OpenList - список узлов, которые нужно рассмотреть (еще не обработаны)
    std::vector<PathNode*> openList;  // Узлы "на границе" исследования

    // ClosedList - список уже обработанных узлов (больше не рассматриваем)
    std::vector<PathNode*> closedList;  // Узлы, которые мы уже проверили

    // Создаем стартовый узел и инициализируем его значения
    PathNode* startNode = new PathNode(startX, startY);  // Создаем узел для стартовой позиции
    startNode->g = 0;  // Стоимость пути от старта до старта = 0 (мы уже здесь)
    startNode->h = Heuristic(startX, startY, goalX, goalY);  // Оценка расстояния до цели
    startNode->f = startNode->h;  // f = g + h = 0 + h (общая стоимость)
    openList.push_back(startNode);  // Добавляем стартовый узел в список для рассмотрения

    // goalNode будет указывать на найденный целевой узел (если путь найден)
    PathNode* goalNode = nullptr;  // Изначально nullptr (путь еще не найден)
    
    // Основной цикл A*: продолжаем, пока есть узлы для рассмотрения
    while (!openList.empty()) {  // Пока openList не пуст
        // Находим узел с минимальной стоимостью f (самый перспективный путь)
        auto minIt = std::min_element(openList.begin(), openList.end(),
            [](PathNode* a, PathNode* b) { return a->f < b->f; });  // Lambda: сравниваем по f

        PathNode* current = *minIt;  // Выбираем узел с минимальным f как текущий
        openList.erase(minIt);  // Удаляем его из openList (больше не на границе)
        closedList.push_back(current);  // Добавляем в closedList (уже обработан)

        // Проверяем: достигли ли мы цели?
        if (current->x == goalX && current->y == goalY) {  // Координаты совпадают с целью
            goalNode = current;  // Сохраняем целевой узел (путь найден!)
            break;  // Выходим из цикла (цель достигнута)
        }
        
        // Получаем список соседних узлов (8 направлений: вверх, вниз, влево, вправо + диагонали)
        std::vector<PathNode*> neighbors = GetNeighbors(current, goalX, goalY, isPassable);

        // Обрабатываем каждого соседа
        for (PathNode* neighbor : neighbors) {  // Для каждого соседнего узла
            // Проверяем: находится ли сосед уже в closedList (уже обработан)?
            auto inClosed = std::find_if(closedList.begin(), closedList.end(),
                [neighbor](PathNode* n) { return n->x == neighbor->x && n->y == neighbor->y; });  // Lambda: сравниваем координаты

            // Если сосед уже обработан (в closedList), пропускаем его
            if (inClosed != closedList.end()) {  // Найден в closedList
                delete neighbor;  // Удаляем временный узел (не нужен)
                continue;  // Переходим к следующему соседу
            }

            // Вычисляем стоимость движения к соседу
            int dx = std::abs(neighbor->x - current->x);  // Разница по X (0 или 1)
            int dy = std::abs(neighbor->y - current->y);  // Разница по Y (0 или 1)
            // Если dx=1 и dy=1, это диагональ (стоимость √2 ≈ 1.414), иначе прямое движение (стоимость 1.0)
            float moveCost = (dx + dy == 2) ? 1.414f : 1.0f;  // Диагональ дороже прямого движения

            // tentativeG - предполагаемая стоимость пути до соседа через current
            float tentativeG = current->g + moveCost;  // g соседа = g текущего + стоимость перехода
            
            // Проверяем: находится ли сосед уже в openList (ожидает обработки)?
            auto inOpen = std::find_if(openList.begin(), openList.end(),
                [neighbor](PathNode* n) { return n->x == neighbor->x && n->y == neighbor->y; });  // Lambda: сравниваем координаты

            // Если сосед уже в openList
            if (inOpen != openList.end()) {  // Найден в openList
                // Проверяем: новый путь лучше (дешевле) существующего?
                if (tentativeG < (*inOpen)->g) {  // Новый g меньше старого
                    (*inOpen)->g = tentativeG;  // Обновляем g (нашли более короткий путь)
                    (*inOpen)->f = (*inOpen)->g + (*inOpen)->h;  // Пересчитываем f = g + h
                    (*inOpen)->parent = current;  // Обновляем родителя (новый путь идет через current)
                }
                delete neighbor;  // Удаляем временный узел (использовали существующий)
            } else {  // Сосед не в openList - это новый узел
                neighbor->g = tentativeG;  // Устанавливаем стоимость пути g
                neighbor->f = neighbor->g + neighbor->h;  // Вычисляем общую стоимость f = g + h
                neighbor->parent = current;  // Устанавливаем родителя (пришли из current)
                openList.push_back(neighbor);  // Добавляем в openList для дальнейшего рассмотрения
            }
        }  // Конец цикла по соседям
    }  // Конец основного цикла while
    
    // Создаем пустой вектор для хранения итогового пути
    std::vector<raylib::Vector2> path;

    // Проверяем: был ли найден путь? (goalNode != nullptr означает, что цель достигнута)
    if (goalNode) {  // Путь найден
        path = ReconstructPath(goalNode);  // Восстанавливаем путь от цели до старта
        LOG_INFO("Path found: %d nodes", path.size());  // Логируем успех (количество узлов в пути)
    } else {  // Путь не найден (goalNode == nullptr)
        LOG_WARN("No path found from (%d,%d) to (%d,%d)", startX, startY, goalX, goalY);  // Логируем предупреждение
    }

    // Очистка памяти: удаляем все динамически созданные узлы
    for (PathNode* node : openList) delete node;  // Удаляем узлы из openList
    for (PathNode* node : closedList) delete node;  // Удаляем узлы из closedList

    return path;  // Возвращаем найденный путь (или пустой вектор, если путь не найден)
}  // Конец метода FindPath

// Heuristic - эвристическая функция для оценки расстояния между двумя точками
// Использует Manhattan distance (манхэттенское расстояние): |x2-x1| + |y2-y1|
// Это оценка минимального количества шагов для достижения цели (если нет препятствий)
float Pathfinder::Heuristic(int x1, int y1, int x2, int y2) {
    // Вычисляем сумму модулей разниц координат (расстояние по сетке без диагоналей)
    return std::abs(x2 - x1) + std::abs(y2 - y1);  // |Δx| + |Δy|
}  // Конец метода Heuristic

// GetNeighbors - возвращает список соседних проходимых узлов для заданного узла
// Проверяет 8 направлений (4 прямых + 4 диагональных) и возвращает только проходимые тайлы
std::vector<PathNode*> Pathfinder::GetNeighbors(PathNode* node, int goalX, int goalY, IsPassableFunc isPassable) {
    std::vector<PathNode*> neighbors;  // Список соседних узлов (будем заполнять)

    // Массивы смещений для 8 направлений движения
    // dx - смещение по X, dy - смещение по Y
    // Индексы 0-3: прямые направления (Up, Right, Down, Left)
    // Индексы 4-7: диагональные направления (UpLeft, UpRight, DownRight, DownLeft)
    const int dx[] = {0, 1, 0, -1,  -1, 1, 1, -1};  // X: 0=вверх, 1=вправо, 0=вниз, -1=влево, ...
    const int dy[] = {-1, 0, 1, 0,  -1, -1, 1, 1};  // Y: -1=вверх, 0=вправо, 1=вниз, 0=влево, ...

    // Перебираем все 8 направлений
    for (int i = 0; i < 8; i++) {  // i = индекс направления (0..7)
        int nx = node->x + dx[i];  // Новая X координата соседа
        int ny = node->y + dy[i];  // Новая Y координата соседа

        // Проверяем: проходим ли целевой тайл?
        if (!isPassable(nx, ny)) continue;  // Если не проходим, пропускаем это направление

        // Для диагональных направлений (i >= 4) проверяем "срезание углов"
        // Пример: если идем UpLeft (↖), но Up и Left - стены, нельзя проходить по диагонали
        if (i >= 4) {  // Это диагональное направление
            // Вычисляем координаты двух соседних прямых тайлов
            int checkX1 = node->x;         // Вертикальный сосед: X не меняется
            int checkY1 = node->y + dy[i]; // Вертикальный сосед: Y меняется
            int checkX2 = node->x + dx[i]; // Горизонтальный сосед: X меняется
            int checkY2 = node->y;         // Горизонтальный сосед: Y не меняется

            // Проверяем: оба соседних прямых тайла непроходимы?
            // Если да, то диагональ заблокирована (нельзя "протиснуться" между двух стен)
            if (!isPassable(checkX1, checkY1) && !isPassable(checkX2, checkY2)) {
                continue;  // Пропускаем это диагональное направление (заблокировано)
            }
        }

        // Создаем новый узел для соседа
        PathNode* neighbor = new PathNode(nx, ny);  // Динамически выделяем память для узла
        neighbor->h = Heuristic(nx, ny, goalX, goalY);  // Вычисляем эвристическую оценку до цели
        neighbors.push_back(neighbor);  // Добавляем соседа в список
    }

    return neighbors;  // Возвращаем список всех проходимых соседей
}  // Конец метода GetNeighbors

// ReconstructPath - восстанавливает путь от конечного узла до начального
// Идет по цепочке parent от endNode до startNode (где parent == nullptr)
// Возвращает путь в правильном порядке (от старта до цели)
std::vector<raylib::Vector2> Pathfinder::ReconstructPath(PathNode* endNode) {
    std::vector<raylib::Vector2> path;  // Список точек пути (изначально пуст)
    PathNode* current = endNode;  // Начинаем с конечного узла (цели)

    // Идем по цепочке parent от конца к началу
    while (current != nullptr) {  // Пока не дошли до начала (parent == nullptr)
        // Добавляем координаты текущего узла в путь
        path.push_back(raylib::Vector2(static_cast<float>(current->x), static_cast<float>(current->y)));
        current = current->parent;  // Переходим к родительскому узлу (шаг назад по пути)
    }

    // Путь был построен от цели к старту, нужно развернуть его
    std::reverse(path.begin(), path.end());  // Разворачиваем вектор (теперь от старта к цели)
    return path;  // Возвращаем восстановленный путь
}  // Конец метода ReconstructPath

}  // namespace Engine (конец пространства имен)
