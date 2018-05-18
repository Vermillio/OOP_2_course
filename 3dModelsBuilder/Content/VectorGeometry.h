#pragma once
#include <DirectXMath.h>
#include <vector>
#include <DirectXCollision.h>

using float3 = DirectX::XMFLOAT3;
using float2 = DirectX::XMFLOAT2;
using float4 = DirectX::XMFLOAT4;
using float4x4 = DirectX::XMFLOAT4X4;

using std::vector;

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
	float epsilon = 0.000001;
	return ((a.x - l0.x) / (l1.x - l0.x) - (a.y - l0.y) / (l1.y - l0.y) < epsilon &&  (a.z - l0.z) / (l1.z - l0.z) - (a.y - l0.y) / (l1.y - l0.y) < epsilon);
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
	float denom = DotProduct(l1 - p, nv);
	if (denom == 0) {
		return (l1 + l0) * 0.5f;
	}
	float k = DotProduct(nv, p) / denom;
	return l0 * k + l1;
}

inline bool intersectLineTriangle(const float3 &a0, const float3 &a1, const float3 &a2, const float3 &l0, const float3 &l1, float3 &i) {
	i = intersectLinePlane(a0, CrossProduct(a1 - a0, a2 - a0), l0, l1);
	if (PointInsideTriangle(i, a0, a1, a2))
		return true;
	else return false;
}

inline vector<float3> intersectTriangles(const float3 &a0, const float3 &a1, const float3 &a2, const float3 &b0, const float3 &b1, const float3 &b2) {

	vector<float3> intersections;
	float3 points1[] = { a0, a1, a2 };
	float3 points2[] = { b0, b1, b2 };
	for (int i = 0; i < 3; ++i)
	{
		float dist;
		float3 d1 = points1[i==2 ? 0 : i+1] - points1[i];
		float3 d2= points2[i==2?0:i+1] - points2[i];
		DirectX::XMVECTOR dir1 = DirectX::XMVector3Normalize(XMLoadFloat3(&d1));
		DirectX::XMVECTOR dir2 = DirectX::XMVector3Normalize(XMLoadFloat3(&d2));
		XMStoreFloat3(&d1, dir1);
		XMStoreFloat3(&d2, dir2);
		if (!DirectX::Internal::XMVector3IsUnit(dir1))
			continue;
		if (DirectX::TriangleTests::Intersects(XMLoadFloat3(&points1[i]), dir1, 
			XMLoadFloat3(&points2[0]), XMLoadFloat3(&points2[1]),
			XMLoadFloat3(&points2[2]), dist)
			) 
		{
			float3 ip = points1[i] +  d1*dist;
			intersections.push_back(ip);
		}
		if (!DirectX::Internal::XMVector3IsUnit(dir2))
			continue;
		if (DirectX::TriangleTests::Intersects(
			XMLoadFloat3(&points2[i]), dir2, 
			XMLoadFloat3(&points1[0]), XMLoadFloat3(&points1[1]), 
			XMLoadFloat3(&points1[2]), dist)
			) 
		{
			float3 ip = points2[i] +  d2 * dist;
			intersections.push_back(ip);
		}
	}
	return intersections;
}