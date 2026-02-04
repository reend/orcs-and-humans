#include "Unit.h"
#include "engine/core/Time.h"
#include "engine/logging/Logger.h"
#include <cmath>

Unit::Unit(raylib::Vector2 position)  // Конструктор: создание юнита
    : position(position)              // Начальная позиция
    , currentPathIndex(0)             // Начинаем с первой точки пути
    , speed(80.0f)                    // Скорость 80 пикселей/секунду (типичная скорость для RTS)
    , currentDirection(Direction::Down)  // По умолчанию смотрит вниз (лицом к экрану)
{
    sprite = new Engine::Sprite("assets/units/orcs/peon/peon.png");  // Загружаем spritesheet
    animController = new Engine::AnimationController(sprite);         // Создаём контроллер анимаций

    LoadAnimations();  // Загружаем все анимации для 8 направлений

    animController->Play("idle_down");  // Запускаем анимацию idle (стоит лицом к экрану)

    LOG_INFO("Unit created at (%.1f, %.1f)", position.x, position.y);  // Логируем создание
}

Unit::~Unit() {  // Деструктор: очистка ресурсов
    delete animController;  // Удаляем контроллер анимаций
    delete sprite;          // Удаляем спрайт
}

void Unit::LoadAnimations() {  // Загрузка всех анимаций из spritesheet
    // IDLE анимации (первый ряд каждой колонки)

    // IDLE UP (колонка 1, ряд 1) - стоит спиной к экрану
    Engine::Animation* idleUp = new Engine::Animation();
    idleUp->AddFrame({0, 0, 51, 40});
    idleUp->SetLoop(true);
    animController->AddAnimation("idle_up", idleUp);

    // IDLE UP-RIGHT (колонка 2, ряд 1)
    Engine::Animation* idleUpRight = new Engine::Animation();
    idleUpRight->AddFrame({51, 0, 51, 40});
    idleUpRight->SetLoop(true);
    animController->AddAnimation("idle_upright", idleUpRight);

    // IDLE RIGHT (колонка 3, ряд 1)
    Engine::Animation* idleRight = new Engine::Animation();
    idleRight->AddFrame({102, 0, 51, 40});
    idleRight->SetLoop(true);
    animController->AddAnimation("idle_right", idleRight);

    // IDLE DOWN-RIGHT (колонка 4, ряд 1)
    Engine::Animation* idleDownRight = new Engine::Animation();
    idleDownRight->AddFrame({153, 0, 51, 40});
    idleDownRight->SetLoop(true);
    animController->AddAnimation("idle_downright", idleDownRight);

    // IDLE DOWN (колонка 5, ряд 1) - стоит лицом к экрану
    Engine::Animation* idleDown = new Engine::Animation();
    idleDown->AddFrame({204, 0, 51, 40});
    idleDown->SetLoop(true);
    animController->AddAnimation("idle_down", idleDown);

    // WALK UP (колонка 1, ряды 2-5)
    Engine::Animation* walkUp = new Engine::Animation();
    walkUp->AddFrame({0, 40, 51, 40});
    walkUp->AddFrame({0, 80, 51, 40});
    walkUp->AddFrame({0, 120, 51, 40});
    walkUp->AddFrame({0, 160, 51, 40});
    walkUp->SetFPS(8);
    walkUp->SetLoop(true);
    animController->AddAnimation("walk_up", walkUp);

    // WALK UP-RIGHT (колонка 2, ряды 2-5)
    Engine::Animation* walkUpRight = new Engine::Animation();
    walkUpRight->AddFrame({51, 40, 51, 40});
    walkUpRight->AddFrame({51, 80, 51, 40});
    walkUpRight->AddFrame({51, 120, 51, 40});
    walkUpRight->AddFrame({51, 160, 51, 40});
    walkUpRight->SetFPS(8);
    walkUpRight->SetLoop(true);
    animController->AddAnimation("walk_upright", walkUpRight);

    // WALK RIGHT (колонка 3, ряды 2-5)
    Engine::Animation* walkRight = new Engine::Animation();
    walkRight->AddFrame({102, 40, 51, 40});
    walkRight->AddFrame({102, 80, 51, 40});
    walkRight->AddFrame({102, 120, 51, 40});
    walkRight->AddFrame({102, 160, 51, 40});
    walkRight->SetFPS(8);
    walkRight->SetLoop(true);
    animController->AddAnimation("walk_right", walkRight);

    // WALK DOWN-RIGHT (колонка 4, ряды 2-5)
    Engine::Animation* walkDownRight = new Engine::Animation();
    walkDownRight->AddFrame({153, 40, 51, 40});
    walkDownRight->AddFrame({153, 80, 51, 40});
    walkDownRight->AddFrame({153, 120, 51, 40});
    walkDownRight->AddFrame({153, 160, 51, 40});
    walkDownRight->SetFPS(8);
    walkDownRight->SetLoop(true);
    animController->AddAnimation("walk_downright", walkDownRight);

    // WALK DOWN (колонка 5, ряды 2-5)
    Engine::Animation* walkDown = new Engine::Animation();
    walkDown->AddFrame({204, 40, 51, 40});
    walkDown->AddFrame({204, 80, 51, 40});
    walkDown->AddFrame({204, 120, 51, 40});
    walkDown->AddFrame({204, 160, 51, 40});
    walkDown->SetFPS(8);
    walkDown->SetLoop(true);
    animController->AddAnimation("walk_down", walkDown);

    LOG_DEBUG("Unit animations loaded (5 idle + 5 walk = 10 animations)");
}

