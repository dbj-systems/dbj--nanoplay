#pragma once

#include "valstat_dbj.h"
//#include <iostream>
#include <future>
#include <thread>

namespace  valstat_testing_space
{
	using namespace std;
	using namespace  valstat_testing_space;

	using dbj::valstat;
	using int_vt = valstat<int>;

	// string dbj::nanoplay::make_status(const char* file, long line, const char* time_stamp, const char* msg)
	inline auto & make_status = dbj::make_status;


	inline auto vt_7 = valstat<int>{ { 7 } , { make_status( __FILE__, __LINE__, __TIMESTAMP__, "sampling future<valstat<T>>" ) } };
	inline auto vt_8 = valstat<int>{ {   } , { make_status( __FILE__, __LINE__, __TIMESTAMP__, "error state" ) } };
	inline auto vt_9 = valstat<int>{ { 9 } , {  } }; // OK

	TU_REGISTER([]
	{
		// from a packaged_task
		packaged_task<int_vt()> task([] { return vt_7; }); // wrap the function
		future<int_vt> f1 = task.get_future();  // get a future
		thread t(move(task)); // launch on a thread

		// from an async()
		future<int_vt> f2 = async(launch::async, [] { return vt_8; });

		// from a promise
		promise<int_vt> p;
		future<int_vt> f3 = p.get_future();
		thread([&p] { p.set_value_at_thread_exit(vt_9); }).detach();

		cout << "Waiting..." << flush;
		f1.wait();
		f2.wait();
		f3.wait();

		using namespace dbj; // for ADL to work

		cout << "All done!\nResults are: \n\n"
			<< f1.get() << "\n\n"
			<< f2.get() << "\n\n" 
			<< f3.get() << '\n';

		t.join();
	});
}