#include <gtest/gtest.h>
#include "logger/logger.test.cpp"
#include "sock_comm/sock_comm.test.cpp"
#include "blocking_queue/blocking_queue.test.cpp"

#include <thread>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
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

int calculate_checksum(std::string& message)
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

int main_temp(int argc, char** argv)
{
  SocketCommunicator sock_comm;	
  sock_comm.Init("", 443);							        

  std::string request = "8=FIX.4.4|9=108|35=A|34=1|49=naveen289|52=$datetime|56=LMXBDM|553=naveen289|554=Nyenburgh12!@|98=0|108=30|141=Y|";
  int index = 0;
  index = request.find("$datetime", index);
  request.replace(index, 9, get_utc_time_now());
  std::cout << "text1: " << request << std::endl;

  std::replace(request.begin(), request.end(), '|', (char)0x01);
  std::cout << "text2: " << request << std::endl;

  int checksum = calculate_checksum(request);
  std::cout << "checksum: " << checksum << std::endl;
  request += "10=";
	request += std::to_string(checksum);
  request += (char)0x01;
												        
  std::cout << "sending text:" << request << std::endl;
	int length = sock_comm.SendPacket(request.c_str());
  std::cout << "send length:" << length;

	std::string rs = sock_comm.RecvPacket();
  std::cout << "receive length:" << rs;

	return 0;

  //::testing::InitGoogleTest(&argc, argv);
  //return RUN_ALL_TESTS();
}
