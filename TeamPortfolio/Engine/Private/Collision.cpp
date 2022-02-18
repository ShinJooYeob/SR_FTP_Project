#include "..\Public\Collision.h"

#include "GameObject.h"
#include "Transform.h"
#include "GameInstance.h"

CCollision::CCollision(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{
}

HRESULT CCollision::Initialize_Prototype(void * pArg)
{
	_uint VertexNumber = 0;

	for (int Z = 0; Z < 2; ++Z)
	{
		for (int Y = 0; Y < 2; ++Y)
		{
			for (int X = 0; X < 2; ++X)
			{
				DefaultCubeVerties[VertexNumber] = _float3(X - 0.5f, Y - 0.5f, Z - 0.5f);
				++VertexNumber;
			}
		}
	}

	return S_OK;
}

HRESULT CCollision::Initialize_Clone(void * pArg)
{
	return S_OK;
}

HRESULT CCollision::Add_CollisionGroup(COLLISIONGROUP eCollisionGroup, CGameObject * pCollisionObject)
{
	if (eCollisionGroup >= COLLISION_END || nullptr == pCollisionObject)
	{
		MSGBOX("Failed to Add_CollisionGroup");
	}

	m_CollisionObjects[eCollisionGroup].push_back(pCollisionObject);

	Safe_AddRef(pCollisionObject);

	return S_OK;
}

HRESULT CCollision::Collision_Obsever(_float fDeltaTime)
{

	for (auto& pFlexibleObjects : m_CollisionObjects[COLLISION_FLEXIBLE])
	{
		CTransform* SourceObject = (CTransform*)pFlexibleObjects->Get_Component(TEXT("Com_Transform")); //Source의 트랜스 폼
		_float3&		SourcePosition = SourceObject->Get_MatrixState(CTransform::STATE_POS);

		//소스의 큐브 정점을 구함 -> 굳이 안쪽에서 같은 연산을 여러번 할 필요없어서 올렸음
		//_Matrix& SourceWorldMatrix = SourceObject->Get_WorldMatrix(); //Source의 월드행렬
		_float3 SourceCubeVerties[8];
		memcpy(SourceCubeVerties, DefaultCubeVerties, sizeof(_float3) * 8);

		for (_uint i = 0; i < 8; ++i)
		{
			SourceCubeVerties[i] = SourceCubeVerties[i] + SourcePosition;
		}

		for (int Collsion_enum = 1; Collsion_enum < COLLISION_END; ++Collsion_enum)
		{

			for (auto& pDestObjects : m_CollisionObjects[Collsion_enum])
			{

				CTransform* DestObject = (CTransform*)pDestObjects->Get_Component(TEXT("Com_Transform")); //Dest의 트랜스 폼

				//COLLISION_FLEXIBLE 오브젝트와 일정거리(루트 2) 이상라면 굳이 충돌연산을 하지 않음
				_float3&	DestPos = DestObject->Get_MatrixState(CTransform::STATE_POS);
				if(SourcePosition.Get_Distance(DestPos) > 1.5f)
					continue;

				//디폴트 큐브 버텍스를 사용할 것이기 때문에 이부분을 사용하지 않아도 됨
				//////////////////////////////////////////////////////////////////////////
				//CVIBuffer_Cube* SourceCube = (CVIBuffer_Cube*)pFlexibleObjects->Get_Component(TEXT("Com_CollisionBuffer")); //Source큐브 컴포넌트 받는중
				//CVIBuffer_Cube* DestCube = (CVIBuffer_Cube*)pDestObjects->Get_Component(TEXT("Com_CollisionBuffer")); //Dest큐브 컴포넌트 받는중

				//VTXTEX* pSourceVertices = (VTXTEX*)SourceCube->Get_Vtxtex(); //Source큐브 정점 받는중
				//VTXTEX* DestVertices = (VTXTEX*)DestCube->Get_Vtxtex(); //Dest큐브 정점 받는중

				//_uint SourceVertices_Number = SourceCube->Get_NumVertices(); //정점 갯수 받는중
				//_uint DestVertices_Number = DestCube->Get_NumVertices();

				//m_CCollision_SourceVertices = new VTXTEX[SourceVertices_Number]; //Source 정점 갯수를 받아서 동적배열로 만듬
				//ZeroMemory(m_CCollision_SourceVertices, sizeof(VTXTEX) * SourceVertices_Number);
				//memcpy(m_CCollision_SourceVertices, pSourceVertices, sizeof(VTXTEX)*SourceVertices_Number);

				//m_CCollision_DestVertices = new VTXTEX[DestVertices_Number]; //Dest 정점 갯수를 받아서 동적배열로 만듬
				//ZeroMemory(m_CCollision_DestVertices, sizeof(VTXTEX) * DestVertices_Number);
				//memcpy(m_CCollision_DestVertices, pSourceVertices, sizeof(VTXTEX)*DestVertices_Number);

				///////////////////////////////////////////////////////////////////////월드행렬 달라야함!!!


				//_Matrix& DesteWorldMatrix = DestObject->Get_WorldMatrix(); //Dest의 월드행렬
				_float3 DestCubeVerties[8];
				memcpy(DestCubeVerties, DefaultCubeVerties, sizeof(_float3) * 8);

				for (_uint i = 0; i < 8; ++i)
				{
					DestCubeVerties[i] = DestCubeVerties[i] + DestPos;
					//DestCubeVerties[i] = DestCubeVerties[i].PosVector_Matrix(DesteWorldMatrix);
					//D3DXVec3TransformCoord(&SourceCubeVerties[i], &m_CCollision_SourceVertices[i].vPosition, &SourceWorldMatrix);
				}
				//for (_uint i = 0; i < 8; ++i)
				//{
				//	D3DXVec3TransformCoord(&m_CCollision_DestVertices[i].vPosition, &m_CCollision_DestVertices[i].vPosition, &DesteWorldMatrix);
				//}

				//					min <= max,															max >= min

				list<_float> fBubbleSortX;
				list<_float> fBubbleSortY;
				list<_float> fBubbleSortZ;


				if (SourceCubeVerties[0].x <= DestCubeVerties[7].x &&
					DestCubeVerties[0].x <= SourceCubeVerties[7].x &&

					SourceCubeVerties[0].y <= DestCubeVerties[7].y &&
					DestCubeVerties[0].y <= SourceCubeVerties[7].y &&

					SourceCubeVerties[0].z <= DestCubeVerties[7].z &&
					DestCubeVerties[0].z <= SourceCubeVerties[7].z)
				{

					fBubbleSortX.push_back(SourceCubeVerties[0].x);
					fBubbleSortX.push_back(SourceCubeVerties[7].x);
					fBubbleSortX.push_back(DestCubeVerties[0].x);
					fBubbleSortX.push_back(DestCubeVerties[7].x);
					fBubbleSortX.sort();

					fBubbleSortY.push_back(SourceCubeVerties[0].y);
					fBubbleSortY.push_back(SourceCubeVerties[7].y);
					fBubbleSortY.push_back(DestCubeVerties[0].y);
					fBubbleSortY.push_back(DestCubeVerties[7].y);
					fBubbleSortY.sort();

					fBubbleSortZ.push_back(SourceCubeVerties[0].z);
					fBubbleSortZ.push_back(SourceCubeVerties[7].z);
					fBubbleSortZ.push_back(DestCubeVerties[0].z);
					fBubbleSortZ.push_back(DestCubeVerties[7].z);
					fBubbleSortZ.sort();

					////정렬 후 계산
					auto tempX = fBubbleSortX.begin();
					++tempX;
					_float fDistanceX = *tempX;
					++tempX;
					fDistanceX = (*tempX) - fDistanceX;

					auto tempY = fBubbleSortY.begin();
					++tempY;
					_float fDistanceY = *tempY;
					++tempY;
					fDistanceY = (*tempY) - fDistanceY;

					auto tempZ = fBubbleSortZ.begin();
					++tempZ;
					_float fDistanceZ = *tempZ;
					++tempZ;
					fDistanceZ = (*tempZ) - fDistanceZ;


					_float3 fCollision_Distance(fDistanceX, fDistanceY, fDistanceZ);

					pFlexibleObjects->Obsever_On_Trigger(pDestObjects ,fCollision_Distance, fDeltaTime);
					pDestObjects->Obsever_On_Trigger(pFlexibleObjects,fCollision_Distance, fDeltaTime);

				}
			}
		}
		
	}



	return S_OK;
}

HRESULT CCollision::Collision_Obsever_Release()
{
	for (_uint i = 0; i < COLLISION_END; ++i)
	{
		for (auto& pCollisionObject : m_CollisionObjects[i])
		{
			Safe_Release(pCollisionObject);
		}
		m_CollisionObjects[i].clear();
	}

	return S_OK;
}

CCollision * CCollision::Create(LPDIRECT3DDEVICE9 pGraphic_Device,void* pArg)
{
	CCollision*	pInstance = new CCollision(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Creating CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollision::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CCollision::Free()
{
	__super::Free();


	for (auto& CollisionObjects : m_CollisionObjects)
	{
		for (auto& pGameObject : CollisionObjects)
		{
			Safe_Release(pGameObject);
		}
		CollisionObjects.clear();
	}
}
