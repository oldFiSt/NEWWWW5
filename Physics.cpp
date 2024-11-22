#include "Physics.h"

double dot(const Point& lhs, const Point& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Physics::Physics(double timePerTick) : timePerTick{timePerTick} {}

void Physics::setWorldBox(const Point& topLeft, const Point& bottomRight) {
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
}

void Physics::update(std::vector<Ball>& balls,std::vector<Dust>& dusts, const size_t ticks){

    for (size_t i = 0; i < ticks; ++i) {
        move(balls);
        moveDusts(dusts);
        collideWithBox(balls, dusts);
        collideBalls(balls, dusts);
    }
}

void Physics::collideBalls(std::vector<Ball>& balls, std::vector<Dust>& dusts) const {
    for (auto a = balls.begin(); a != balls.end(); ++a) {
        for (auto b = std::next(a); b != balls.end(); ++b) {
            const double distanceBetweenCenters2 =
                distance2(a->getCenter(), b->getCenter());
            const double collisionDistance = a->getRadius() + b->getRadius();
            const double collisionDistance2 =
                collisionDistance * collisionDistance;

            if (distanceBetweenCenters2 < collisionDistance2) {
                processCollision(*a, *b, distanceBetweenCenters2);
                for (int i = 0; i < 10; i++) {
                        double angle = (i * 2 * M_PI / 10); 
                        Velocity velocity(100 * i, angle); 
                        Dust dust(velocity, a->getCenter(), 8, a->getColor(), false, 0.8);
                        dusts.push_back(dust); 
                }
        }
    }
}
}

void Physics::collideWithBox(std::vector<Ball>& balls, std::vector<Dust>& dusts) const {
    for (Ball& ball : balls) {
        const Point p = ball.getCenter();
        const double r = ball.getRadius();
        // определяет, находится ли v в диапазоне (lo, hi) (не включая границы)
        auto isOutOfRange = [](double v, double lo, double hi) {
            return v < lo || v > hi;
        };

        if (isOutOfRange(p.x, topLeft.x + r, bottomRight.x - r)) {
            Point vector = ball.getVelocity().vector();
            vector.x = -vector.x;
            Velocity velocity;
            Dust dust(velocity, ball.getCenter(), 8, ball.getColor(), true, 0.8);
            dusts.push_back(dust);
            ball.setVelocity(vector);
        
        } else if (isOutOfRange(p.y, topLeft.y + r, bottomRight.y - r)) {
            Point vector = ball.getVelocity().vector();
            vector.y = -vector.y;
            Velocity velocity;
            Dust dust(velocity, ball.getCenter(), 8, ball.getColor(), true, 0.8);
            dusts.push_back(dust);
            ball.setVelocity(vector);
        }
    }
}

void Physics::move(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {
        Point newPos =
            ball.getCenter() + ball.getVelocity().vector() * timePerTick;
        ball.setCenter(newPos);
    }
}

void Physics::processCollision(Ball& a, Ball& b,
                               double distanceBetweenCenters2) const {
    // нормированный вектор столкновения
    const Point normal =
        (b.getCenter() - a.getCenter()) / std::sqrt(distanceBetweenCenters2);

    // получаем скорость в векторном виде
    const Point aV = a.getVelocity().vector();
    const Point bV = b.getVelocity().vector();

    // коэффициент p учитывает скорость обоих мячей
    const double p =
        2 * (dot(aV, normal) - dot(bV, normal)) / (a.getMass() + b.getMass());

    // задаем новые скорости мячей после столкновения
    a.setVelocity(Velocity(aV - normal * p * a.getMass()));
    b.setVelocity(Velocity(bV + normal * p * b.getMass()));
}

bool Physics::checkCollision(const Ball& ballA, const Ball& ballB) {
    Point centerA = ballA.getCenter();
    Point centerB = ballB.getCenter();
    double radiusA = ballA.getRadius();
    double radiusB = ballB.getRadius();

    double dx = centerA.x - centerB.x;
    double dy = centerA.y - centerB.y;
    double distanceSquared = dx * dx + dy * dy;

    double radiiSum = radiusA + radiusB;
    return distanceSquared <= (radiiSum * radiiSum);
}

// Реализация функции resolveCollision
void Physics::resolveCollision(Ball& ballA, Ball& ballB) {
    Velocity velocityA = ballA.getVelocity();
    Velocity velocityB = ballB.getVelocity();

    // Простое разрешение коллизии: меняем скорости местами
    ballA.setVelocity(velocityB);
    ballB.setVelocity(velocityA);

    Point centerA = ballA.getCenter();
    Point centerB = ballB.getCenter();

    double dx = centerA.x - centerB.x;
    double dy = centerA.y - centerB.y;
    
    double distance = std::sqrt(dx * dx + dy * dy);
    
    if (distance == 0) {
        return; // Избегаем деления на ноль
    }

    double overlap = (ballA.getRadius() + ballB.getRadius()) - distance;

    // Перемещаем шары по нормализованному вектору
    double offsetX = (dx / distance) * (overlap / 2.0);
    double offsetY = (dy / distance) * (overlap / 2.0);

    ballA.setCenter(Point(centerA.x + offsetX, centerA.y + offsetY));
    ballB.setCenter(Point(centerB.x - offsetX, centerB.y - offsetY));
}

// Реализация метода checkCollisions

void Physics::checkCollisions() {
    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            Ball& ballA = balls[i]; // Используем ссылки для изменения объектов
            Ball& ballB = balls[j];

            if (checkCollision(ballA, ballB)) {
                resolveCollision(ballA, ballB); // Здесь вызываем функцию
            }
        }
    }
}

void Physics::moveDusts(std::vector<Dust>& dusts) const {
    for (Dust& dust : dusts) {
        if (dust.ifAlive())
        {
            Point newPos = dust.getCenter() + dust.getVelocity().vector() * timePerTick;
            dust.setCenter(newPos);
        }
    }
}