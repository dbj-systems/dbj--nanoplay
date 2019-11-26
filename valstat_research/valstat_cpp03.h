#pragma once

#include "../common.h"

namespace cpp03 {

	template<typename T, typename OP >
	struct holder final
	{
		friend OP;
		using type = holder;
		using value_type = T;

		const T* operator ! () const noexcept {
			if (empty_) return nullptr;
			return &v_;
		}
		~holder() { if (!empty_) { v_.~T(); empty_ = true; } }
	private:
		bool empty_{ true };
		// T has to be default constructible, moveable, copyable
		T v_;
		void set(T new_v_) { v_ = new_v_; empty_ = false; }
	};

	template<typename T>
	struct vstat {
		using type = vstat;
		using value_type = T;
		using status_type = std::string;

		holder<value_type, type> value_proxy;
		holder<status_type, type> status_proxy;

		const value_type* value() const noexcept {
			if (!value_set_) return nullptr;
			return &value_;
		}
		const status_type* status() const noexcept {
			if (!status_set_) return nullptr;
			return &status_;
		}

		// 4 makers for 4 states
		static type ok(value_type value_arg_) noexcept
		{
			type retval_;
			retval_.value_proxy.set(value_arg_);
			retval_.value(value_arg_);
			return retval_;
		}

		static type error(status_type status_arg_) noexcept
		{
			type retval_;
			retval_.status(status_arg_);
			return retval_;
		}

		static type info(value_type value_arg_, status_type status_arg_) noexcept
		{
			type retval_;
			retval_.value(value_arg_);
			retval_.status(status_arg_);
			return retval_;
		}

		static type empty() noexcept
		{
			type retval_;
			return retval_;
		}

	private:
		vstat() { /* barred */ }
		// one write 
		value_type* value(value_type value_arg_) noexcept {
			if (!value_set_) {
				// standard C++ preffers value based semantics
				value_ = value_arg_;
				value_set_ = true;
			}
			return &value_;
		}
		status_type* status(status_type status_arg_) noexcept {
			if (!status_set_) {
				status_ = status_arg_;
				status_set_ = true;
			}
			return &status_;
		}

		bool  value_set_{};
		bool  status_set_{};
		// both types must conform to standard C++ value types
		value_type value_;
		status_type status_;
	};
	/* usage ------------------------------------------------------------*/
	using int_vstat = vstat<int>;

	int_vstat ref_signal(int& input_ref_)
	{
		if (input_ref_ < 42) {
			input_ref_ = SIG_ATOMIC_MAX;
			return int_vstat::ok(SIG_ATOMIC_MAX); // { value, empty }
		}
		// one chance to set
		return int_vstat::error("error: input must be bigger than magical constant");
	}

	template<typename VT >
	inline void cpp03_driver(VT valstat_)
	{
		using namespace std;

		typename VT::value_type  const* value = valstat_.value();
		typename VT::status_type const* status = valstat_.status();

		cout << endl;
		if (value) {
			cout << "\nvalue: " << *value ;
		}

		if (status) {
			cout << "\nstatus: " << *status;
		}
		cout << endl;
	}; // driver

	TU_REGISTER([] {
		int arg = 0;
		// no structured binding
		cpp03_driver(ref_signal(arg));
		int arg2 = 44;
		cpp03_driver(ref_signal(arg2));
	});

}
