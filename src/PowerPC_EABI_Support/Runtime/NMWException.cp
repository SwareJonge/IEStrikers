#include "PowerPC_EABI_Support/Runtime/NMWException.h"

extern "C"
{
#include <ppcdis.h>

	void abort();
}

namespace std
{
	static void dthandler()
	{
		abort();
	}

	static terminate_handler thandler = dthandler;

	static void duhandler()
	{
		terminate();
	}

	extern void terminate()
	{
		thandler();
	}

	static unexpected_handler uhandler = duhandler;

	extern void unexpected() {
		uhandler();
	}
}

extern "C" asm void __throw_catch_compare() {
#include "asm/802b1f94.s"
}

extern "C" asm void __construct_new_array()
{
#include "asm/802b21c8.s"
}

class __partial_array_destructor
{
private:
	void *p;
	size_t size;
	size_t n;
	void *dtor;

public:
	size_t i;

	__partial_array_destructor(void *array, size_t elementsize, size_t nelements, void *destructor)
	{
		p = array;
		size = elementsize;
		n = nelements;
		dtor = destructor;
		i = n;
	}

	~__partial_array_destructor()
	{
		char *ptr;

		if (i < n && dtor)
		{
			for (ptr = (char *)p + size * i; i > 0; i--)
			{
				ptr -= size;
				DTORCALL_COMPLETE(dtor, ptr);
			}
		}
	}
};

extern "C" asm void __construct_array(void *ptr, void *ctor, void *dtor, size_t size, size_t n)
{
#include "asm/802b2388.s"
}

extern "C" asm void __destroy_arr(void *block, void *dtor, size_t size, size_t n)
{
#include "asm/802b2480.s"
}
