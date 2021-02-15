#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include "tools/logging_allocator.h"

#include "Demonstratator.h"


namespace container
{
	// lab 3 var.№6 Task: Stack based on std::vector
	// 
	// Реализовано так, чтобы время выполнения операций вставки и удаления не было слишком медленным.
	// Для этого началом стека считается конец вектора
	//	т.к. операции вставки и удаления в конец вектора намного быстрее операций с началом.
	//	(быстрее т.к. такие операции не требуют каждый раз копировать содержимое массива внутри вектора)
	//
	// Поддержанные операции подчёркивают сильные стороны реализации,
	// по аналогии с стл-ными контейнерами.
	template<typename T, typename AllocatorT = std::allocator<T>>
	class Stack
	{
		std::vector<T, AllocatorT> _data;

		using _allocator_traits_type = std::allocator_traits<AllocatorT>;
	public:
		using value_type = typename decltype(_data)::value_type;
		using allocator_type = typename decltype(_data)::allocator_type;
		using pointer = typename decltype(_data)::pointer;
		using const_pointer = typename decltype(_data)::const_pointer;
		using reference = typename decltype(_data)::reference;
		using const_reference = typename decltype(_data)::const_reference;
		using size_type = typename decltype(_data)::size_type;
		using difference_type = typename decltype(_data)::difference_type;
		using iterator = typename decltype(_data)::iterator;
		using const_iterator = typename decltype(_data)::const_iterator;
		using reverse_iterator = typename decltype(_data)::reverse_iterator;
		using const_reverse_iterator = typename decltype(_data)::const_reverse_iterator;

		~Stack() noexcept = default;
		Stack() noexcept(std::is_nothrow_default_constructible<AllocatorT>::value) = default;
		
		explicit Stack(AllocatorT const &allocator) noexcept
			: _data(allocator)
		{
		}
		explicit Stack(size_type const count, AllocatorT const &allocator = AllocatorT{})
			: _data(count, allocator)
		{
		}
		Stack(size_type const count, const value_type &value, AllocatorT const &allocator = AllocatorT{})
			: _data(count, value, allocator)
		{
		}

		template<typename IteratorT, typename = typename std::enable_if<std::_Is_iterator_v<IteratorT>>::type>
		Stack(IteratorT first, IteratorT last, const AllocatorT &allocator = AllocatorT())
			: _data(first, last, allocator)
		{
		}
		Stack(std::initializer_list<T> r_init_list, const AllocatorT &allocator = AllocatorT())
			: _data(std::move(r_init_list), allocator)
		{
		}
		Stack(Stack const &other)
			: _data(other._data)
		{
		}
		Stack(Stack const &other, AllocatorT const &allocator)
			: _data(other, allocator)
		{
		}

		Stack(Stack &&other) noexcept
			: _data(std::move(other._data))
		{
		}
		Stack(Stack &&other, AllocatorT const &allocator) noexcept(_allocator_traits_type::is_always_equal::value)
			: _data(std::move(other._data), allocator)
		{
		}

		Stack& operator=(Stack &&other) noexcept(std::_Always_equal_after_move<AllocatorT>)
		{
			_data = std::move(other._data);
			return (*this);
		}

		Stack& operator=(Stack const &other)
		{
			_data = other._data;
			return (*this);
		}
		Stack& operator=(std::initializer_list<T> init_list)
		{
			_data = std::move(init_list);
			return (*this);
		}

		T& first()
		{
			return _data.back();
		}
		T const& first() const
		{
			return _data.back();
		}

		// в случае добавления содержимого контейнера, первый элемент контейнера станет последним элементом стека,
		//									   послений элемент контейнера - первым
		template<typename ValueType>
		void push(ValueType &&value)
		{
			_data.push_back(std::forward<ValueType>(value));
		}

		void pop()
		{
			_data.pop_back();
		}

		iterator erase(const_iterator where)
		{
			return _data.erase(where);
		}

		// в случае добавления значения контейнера вроде вектора, его первый элемент будет находиться глубже остальных в стеке
		template<typename ...ValueTypes>
		void emplace(ValueTypes&& ...values)
		{
			_data.emplace_back(std::forward<ValueTypes>(values)...);
		}

		void assign(size_type const new_size, const T& value)
		{
			_data.assign(new_size, value);
		}

		// заменить содержимым, где first станет последним элементом стека, last - первым
		template<typename Iterator, typename = std::enable_if_t<std::_Is_iterator_v<Iterator>>>
		void assign(Iterator first, Iterator last)
		{
			_data.assign(first, last);
		}
		// заменить содержимым, где первый элемент списка инициализации станет последним элементом стека,
		//									 а последний этемент списка станет первым элементом стека.
		void assign(std::initializer_list<T> init_list)
		{
			_data.assign(std::move(init_list));
		}

		void resize(size_type const new_size)
		{
			_data.resize(new_size);
		}
		void resize(size_type const new_size, T const &value)
		{
			_data.resize(new_size, value);
		}

		void reserve(size_type const new_capacity)
		{
			_data.reserve(new_capacity);
		}

		void shrink_to_fit()
		{
			_data.shrink_to_fit();
		}

		void clear() noexcept
		{
			_data.clear();
		}

		void swap(Stack &other) noexcept
		{
			_data.swap(other._data);
		}

		T* rdata() noexcept
		{
			return _data.data();
		}
		T const* rdata() const noexcept
		{
			return _data.data();
		}


