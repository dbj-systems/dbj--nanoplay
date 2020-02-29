#pragma once
#include "../common.h"
#include "valstat_dbj_status.h"

namespace  valstat_testing_space
{
	using namespace std;

	using dbj::valstat;
	using int_vt = valstat<int>;

	inline auto & make_status = dbj::make_status;

	inline auto vt_7 = valstat<int>{ { 7 } , { make_status( __FILE__, __LINE__, __TIMESTAMP__, "sampling future<valstat<T>>" ) } };
	inline auto vt_8 = valstat<int>{ {   } , { make_status( __FILE__, __LINE__, __TIMESTAMP__, "error state" ) } };
	inline auto vt_9 = valstat<int>{ { 9 } , {  } }; // OK

	TU_REGISTER([]
	{
			using dbj::nanolib::logging::log;

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

		log("Waiting...");
		f1.wait();
		f2.wait();
		f3.wait();

		using namespace dbj; // for ADL to work

		log( "All done!\nResults are: \n\n"
			, f1.get() , "\n\n"
			, f2.get() , "\n\n" 
			, f3.get() , '\n');

		t.join();
	});
}