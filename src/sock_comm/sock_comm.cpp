#include <iostream>
#include "sock_comm.hpp"
#include "../globals/globals.hpp"

int SocketCommunicator::Init(std::string hostname, int port)
{
	std::cout << "connecting: " << hostname << std::endl;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (!sock) 
	{
		return -1;
	}

	struct hostent *he;
	he = gethostbyname(hostname.c_str());

	struct sockaddr_in sa;
	memset (&sa, 0, sizeof(sa));
	memcpy(&sa.sin_addr.s_addr, he->h_addr_list[0], he->h_length);
    sa.sin_family      = AF_INET;
    sa.sin_port        = htons (port); 

	socklen_t socklen = sizeof(sa);
	if (connect(sock, (struct sockaddr *)&sa, socklen)) 
	{
        this->IsSocketConnected = false;
		return -1;
	}

	SSL_library_init();					        
	SSLeay_add_ssl_algorithms();						
	SSL_load_error_strings();						        

	SSL_CTX *ctx = SSL_CTX_new (TLSv1_2_client_method());
	this->ssl = SSL_new (ctx);

	if (!this->ssl)
	{				
        this->IsSocketConnected = false;					            
		return -1;												        
	}

	this->socketId = SSL_get_fd(this->ssl);									    
	SSL_set_fd(this->ssl, sock);

	int error = SSL_connect(this->ssl);										    
	if (error <= 0) 
	{			
        this->IsSocketConnected = false;							            
		return -1;															        
	}

	this->IsSocketConnected = true;
	std::cout << "connect succeeded" << std::endl;
}

std::string SocketCommunicator::RecvPacket()
{    
    if(this->IsSocketConnected == false) return 0;

	int length = 0;       
	char buffer[::Globals::BufferLength];	    
	do {
			            
		length = SSL_read(ssl, buffer, ::Globals::BufferLength);
		buffer[length] = 0;
		return std::string(buffer);
	} while (length > 0);
		        
	if (length < 0) {
				        
		int err = SSL_get_error(ssl, length);		        
		if (err == SSL_ERROR_WANT_READ)	            
			throw 0;
						        
		if (err == SSL_ERROR_WANT_WRITE)			            
			throw 0;
							        
		if (err == SSL_ERROR_ZERO_RETURN || err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL)			            
			throw -1;
	}

	return std::string{};
}

int SocketCommunicator::SendPacket(const char *buffer)
{    
    if(this->IsSocketConnected == false) return 0;

	int length = SSL_write(ssl, buffer, strlen(buffer));        
	if (length < 0) 
	{	      
		int err = SSL_get_error(ssl, length);		        
		switch (err) 
		{			        
			case SSL_ERROR_WANT_WRITE: 
                return 0;
			case SSL_ERROR_WANT_READ: 
                return 0;
			case SSL_ERROR_ZERO_RETURN:								        
			case SSL_ERROR_SYSCALL:								        
			case SSL_ERROR_SSL:								        
			default:					            
				return -1;

		}
	}
    return length;
}