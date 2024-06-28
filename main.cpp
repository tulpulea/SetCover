#include "SetCover.hpp"
#include <iostream>
using std::cout;
using std::endl;

int main(){
    SetCover s = SetCover();
    s.read("input43.txt");
    cout << s.find_min() << endl;
    return 0;
}