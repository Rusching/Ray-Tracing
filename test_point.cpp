#include<iostream>
using namespace std;

struct test_point
{
    int x, y, z;

};


int main() {
    int a = 10;
    int *p = &a;
    test_point as;
    test_point *bs;
    as.x = 1;
    as.y = 2;
    as.z = 3;
    bs->x = 1;
    cout<< as.x;
    cout << "a: " << a << '\n';
    cout << "a\'point: " << p << '\n';
    return 0;
}