/* 
 * File:   main.cpp
 * Author: s
 *
 * Created on July 6, 2015, 10:04 PM
 */

#include <cstdlib>
#include "BitBunch.h"

using namespace std;
using namespace LobKo;

/*
 * 
 */
int main(int argc, char** argv) {
    BitBunch b;
    b.append_one_bit();
    b.append_one_bit();
    b.append_zero_bit();
    b.append_zero_bit();
    b.append_zero_bit();
    b.append_one_bit();
    b.append_one_bit();
    b.append_one_bit();
    //
    b.append_one_bit();
    b.append_one_bit();
    b.append_one_bit();
    b.append_zero_bit();
    b.append_zero_bit();
    b.append_zero_bit();
    b.append_one_bit();
    b.append_one_bit();
    //
    b.append_one_bit();
    b.append_one_bit();
    b.append_one_bit();
    b.append_zero_bit();
    b.append_zero_bit();
    b.append_zero_bit();
    b.append_one_bit();
    b.append_one_bit();
    //
    //
    b.append_one_bit();
    b.append_one_bit();
    b.append_one_bit();
    b.append_zero_bit();
    b.append_zero_bit();
    b.append_zero_bit();
    b.append_one_bit();
    b.append_one_bit();
    //
    b.append_one_bit();
    b.append_one_bit();
    b.append_zero_bit();
    b.append_zero_bit();
    b.append_zero_bit();
    b.append_one_bit();
    b.append_one_bit();
    b.append_one_bit();

    b.append_one_bit();


    LobKo::BitBunch_dump(b);
    BitBunch c(b);
    std::cout << "c:\n";
    LobKo::BitBunch_dump(c);
    
    return 0;
}

