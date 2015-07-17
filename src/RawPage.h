/* 
 * File:   RawPage.h
 * Author: s
 *
 * Created on July 14, 2015, 3:39 PM
 */

#ifndef RAWPAGE_H
#define	RAWPAGE_H

#include <cstdint>

namespace LobKo {

    class HammanData;

    class RawPage {
    public:
        //MAX_RAW_PAGE_SIZE = 1024 * 1024;
        RawPage(uint32_t size, uint32_t page_number);
        RawPage(uint8_t * buffer, uint32_t size, uint32_t page_number);
        virtual ~RawPage();

        static const uint32_t MAX_RAW_PAGE_SIZE;
        inline uint8_t * buffer() ;
        inline uint32_t size() const;
    private:
        uint8_t * buffer_;
        uint32_t size_;
        uint32_t page_number_;

        RawPage(const RawPage& orig);
        const RawPage& operator=(const RawPage& rhs);
    };

    uint8_t * RawPage::buffer()  {
        return buffer_;
    }

    uint32_t RawPage::size() const {
        return size_;
    }
};
#endif	/* RAWPAGE_H */

