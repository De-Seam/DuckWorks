#include <Precomp.h>
#include <Core/Math/Matrix.h>
#include <Core/Math/Quaternion.h>

Mat4 gTranslate(Mat4 matrix, Vec3 vector)
{
	matrix.w = matrix.x * vector.mX + matrix.y * vector.mY + matrix.z * vector.mZ + matrix.w;
	return matrix;
}

Mat4 gScale(Mat4 matrix, Vec3 vector)
{
	matrix[0] = matrix[0] * vector[0];
	matrix[1] = matrix[1] * vector[1];
	matrix[2] = matrix[2] * vector[2];
	return matrix;
}

Mat4 gRotate(Mat4 m, float angle, Vec3 v)
{
	const float a = angle;
	const float c = cos(a);
	const float s = sin(a);

	Vec3 axis(gNormalize(v));
	Vec3 temp(axis * (1.f - c));

	Mat4 temp_matrix;
	temp_matrix[0][0] = c + temp[0] * axis[0];
	temp_matrix[0][1] = temp[0] * axis[1] + s * axis[2];
	temp_matrix[0][2] = temp[0] * axis[2] - s * axis[1];

	temp_matrix[1][0] = temp[1] * axis[0] - s * axis[2];
	temp_matrix[1][1] = c + temp[1] * axis[1];
	temp_matrix[1][2] = temp[1] * axis[2] + s * axis[0];

	temp_matrix[2][0] = temp[2] * axis[0] + s * axis[1];
	temp_matrix[2][1] = temp[2] * axis[1] - s * axis[0];
	temp_matrix[2][2] = c + temp[2] * axis[2];

	Mat4 result;
	result[0] = m[0] * temp_matrix[0][0] + m[1] * temp_matrix[0][1] + m[2] * temp_matrix[0][2];
	result[1] = m[0] * temp_matrix[1][0] + m[1] * temp_matrix[1][1] + m[2] * temp_matrix[1][2];
	result[2] = m[0] * temp_matrix[2][0] + m[1] * temp_matrix[2][1] + m[2] * temp_matrix[2][2];
	result[3] = m[3];
	return result;
}

Mat4 GetPerspective(float fovy, float aspect, float znear, float zfar)
{
	assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0.f);

	const float tanHalfFovy = tan(fovy / 2.f);

	Mat4 result(1.f);
	result[0][0] = 1.f / (aspect * tanHalfFovy);
	result[1][1] = 1.f / (tanHalfFovy);
	result[2][2] = -(zfar + znear) / (zfar - znear);
	result[2][3] = -1.f;
	result[3][2] = -2.f * zfar * znear / (zfar - znear);
	return result;
}

Mat4 gLookAt(const Vec3& eye, const Vec3& target, const Vec3& up)
{
	Vec3 zaxis = gNormalize(eye - target);
	Vec3 xaxis = gNormalize(up.Cross(zaxis));
	Vec3 yaxis = zaxis.Cross(xaxis);

	Mat4 view;
	view[0][0] = xaxis.mX;
	view[1][0] = xaxis.mY;
	view[2][0] = xaxis.mZ;
	view[3][0] = -xaxis.Dot(eye);

	view[0][1] = yaxis.mX;
	view[1][1] = yaxis.mY;
	view[2][1] = yaxis.mZ;
	view[3][1] = -yaxis.Dot(eye);

	view[0][2] = zaxis.mX;
	view[1][2] = zaxis.mY;
	view[2][2] = zaxis.mZ;
	view[3][2] = -zaxis.Dot(eye);

	return view;
}

Mat3 gToMat3(const Quat& q)
{
	Mat3 result(1.f);
	float qxx(q.mX * q.mX);
	float qyy(q.mY * q.mY);
	float qzz(q.mZ * q.mZ);
	float qxz(q.mX * q.mZ);
	float qxy(q.mX * q.mY);
	float qyz(q.mY * q.mZ);
	float qwx(q.mW * q.mX);
	float qwy(q.mW * q.mY);
	float qwz(q.mW * q.mZ);

	result[0][0] = 1.f - 2.f * (qyy + qzz);
	result[0][1] = 2.f * (qxy + qwz);
	result[0][2] = 2.f * (qxz - qwy);

	result[1][0] = 2.f * (qxy - qwz);
	result[1][1] = 1.f - 2.f * (qxx + qzz);
	result[1][2] = 2.f * (qyz + qwx);

	result[2][0] = 2.f * (qxz + qwy);
	result[2][1] = 2.f * (qyz - qwx);
	result[2][2] = 1.f - 2.f * (qxx + qyy);
	return result;
}

Mat4 gToMat4(const Quat& q)
{
	Mat3 matrix3 = gToMat3(q);
	Mat4 matrix4 = Mat4(matrix3);
	return matrix4;
}
