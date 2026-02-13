#pragma once

#include <cstdint>
#include <limits>

namespace NocEngine {

	using Entity = uint32_t;
	inline constexpr Entity INVALID_ENTITY = std::numeric_limits<Entity>::max();

}