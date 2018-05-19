#pragma once
#include <DirectXMath.h>
#include <vector>
#include <DirectXCollision.h>

using float3 = DirectX::XMFLOAT3;
using float2 = DirectX::XMFLOAT2;
using float4 = DirectX::XMFLOAT4;
using float4x4 = DirectX::XMFLOAT4X4;
using DirectX::XMMATRIX;
using DirectX::XMVECTOR;

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

inline bool operator==(float3 a, float3 b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

float4x4 operator*(float4x4 &x, float4x4 &y) {
	XMMATRIX mX = XMLoadFloat4x4(&x);
	XMMATRIX mY = XMLoadFloat4x4(&y);
	XMMATRIX resM = XMMatrixMultiply(mX, mY);
	float4x4 res;
	XMStoreFloat4x4(&res, resM);
	return res;
}

inline float3 transformFloat3(const float3 &vector, const float4x4 &matrix) {
	XMVECTOR vec = XMVector3Transform(XMLoadFloat3(&vector), XMMatrixTranspose(XMLoadFloat4x4(&matrix)));
	float3 vec_;
	XMStoreFloat3(&vec_, vec);
	return vec_;
}

inline vector<float3> intersectTriangles(const float3 &a0, const float3 &a1, const float3 &a2, const float3 &b0, const float3 &b1, const float3 &b2) {
	
	vector<float3> intersections;
	float3 points1[] = { a0, a1, a2 };
	float3 points2[] = { b0, b1, b2 };
	for (int i = 0; i < 3; ++i)
	{
		int cur = i, next = i == 2 ? 0 : i + 1;
		float distCur, distNext;
		float3 d1 = points1[next] - points1[cur];
		float3 d2 = points2[next] - points2[cur];
		float3 d1_inv = d1 * (-1);
		float3 d2_inv = d2 * (-1);
		DirectX::XMVECTOR v1 = DirectX::XMVector3Normalize(XMLoadFloat3(&d1));
		DirectX::XMVECTOR v1_inv = DirectX::XMVector3Normalize(XMLoadFloat3(&d1_inv));
		DirectX::XMVECTOR v2 = DirectX::XMVector3Normalize(XMLoadFloat3(&d2));
		DirectX::XMVECTOR v2_inv = DirectX::XMVector3Normalize(XMLoadFloat3(&d2_inv));

		if (d1 == float3(0, 0, 0))
			continue;
		if (DirectX::TriangleTests::Intersects(XMLoadFloat3(&points1[cur]), v1,
			XMLoadFloat3(&points2[0]), XMLoadFloat3(&points2[1]),
			XMLoadFloat3(&points2[2]), distCur)
			&&
			DirectX::TriangleTests::Intersects(XMLoadFloat3(&points1[next]), v1_inv,
				XMLoadFloat3(&points2[0]), XMLoadFloat3(&points2[1]),
				XMLoadFloat3(&points2[2]), distNext)
			) 
		{
			XMStoreFloat3(&d1, v1);
			float3 ip = points1[cur] + d1 * distCur;
			intersections.push_back(ip);

			XMStoreFloat3(&d1_inv, v1_inv);
			ip = points1[next] + d1_inv * distNext;
			intersections.push_back(ip);
		}
		else if	(DirectX::TriangleTests::Intersects(XMLoadFloat3(&points1[cur]), v1_inv,
				XMLoadFloat3(&points2[0]), XMLoadFloat3(&points2[1]),
				XMLoadFloat3(&points2[2]), distCur)
				&&
				DirectX::TriangleTests::Intersects(XMLoadFloat3(&points1[next]), v1,
					XMLoadFloat3(&points2[0]), XMLoadFloat3(&points2[1]),
					XMLoadFloat3(&points2[2]), distNext)
				)
		{
			XMStoreFloat3(&d1, v1);
			float3 ip = points1[next] + d1 * distNext;
			intersections.push_back(ip);

			XMStoreFloat3(&d1_inv, v1_inv);
			ip = points1[cur] + d1_inv * distCur;
			intersections.push_back(ip);
		}

		if (d2 == float3(0, 0, 0))
			continue;
		if (DirectX::TriangleTests::Intersects(
			XMLoadFloat3(&points2[cur]), v2,
			XMLoadFloat3(&points1[0]), XMLoadFloat3(&points1[1]),
			XMLoadFloat3(&points1[2]), distCur)
			&&
			DirectX::TriangleTests::Intersects(XMLoadFloat3(&points2[next]), v2_inv,
				XMLoadFloat3(&points1[0]), XMLoadFloat3(&points1[1]),
				XMLoadFloat3(&points1[2]), distNext)
			) {
			XMStoreFloat3(&d2, v2);
			float3 ip = points2[cur] + d2 * distCur;
			intersections.push_back(ip);

			XMStoreFloat3(&d2_inv, v2_inv);
			ip = points2[next] + d2_inv * distNext;
			intersections.push_back(ip);
		}
		else if	(DirectX::TriangleTests::Intersects(
				XMLoadFloat3(&points2[cur]), v2_inv,
				XMLoadFloat3(&points1[0]), XMLoadFloat3(&points1[1]),
				XMLoadFloat3(&points1[2]), distCur)
				&&
				DirectX::TriangleTests::Intersects(XMLoadFloat3(&points2[next]), v2,
					XMLoadFloat3(&points1[0]), XMLoadFloat3(&points1[1]),
					XMLoadFloat3(&points1[2]), distNext)
				)			
			{
				XMStoreFloat3(&d2, v2);
				float3 ip = points2[next] + d2 * distNext;
				intersections.push_back(ip);

				XMStoreFloat3(&d2_inv, v2_inv);
				ip = points2[cur] +  d2_inv * distCur;
				intersections.push_back(ip);
			}
	}
	return intersections;
}