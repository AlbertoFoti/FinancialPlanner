#pragma once

#include <iostream>
#include <string>
#include <memory>

struct Date_format {
    int Day;
    int Month;
    int Year;
};
using Date_format_p = std::shared_ptr<Date_format>;

/**
 * @brief Calendar Selection Widget
 * 
 * @return Date_format_p 
 */
Date_format_p calendarSelection();