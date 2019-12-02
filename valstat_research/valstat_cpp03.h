#pragma once

#include "../common.h"

namespace cpp03 {

	template<typename T, typename OP >
	struct holder final
	{
		friend OP;
		using type = holder;
		using value_type = T;

		const T & operator * () const noexcept {
			assert (false == empty_) ;
			return v_ ;
		}

		operator bool () const noexcept {
			if (empty_) return false;
			return true ;
		}

		~holder() { if (!empty_) { v_.~T(); empty_ = true; } }
	private:
		bool empty_{ true };
		// T has to be default constructible, moveable, copyable
		T v_;
		void set(T new_v_) { v_ = new_v_; empty_ = false; }
	};

	template<typename T>
	struct [[nodiscard]] valstat
	{
		using type = valstat;
		using value_type = T;
		using status_type = std::string;
		using value_proxy_type = holder<value_type, type>;
		using status_proxy_type = holder<status_type, type>;

		value_proxy_type	value;
		status_proxy_type	status;

		// 4 makers for 4 states
		static type ok(value_type value_arg_) noexcept
		{
			type retval_;
			retval_.value.set(value_arg_);
			// retval_.value(value_arg_);
			return retval_;
		}

		static type error(status_type status_arg_) noexcept
		{
			type retval_;
			retval_.status.set(status_arg_);
			return retval_;
		}

		static type info(value_type value_arg_, status_type status_arg_) noexcept
		{
			type retval_;
			retval_.value.set(value_arg_);
			retval_.status.set(status_arg_);
			return retval_;
		}

		static type empty() noexcept
		{
			type retval_;
			return retval_;
		}
	};

	/* usage ------------------------------------------------------------*/
	using int_vstat = valstat<int>;

	int_vstat ref_signal(int& input_ref_)
	{
		using namespace std;

		cout << endl << __FUNCSIG__ ;

		if (input_ref_ < 42) {
			input_ref_ = SIG_ATOMIC_MAX;
			cout << " OK return" << endl ;
			return int_vstat::ok(SIG_ATOMIC_MAX); // { value, empty }
		}
		cout << " Error return" << endl;
		return int_vstat::error("error: input must be bigger than magical constant");
	}

	TU_REGISTER([] 
	{
			using namespace std;
		int arg = 0, arg2 = 44;
		{
			auto [value, status] = ref_signal(arg);
			cout << endl;
			if (value) {
				cout << "\nvalue: " << *value;
			}
			if (status) {
				cout << "\nstatus: " << *status;
			}
			cout << endl;
		} 
		{
			auto [value, status] = ref_signal(arg2);
			cout << endl;
			if (value) {
				cout << "\nvalue: " << *value;
			}
			if (status) {
				cout << "\nstatus: " << *status;
			}
			cout << endl;
		} 
	});

}
