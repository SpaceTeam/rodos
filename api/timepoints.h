
/**
 * @file timepoints.h
 * @date 2008/06/12 9:51
 * @author Sergio Montenegro
 *
 *
 * @brief %Time tagged text (header)
 *
 */

#pragma once
#include <stdint.h>


namespace RODOS {

/**
 * @class TimePoints
 * @brief %Time tagged text
 *
 * TimePoints registers timepoints associated with a free text comment.
 * This is usefull to trace applications and system behavior.
 * @warning this is not thread safe !!!
*/

template <int POINTS>
class TimePoints {
public:
    int64_t      time[POINTS];
    const char*  text[POINTS];
    uint32_t     index;  ///< how many entries are written (add counter)

    TimePoints(void) { index = 0; }
    void clear(void) { index = 0; }

    void addRaw(const char* const name) {
        time[index] = NOW();
        text[index] = name; // ony the pointer which shall be a constant
        if (index < (POINTS-2)) { index++; }
    }

    void add(const char* const name) {
        PRIORITY_CEILER_IN_SCOPE();
        addRaw(name);
    }

    bool isEmpty() { return index==0; }

    void printAll(void) {  ///< prints and clear
        PRINTF("time(secs)      text/name\n");
        PRINTF("-------------------------------\n");
        for (uint32_t i = 0; i < index; i++)  {
            PRINTF("%3.9f  %s\n", (double)time[i]/SECONDS, text[i]);
        }
    }

};

extern TimePoints<20>  errorLog;

}   // namespace

