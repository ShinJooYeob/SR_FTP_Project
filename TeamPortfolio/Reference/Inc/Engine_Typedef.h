#pragma once

namespace Engine
{
	typedef unsigned char				_ubyte;
	typedef signed char					_byte;

	typedef unsigned short				_ushort;
	typedef signed short				_short;

	typedef unsigned int				_uint;
	typedef signed int					_int;

	typedef unsigned long				_ulong;
	typedef signed long					_long;

	typedef float						_float;
	typedef double						_double;

	typedef bool						_bool;

	typedef wchar_t						_tchar;



	typedef struct tagMyMatrix  _Matrix;
	typedef struct tagMyVector3  _float3;
	typedef struct tagMyVector2  _float2;

	struct tagMyMatrix : public D3DXMATRIX
	{
		_Matrix() { };
		_Matrix(const D3DXMATRIX& _Param) { memcpy(m, _Param.m, sizeof(m)); };
		_Matrix(
			_float __11, _float __12, _float __13, _float __14,
			_float __21, _float __22, _float __23, _float __24,
			_float __31, _float __32, _float __33, _float __34,
			_float __41, _float __42, _float __43, _float __44) 
		{
			_11 = __11; _12 = __12; _13 = __13; _14 = __14;
			_21 = __21; _22 = __22; _23 = __23; _24 = __24;
			_31 = __31; _32 = __32; _33 = __33; _34 = __34;
			_41 = __41; _42 = __42; _43 = __43; _44 = __44;
		};

		 

		D3DXMATRIX InverseMatrix() {
			D3DXMATRIX InversMatrix;
			_float temp = 0;

			D3DXMatrixInverse(&InversMatrix, &temp, this);
			 
			if (temp != 0)
				return InversMatrix;
			else
				MessageBox(0, TEXT("Failed to Make Inverse Matrix"), TEXT("System Message"), MB_OK);
				return InversMatrix;
		}

		//(�Ű�����)��ġ ���ͷ� �����ϰ� ���� ��İ� ���Ͽ� ����� ��ȯ
		D3DXVECTOR3 PosVector_Matrix(D3DXVECTOR3& Vector) {
			D3DXVECTOR3 ResultVector;
			D3DXVec3TransformCoord(&ResultVector, &Vector, this);
			return ResultVector;
		}

		//(�Ű�����)���� ���ͷ� �����ϰ� ���� ��İ� ���Ͽ� ����� ��ȯ
		D3DXVECTOR3 DirVector_Matrix(D3DXVECTOR3& Vector) {
			D3DXVECTOR3 ResultVector;
			D3DXVec3TransformCoord(&ResultVector, &Vector, this);
			return ResultVector;
		}
	};

	struct tagMyVector3 : public D3DXVECTOR3
	{

		_float3(){ };
		_float3(const D3DXVECTOR3& _Param) { x = _Param.x; y = _Param.y; z = _Param.z; };
		_float3(float _X, float _Y, float _Z) { x = _X; y = _Y; z = _Z; };


		//////////////////////������ �����ε�/////////////////
		/////////////////�� ���ص� �� �Ǵ°���...;;;/////////////

		_float3 operator-(_float3& _Param)
		{
			return tagMyVector3(x - _Param.x, y - _Param.y, z - _Param.z);
		}
		_float3 operator-(D3DXVECTOR3& _Param)
		{
			return _float3(x - _Param.x, y - _Param.y, z - _Param.z);
		}
		_float3 operator+(D3DXVECTOR3& _Param)
		{
			return _float3(x + _Param.x, y + _Param.y, z + _Param.z);
		}

		//_float3& operator=(_float3 _Param)
		//{
		//	x = _Param.x;		y = _Param.y; 		z = _Param.z;
		//	return (*this);
		//}
		//_float3& operator=(D3DXVECTOR3& _Param)
		//{
		//	x = _Param.x;		y = _Param.y; 		z = _Param.z;
		//	return (*this);
		//}


		//////////////////////������ �Լ� ����/////////////////

		//Scalar ��ȯ
		_float Get_Lenth() 
		{
			return sqrtf(x*x + y*y + z*z);
		}

		//���� ���� ��ȯ
		_float3 Get_Nomalize()
		{
			_float lenth = Get_Lenth();
			return _float3(x / lenth, y / lenth, z / lenth);
		}

		//(�Ű� ����)���Ϳ� ���� ��Į��(float) ��ȯ
		_float Get_Dot(const _float3& _Param)
		{
			return D3DXVec3Dot(this, &_Param);
		}

		//(�Ű� ����)���Ϳ� ���� ����(_float3) ��ȯ
		_float3 Get_Cross(const _float3& _Param)
		{
			_float3 ResultVector;
			D3DXVec3Cross(&ResultVector, this, &_Param);
			return ResultVector;
		}

		_float3 Get_Inverse()
		{
			return ((*this) * -1);
		}

		//(�Ű� ����)���Ϳ��� �Ÿ� ��ȯ
		_float Get_Distance(const _float3& _Param)
		{
			return sqrtf((_Param.x - x) * (_Param.x - x) + (_Param.y - y) *  (_Param.y - y) + (_Param.z - z) * (_Param.z - z));
		}


		//�ڽ��� ��ġ ���ͷ� �����ϰ� (�Ű�����)���� ��İ� ���Ͽ� ����� ��ȯ
		_float3 PosVector_Matrix(D3DXMATRIX& WorldMatrix) {
			_float3 ResultVector;
			D3DXVec3TransformCoord(&ResultVector, this, &WorldMatrix);
			return ResultVector;
		}

		//�ڽ��� ���� ���ͷ� �����ϰ� (�Ű�����)���� ��İ� ���Ͽ� ����� ��ȯ
		_float3 DirVector_Matrix(D3DXMATRIX& WorldMatrix) {
			_float3 ResultVector;
			D3DXVec3TransformNormal(&ResultVector, this, &WorldMatrix);
			return ResultVector;
		}

	};	

	struct tagMyVector2: public D3DXVECTOR2
	{
		_float2() { };
		_float2(const D3DXVECTOR2& _Param) { x = _Param.x; y = _Param.y;  };
		_float2(float _X, float _Y) { x = _X; y = _Y;};

		/////////////�� ������ ������ �𸣰ٳ�..

	};
}