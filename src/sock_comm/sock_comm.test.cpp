#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <chrono>
#include "sock_comm.hpp"

TEST(SocketTest, BasicConnectionTest)
{
	std::string request = "8=FIX.4.4|9=108|35=A|34=1|49=naveen289|52=20190906-12:06:21.797|56=LMXBDM|553=naveen289|554=Nyenburgh12!@|98=0|108=30|141=Y|10=155|";
	replace(request.begin(), request.end(), '|', (char)0x01);

    SocketCommunicator sock_comm;										        
	sock_comm.SendPacket(request.c_str());

    std::this_thread::sleep_for(std::chrono::seconds(3));

	std::string res = sock_comm.RecvPacket();

    ASSERT_GT(res.size(), 0);
}