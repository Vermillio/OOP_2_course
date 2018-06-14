///This file contains a simple implementation of vertex structure, containing information about position, color and normal vector.
///Used for rendering.


#pragma once

using float3 = DirectX::XMFLOAT3;

namespace _3dModelsBuilder {
	struct Vertex {
		float3 pos;
		float3 color;
		float3 normal;

		Vertex projXY() {
			Vertex p(*this);
			p.pos = float3(pos.x, pos.y, 0);
			return p;
		}

		Vertex projZX() {
			Vertex p(*this);
			p.pos = float3(pos.x, 0, pos.z);
			return p;
		}

		Vertex projZY() {
			Vertex p(*this);
			p.pos = float3(0, pos.y, pos.z);
			return p;
		}
	};
}