#include "hello.hh"
#include <iostream>
#include <CORBA.h>
#include <Naming.hh>

using namespace std;

int main(int argc, char ** argv)
{
	try {
		// init ORB
		CORBA::ORB_ptr orb = CORBA::ORB_init(argc, argv);

		// resolve service
		Hello_ptr hello = 0;
		try {
			CORBA::Object_var ns_obj = orb->resolve_initial_references("NameService");
			if (!CORBA::is_nil(ns_obj)) {
				CosNaming::NamingContext_ptr nc = CosNaming::NamingContext::_narrow(ns_obj);
				CosNaming::Name name;
				name.length(1);
				name[0].id = CORBA::string_dup("TestServer");
				name[0].kind = CORBA::string_dup("");
				CORBA::Object_ptr obj = nc->resolve(name);
				if (!CORBA::is_nil(obj)) {
					hello = Hello::_narrow(obj);
				}
			}
		} catch (CosNaming::NamingContext::NotFound &) {
			cerr << "not found" << endl;
		} catch (CosNaming::NamingContext::InvalidName &) {
			cerr << "invalid name" << endl;
		} catch (CosNaming::NamingContext::CannotProceed &) {
			cerr << "cannot proceed" << endl;
		}

		if (!CORBA::is_nil(hello)) {
			char * server = hello->say_hello("omniORB C++ client");
			cout << "answer from: " << server << endl;
			CORBA::string_free(server);
		}

		// destroy ORB
		orb->destroy();
	} catch (CORBA::UNKNOWN) {}
}

// EOF
