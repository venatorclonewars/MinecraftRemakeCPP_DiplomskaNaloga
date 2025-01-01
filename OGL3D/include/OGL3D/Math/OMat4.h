#pragma once
#include <OGL3D/OPrerequisites.h>
#include <OGL3D/Math/OVec4.h>
#include <OGL3D/Math/OVec3.h>

class OMat4
{
public:
	OMat4()
	{
		setIdentity();
	}

	void setIdentity()
	{
		::memset(mat, 0, sizeof(mat));
		mat[0][0] = 1;
		mat[1][1] = 1;
		mat[2][2] = 1;
		mat[3][3] = 1;
	}

	void setScale(const OVec3& scale) 
	{
		mat[0][0] = scale.x;
		mat[1][1] = scale.y;
		mat[2][2] = scale.z;
	}

	void setTranslation(const OVec3& translate)
	{
		mat[3][0] = translate.x;
		mat[3][1] = translate.y;
		mat[3][2] = translate.z;
	}

	void setRotationX(f32 x)
	{
		mat[1][1] = cos(x);
		mat[1][2] = sin(x);
		mat[2][1] = -sin(x);
		mat[2][2] = cos(x);
	}

	void setRotationY(f32 y)
	{
		mat[0][0] = cos(y);
		mat[0][2] = -sin(y);
		mat[2][0] = sin(y);
		mat[2][2] = cos(y);
	}

	void setRotationZ(f32 z)
	{
		mat[0][0] = cos(z);
		mat[0][1] = sin(z);
		mat[1][0] = -sin(z);
		mat[1][1] = cos(z);
	}

	float getDeterminant()
	{
		OVec4 minor, v1, v2, v3;
		float det;

		v1 = OVec4(this->mat[0][0], this->mat[1][0], this->mat[2][0], this->mat[3][0]);
		v2 = OVec4(this->mat[0][1], this->mat[1][1], this->mat[2][1], this->mat[3][1]);
		v3 = OVec4(this->mat[0][2], this->mat[1][2], this->mat[2][2], this->mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->mat[0][3] * minor.x + this->mat[1][3] * minor.y + this->mat[2][3] * minor.z +
			this->mat[3][3] * minor.w);
		return det;
	}


	void inverse()
	{
		int a, i, j;
		OMat4 out;
		OVec4 v;
		OVec4 vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].x = (this->mat[j][0]);
					vec[a].y = (this->mat[j][1]);
					vec[a].z = (this->mat[j][2]);
					vec[a].w = (this->mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.mat[0][i] = pow(-1.0f, i) * v.x / det;
			out.mat[1][i] = pow(-1.0f, i) * v.y / det;
			out.mat[2][i] = pow(-1.0f, i) * v.z / det;
			out.mat[3][i] = pow(-1.0f, i) * v.w / det;
		}

		*this = out;
	}

	void operator *= (const OMat4& matrix)
	{
		OMat4 out;

		for (auto i = 0; i < 4; i++)
		{
			for (auto j = 0; j < 4; j++)
			{
				out.mat[i][j] = 
					mat[i][0] * matrix.mat[0][j] +
					mat[i][1] * matrix.mat[1][j] +
					mat[i][2] * matrix.mat[2][j] +
					mat[i][3] * matrix.mat[3][j];
			}
		}

		*this = out;
	}

	OVec3 getXDirection()
	{
		return OVec3(mat[0][0], mat[0][1], mat[0][2]);
	}

	OVec3 getYDirection()
	{
		return OVec3(mat[1][0], mat[1][1], mat[1][2]);
	}

	OVec3 getZDirection()
	{
		return OVec3(mat[2][0], mat[2][1], mat[2][2]);
	}

	OVec3 getTranslation()
	{
		return OVec3(mat[3][0], mat[3][1], mat[3][2]);
	}

	void setPerspectiveFovH(f32 fov, f32 aspect, f32 zNear, f32 zFar)
	{
		f32 yScale = 1.0f / tan(fov / 2.0f);
		f32 xScale = yScale / aspect;

		mat[0][0] = xScale;
		mat[1][1] = yScale;
		mat[2][2] = zFar / (zFar - zNear);
		mat[2][3] = 1.0f;
		mat[3][2] = (-zNear * zFar) / (zFar / zNear);
		mat[3][3] = 0.0f;

	}

	void setOrthoLH(f32 width, f32 height, f32 nearPlane, f32 farPlane)
	{
		mat[0][0] = 2.0f / width;
		mat[1][1] = 2.0f / height;
		mat[2][2] = 1.0f / (farPlane - nearPlane);
		mat[3][2] = -(nearPlane / (farPlane - nearPlane));

	}

public:
	f32 mat[4][4] = {};
};