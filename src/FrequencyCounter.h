/* 
 * File:   FrequencyCounter.h
 * Author: s
 *
 * Created on July 20, 2015, 1:08 PM
 */

#ifndef FREQUENCYCOUNTER_H
#define	FREQUENCYCOUNTER_H

#include <array>
#include <memory>
#include <queue>
#include <mutex>

namespace LobKo {
    class FrequencyCounter;
    struct Array;
    typedef std::shared_ptr<Array> spArray;
    class RawPage;
    typedef std::shared_ptr<RawPage> spRawPage;

    class FrequencyCounter {
    public:
        FrequencyCounter(std::array< uint32_t, 256>& result_character_frequency, std::mutex& mutex_result_character_frequency);
        virtual ~FrequencyCounter();
        
        void operator()(std::queue<spRawPage>& raw_pages_queue, std::mutex& mutex_reading_queue, bool& reading_done, std::mutex& mutex_reading_done);
    private:
        //        FrequencyCounter(const FrequencyCounter& orig);

        struct Array {
            Array();

            std::array<uint32_t, 256> character_frequency_;
            const Array& operator+=(const FrequencyCounter::Array& rhs);
        private:
            //Array(const Array& orig);
        };
    public:
        typedef std::shared_ptr<Array> spArray;
        spArray sp_array_;
        std::array<uint32_t, 256>& result_character_frequency_;
        std::mutex& mutex_result_character_frequency_;

    };
}

#endif	/* FREQUENCYCOUNTER_H */

