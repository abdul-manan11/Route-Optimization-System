#ifndef CITY_H
#define CITY_H

#include <string>

using namespace std;

class City {
private:
    int id;
    string name;
    double x;
    double y;

public:
    City();
    City(int id, const string& name, double x, double y);

    int getId() const;
    void setId(int id);

    string getName() const;
    void setName(const string& name);

    double getX() const;
    void setX(double x);

    double getY() const;
    void setY(double y);
};

#endif
