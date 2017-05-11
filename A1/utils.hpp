#pragma once

#include <vector>

namespace utils {

	template<typename T>
	size_t sizeOfVector(const std::vector<T>& vec) {
	    return sizeof(T) * vec.size();
	}

	template<typename T>
	const T* vectorToPointer(const std::vector<T>& vec) {
		return &vec[0];
	}
}
