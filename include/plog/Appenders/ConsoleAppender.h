#ifndef _PLOG_CONSOLE_APPENDER_H_
#define _PLOG_CONSOLE_APPENDER_H_

#include <iostream>

namespace plog
{
    template<class Formatter>
    class ConsoleAppender : public IAppender
    {
    public:
        ConsoleAppender()
        {
#ifdef _WIN32
            ::setlocale(LC_ALL, "");
#endif
        }

        virtual void write(const Record& record)
        {
            std::cout << Formatter::format(record) << std::flush;
        }
    };
}

#endif
