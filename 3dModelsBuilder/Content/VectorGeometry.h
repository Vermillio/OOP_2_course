#pragma once
#include <DirectXMath.h>
#include <vector>

using float3 = DirectX::XMFLOAT3;
using float2 = DirectX::XMFLOAT2;
using float4 = DirectX::XMFLOAT4;
using float4x4 = DirectX::XMFLOAT4X4;

inline float DotProduct(float3 a, float3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float3 CrossProduct(float3 a, float3 b) {
	float x = a.y*b.z - a.z*b.y;
	float y = a.z*b.x - a.x*b.z;
	float z = a.x*b.y - a.y*b.x;
	return float3(x, y, z);
}

inline float3 operator-(float3 a) { return float3(-a.x, -a.y, -a.z); };

inline float3 operator*(float3 a, float k) {
	return float3(a.x *k, a.y *k, a.z *k);
}

inline float3 operator+(float3 a, float3 b) {
	return float3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline float3 operator-(float3 a, float3 b) {
	return float3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline bool operator<=(float3 a, float3 b) {
	return a.x <= b.x && a.y <= b.y && a.z <= b.z;
}

inline float3 minP(float3 a, float3 b) {
	return a <= b ? a : b;
}

inline float3 maxP(float3 a, float3 b) {
	return b <= a ? a : b;
}

inline bool operator==(float3 a, float3 b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline float norm_2(const float3 &a) {
	return a.x*a.x + a.y*a.y + a.z*a.z;
}

float3 middle(const float3 &a, const float3 &b, const float3 &c) {
	if (a <= b) {
		if (b <= c)
			return b;
		else return a;
	}
	else if (a <= c)
		return a;
	else return c;
}

inline float3 transformFloat3(const float3 &_vector, const float4x4 &matrix) {
	float4 vector = float4(_vector.x, _vector.y, _vector.z, 1.0f);
	DirectX::XMVECTOR vec = XMLoadFloat4(&vector);
	DirectX::XMMATRIX mat = XMLoadFloat4x4(&matrix);
	vec = XMVector4Transform(vec, mat);
	float4 vec_;
	XMStoreFloat4(&vec_, vec);
	return float3(vec_.x, vec_.y, vec_.z);
}

inline float sign(const float3 &p1, const float3 &p2, const float3 &p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

inline bool pointOnLine(const float3 &a, const float3 &l0, const float3 &l1) {
	return ((a.x - l0.x) / (l1.x - l0.x) == (a.y - l0.y) / (l1.y - l0.y) == (a.z - l0.z) / (l1.z - l0.z));
}

inline bool pointOnPlane(const float3 &a, const float3 &nv, const float3 &p) {
	if (nv.x*(p.x - a.x) + nv.y*(p.y - a.y) + nv.z*(p.z - a.z) == 0)
		return true;
}

inline bool lineOnPlane(const float3 &a, const float3 &nv, const float3 &l0, const float3 &l1) {
	return pointOnPlane(a, nv, l0) && pointOnPlane(a, nv, l1);
}

inline int trianglesOnSamePlane(const float3 &a0, const float3 &a1, const float3 &a2, const float3 &b0, const float3 &b1, const float3 &b2) {
	float3 nv = CrossProduct(a1 - a0, a2 - a0);
	return	(int)pointOnPlane(a0, nv, b0) +
		(int)pointOnPlane(a0, nv, b1) +
		(int)pointOnPlane(a0, nv, b2)
		;
}

inline bool PointInsideTriangle(const float3 &pt, const float3 &v1, const float3 &v2, const float3 & v3)
{
	bool b1 = sign(pt, v1, v2) < 0.0f;
	bool b2 = sign(pt, v2, v3) < 0.0f;
	bool b3 = sign(pt, v3, v1) < 0.0f;
	return ((b1 == b2) && (b2 == b3));
}

inline bool intersectSeg(const float3& a0, const float3 &a1, const float3 &b0, const float3 &b1, float3 &ip) {
	float3 da = a1 - a0;
	float3 db = b1 - b0;
	float3 dc = b0 - a0;

	if (DotProduct(dc, CrossProduct(da, db)) != 0.0) // lines are not coplanar
		return false;

	float s = DotProduct(CrossProduct(dc, db), CrossProduct(da, db)) / norm_2(CrossProduct(da, db));
	if (s >= 0.0 && s <= 1.0)
	{
		ip = a0 + da * s;
		return true;
	}
	return false;
}

inline float3 intersectLinePlane(const float3 &p, const float3 &nv, const float3 &l0, const float3 &l1) {
	float k = DotProduct(nv, p) / DotProduct(l1 - p, nv);
	return l0 * k + l1;
}

inline bool intersectLineTriangle(const float3 &a0, const float3 &a1, const float3 &a2, const float3 &l0, const float3 &l1, float3 &i) {
	i = intersectLinePlane(a0, CrossProduct(a1 - a0, a2 - a0), l0, l1);
	if (PointInsideTriangle(i, a0, a1, a2))
		return true;
	else return false;
}

inline void intersectTriangles(const float3 &a0, const float3 &a1, const float3 &a2, const float3 &b0, const float3 &b1, const float3 &b2, std::vector<float3> &ips) {

	//degenerative cases
	if (a0 == a1) {
		if (a1 == a2)
			if (a2 == b0) {
				ips.push_back(a0);
				return;
			}
			else if (a2 == b1) {
				ips.push_back(a0);
				return;
			}
			else if (a2 == b2) {
				ips.push_back(a0);
				return;
			}
			else return;
			if (b0 == b1) {
				if (b1 == b2)
					if (b2 == a0) {
						ips.push_back(b0);
						return;
					}
					else if (b2 == a1) {
						ips.push_back(b0);
						return;
					}
					else if (b2 == a2) {
						ips.push_back(b0);
						return;
					}
					else return;
			}
	}


	float3 points1[] = { a0, a1, a2 };
	float3 points2[] = { b0, b1, b2 };

	for (int i = 0; i < 3; ++i) {
		int i_1 = (i + 1) % 3;
		bool hasMutualPlane = false;
		for (int j = 0; j < 3; ++j) {
			int j_1 = (j + 1) % 3;

			float3 cross = CrossProduct(points1[i_1] - points1[i_1], points2[j_1] - points1[i]);
			if (cross == float3(0, 0, 0)) {
				if (pointOnLine(points1[i], points1[i_1], points2[j])) {

					if (points1[i] <= points1[i_1]) {
						if (points1[i_1] <= points2[j]) {

							ips.push_back(points1[i_1]);
							if (points2[j] <= points2[j_1])
								ips.push_back(points2[j]);
							else if (points1[i] <= points2[j_1])
								ips.push_back(points2[j_1]);
							else ips.push_back(points1[i]);

						}
						else if (points1[i] <= points2[j]) {
							ips.push_back(points2[j]);
							if (points2[j_1] <= points1[i])
								ips.push_back(points1[i]);
							else if (points1[i_1] <= points2[j_1])
								ips.push_back(points1[i_1]);
							else ips.push_back(points2[j_1]);
						}
						else {
							ips.push_back(points1[i]);
							if (points2[j_1] <= points2[j])
								ips.push_back(points2[j]);
							else if (points1[i_1] <= points2[j_1])
								ips.push_back(points1[i_1]);
							else ips.push_back(points2[j_1]);
						}
					}
					else if (points1[i] <= points2[j]) {
						ips.push_back(points1[i]);
						if (points2[j] <= points2[j_1])
							ips.push_back(points2[j]);
						else if (points1[i_1] <= points2[j_1])
							ips.push_back(points2[j_1]);
						else ips.push_back(points1[j_1]);
					}
					else if (points1[i_1] <= points2[j]) {
						ips.push_back(points2[j]);
						if (points2[j_1] <= points1[i_1])
							ips.push_back(points1[i_1]);
						else if (points1[i] <= points2[j_1])
							ips.push_back(points1[i]);
						else ips.push_back(points2[j_1]);
					}
					else {
						ips.push_back(points1[i_1]);
						if (points2[j_1] <= points2[j])
							ips.push_back(points2[j]);
						else if (points1[i] <= points2[j_1])
							ips.push_back(points1[i]);
						else ips.push_back(points2[j_1]);
					}
					continue;
				}
				continue;
			}

			else if (pointOnPlane(points1[i], cross, points2[j_1]))
			{
				hasMutualPlane = true;
				float3 ip;
				if (intersectSeg(points1[i], points1[i_1], points2[j], points2[j_1], ip))
				{
					ips.push_back(ip);
				}
			}
		}
		float3 ip;
		if (!hasMutualPlane && intersectLineTriangle(b0, b1, b2, points1[i], points1[i_1], ip)) {
			ips.push_back(ip);
			//general case
		}
	}
	return;
}