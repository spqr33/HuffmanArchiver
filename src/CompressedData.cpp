/* 
 * File:   CompressedData.cpp
 * Author: s
 * 
 * Created on July 18, 2015, 12:36 PM
 */

#include "CompressedData.h"
namespace LobKo {

    CompressedData::CompressedData(uint32_t page_number) :
    page_number_(page_number) {
    }

    CompressedData::~CompressedData() {
    }

    bool copmressed_data_copmarision(spCompressedData a, spCompressedData b) {
        if ( a->page_number() > b->page_number() ) {
            return true;
        }
        return false;
    }
}