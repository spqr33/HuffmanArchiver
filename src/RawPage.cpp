/* 
 * File:   RawPage.cpp
 * Author: s
 * 
 * Created on July 14, 2015, 3:39 PM
 */

#include "RawPage.h"
#include <stdexcept>
#include <cassert>

namespace LobKo {
    const uint32_t RawPage::MAX_RAW_PAGE_SIZE = 1024 * 1024;

    RawPage::RawPage(uint32_t size, uint32_t page_number) :
    size_(size),
    page_number_(page_number) {
        if ( size > MAX_RAW_PAGE_SIZE ) {
            throw std::out_of_range("More than the page limit requested");
        }
        buffer_ = new uint8_t[size];
    }

    RawPage::RawPage(const RawPage& orig) {
    }

    RawPage::~RawPage() {
        delete [] buffer_;
    }



}