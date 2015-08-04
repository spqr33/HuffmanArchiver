/* 
 * File:   Decompression.cpp
 * Author: s
 * 
 * Created on July 30, 2015, 3:23 PM
 */

#include "Decompression.h"

namespace LobKo {

    Decompression::Decompression() :
    reading_done_(false),
    writing_done_(false),
    ready_for_write_data_priority_q_(LobKo::comparision_page_number),
    last_page_num_calculated_(false),
    header_calculated_(false){
        ;
    }

    Decompression::~Decompression() {
    }
}
