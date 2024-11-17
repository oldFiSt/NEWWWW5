#pragma once
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"

class Ball {
public:
    Ball(const Velocity velocity, const Point center, double radius, const Color color, bool isCollidable);

    void setVelocity(const Velocity& velocity);
    
    Velocity getVelocity() const;
    void draw(Painter& painter) const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    Color getColor() const;
    bool ifCollidable() const;

private:
    double radius_;
    Point center_;
    Color color_;
    Velocity velocity_;
    bool isCollidable_;
};
