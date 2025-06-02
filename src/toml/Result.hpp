#pragma once

#include <cstdlib>
#include <iostream>
#include <ostream>

// template <typename SuccessType, typename ErrorType>
// class Result {
// public:
// 	Result(SuccessType value) {
// 		is_success_ = true;
// 		this->value_ = value;
// 	}

// 	Result() {
// 		is_success_ = false;
// 	}

// 	Result(ErrorType error) {
// 		is_success_ = false;
// 		this->error_ = error;
// 	}

// 	const SuccessType& ok() const {
// 		if (!is_success_) {
// 			std::cerr << "abort: Accessing result when it's an error." << std::endl;
// 			std::cerr << this->error_.as_str() << std::endl;
// 			abort();
// 		}
// 		return value_;
// 	}

// 	ErrorType err() {
// 		if (is_success_) {
// 			std::cerr << "abort: Accessing error when result is successful." << std::endl;
// 			abort();
// 		}
// 		return error_;
// 	}

// 	bool is_ok() const {
// 		return is_success_;
// 	}

// private:
// 	bool is_success_;
// 	SuccessType value_;
// 	ErrorType error_;
// };

template <typename T, typename E>
class Result {
public:
	Result(T value) {
		fine = true;
		this->value = value;
	}

	Result() {
		fine = false;
	}

	Result(E error) {
		fine = false;
		this->error = error;
	}

	T& ok() {
		if (!fine) {
			std::cerr << "abort: " << std::endl;
			std::cerr << this->error.as_str() << std::endl;
			abort();
		}
		return value;
	}
	E err() {
		if (fine) {
			std::cerr << "abort: access Result err while Result is VALUE" << std::endl;
			abort();
		}
		return error;
	}

	bool is_ok() {
		return fine;
	}

private:
	bool fine;
	T value;
	E error;
};
