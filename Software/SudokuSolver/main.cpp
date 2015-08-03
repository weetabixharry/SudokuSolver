#include "sudoku_front_end.h"

#include <cstdlib>
#include <cassert>
#include <iostream>

using namespace std;


int main()
{

    sudoku::front_end sdku("../../sudoku.txt");
    sdku.solve();
    
    return EXIT_SUCCESS;
}
