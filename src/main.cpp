/* 
 * File:   main.cpp
 * Author: s
 *
 * Created on July 6, 2015, 10:04 PM
 */

#include <cstdlib>
#include "BitBunch.h"
#include "RawPage.h"
#include "HammanData.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>

using namespace std;
using namespace LobKo;

/*
 * 
 */

int main(int argc, char** argv) {
    {
        string file_name("test_file");
        std::ifstream in(file_name.c_str(), ios_base::binary);
        if ( !in.is_open() ) {
            std::cout << "Error while opening file\n";
            return 1;
        }
        
        
        
        RawPage page1(1024, 1);
        in.read((char*) page1.buffer(), page1.size());
        if ( in.fail() || in.bad() ) {
            cerr << "Error occurs while file reading \n";
            return 1;
        }
        std::cout << "____\n";
        HammanData hd(page1);
    }
    return 0;
}

