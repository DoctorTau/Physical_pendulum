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


class Double_pendulum {
    Point top1;
    Point top2;
    Point body;
    double m1;
    double m2;
    double angle1;
    double angle2;
    double L1;
    double L2;
    double w1;
    double w2;
    double a1;
    double a2;

public:
    Double_pendulum(int x_top1, int y_top1, double r1, double r2, double m1, double m2, double angle1, double angle2) {
        this->top1.x = x_top1;
        this->top1.y = y_top1;
        this->angle1 = angle1;
        this->angle2 = angle2;
        this->top2.x = L1 * sin(angle1);
        this->top2.y = -L1 * cos(angle1);
        this->body.x = top2.x + L2 * sin(angle2);
        this->body.y = top2.y - L2 * cos(angle2);

        this->m1 = m1;
        this->m2 = m2;
        this->a1 = 0;
        this->a2 = 0;
        this->w1 = 0;
        this->w2 = 0;
    }

    void Process() {
        double g = 9.81;

        this->a1 = -(g * (2 * m1 - m2) * sin(this->angle1) - m2 * g * sin(angle1 - 2 * angle2) - 2 *
            sin(angle1 - angle2) * m2 * (pow(a2, 2) * L2 + pow(a1, 2) * L1 * cos(angle1 - angle2))) /
            (L1 * (2 * m1 + m2 - m2 * cos(2 * (angle1 - angle2))));

        this->a2 = (2 * sin(angle1 - angle2) * (pow(a1, 2) * L1 * (m1 + m2) + g * (m1 + m2) *
            cos(angle1) + pow(a2, 2) * L2 * m2 * cos(angle1 - angle2))) /
            (L2 * (2 * m1 + m2 - m2 * cos(2 * (angle1 - angle2))));

        this->w1 += this->a1;
        this->w2 += this->a2;
        this->angle1 += this->w1;
        this->angle2 += this->w2;
        this->top2.x = L1 * sin(angle1);
        this->top2.y = -L1 * cos(angle1);
        this->body.x = top2.x + L2 * sin(angle2);
        this->body.y = top2.y - L2 * cos(angle2);

    }

    void draw() {

    }
};


int main() {


    return 0;
}