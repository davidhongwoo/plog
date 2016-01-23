#ifndef _PLOG_CSV_FOMMATTER_H_
#define _PLOG_CSV_FOMMATTER_H_

#include <iomanip>
#include <string>
#include <plog/Util.h>

namespace plog
{
    class CsvFormatter
    {
    public:
        static std::string header()
        {
            return PLOG_NSTR("Date;Time;Severity;TID;This;Function;Message\n");
        }

        static std::string format(const Record& record)
        {
            tm t;
            util::localtime_s(&t, &record.getTime().time);

            std::stringstream ss;
            ss << t.tm_year + 1900 << "/" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1 << "/" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << ";";
            ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << "." << std::setfill(PLOG_NSTR('0')) << std::setw(3) << record.getTime().millitm << ";";
            ss << severityToString(record.getSeverity()) << ";";
            ss << record.getTid() << ";";
            ss << record.getObject() << ";";
            ss << record.getFunc().c_str() << "@" << record.getLine() << ";";

            std::string message = record.getMessage();

            if (message.size() > kMaxMessageSize)
            {
                message.resize(kMaxMessageSize);
                message.append(PLOG_NSTR("..."));
            }

            std::stringstream split(message);
            std::string token;

            while (!split.eof())
            {
                std::getline(split, token, PLOG_NSTR('"'));
                ss << "\"" << token << "\"";
            }

            ss << "\n";

            return ss.str();
        }

        static const size_t kMaxMessageSize = 32000;
    };
}

#endif
