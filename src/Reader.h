/* 
 * File:   Reader.h
 * Author: s
 *
 * Created on July 19, 2015, 11:57 PM
 */

#ifndef READER_H
#define	READER_H

#include <fstream>
#include <queue>
#include <mutex>
#include <memory>
#include "RawPage.h"

namespace LobKo {
    class RawPage;
    typedef std::shared_ptr<RawPage> spRawPage;

    class Reader {
    public:
        explicit Reader(std::ifstream& file_in);
        virtual ~Reader();

        void reset_reading_counter();
        inline void reset_seek();
        inline uint64_t reading_counter() const;
        void operator()(std::queue<spRawPage>& raw_pages_queue, std::mutex& mutex_reading_queue, bool& reading_done, std::mutex& mutex_reading_done, uint64_t& last_page_num) const;
        inline uint64_t get_file_size() const;
    private:
        std::ifstream& file_in_;
        mutable uint64_t reading_counter_;
        uint64_t file_size_;
    };

    uint64_t Reader::get_file_size() const {
        return file_size_;
    }

    void Reader::reset_seek() {
        file_in_.seekg(0, file_in_.beg);
    }
    
    uint64_t Reader::reading_counter() const{
        return reading_counter_;
    }
}
#endif	/* READER_H */

