//
// CustomAppender - shows how to implement a custom appender that stores log messages in memory.
//

#include <plog/Log.h>
#include <plog/Appenders/InMemoryAppender.h>
#include <deque>
#include <iostream>

int main()
{
	static plog::InMemoryAppender<plog::TxtFormatter> myAppender(10); // Create our custom appender. 
	plog::init(plog::debug, &myAppender); // Initialize the logger with our appender.

	LOGD << "A debug message!";
	LOGD << "A debug message 2!";

	const std::deque<std::string>& logList = myAppender.getLogList(); // This returns a list of stored log messages. 

	std::deque<std::string>::const_iterator iter = logList.begin();
	std::deque<std::string>::const_iterator endIter = logList.end();
	for (; iter != endIter; ++iter) {
		std::cout << *iter;
	}

	return 0;
}
