#pragma once

#include <cstdlib>
#include <iostream>
#include <ostream>

template <typename SuccessType, typename ErrorType>
class Result {
public:
	Result(SuccessType value) {
		fine = true;
		this->value = value;
	}

	Result() {
		fine = false;
	}

	Result(ErrorType error) {
		fine = false;
		this->error = error;
	}

	SuccessType& Ok() {
		if (!fine) {
			std::cerr << "abort: " << std::endl;
			std::cerr << this->error.asStr() << std::endl;
			abort();
		}
		return value;
	}
	ErrorType Err() {
		if (fine) {
			std::cerr << "abort: access Result err while Result is VALUE" << std::endl;
			abort();
		}
		return error;
	}

	bool isOk() {
		return fine;
	}

private:
	bool fine;
	SuccessType value;
	ErrorType error;
};
