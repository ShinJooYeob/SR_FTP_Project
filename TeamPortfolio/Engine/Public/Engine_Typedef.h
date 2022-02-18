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
	typedef struct tagMyVector4  _float4;

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

		//(매개변수)위치 벡터로 가정하고 월드 행렬과 곱하여 결과값 반환
		D3DXVECTOR3 PosVector_Matrix(D3DXVECTOR3& Vector) {
			D3DXVECTOR3 ResultVector;
			D3DXVec3TransformCoord(&ResultVector, &Vector, this);
			return ResultVector;
		}

		//(매개변수)방향 벡터로 가정하고 월드 행렬과 곱하여 결과값 반환
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


		//////////////////////연산자 오버로딩/////////////////
		/////////////////왜 안해도 잘 되는거지...;;;/////////////

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


		//////////////////////쓸만한 함수 정의/////////////////

		//Scalar 반환
		_float Get_Lenth() 
		{
			return sqrtf(x*x + y*y + z*z);
		}

		//단위 벡터 반환
		_float3 Get_Nomalize()
		{
			_float lenth = Get_Lenth();
			return _float3(x / lenth, y / lenth, z / lenth);
		}

		//(매개 변수)백터와 내적 스칼라(float) 반환
		_float Get_Dot(const _float3& _Param)
		{
			return D3DXVec3Dot(this, &_Param);
		}

		//(매개 변수)백터와 외적 벡터(_float3) 반환
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

		//(매개 변수)백터와의 거리 반환
		_float Get_Distance(const _float3& _Param)
		{
			return sqrtf((_Param.x - x) * (_Param.x - x) + (_Param.y - y) *  (_Param.y - y) + (_Param.z - z) * (_Param.z - z));
		}


		//자신을 위치 벡터로 가정하고 (매개변수)월드 행렬과 곱하여 결과값 반환
		_float3 PosVector_Matrix(D3DXMATRIX& WorldMatrix) {
			_float3 ResultVector;
			D3DXVec3TransformCoord(&ResultVector, this, &WorldMatrix);
			return ResultVector;
		}

		//자신으 방향 벡터로 가정하고 (매개변수)월드 행렬과 곱하여 결과값 반환
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

		/////////////뭘 넣으면 좋을지 모르겟네..

	};

	struct tagMyVector4 : public D3DXVECTOR4
	{
		_float4() { };
		_float4(const D3DXVECTOR4& _Param) 
		{	x = _Param.x; y = _Param.y; z = _Param.z; w = _Param.w; 	};
		_float4(float _X, float _Y, float _Z, float _W)
		{		x = _X; y = _Y;  z = _Z; w = _W;		};

		/////////////뭘 넣으면 좋을지 모르겟네..

	};
}