void Unit::SetPath(const std::vector<raylib::Vector2>& newPath) {  // Задать новый путь
    path = newPath;             // Копируем путь
    currentPathIndex = 0;       // Начинаем с первой точки

    if (!path.empty()) {  // Если путь не пустой
        Direction dir = GetDirectionToTarget(path[0]);  // Определяем направление к первой точке
        SetDirection(dir);  // Устанавливаем соответствующую анимацию
        LOG_DEBUG("Unit path set: %d nodes", path.size());
    }
}

void Unit::Update(float deltaTime) {  // Обновление состояния каждый кадр
    UpdateMovement(deltaTime);        // Обновляем движение по пути
    animController->Update(deltaTime);  // Обновляем анимацию (смена кадров)
}

void Unit::UpdateMovement(float deltaTime) {  // Логика движения по пути
    if (path.empty()) return;  // Нет пути - ничего не делаем

    raylib::Vector2 target = path[currentPathIndex];  // Целевая точка (текущая точка в пути)
    raylib::Vector2 direction = {target.x - position.x, target.y - position.y};  // Вектор направления к цели

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);  // Расстояние до цели

    LOG_DEBUG("Unit moving: pos=(%.1f,%.1f) target=(%.1f,%.1f) dist=%.1f speed=%.1f dt=%.4f",
              position.x, position.y, target.x, target.y, distance, speed, deltaTime);

    if (distance < 2.0f) {  // Достигли текущей точки (погрешность 2 пикселя)
        LOG_DEBUG("Unit reached waypoint %d/%d at (%.1f, %.1f)",
                  currentPathIndex + 1, (int)path.size(), position.x, position.y);
        currentPathIndex++;  // Переходим к следующей точке

        if (currentPathIndex >= path.size()) {  // Достигли конца пути
            path.clear();                      // Очищаем путь
            currentPathIndex = 0;              // Сбрасываем индекс

            // Переключаемся на IDLE анимацию в текущем направлении
            std::string idleAnim = "idle_";
            switch (currentDirection) {
                case Direction::Up:        idleAnim += "up"; break;
                case Direction::UpRight:   idleAnim += "upright"; break;
                case Direction::Right:     idleAnim += "right"; break;
                case Direction::DownRight: idleAnim += "downright"; break;
                case Direction::Down:      idleAnim += "down"; break;
                case Direction::DownLeft:  idleAnim += "downright"; break;  // Используем downright с flip
                case Direction::Left:      idleAnim += "right"; break;      // Используем right с flip
                case Direction::UpLeft:    idleAnim += "upright"; break;    // Используем upright с flip
            }
            animController->Play(idleAnim);  // Запускаем idle анимацию

            LOG_INFO("Unit reached final destination at (%.1f, %.1f)", position.x, position.y);
            return;
        }

        // Обновляем направление к новой точке
        Direction newDir = GetDirectionToTarget(path[currentPathIndex]);
        SetDirection(newDir);
    }

    // Нормализуем вектор направления (делаем длину = 1)
    if (distance > 0.01f) {
        direction.x /= distance;
        direction.y /= distance;

        // Двигаемся к цели со скоростью speed (frame-independent благодаря deltaTime)
        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;
    }
}

