#include "Hello_impl.h"
#include <iostream>

using namespace std;

char * Hello_impl::say_hello(const char * client)
{
	cout << "omniORB C++ server: " << client << endl;
	char * server = CORBA::string_alloc(32);
	strncpy(server, "omniORB C++ server", 32);
	return server;
}

// EOF
