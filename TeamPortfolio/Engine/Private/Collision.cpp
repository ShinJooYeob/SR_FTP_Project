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

		for (int Collsion_enum = 0; Collsion_enum < COLLISION_END; ++Collsion_enum)
		{
			for (auto& pDestObjects : m_CollisionObjects[Collsion_enum])
			{
				if (&pFlexibleObjects == &pDestObjects)
				{
					continue;
				}

				CTransform* SourceObject = (CTransform*)pFlexibleObjects->Get_Component(TEXT("Com_Transform")); //Source�� Ʈ���� ��
				CTransform* DestObject = (CTransform*)pDestObjects->Get_Component(TEXT("Com_Transform")); //Dest�� Ʈ���� ��



				_float3&		SourcePosition = SourceObject->Get_MatrixState(CTransform::STATE_POS); //SourceObject�� ������
				_float3&		DestPosition = DestObject->Get_MatrixState(CTransform::STATE_POS); //DestPosition�� ������


				CVIBuffer_Cube* SourceCube = (CVIBuffer_Cube*)pFlexibleObjects->Get_Component(TEXT("Com_VIBuffer_Cube")); //Sourceť�� ������Ʈ �޴���
				CVIBuffer_Cube* DestCube = (CVIBuffer_Cube*)pDestObjects->Get_Component(TEXT("Com_VIBuffer_Cube")); //Destť�� ������Ʈ �޴���

				VTXTEX* pSourceVertices = (VTXTEX*)SourceCube->Get_Vtxtex(); //Sourceť�� ���� �޴���
				VTXTEX* pDestVertices = (VTXTEX*)DestCube->Get_Vtxtex(); //Destť�� ���� �޴���

				_uint SourceVertices_Number = SourceCube->Get_NumVertices(); //���� ���� �޴���
				_uint DestVertices_Number = DestCube->Get_NumVertices();

				m_CCollision_SourceVertices = new VTXTEX[SourceVertices_Number]; //Source ���� ������ �޾Ƽ� �����迭�� ����
				ZeroMemory(m_CCollision_SourceVertices, sizeof(VTXTEX) * SourceVertices_Number);
				memcpy(m_CCollision_SourceVertices, pSourceVertices, sizeof(VTXTEX)*SourceVertices_Number);

				m_CCollision_DestVertices = new VTXTEX[DestVertices_Number]; //Dest ���� ������ �޾Ƽ� �����迭�� ����
				ZeroMemory(m_CCollision_DestVertices, sizeof(VTXTEX) * DestVertices_Number);
				memcpy(m_CCollision_DestVertices, pDestVertices, sizeof(VTXTEX)*DestVertices_Number);


				///////////////////////////////////////////////////////////////////////��������� �޶����!!!

				_Matrix& SourceWorldMatrix = SourceObject->Get_WorldMatrix(); //Source�� �������
				_Matrix& DesteWorldMatrix = DestObject->Get_WorldMatrix(); //Dest�� �������


				for (_uint i = 0; i < SourceVertices_Number; ++i)
				{
					D3DXVec3TransformCoord(&m_CCollision_SourceVertices[i].vPosition, &m_CCollision_SourceVertices[i].vPosition, &SourceWorldMatrix);
				}
				for (_uint i = 0; i < DestVertices_Number; ++i)
				{
					D3DXVec3TransformCoord(&m_CCollision_DestVertices[i].vPosition, &m_CCollision_DestVertices[i].vPosition, &DesteWorldMatrix);
				}
				//					min <= max,															max >= min

				list<_float> fBubbleSortX;
				list<_float> fBubbleSortY;
				list<_float> fBubbleSortZ;

				if (m_CCollision_SourceVertices[0].vPosition.x <= m_CCollision_DestVertices[7].vPosition.x &&
					m_CCollision_DestVertices[0].vPosition.x <= m_CCollision_SourceVertices[7].vPosition.x &&

					m_CCollision_SourceVertices[0].vPosition.y <= m_CCollision_DestVertices[7].vPosition.y &&
					m_CCollision_DestVertices[0].vPosition.y <= m_CCollision_SourceVertices[7].vPosition.y &&

					m_CCollision_SourceVertices[0].vPosition.z <= m_CCollision_DestVertices[7].vPosition.z &&
					m_CCollision_DestVertices[0].vPosition.z <= m_CCollision_SourceVertices[7].vPosition.z)
				{

					fBubbleSortX.push_back(m_CCollision_SourceVertices[0].vPosition.x);
					fBubbleSortX.push_back(m_CCollision_SourceVertices[7].vPosition.x);
					fBubbleSortX.push_back(m_CCollision_DestVertices[0].vPosition.x);
					fBubbleSortX.push_back(m_CCollision_DestVertices[7].vPosition.x);
					fBubbleSortX.sort();

					fBubbleSortY.push_back(m_CCollision_SourceVertices[0].vPosition.y);
					fBubbleSortY.push_back(m_CCollision_SourceVertices[7].vPosition.y);
					fBubbleSortY.push_back(m_CCollision_DestVertices[0].vPosition.y);
					fBubbleSortY.push_back(m_CCollision_DestVertices[7].vPosition.y);
					fBubbleSortY.sort();

					fBubbleSortZ.push_back(m_CCollision_SourceVertices[0].vPosition.z);
					fBubbleSortZ.push_back(m_CCollision_SourceVertices[7].vPosition.z);
					fBubbleSortZ.push_back(m_CCollision_DestVertices[0].vPosition.z);
					fBubbleSortZ.push_back(m_CCollision_DestVertices[7].vPosition.z);
					fBubbleSortZ.sort();

					////���� �� ���
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

					//SourcePosition
					//DestPosition

					_float3 fCollision_Distance;

					if (fDistanceX > fDistanceY)
					{
						if (fDistanceY > fDistanceZ)
						{
							if (SourcePosition.z > DestPosition.z)
							{
								fCollision_Distance.x = fDistanceX;
								fCollision_Distance.y = fDistanceY;
								fCollision_Distance.z = fDistanceZ;

								pFlexibleObjects->Obsever_On_Trigger(pDestObjects, fCollision_Distance, fDeltaTime);

								fCollision_Distance.z = -fDistanceZ;
								pDestObjects->Obsever_On_Trigger(pFlexibleObjects, fCollision_Distance, fDeltaTime);
							}
							else
							{
								fCollision_Distance.x = fDistanceX;
								fCollision_Distance.y = fDistanceY;
								fCollision_Distance.z = -fDistanceZ;

								pFlexibleObjects->Obsever_On_Trigger(pDestObjects, fCollision_Distance, fDeltaTime);

								fCollision_Distance.z = fDistanceZ;
								pDestObjects->Obsever_On_Trigger(pFlexibleObjects, fCollision_Distance, fDeltaTime);

							}
						}
						else
						{
							if (SourcePosition.y > DestPosition.y)
							{
								fCollision_Distance.x = fDistanceX;
								fCollision_Distance.y = fDistanceY;
								fCollision_Distance.z = fDistanceZ;

								pFlexibleObjects->Obsever_On_Trigger(pDestObjects, fCollision_Distance, fDeltaTime);

								fCollision_Distance.y = -fDistanceY;
								pDestObjects->Obsever_On_Trigger(pFlexibleObjects, fCollision_Distance, fDeltaTime);
							}
							else
							{
								fCollision_Distance.x = fDistanceX;
								fCollision_Distance.y = -fDistanceY;
								fCollision_Distance.z = fDistanceZ;

								pFlexibleObjects->Obsever_On_Trigger(pDestObjects, fCollision_Distance, fDeltaTime);

								fCollision_Distance.y = fDistanceY;
								pDestObjects->Obsever_On_Trigger(pFlexibleObjects, fCollision_Distance, fDeltaTime);
							}
						}
					}
					else
					{
						if (fDistanceX > fDistanceZ)
						{
							if (SourcePosition.z > DestPosition.z)
							{
								fCollision_Distance.x = fDistanceX;
								fCollision_Distance.y = fDistanceY;
								fCollision_Distance.z = fDistanceZ;

								pFlexibleObjects->Obsever_On_Trigger(pDestObjects, fCollision_Distance, fDeltaTime);

								fCollision_Distance.z = -fDistanceZ;
								pDestObjects->Obsever_On_Trigger(pFlexibleObjects, fCollision_Distance, fDeltaTime);
							}
							else
							{
								fCollision_Distance.x = fDistanceX;
								fCollision_Distance.y = fDistanceY;
								fCollision_Distance.z = -fDistanceZ;

								pFlexibleObjects->Obsever_On_Trigger(pDestObjects, fCollision_Distance, fDeltaTime);

								fCollision_Distance.z = fDistanceZ;
								pDestObjects->Obsever_On_Trigger(pFlexibleObjects, fCollision_Distance, fDeltaTime);
							}
						}
						else
						{
							if (SourcePosition.x > DestPosition.x)
							{
								fCollision_Distance.x = fDistanceX;
								fCollision_Distance.y = fDistanceY;
								fCollision_Distance.z = fDistanceZ;

								pFlexibleObjects->Obsever_On_Trigger(pDestObjects, fCollision_Distance, fDeltaTime);

								fCollision_Distance.x = -fDistanceX;
								pDestObjects->Obsever_On_Trigger(pFlexibleObjects, fCollision_Distance, fDeltaTime);
							}
							else
							{
								fCollision_Distance.x = -fDistanceX;
								fCollision_Distance.y = fDistanceY;
								fCollision_Distance.z = fDistanceZ;

								pFlexibleObjects->Obsever_On_Trigger(pDestObjects, fCollision_Distance, fDeltaTime);

								fCollision_Distance.x = fDistanceX;
								pDestObjects->Obsever_On_Trigger(pFlexibleObjects, fCollision_Distance, fDeltaTime);
							}
						}
					}

					//SourceObject->Set_MatrixState(CTransform::STATE_POS, fCollision_Distance);
				}



				Safe_Delete_Array(m_CCollision_SourceVertices);
				m_CCollision_SourceVertices = nullptr;

				Safe_Delete_Array(m_CCollision_DestVertices);
				m_CCollision_DestVertices = nullptr;
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

CCollision * CCollision::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
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
