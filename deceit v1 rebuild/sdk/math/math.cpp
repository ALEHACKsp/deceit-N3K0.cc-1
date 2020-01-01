#include "math.h"


Matrix33 quat_to_matrix33(Quat q) {
	Matrix33 m;
	Vector v2 = q.v + q.v;
	float xx = 1 - v2.x * q.v.x;
	float yy = v2.y * q.v.y;
	float xw = v2.x * q.w;
	float xy = v2.y * q.v.x;
	float yz = v2.z * q.v.y;
	float yw = v2.y * q.w;
	float xz = v2.z * q.v.x;
	float zz = v2.z * q.v.z;
	float zw = v2.z * q.w;
	m.m00 = (float)(1 - yy - zz);
	m.m01 = (float)(xy - zw);
	m.m02 = (float)(xz + yw);
	m.m10 = (float)(xy + zw);
	m.m11 = (float)(xx - zz);
	m.m12 = (float)(yz - xw);
	m.m20 = (float)(xz - yw);
	m.m21 = (float)(yz + xw);
	m.m22 = (float)(xx - yy);
	return m;
}