#include <iostream>
#include <cmath>

using namespace std;

struct Point {
    int x;
    int y;
    Point(int x = 0, int y = 0) {
        this->x = x;
        this->y = y;
    }
};

class Pendulum {
    Point top;
    Point body;
    int mas;
    double angle;
    double w;
    double max_angle;
    double leght;

public:

    Pendulum(int x_top, int y_top, int x_body, int y_body, double T, double max_angle, int mas) {
        this->top.x = x_top;
        this->top.y = y_top;
        this->body.x = x_body;
        this->body.y = y_body;
        this->max_angle = max_angle;
        this->w = 2 * M_PI / T;
        this->angle = max_angle;
        this->leght = 9.81 * pow(T / (2 * M_1_PI), 2);
        this->mas = mas;
    }

    void Draw() {

    }

    void Process(int t) {
        this->angle = this->max_angle * cos(this->w * t + this->max_angle);
        this->body.y *= floor(1 - cos(this->angle));
        this->body.x *= floor(sin(this->angle));
    }


};

int main() {


    return 0;
}