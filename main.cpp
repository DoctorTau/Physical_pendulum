#include <iostream>
#include <windows.h> 
#include <vector> 
#define _USE_MATH_DEFINES
#include <cmath> 
#include <dos.h>
#include <stdio.h>
#include <time.h>

using namespace std;

SHORT console_top_offset = 0; //for SetPosition
int vec_size = 60;
vector<vector<bool>> screen_vector(vec_size, vector<bool>(vec_size));
vector<vector<bool>> screen_vector_old = screen_vector;

void SetPosition(SHORT x, SHORT y)
{
    y += console_top_offset;

    HANDLE winHandle;
    COORD pos = { x,y };
    winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(winHandle, pos);
}

void DrawPoint(int x, int y, char sym) {
    SetPosition(x, y);
    cout << sym;
}

void DrawVector(vector<vector<bool>>& vnuk, char sym) {
    for ( int boka = 0; boka < vnuk.size(); boka++ ) {
        for ( int joka = 0; joka < vnuk[boka].size(); joka++ ) {
            if ( vnuk[boka][joka] ) {
                DrawPoint(joka, boka, sym);
            }
            else {
                if ( screen_vector_old[boka][joka] ) {
                    DrawPoint(joka, boka, ' ');
                }
            }
        }
    }
}

void DrawVectorIron() {
    for ( auto boka : screen_vector ) {
        for ( auto joka : boka ) {
            cout << joka << ' ';
        }
        cout << endl;
    }
}

void ClearConsole() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    cout << "\x1B[2J\x1B[H";
}

struct Point {
    int x;
    int y;
    Point(int x = 0, int y = 0) {
        this->x = x;
        this->y = y;
    }
};

void DrawLineToVector(int x1, int y1, int x2, int y2) {
    Point p1; Point p2; Point p_tmp;
    p1.x = x1; p1.y = y1;
    p2.x = x2; p2.y = y2;
    if ( p1.x > p2.x ) {
        p_tmp = p2;
        p2 = p1;
        p1 = p_tmp;
    }
    x1 = p1.x; x2 = p2.x;
    y1 = p1.y; y2 = p2.y;


    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    screen_vector[y2][x2] = 1;
    while ( x1 != x2 || y1 != y2 )
    {
        screen_vector[y1][x1] = 1;
        int error2 = error * 2;
        if ( error2 > -deltaY )
        {
            error -= deltaY;
            x1 += signX;
        }
        if ( error2 < deltaX )
        {
            error += deltaX;
            y1 += signY;
        }
    }

}

void ClearVector() {
    for ( int boka = 0; boka < screen_vector.size(); boka++ ) {
        for ( int joka = 0; joka < screen_vector[boka].size(); joka++ ) {
            screen_vector[boka][joka] = 0;
        }
    }
}

class Pendulum {
    Point top;
    Point body;
    int mas;
    double r;
    double angle;
    double w;
    double max_angle;
    double length;
    int offset_x;
    int offset_y;

public:

    Pendulum(int x_top, int y_top, int r, double T, double max_angle, int mas) {
        this->top.x = x_top;
        this->top.y = y_top;
        this->r = r;
        this->max_angle = max_angle;
        this->w = 2 * M_PI / T;
        this->angle = max_angle;
        this->mas = mas;

        this->body.x = x_top + floor(r * sin(this->angle));
        this->body.y = y_top + floor(r * cos(this->angle));

        this->offset_x = x_top;
        this->offset_y = y_top;
    }

    void Draw() {
        screen_vector_old = screen_vector;
        ClearVector();
        DrawLineToVector(this->top.x, this->top.y, this->body.x, this->body.y);
        DrawVector(screen_vector, '1');
        DrawPoint(this->body.x, this->body.y, '@');
    }

    void Process(double t) {
        this->angle = this->max_angle * cos(this->w * t + this->max_angle);
        this->body.x = this->top.x + floor(r * sin(this->angle));
        this->body.y = this->top.y + floor(r * cos(this->angle));
    }


};

int x = 25;
int y = 1;
int r = 15;
double T = 4;
double start_angle = 5 * M_PI / 11;
double mas = 1;
double g = 9.8;
int fps = 60;

int main() {
    clock_t before = clock();
    clock_t internal_time_ms = clock() - before;

    Pendulum pend(x, y, r, T, start_angle, mas);

    while ( true ) {
        internal_time_ms = ( clock() - before );
        double internal_time = ( float )( internal_time_ms ) / 1000;
        pend.Process(internal_time);
        pend.Draw();
        Sleep(1000 / fps);
    }

    return 0;
}