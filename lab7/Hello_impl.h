#ifndef __HELLO_IMPL_H__
#define __HELLO_IMPL_H__

#include "hello.hh"

class Hello_impl : public POA_Hello
{
	public:
		virtual char * say_hello(const char * client);
};

#endif
