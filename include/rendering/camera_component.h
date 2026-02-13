#pragma once

#include "components.h"

namespace NocEngine
{
	struct CCamera : CComponent {
		enum class ProjectionType {
			Perspective,
			Orthographic
		};
		
		float projection_fov{ 75.f };
		float ortho_down{ 0.f };
		float ortho_top{ 800.f };
		float ortho_left{ 0.f };
		float ortho_right{ 600.f };
		
		float near_plane{ 0.01f };
		float far_plane{ 100.f };
		
		ProjectionType projection_type{ ProjectionType::Perspective };
	};
}