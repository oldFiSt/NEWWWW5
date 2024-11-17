#pragma once
#include "Ball.hpp"
#include "Dust.hpp"
#include <vector>

class Physics {
  public:
    Physics(double timePerTick = 0.001);
    void setWorldBox(const Point& topLeft, const Point& bottomRight);
    void update(std::vector<Ball>& balls, std::vector<Dust>& dusts, size_t ticks);
    void checkCollisions();
  
  private:
    void collideBalls(std::vector<Ball>& balls, std::vector<Dust>& dusts) const;
    void collideWithBox(std::vector<Ball>& balls, std::vector<Dust>& dusts) const;
    void move(std::vector<Ball>& balls) const;
    void moveDusts(std::vector<Dust>& dusts) const;
    void processCollision(Ball& a, Ball& b,
                          double distanceBetweenCenters2) const;
    bool checkCollision(const Ball& ballA, const Ball& ballB);
    void resolveCollision(Ball& ballA, Ball& ballB);                     

  private:
    std::vector<Ball> balls;
    Point topLeft;
    Point bottomRight;
    double timePerTick;
};
