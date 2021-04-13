#include <iostream>
#include <windows.h> 
#include <vector> 
#include <cmath> 
#include <dos.h>
#include <stdio.h>

using namespace std;

SHORT console_top_offset = 2; //for SetPosition

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
void ClearConsole() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    cout << "\x1B[2J\x1B[H";
}

void SetPosition(SHORT x, SHORT y)
{
    y += console_top_offset;

    HANDLE winHandle;
    COORD pos = { x,y };
    winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(winHandle, pos);
}


void DrawPointOld(int x, int y, char sym) {
    for (int i = 1; i < y; i++) {
        cout << endl;
    }
    for (int i = 1; i < x; i++) {
        cout << ' ';
    }
    cout << sym << endl;
}

void DrawPoint(int x, int y, char sym) {
    SetPosition(x, y);
    cout << sym;
}

vector<vector<bool>> DrawLineToVector(vector<vector<bool>> mas, int x1, int y1, int x2, int y2, char sym) {
    double y_x = static_cast<double>(y2 - y1 + 1) / static_cast<double>(x2 - x1 + 1);
    double x_y = static_cast<double>(x2 - x1 + 1) / static_cast<double>(y2 - y1 + 1);


    for (int y_cur = 1; y_cur <= y2; y_cur++) {
        double x_cur = x_y * y_cur;
        int x_cur_int = (int)(x_cur);

        if (x_cur_int >= x1 && x_cur_int <= x2) {
            if (x_cur - floor(x_cur) == 0.5) {
                mas[y_cur][x_cur_int] = 1;
            }
            mas[y_cur - 1][x_cur_int - 1] = 1;
        }
    }

    for (int x_cur = 1; x_cur <= x2; x_cur++) {
        double y_cur = y_x * x_cur;
        double y_cur_int = (int)(y_cur);

        if (y_cur_int >= y1 && y_cur_int <= y2) {
            if (y_cur - floor(y_cur) == 0.5) {
                mas[y_cur_int][x_cur] = 1;
            }
            mas[y_cur_int - 1][x_cur - 1] = 1;
        }
    }



    return mas;
}

void DrawVector(vector<vector<bool>> mas, char sym) {
    for (auto boka : mas) {
        for (auto joka : boka) {
            if (joka) cout << sym;
            else cout << ' ';
        }
        cout << endl;
    }
}




int main() {
    vector<vector<bool>> mas
    {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    // int k = 2;

    // mas = DrawLineToVector(mas, 1, 1, 1, 2, '1');
    // DrawVector(mas, '1');


    DrawPoint(1, 1, '1');
    DrawPoint(1, 5, '1');
    //DrawPoint(1, 1, '\b');

    return 0;
}