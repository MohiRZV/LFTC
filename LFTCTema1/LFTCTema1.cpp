// LFTCTema1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

void circlePerimeterArea() {
    /*
        computes and displays the perimeter and area of a circle
        in: radius - double ; the radius of the circle
        out: -
        console outputs the perimeter and area
    */
    double radius;
    std::cin >> radius;

    double perimeter;
    double area;
    double PI = 3.14;
    perimeter = 2 * PI * radius;
    area = PI * radius * radius;

    std::cout << perimeter << "\n";

    std::cout << area;
}



void circlePerimeterAreaMyMLP() {
   /* radius: double;
    gather => radius;

    perimeter: double;
    area: double;
    PI: double;
    PI = 3.14;
    perimeter = 2 * PI * radius;
    area = PI * radius * radius;

    dash <= perimeter;
    dash <= area;*/
}

void cmmdc() {
    /*
        computes and displays the greatest common divisor of two numbers
        in: a,b - integer ; the two numbers
        out: -
        console outputs the gcd of the given numbers
    */
    int a;
    int b;
    std::cin >> a;
    std::cin >> b;

    while (a != b)
    {
        if (a > b)
            a = a - b;
        else
            b = b - a;
    }
    std::cout << a;
}

void cmmdcMyMLP() {
    /*a: integer;
    b: integer;
    gather => a;
    gather => b;

    given(a != b) {
        supposing(a > b) thereupon a = a - b;
        supposing(a <= b) thereupon b = b - a;
    }

    dash <= a;*/
}

void sumOfNNumbers() {
    /*
        computes and displays the sum of n given numbers
        in: x1,x2..,xn - integer
        out: -
            console outputs the sum of the n numbers
    */
    int s = 0;
    int n;
    int x;
    std::cin >> n;
    for (int i = 1; i <= n; i++) {
        std::cin >> x;
        s = s + x;
    }
    std::cout << s;
}

void sumOfNNumbersMyMLP() {
    /*s: integer;
    s = 0;
    n: integer;
    x: integer;
    i: integer;
    gather => n;
    purge(i = 0; i < n; i++) {
        gather => x;
        s = s + x;
    }
    dash <= s;*/
}

//erori din MLP care sunt erori si in C++
void errorFirst() {
    ////no ; after instruction
    //s: integer
    //s = 0;
    //n: integer;
    //x: integer;
    //i: integer;
    //gather => n;
    ////no { at the beggining of instructions list in loop
    //purge(i = 0; i < n; i++) 
    //    gather = > x;
    //    s = s + x;
    //}
    //dash <= s;
}

//erori din MLP care nu sunt erori in C++
void errorSecond() {
     //variable name is more than 8 characters
     //a12345678: integer;
     //b: integer;
     //gather => a12345678;
     //gather => b;

     //given(a != b) {
     //    supposing(a12345678 > b) thereupon a12345678 = a12345678 - b;
     //    //else is not specified in the MLP
     //    else b = b - a12345678;
     //}

     //dash <= a12345678;
}

int main()
{
    circlePerimeterArea();
    std::cout << "\n";
    cmmdc();
    std::cout << "\n";
    sumOfNNumbers();
}

