#include <iostream>
#include <windows.h> 
#include <vector> 
#define _USE_MATH_DEFINES
#include <cmath> 
#include <dos.h>
#include <stdio.h>
#include <time.h>

using namespace std;

SHORT console_top_offset = 3; //for SetPosition
int vec_size = 50;
vector<vector<bool>> screen_vector(vec_size, vector<bool>(vec_size));

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
            if ( vnuk[boka][joka] ) DrawPoint(joka, boka, sym);
        }
    }
}

//draws screen_vector
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

void DrawPointOld(int x, int y, char sym) {
    for ( int i = 1; i < y; i++ ) {
        cout << endl;
    }
    for ( int i = 1; i < x; i++ ) {
        cout << ' ';
    }
    cout << sym << endl;
}

vector<vector<bool>> DrawLineToVectorOld(vector<vector<bool>>& mas, int x1, int y1, int x2, int y2) {
    int x_offset = 10;
    x1 += x_offset;
    x2 += x_offset;
    double y_x = static_cast< double >( y2 - y1 + 1 ) / static_cast< double >( x2 - x1 + 1 );
    double x_y = static_cast< double >( x2 - x1 + 1 ) / static_cast< double >( y2 - y1 + 1 );

    for ( int y_cur = y1; y_cur <= y2; y_cur++ ) {
        double x_cur = x_y * y_cur;
        int x_cur_int = ( int )( x_cur );

        if ( x_cur_int >= x1 && x_cur_int <= x2 ) {
            if ( x_cur - floor(x_cur) == 0.5 ) {
                mas[y_cur][x_cur_int] = 1;
            }
            mas[y_cur - 1][x_cur_int - 1] = 1;
        }
    }

    for ( int x_cur = x1; x_cur <= x2; x_cur++ ) {
        double y_cur = y_x * x_cur;
        double y_cur_int = ( int )( y_cur );

        if ( y_cur_int >= y1 && y_cur_int <= y2 ) {
            if ( y_cur - floor(y_cur) == 0.5 ) {
                mas[y_cur_int][x_cur] = 1;
            }
            mas[y_cur_int - 1][x_cur - 1] = 1;
        }
    }
    return mas;
}

vector<vector<bool>> DrawLineToVector(vector<vector<bool>>& mas, int x1, int y1, int x2, int y2) {
    int deltax = abs(x2 - x1);
    int deltay = abs(y2 - y1);
    int error = 0;
    int deltaerr = ( deltay + 1 );
    int y = y1;
    int diry = y2 - y1;

    if ( diry > 0 ) {
        diry = 1;
    }
    if ( diry < 0 ) {
        diry = -1;
    }
    for ( int x = x1; x < x2; x++ ) {
        mas[y - 1][x - 1] = 1;
        error += deltaerr;
        if ( error >= deltax + 1 ) {
            y += diry;
            error -= ( deltax + 1 );
        }
    }
    return mas;
}

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
    double r;
    double angle;
    double w;
    double max_angle;
    double length;

public:

    Pendulum(int x_top, int y_top, int r, double T, double max_angle, int mas) {
        this->top.x = x_top;
        this->top.y = y_top;
        this->r = r;
        this->max_angle = max_angle;
        this->w = 2 * M_PI / T;
        this->angle = max_angle;
        this->length = 9.81 * pow(T / ( 2 * M_PI ), 2);
        this->mas = mas;

        this->body.x = floor(r * sin(this->angle));
        this->body.y = floor(r * cos(this->angle));
    }

    void Draw() {
        screen_vector = DrawLineToVector(screen_vector,
            min(this->top.x, this->body.x),
            min(this->top.y, this->body.y),
            max(this->top.x, this->body.x),
            max(this->top.y, this->body.y));
        DrawVectorIron();
        //DrawVector(screen_vector, '1');
        //DrawPoint(this->body.x, this->body.y, '@');
    }

    void Process(double t) {
        this->angle = this->max_angle * cos(this->w * t + this->max_angle);
        this->body.x = floor(r * sin(this->angle));
        this->body.y = floor(r * cos(this->angle));
    }


};

int x = 30;
int y = 1;
int r = 10;
double T = 4;
double start_angle = M_PI / 4;
double mas = 1;
double g = 9.8;
int fps = 2;

int main() {
    clock_t before = clock();
    clock_t internal_time_ms = clock() - before;

    Pendulum pend(x, y, r, T, start_angle, mas);

    while ( true ) {
        internal_time_ms = ( clock() - before );
        double internal_time = ( float )( internal_time_ms ) / 1000;
        pend.Process(internal_time);
        pend.Draw();
        //cout << internal_time << endl;
        Sleep(1000 / fps);
    }

    //screen_vector = DrawLineToVector(screen_vector, 1, 1, 19, 10);
    //DrawVector(screen_vector, '1');



    return 0;
}