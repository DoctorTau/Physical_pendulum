
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