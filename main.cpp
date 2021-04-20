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
int vec_size = 200;
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
        this->L1 = r1;
        this->L2 = r2;
        this->top2.x = x_top1 + floor(L1 * sin(angle1));
        this->top2.y = y_top1 + floor(L1 * cos(angle1));
        this->body.x = top2.x + floor(L2 * sin(angle2));
        this->body.y = top2.y + floor(L2 * cos(angle2));

        this->m1 = m1;
        this->m2 = m2;
        this->a1 = 0;
        this->a2 = 0;
        this->w1 = 0;
        this->w2 = 0;
    }

    void Process() {
        double g = 0.0000005;

        // this->a1 = -( g * ( 2 * m1 - m2 ) * sin(this->angle1) - m2 * g * sin(angle1 - 2 * angle2) - 2 *
        //     sin(angle1 - angle2) * m2 * ( pow(a2, 2) * L2 + pow(a1, 2) * L1 * cos(angle1 - angle2) ) ) /
        //     ( L1 * ( 2 * m1 + m2 - m2 * cos(2 * ( angle1 - angle2 )) ) );

        // this->a2 = ( 2 * sin(angle1 - angle2) * ( pow(a1, 2) * L1 * ( m1 + m2 ) + g * ( m1 + m2 ) *
        //     cos(angle1) + pow(a2, 2) * L2 * m2 * cos(angle1 - angle2) ) ) /
        //     ( L2 * ( 2 * m1 + m2 - m2 * cos(2 * ( angle1 - angle2 )) ) );

        double num1 = g * ( 2 * m1 + m2 ) * sin(angle1);
        double num2 = m2 * g * sin(angle1 - 2 * angle2);
        double num3 = 2 * sin(angle1 - angle2) * m2 * ( pow(w2, 2) * L2 + pow(w1, 2) * L1 * cos(angle1 - angle2) );
        double numerator1 = -num1 - num2 - num3;
        double denominator1 = L1 * ( 2 * m1 + m2 - m2 * cos(2 * angle1 - 2 * angle2) );
        this->a1 = numerator1 / denominator1;


        double num6 = pow(w1, 2) * L1 * ( m1 + m2 );
        double num7 = g * ( m1 + m2 ) * cos(angle1);
        double num8 = pow(w2, 2) * L2 * m2 * cos(angle1 - angle2);
        double num5 = num6 + num7 + num8;
        double numerator2 = 2 * sin(angle1 - angle2) * num5;
        double denominator2 = L2 * ( 2 * m1 + m2 - m2 * cos(2 * angle1 - 2 * angle2) );
        this->a2 = numerator2 / denominator2;

        this->w1 += this->a1;
        this->w2 += this->a2;

        this->angle1 += this->w1;
        this->angle2 += this->w2;

        this->top2.x = top1.x + floor(L1 * sin(angle1));
        this->top2.y = top1.y + floor(L1 * cos(angle1));

        this->body.x = top2.x + floor(L2 * sin(angle2));
        this->body.y = top2.y + floor(L2 * cos(angle2));

    }

    void Draw() {
        //cout << 1 << ' ' << top1.x << ' ' << top1.y << endl;
        //cout << 2 << ' ' << top2.x << ' ' << top2.y << endl;
        //cout << 3 << ' ' << body.x << ' ' << body.y << endl;
        screen_vector_old = screen_vector;
        ClearVector();
        DrawLineToVector(this->top1.x, this->top1.y, this->top2.x, this->top2.y);
        DrawLineToVector(this->top2.x, this->top2.y, this->body.x, this->body.y);
        DrawVector(screen_vector, '1');
        DrawPoint(this->body.x, this->body.y, '@');
        DrawPoint(this->top2.x, this->top2.y, '@');

    }
};

int main() {

    int x = 40;
    int y = 20;
    int r = 10;
    double T = 4;
    double start_angle = M_PI / 4;
    double mas = 1;
    int fps = 10000;

    clock_t before = clock();
    clock_t internal_time_ms = clock() - before;

    //Pendulum pend(x, y, r, T, start_angle, mas);

    Double_pendulum pend(x, y, r, r, 1, 1, 5 * M_PI / 6, M_PI / 4);
    int k = 0;
    ClearConsole();
    while ( true ) {
        internal_time_ms = ( clock() - before );
        double internal_time = ( float )( internal_time_ms ) / 1000;
        pend.Process();
        if ( k % 500 == 0 ) pend.Draw();
        Sleep(1000 / fps);
        k += 1;
    }

    return 0;
}