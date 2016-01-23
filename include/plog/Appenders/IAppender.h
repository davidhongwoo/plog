#ifndef _PLOG_IAPPENDER_H_
#define _PLOG_IAPPENDER_H_

namespace plog
{
    class IAppender
    {
    public:
        virtual ~IAppender()
        {
        }

        virtual void write(const Record& record) = 0;
    };
}

#endif
