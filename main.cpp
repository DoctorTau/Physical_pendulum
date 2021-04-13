#include <iostream>
#include <windows.h> 
#include <vector> 
#include <cmath> 

using namespace std;

struct Point {
    int x;
    int y;
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};


void DrawPoint(int x, int y, char sym) {
    for ( int i = 1; i < y; i++ ) {
        cout << endl;
    }
    for ( int i = 1; i < x; i++ ) {
        cout << ' ';
    }
    cout << sym << endl;
}

vector<vector<bool>> DrawLineToVector(vector<vector<bool>> mas, int x1, int y1, int x2, int y2, char sym) {
    double y_x = static_cast< double >( y2 - y1 + 1 ) / static_cast< double >( x2 - x1 + 1 );
    double x_y = static_cast< double >( x2 - x1 + 1 ) / static_cast< double >( y2 - y1 + 1 );


    for ( int y_cur = 1; y_cur <= y2; y_cur++ ) {
        double x_cur = x_y * y_cur;
        int x_cur_int = ( int )( x_cur );

        if ( x_cur_int >= x1 && x_cur_int <= x2 ) {
            if ( x_cur - floor(x_cur) == 0.5 ) {
                mas[y_cur][x_cur_int] = 1;
            }
            mas[y_cur - 1][x_cur_int - 1] = 1;
        }
    }

    for ( int x_cur = 1; x_cur <= x2; x_cur++ ) {
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

void DrawVector(vector<vector<bool>> mas, char sym) {
    for ( auto boka : mas ) {
        for ( auto joka : boka ) {
            if ( joka ) cout << sym;
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

    mas = DrawLineToVector(mas, 1, 1, 3, 3, '1');
    DrawVector(mas, '1');

    return 0;
}