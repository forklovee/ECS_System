#pragma once

#include <unordered_set>
#include <string>

class StringPool {
	std::unordered_set<std::string> m_pool;

public:
	const char* intern(const std::string& str) {
		auto [iterator, inserted_value] = m_pool.insert(str);
		return iterator->c_str();
	}
};