Direction Unit::GetDirectionToTarget(raylib::Vector2 target) {  // Определить направление к цели
    float dx = target.x - position.x;  // Разница по X
    float dy = target.y - position.y;  // Разница по Y

    float angle = std::atan2(dy, dx) * 180.0f / 3.14159f;  // Угол в градусах (-180 до 180)

    // Нормализуем угол от 0 до 360
    if (angle < 0) angle += 360.0f;

    // Определяем направление по углу (8 направлений по 45 градусов каждое)
    if (angle >= 337.5f || angle < 22.5f)        return Direction::Right;      // 0° (→)
    if (angle >= 22.5f && angle < 67.5f)         return Direction::DownRight;  // 45° (↘)
    if (angle >= 67.5f && angle < 112.5f)        return Direction::Down;       // 90° (↓)
    if (angle >= 112.5f && angle < 157.5f)       return Direction::DownLeft;   // 135° (↙)
    if (angle >= 157.5f && angle < 202.5f)       return Direction::Left;       // 180° (←)
    if (angle >= 202.5f && angle < 247.5f)       return Direction::UpLeft;     // 225° (↖)
    if (angle >= 247.5f && angle < 292.5f)       return Direction::Up;         // 270° (↑)
    if (angle >= 292.5f && angle < 337.5f)       return Direction::UpRight;    // 315° (↗)

    return Direction::Down;  // По умолчанию вниз
}

void Unit::SetDirection(Direction dir) {  // Установить направление (выбрать анимацию)
    if (dir == currentDirection) return;  // Направление не изменилось - ничего не делаем

    currentDirection = dir;  // Обновляем текущее направление

    // Выбираем walk анимацию для нового направления
    std::string animName = "walk_";
    switch (dir) {
        case Direction::Up:        animName += "up"; break;
        case Direction::UpRight:   animName += "upright"; break;
        case Direction::Right:     animName += "right"; break;
        case Direction::DownRight: animName += "downright"; break;
        case Direction::Down:      animName += "down"; break;
        case Direction::DownLeft:  animName += "downright"; break;  // Зеркалируем downright
        case Direction::Left:      animName += "right"; break;      // Зеркалируем right
        case Direction::UpLeft:    animName += "upright"; break;    // Зеркалируем upright
    }

    animController->Play(animName);  // Запускаем анимацию ходьбы
}

void Unit::Draw() {  // Отрисовка юнита
    // Определяем нужно ли зеркалирование (LEFT направления)
    bool flipHorizontal = (currentDirection == Direction::Left ||
                          currentDirection == Direction::DownLeft ||
                          currentDirection == Direction::UpLeft);

    // Получаем текущий кадр анимации
    raylib::Rectangle sourceRect = animController->GetCurrentAnimation()->GetCurrentFrame();

    // Если нужно зеркалирование - инвертируем ширину sourceRect
    if (flipHorizontal) {
        sourceRect.width = -sourceRect.width;  // Отрицательная ширина = flip
    }

    // Рисуем спрайт в позиции юнита
    DrawTexturePro(
        *sprite->GetTexture(),              // Текстура spritesheet
        sourceRect,                         // Исходный прямоугольник (текущий кадр анимации)
        {position.x, position.y, 51, 40},   // Целевой прямоугольник (где рисовать)
        {0, 0},                             // Origin (pivot point)
        0.0f,                               // Rotation (без вращения)
        WHITE                               // Tint (белый = без окрашивания)
    );
}
