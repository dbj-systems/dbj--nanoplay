#pragma once

#include "../common.h"

namespace cpp03 {

	using dbj::nanolib::logging::log;

	/// <summary>
	/// std free valstat 
	/// </summary>
	template<typename T, typename S>
	struct [[nodiscard]] valstat final
	{

	template<typename T, typename OP >
	struct holder final
	{
		friend OP;
		using type = holder;
		using value_type = T;

		friend T const& get(type const& holder_) noexcept {
			if (&holder_)
				return *(&holder_);
			perror("null holder deref.");
			exit(1);
		}

		constexpr T const* operator & () const noexcept {
			if (false == empty_) {
				return nullptr;
			}
			return &v_;
		}

		constexpr operator bool() const noexcept {
			if (empty_) return false;
			return true;
		}

		~holder() { if (!empty_) { v_.~T(); empty_ = true; } }
	private:
		bool empty_{ true };
		// T has to be default constructible, moveable, copyable
		T v_;
		void set(T new_v_) { v_ = new_v_; empty_ = false; }
	};

		using type = valstat;
		using value_type = T;
		using status_type = S;
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
	using int_vstat = valstat<int, std::string>;

	int_vstat ref_signal(int& input_ref_)
	{
		using namespace std;

		log("\n" __FUNCSIG__ );

		if (input_ref_ < 42) {
			input_ref_ = SIG_ATOMIC_MAX;
			log( " OK return\n" );
			return int_vstat::ok(SIG_ATOMIC_MAX); // { value, empty }
		}
		log(" Error return\n");
		return int_vstat::error("error: input must be bigger than magical constant");
	}

	TU_REGISTER([]
		{
			using namespace std;

			int arg = 0;

			{
				auto [value, status] = ref_signal(arg);
				log("\n");
				if (value && !status)
					log(  "\nOK state -- value: " , get(value) , ", status: [empty]");

				if (value && status)
					log(  "\nINFO state -- value: " , get(value) , ", status: " , get(status));

				if (!value && !status)
					log(  "\nEMPTY state --value: [empty], status : [empty] ");

				if (!value && status)
					log(  "\nERROR state -- value: [empty], status: " , get(status));
				log("\n");
			};

		});

}
