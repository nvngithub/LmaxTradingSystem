#include "request_base.hpp"

#include <iomanip> 
#include <sstream>
#include <chrono>
#include <ctime>
#include <cmath>
#include <algorithm>

std::string get_utc_time_now()
{
	time_t current_time = time(nullptr);
	tm *gmt = gmtime(&current_time);

  std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	std::stringstream timestamp;
	timestamp << (1900 + gmt->tm_year) 
		  << (gmt->tm_mon < 10 ? "0" : "") << (gmt->tm_mon + 1) 
		  << (gmt->tm_mday < 10 ? "0" : "") << gmt->tm_mday 
		  << "-" 
		  << (gmt->tm_hour < 10 ? "0" : "") << gmt->tm_hour 
		  << ":" 
		  << (gmt->tm_min < 10 ? "0" : "") << gmt->tm_min
		  << ":" 
		  << (gmt->tm_sec < 10 ? "0" : "") << gmt->tm_sec
          << "." 
		  << std::setw(3) << std::setfill('0') << (milliseconds % 1000);


	return timestamp.str();
}

int calculate_checksum(const std::string& message)
{
	int message_length = message.size();
	
	int total = 0;
	const char* message_bytes = message.c_str();
	for (int i = 0; i < message_length; i++)
	{
		total += message_bytes[i];
	}
   	
	return total % 256;
}

std::string RequestBase::GetFixHeader(const std::string& msgType) {
    std::string header = "8=FIX.4.4|9=#length|35=#msgtype|34=#seqnum|49=#sendercmpid|52=#datetime|56=#targetcmpid|";
    
    int index = 0;
    
    // set message type
    index = header.find("#msgtype", index);
    header.replace(index, 8, msgType);

    // set current date and time
    index = header.find("#datetime", index);
    header.replace(index, 9, get_utc_time_now());

    // set serder cmp id
    index = header.find("#sendercmpid", 0);
    header.replace(index, 12, ::Globals::SenderCompID);

    // set target cmp id
    index = header.find("#targetcmpid", 0);
    header.replace(index, 12, static_cast<std::string>(this->isMarketDataConnection ? ::Globals::TargetMarketDataCompID: ::Globals::TargetTradingCompID));

    std::replace(header.begin(), header.end(), '|', (char)0x01);

    return header;
}
    
std::string RequestBase::GetFixTrailer(const std::string& fix) {
    std::string trailer { "10=" };
	trailer += std::to_string(calculate_checksum(fix));
    trailer += (char)0x01;

    return trailer;
}

void RequestBase::SetSeqNum (std::string& fix, int seqnum) {
    int index = fix.find("#seqnum", 0);
    fix.replace(index, 7, std::to_string(seqnum));
}

void RequestBase::SetMsgLength (std::string& fix) {
    int index = fix.find("#length", 0);
    int start_index = fix.find("35=", 0);
    fix.replace(index, 7, std::to_string(fix.size() - start_index));
}
