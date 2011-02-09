/*
 * Copyright (C) 2010-2011 Dmitry Marakasov
 *
 * This file is part of glosm.
 *
 * glosm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * glosm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with glosm.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXCEPTION_HH
#define EXCEPTION_HH

#include <exception>
#include <sstream>

namespace Private {
	/**
	 * Safe string buffer to be used in exceptions.
	 *
	 * As you may know, std::exception::what() is has no-throw
	 * exception specifier, thus you should not throw in what(),
	 * or else the program may terminate during exception
	 * handling (even if all exceptions are handled properly).
	 *
	 * Stock IOstream stringbuf is unsafe from this point of view
	 * as its str() method does construct string object which may
	 * throw, so we use our own buffer here. It may be normally
	 * used with std::ostream, yet it has safe c_str() method which
	 * never throws. In addition to that, it handles space reserve
	 * so data may be appended
	 */
	class SafeStringBuffer : public std::streambuf {
	protected:
		/** initial size of a buffer */
		static const unsigned int initial_size_ = 64;

	protected:
		unsigned int reserve_;
		unsigned int allocated_;
		unsigned int used_;
		char* buffer_;

	protected:
		/**
		 * Ensures that buffer has enough space expanding it if necessary
		 *
		 * @param size required buffer size
		 */
		void EnsureSize(unsigned int size);

	public:
		SafeStringBuffer(unsigned int reserve = 0);
		SafeStringBuffer(const char* message);
		SafeStringBuffer(const SafeStringBuffer& s);
		~SafeStringBuffer();

		/**
		 * Changes reserve for the buffer expanding it if necessry
		 *
		 * @param reserve new reserve value
		 */
		void SetReserve(unsigned int reserve);

		/**
		 * Append data to the buffer
		 *
		 * This function is used from ostream to append data to the buffer.
		 *
		 * @param s pointer to data
		 * @param n data length
		 *
		 * @return number of bytes appended (always n)
		 */
		std::streamsize xsputn(const char* s, std::streamsize n);

		/**
		 * Safely append data to the buffer within the reserve
		 *
		 * This may be safely used in what() to append additional data
		 * (such ass strerror) to the end of message.
		 *
		 * @param s pointer to data
		 * @param n data length
		 *
		 * @return number of bytes appended
		 */
		std::streamsize AppendReserve(const char* s, std::streamsize n) throw();

		/**
		 * Safely get pointer to buffer contents
		 *
		 * @return pointer to null-terminated buffer contents
		 */
		const char* c_str() throw();
	};

	/**
	 * Convenient exception class which supports stream-like appending
	 *
	 * Example:
	 * @code throw Exception("foo") << ", also baz and " << 1 << 2 << 3; @endcode
	 */
	class Exception: public std::exception {
	protected:
		mutable SafeStringBuffer message_;

	public:
		Exception();
		Exception(const Exception& e);
		virtual ~Exception() throw();

		template <class T>
		void Append(const T& t) const {
			std::ostream stream(&message_);
			(std::ostream&)stream << t;
		}

		virtual const char* what() const throw();
	};

	template <class E, class T>
	static const E& operator<<(const E& e, const T& t) {
		e.Append(t);

		return e;
	}
};

/**
 * Generic exception
 */
class Exception: public Private::Exception {
};

/**
 * System error that automatically appends strerror
 */
class SystemError: public Exception {
protected:
	int errno_;

public:
	SystemError();
	SystemError(const SystemError& e);
	virtual ~SystemError() throw();
	virtual const char* what() const throw();
};

#endif
