#pragma once

#include <cstdlib>
#include <iostream>
#include <ostream>

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

	T& Ok() {
		if (!fine) {
			std::cerr << "abort: " << std::endl;
			std::cerr << this->error.as_str() << std::endl;
			abort();
		}
		return value;
	}
	E Err() {
		if (fine) {
			std::cerr << "abort: access Result Err while Result is VALUE" << std::endl;
			abort();
		}
		return error;
	}

	bool isOk() {
		return fine;
	}

private:
	bool fine;
	T value;
	E error;
};
