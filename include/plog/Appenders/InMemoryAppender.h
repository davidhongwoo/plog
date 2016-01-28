#ifndef _IN_MEMORY_APPENDER_H_ 
#define _IN_MEMORY_APPENDER_H_ 0 

#include <deque>
#include <string>

namespace plog
{
	template<class Formatter>
	class InMemoryAppender : public IAppender
	{
	public:
		InMemoryAppender(size_t logMaxSize) : m_logMaxSize(logMaxSize)
		{
		}

		virtual void write(const plog::Record& record)
		{
			std::string str = Formatter::format(record);
			m_logList.push_back(str);
			if (m_logList.size() > m_logMaxSize)
				m_logList.pop_front();
		}

		const std::deque<std::string>& getLogList()
		{
			return m_logList;
		}

	private:
		std::deque<std::string> m_logList;
		const size_t m_logMaxSize;

		InMemoryAppender& operator=(const InMemoryAppender&);
	};
}

#endif
