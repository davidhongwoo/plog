#ifndef _PLOG_FUNC_MESSAGE_FOMATTER_H_
#define _PLOG_FUNC_MESSAGE_FOMATTER_H_

#include <iomanip>
#include <plog/Util.h>

namespace plog
{
    class FuncMessageFormatter
    {
    public:
        static std::string header()
        {
            return std::string();
        }

        static std::string format(const Record& record)
        {
            std::stringstream ss;
            ss << record.getFunc().c_str() << "@" << record.getLine() << ": ";
            ss << record.getMessage().c_str() << "\n";

            return ss.str();
        }
    };
}

#endif
