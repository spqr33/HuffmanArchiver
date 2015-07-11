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
    BitBunch b(10);
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
    return 0;
}

