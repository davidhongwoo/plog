#ifndef _PLOG_ROLLING_FILE_APPENDER_H_
#define _PLOG_ROLLING_FILE_APPENDER_H_

#include <algorithm>
#include <plog/Util.h>

namespace plog
{
    template<class Formatter>
    class RollingFileAppender : public IAppender
    {
    public:
        RollingFileAppender(const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
            : m_fileSize()
            , m_maxFileSize((std::max)(maxFileSize, static_cast<size_t>(1000))) // set a lower limit for the maxFileSize
            , m_lastFileNumber((std::max)(maxFiles - 1, 0))
            , m_firstWrite(true)
        {
            util::splitFileName(fileName, m_fileNameNoExt, m_fileExt);
        }

        virtual void write(const Record& record)
        {
            util::MutexLock lock(m_mutex);

            if (m_firstWrite)
            {
                openLogFile();
                m_firstWrite = false;
            }
            else if (m_lastFileNumber > 0 && m_fileSize > m_maxFileSize && static_cast<size_t>(-1) != m_fileSize)
            {
                rollLogFiles();
            }

            int bytesWritten = m_file.write(Formatter::format(record));

            if (bytesWritten > 0)
            {
                m_fileSize += bytesWritten;
            }
        }

    private:
        void rollLogFiles()
        {
            m_file.close();

            std::string lastFileName = buildFileName(m_lastFileNumber);
            util::File::unlink(lastFileName.c_str());

            for (int fileNumber = m_lastFileNumber - 1; fileNumber >= 0; --fileNumber)
            {
                std::string currentFileName = buildFileName(fileNumber);
                std::string nextFileName = buildFileName(fileNumber + 1);

                util::File::rename(currentFileName.c_str(), nextFileName.c_str());
            }

            openLogFile();
        }

        void openLogFile()
        {
            std::string fileName = buildFileName();
            m_fileSize = m_file.open(fileName.c_str());

            if (0 == m_fileSize)
            {
                int bytesWritten = m_file.write(Formatter::header());

                if (bytesWritten > 0)
                {
                    m_fileSize += bytesWritten;
                }
            }
        }

        std::string buildFileName(int fileNumber = 0)
        {
            std::stringstream ss;
            ss << m_fileNameNoExt;
            
            if (fileNumber > 0)
            {
                ss << '.' << fileNumber;
            }
            
            if (!m_fileExt.empty())
            {
                ss << '.' << m_fileExt;
            }

            return ss.str();
        }

    private:
        util::Mutex     m_mutex;
        util::File      m_file;
        size_t          m_fileSize;
        const size_t    m_maxFileSize;
        const int       m_lastFileNumber;
        std::string   m_fileExt;
        std::string   m_fileNameNoExt;
        bool            m_firstWrite;
    };
}

#endif
