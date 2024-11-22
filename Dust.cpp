#include "Dust.hpp"
#include <cmath>

Dust::Dust(const Velocity velocity, const Point center, double radius, const Color color, bool isCollidable, double lifetime)
    : velocity_(velocity), center_(center), radius_(radius), color_(color), isCollidable_(isCollidable), lifetime_(lifetime) {}
/**
 * Задает скорость объекта
 * @param velocity новое значение скорости
 */
void Dust::setVelocity(const Velocity& velocity) {
    this->velocity_ = velocity;
    
}

/**
 * @return скорость объекта
 */
Velocity Dust::getVelocity() const {
    return velocity_;
}

/**
 * @brief Выполняет отрисовку объекта
 * @details объект Ball абстрагирован от конкретного
 * способа отображения пикселей на экране. Он "знаком"
 * лишь с интерфейсом, который предоставляет Painter
 * Рисование выполняется путем вызова painter.draw(...)
 * @param painter контекст отрисовки
 */
void Dust::draw(Painter& painter) const {
    painter.draw(center_, radius_, color_);
}

/**
 * Задает координаты центра объекта
 * @param center новый центр объекта
 */
void Dust::setCenter(const Point& center) {
    this->center_ = center;
}

/**
 * @return центр объекта
 */
Point Dust::getCenter() const {
    return center_;
}

/**
 * @brief Возвращает радиус объекта
 * @details обратите внимание, что метод setRadius()
 * не требуется
 */
double Dust::getRadius() const {
    return radius_;
}

/**
 * @brief Возвращает массу объекта
 * @details В нашем приложении считаем, что все шары
 * состоят из одинакового материала с фиксированной
 * плотностью. В этом случае масса в условных единицах
 * эквивалентна объему: PI * radius^3 * 4. / 3.
 */
double Dust::getMass() const {
    return  M_PI * pow(radius_, 3) * 4. / 3.;
}

bool Dust::ifAlive() const {
    return isAlive_;
}


void Dust::dying(double time){
    lifetime_ -= time;
    if (lifetime_ <= 0){ isAlive_ = false; }
}