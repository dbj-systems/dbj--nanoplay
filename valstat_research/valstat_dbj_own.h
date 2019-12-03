#pragma once

#include "../common.h"

namespace cpp03 {

	namespace detail {

		// STRUCT TEMPLATE remove_cv
		template <class T_>
		struct remove_cv { // remove top-level const and volatile qualifiers
			using type = T_;
		};

		template <class T_>
		struct remove_cv<const T_> {
			using type = T_;
		};

		template <class T_>
		struct remove_cv<volatile T_> {
			using type = T_;
		};

		template <class T_>
		struct remove_cv<const volatile T_> {
			using type = T_;
		};

		template <class T_>
		using remove_cv_t = typename remove_cv<T_>::type;
		
		// STRUCT TEMPLATE remove_reference
		template <class T_>
		struct remove_reference {
			using type = T_;
		};

		template <class T_>
		struct remove_reference<T_&> {
			using type = T_;
		};

		template <class T_>
		struct remove_reference<T_&&> {
			using type = T_;
		};

		template <class T_>
		using remove_reference_t = typename remove_reference<T_>::type;

		template <class T_>
		using _remove_cvref_t = remove_cv_t<remove_reference_t<T_>>;

#if ! _DBH_HAS_CXX20
		template <class T_>
		using remove_cvref_t = _remove_cvref_t<T_>;

		template <class T_>
		struct remove_cvref {
			using type = remove_cvref_t<T_>;
		};
#endif // _DBH_HAS_CXX20
		
		template <class T> T& FUN(T& t) noexcept { return t; }
		template <class T> void FUN(T&&) = delete;


	}

	template <class T>
	class ref_wrap {
	public:
		// types
		typedef T type;

		// construct/copy/destroy
		template <class U, class = decltype(
			detail::FUN<T>(std::declval<U>()),
			std::enable_if_t<!std::is_same_v<ref_wrap, detail::remove_cvref_t<U>>>()
			)>
			constexpr ref_wrap(U&& u) noexcept(noexcept(detail::FUN<T>(std::forward<U>(u))))
			: ptr_(std::addressof(detail::FUN<T>(std::forward<U>(u)))) {}
		
		// DBJ ADDED
		constexpr ref_wrap() : ptr_(nullptr) {}
		// DBJ ADDED
		constexpr operator bool() const noexcept { return (ptr_ != nullptr); }
		
		ref_wrap(const ref_wrap&) noexcept = default;

		// assignment
		ref_wrap& operator=(const ref_wrap& x) noexcept = default;

		// access
		// DBJ added
		constexpr T& operator * () const noexcept {
			if (ptr_ == nullptr) {
				assert(ptr_);
				exit(1);
			}
			return *ptr_;
		}

		constexpr operator T& () const noexcept {
			if (ptr_ == nullptr) {
				assert( ptr_ );
				exit(1);
			}
			return *ptr_; 
		}
		constexpr T& get() const noexcept { 
			if (ptr_ == nullptr) {
				assert(ptr_);
				exit(1);
			}
			return *ptr_;
		}

		template< class... ArgTypes >
		constexpr std::invoke_result_t<T&, ArgTypes...>
			operator() (ArgTypes&&... args) const {
			return std::invoke(get(), std::forward<ArgTypes>(args)...);
		}

	private:
		T* ptr_;
	};

	// deduction guides
	template<class T>
	ref_wrap(T&)->ref_wrap<T>;

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

			friend T const & get(type const& holder_) noexcept { 
				if (&holder_)
					return *(&holder_);
				perror("null holder deref.");
				exit(1);
			}

			T const * operator & () const noexcept {
				if (false == empty_){
					return nullptr;
				}
				return & v_;
			}

			operator bool() const noexcept {
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

			int f2 = 42;

			ref_wrap<int> rint{ f2 };

		int arg = 0, arg2 = 44;

		{
			auto [value, status] = ref_signal(arg);
			cout << endl;
			if (value && !status)
				cout << "\nOK state -- value: " << get(value) << ", status: [empty]";

			if (value && status)
				cout << "\nINFO state -- value: " << get(value) << ", status: " << get(status) ;

			if (!value && !status)
				cout << "\nEMPTY state --value: [empty], status : [empty] ";

			if (!value && status)
				cout << "\nERROR state -- value: [empty], status: " << get(status) ;
			cout << endl;
		};

	});

}