		reverse_iterator begin() noexcept
		{
			return _data.rbegin();
		}
		const_reverse_iterator begin() const noexcept
		{
			return _data.rbegin();
		}
		reverse_iterator end() noexcept
		{
			return _data.rend();
		}
		const_iterator end() const noexcept
		{
			return _data.rend();
		}

		iterator rbegin() noexcept
		{
			return _data.begin();
		}
		const_iterator rbegin() const noexcept
		{
			return _data.begin();
		}
		iterator rend() noexcept
		{
			return _data.end();
		}
		const_iterator rend() const noexcept
		{
			return _data.end();
		}

		const_reverse_iterator cbegin() const noexcept
		{
			return rbegin();
		}
		const_reverse_iterator cend() const noexcept
		{
			return rend();
		}
		const_iterator crbegin() const noexcept
		{
			return begin();
		}
		const_iterator crend() const noexcept
		{
			return end();
		}

		bool empty() const noexcept
		{
			return _data.empty();
		}

		size_type size() const noexcept
		{
			return _data.size();
		}

		size_type max_size() const noexcept
		{
			return _data.max_size();
		}

		size_type capacity() const noexcept
		{
			return _data.capacity();
		}
	};

	template<class ValueT, class Alloc>
	void swap(Stack<ValueT, Alloc> &left, Stack<ValueT, Alloc> &right) noexcept
	{
		left._data.swap(right._data);
	}

	template<class ValueT, class Alloc>
	bool operator==(Stack<ValueT, Alloc> const &left, Stack<ValueT, Alloc> const &right)
	{
		return left._data.size() == right._data.size()
			&& std::equal(left._data.rbegin(), left._data.rend(), right._data.rbegin());
	}

	template<class ValueT, class Alloc>
	bool operator!=(Stack<ValueT, Alloc> const &left, Stack<ValueT, Alloc> const &right)
	{
		return !(left == right);
	}

	template<class ValueT, class Alloc>
	bool operator<(Stack<ValueT, Alloc> const &left, Stack<ValueT, Alloc> const &right)
	{
		return std::lexicographical_compare(
			left._data.rbegin(), left._data.rend(),
			right._data.rbegin(), right._data.rend()
		);
	}

	template<class ValueT, class Alloc>
	bool operator>(Stack<ValueT, Alloc> const &left, Stack<ValueT, Alloc> const &right)
	{
		return right < left;
	}

	template<class ValueT, class Alloc>
	bool operator<=(Stack<ValueT, Alloc> const &left, Stack<ValueT, Alloc> const &right)
	{
		return !(right < left);
	}

	template<class ValueT, class Alloc>
	bool operator>=(Stack<ValueT, Alloc> const &left, Stack<ValueT, Alloc> const &right)
	{
		return !(left < right);
	}

}

template<typename T>
struct Demonstratator<container::Stack<T>>
{
	template<typename ContainerT>
	static std::string to_string(std::string name, ContainerT &container)
	{
		std::stringstream ss;
		ss << name << "{";
		std::copy(
			container.begin(), container.end()-1,
			std::ostream_iterator<T>(ss, ",")
		);
		ss << *(container.end()-1) << "}";
		return ss.str();
	}

	static void demonstrate(configuration::Configuration const*)
	{
		{
			container::Stack<T, tools::logging_allocator<T>> stack;
			T i = 0;

			std::cout << "________________________stack<T>.push(lval);______________________________________" << std::endl;
			stack.push(i++);
			std::cout << "________________________stack<T>.emplace(lval);______________________________________" << std::endl;
			stack.emplace(i++);
			std::cout << "________________________result stack<vector<T>>______________________________________" << std::endl;
			std::cout << to_string("stack<T>", stack) << std::endl;

			for (auto const &element : stack);

			std::cout << "________________________~stack<T>______________________________________" << std::endl;
		}

		{
			using value_type = std::vector<T>;
			container::Stack<value_type, tools::logging_allocator<value_type>> stack;
			T i = 0;

			std::cout << "________________________stack<vector<T>>.push(rval_vect);______________________________________" << std::endl;
			stack.push(value_type{ i++, i++, i++ });

			{
				std::cout << "________________________stack<vector<T>>.push(lval_vect);______________________________________" << std::endl;
				value_type vect{ i++, i++, i++ };
				stack.push(vect);

				std::cout << "________________________stack<vector<T>>.push(std::move(lval_vect));______________________________________" << std::endl;
				value_type vect2{ i++, i++, i++ };
				stack.push(std::move(vect2));
			}
			std::cout << "________________________stack<vector<T>>.emplace(rval_initializer_list);______________________________________" << std::endl;
			stack.emplace(std::initializer_list<T>{i++, i++, i++});

			
			std::cout << "________________________result stack<vector<T>>______________________________________" << std::endl;
			std::cout << "stack<vector<T>>{\n\t";
			std::transform(
				stack.begin(), stack.end()-1,
				std::ostream_iterator<std::string>(std::cout, "\n\t"),
				std::bind(&Demonstratator::template to_string<decltype(*stack.begin())>, "vector<T>", std::placeholders::_1)
			);
			std::cout << Demonstratator::template to_string("vector<T>", *(stack.end()-1)) << std::endl
					<< "}" << std::endl;

			std::cout << "________________________~stack<vector<T>>______________________________________" << std::endl;
		}
	}
};