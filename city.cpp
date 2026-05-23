#include "City.h"

City::City() : id(-1), name(""), x(0.0), y(0.0) {}

City::City(int id, const string& name, double x, double y)
    : id(id), name(name), x(x), y(y) {}

int City::getId() const {
    return id;
}

void City::setId(int id) {
    this->id = id;
}

string City::getName() const {
    return name;
}

void City::setName(const string& name) {
    this->name = name;
}

double City::getX() const {
    return x;
}

void City::setX(double x) {
    this->x = x;
}

double City::getY() const {
    return y;
}

void City::setY(double y) {
    this->y = y;
}
