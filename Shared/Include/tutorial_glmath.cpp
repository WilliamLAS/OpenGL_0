#include <glew.h>
#include "tutorial_glmath.h"
#include <cmath>


namespace tutorial
{
	// Definitions
	vec3::vec3()
		: x(0.0f), y(0.0f), z(0.0f)
	{ }

	vec3::vec3(GLfloat _x, GLfloat _y, GLfloat _z)
		: x(_x), y(_y), z(_z)
	{ }

	vec3 vec3::operator *(GLfloat const& right) const
	{
		return vec3(x * right, y * right, z * right);
	}

	vec3& vec3::ToNormalized()
	{
		float magnitude = GetMagnitude();
		if (magnitude > 0.0f)
		{
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}
		else
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		return *this;
	}

	GLfloat vec3::GetSquaredMagnitude() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	GLfloat vec3::GetMagnitude() const
	{
		return std::sqrtf(GetSquaredMagnitude());
	}

	vec3 vec3::GetNormalized() const
	{
		return vec3(*this).ToNormalized();
	}

	GLboolean vec3::IsNormalized() const
	{
		return std::fabsf(GetSquaredMagnitude() - 1.0f) < epsilon;
	}


	mat4x4::mat4x4()
		: mat4x4(
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f)
	{ }

	mat4x4::mat4x4(
		GLfloat row00, GLfloat row01, GLfloat row02, GLfloat row03,
		GLfloat row10, GLfloat row11, GLfloat row12, GLfloat row13,
		GLfloat row20, GLfloat row21, GLfloat row22, GLfloat row23,
		GLfloat row30, GLfloat row31, GLfloat row32, GLfloat row33)
	{
		data[0][0] = row00; data[0][1] = row01; data[0][2] = row02; data[0][3] = row03;
		data[1][0] = row10; data[1][1] = row11; data[1][2] = row12; data[1][3] = row13;
		data[2][0] = row20; data[2][1] = row21; data[2][2] = row22; data[2][3] = row23;
		data[3][0] = row30; data[3][1] = row31; data[3][2] = row32; data[3][3] = row33;
	}

	mat4x4 mat4x4::operator *(mat4x4 const& right) const
	{
		mat4x4 result;
		for (size_t row = 0; row < 4; row++) {
			for (size_t column = 0; column < 4; column++) {
				result.data[row][column] =
					data[row][0] * right.data[0][column] +
					data[row][1] * right.data[1][column] +
					data[row][2] * right.data[2][column] +
					data[row][3] * right.data[3][column];
			}
		}
		return result;
	}

	mat4x4& mat4x4::ToIdentity()
	{
		data[0][0] = 1.0f; data[0][1] = 0.0f; data[0][2] = 0.0f; data[0][3] = 0.0f;
		data[1][0] = 0.0f; data[1][1] = 1.0f; data[1][2] = 0.0f; data[1][3] = 0.0f;
		data[2][0] = 0.0f; data[2][1] = 0.0f; data[2][2] = 1.0f; data[2][3] = 0.0f;
		data[3][0] = 0.0f; data[3][1] = 0.0f; data[3][2] = 0.0f; data[3][3] = 1.0f;
		return *this;
	}


	quat::quat()
	{
		ToIdentity();
	}

	quat& quat::FromEuler(GLfloat pitchXInDegree, GLfloat yawYInDegree, GLfloat rollZInDegree)
	{
		float halfPitchXInRadian = -GetRadianFromDegree(pitchXInDegree) * 0.5f;
		float halfYawYInRadian = GetRadianFromDegree(yawYInDegree) * 0.5f;
		float halfRollZInRadian = -GetRadianFromDegree(-rollZInDegree) * 0.5f;

		float sinX = sinf(halfPitchXInRadian);
		float cosX = cosf(halfPitchXInRadian);
        float sinY = sinf(halfYawYInRadian);
		float cosY = cosf(halfYawYInRadian);
        float sinZ = sinf(halfRollZInRadian);
		float cosZ = cosf(halfRollZInRadian);

        x = sinX * cosY * cosZ - cosX * sinY * sinZ; // pitch
        y = cosX * sinY * cosZ + sinX * cosY * sinZ; // yaw
        z = cosX * cosY * sinZ - sinX * sinY * cosZ; // roll
        w = cosX * cosY * cosZ + sinX * sinY * sinZ;
		return *this;
	}

	quat& quat::FromAxisAngle(vec3 axis, GLfloat angleInDegree)
	{
		if (axis.GetSquaredMagnitude() == 0.0f)
			return ToIdentity();

		axis = axis.GetNormalized();
		float halfAngleInRadian = GetRadianFromDegree(angleInDegree) * 0.5f;

		x = axis.x * std::sinf(-halfAngleInRadian);
		y = axis.y * std::sinf(halfAngleInRadian);
		z = axis.z * std::sinf(-halfAngleInRadian);
		w = std::cosf(halfAngleInRadian);
		return *this;
	}

	quat& quat::ToIdentity()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f; // Because cos(angle/2) = cos(0) = 1
		return *this;
	}

	quat& quat::ToInverted()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	quat quat::GetInverse() const
	{
		return quat(*this).ToInverted();
	}

	mat4x4 quat::GetMatrix() const
	{
		float xx = x * x;
		float yy = y * y;
		float zz = z * z;
		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;

		return mat4x4(
			1.0f - 2.0f * (yy + zz),	2.0f * (xy - wz),			2.0f * (xz + wy),			0.0f,
			2.0f * (xy + wz),			1.0f - 2.0f * (xx + zz),	2.0f * (yz - wx),			0.0f,
			2.0f * (xz - wy),			2.0f * (yz + wx),			1.0f - 2.0f * (xx + yy),	0.0f,
			0.0f,						0.0f,						0.0f,						1.0f
		);
	}



	// Variables
	GLfloat const pi = acosf(-1.0f);
	GLfloat const epsilon = 1.1920929e-07F;



	// Helpers
	GLfloat GetRadianFromDegree(GLfloat degree)
	{
		return degree * (pi / 180.0f);
	}

	mat4x4 GetPerspectiveProjectionMatrix(GLfloat FOVyInDegree, GLfloat windowAspectRatio, GLfloat nearZ, GLfloat farZ)
	{
		GLfloat xyScale = 1.0f / tanf(GetRadianFromDegree(FOVyInDegree / 2.0f));
		float zRange = farZ - nearZ;

		return mat4x4(
			xyScale / windowAspectRatio,	0.0f,			0.0f,								0.0f,
			0.0f,							xyScale,		0.0f,								0.0f,
			0.0f,							0.0f,			-(farZ + nearZ) / zRange,			-(2.0f * farZ * nearZ) / zRange,
			0.0f,							0.0f,			-1.0f,								0.0f);
	}
}