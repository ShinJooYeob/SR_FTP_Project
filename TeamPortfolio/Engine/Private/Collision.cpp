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

HRESULT CCollision::Collision_Pushed(CTransform * pMyTansform, _float3 fCollision_Distance, _float fDeltaTime)
{

	if (abs(fCollision_Distance.x) > abs(fCollision_Distance.y))
	{
		if (abs(fCollision_Distance.y) > abs(fCollision_Distance.z))
		{
			pMyTansform->MovetoDir(_float3(0, 0, fCollision_Distance.z), fDeltaTime);
		}
		else
		{
			pMyTansform->MovetoDir(_float3(0, fCollision_Distance.y, 0), fDeltaTime);
		}
	}
	else
	{
		if (abs(fCollision_Distance.x) > abs(fCollision_Distance.z))
		{
			pMyTansform->MovetoDir(_float3(0, 0, fCollision_Distance.z), fDeltaTime);
		}
		else
		{
			pMyTansform->MovetoDir(_float3(fCollision_Distance.x, 0, 0), fDeltaTime);
		}
	}
	return S_OK;
}

HRESULT CCollision::Collision_Suck_In(CTransform * pMyTansform, _float3 fCollision_Distance, _float fDeltaTime)
{
	fCollision_Distance = fCollision_Distance.Get_Inverse();
	fDeltaTime *= 0.25f;


	if (abs(fCollision_Distance.x) > abs(fCollision_Distance.y))
	{
		if (abs(fCollision_Distance.y) > abs(fCollision_Distance.z))
		{
			pMyTansform->MovetoDir(_float3(0, 0, fCollision_Distance.z), fDeltaTime);
		}
		else
		{
			pMyTansform->MovetoDir(_float3(0, fCollision_Distance.y, 0), fDeltaTime);
		}
	}
	else
	{
		if (abs(fCollision_Distance.x) > abs(fCollision_Distance.z))
		{
			pMyTansform->MovetoDir(_float3(0, 0, fCollision_Distance.z), fDeltaTime);
		}
		else
		{
			pMyTansform->MovetoDir(_float3(fCollision_Distance.x, 0, 0), fDeltaTime);
		}
	}

	return S_OK;
}

HRESULT CCollision::Collision_Obsever(_float fDeltaTime)
{

	for (auto& pFlexibleObjects : m_CollisionObjects[COLLISION_FLEXIBLE])
	{

		//�ҽ��� ť�� ������ ���� -> ���� ���ʿ��� ���� ������ ������ �� �ʿ��� �÷���
		//_Matrix& SourceWorldMatrix = SourceObject->Get_WorldMatrix(); //Source�� �������
		_float3 SourceCubeVerties[8];
		memcpy(SourceCubeVerties, DefaultCubeVerties, sizeof(_float3) * 8);
		CTransform* SoruceTransform = (CTransform*)pFlexibleObjects->Get_Component(TEXT("Com_Transform")); //Dest�� Ʈ���� ��
		_float3  SourcePosition = SoruceTransform->Get_MatrixState(CTransform::STATE_POS);

		for (_uint i = 0; i < 8; ++i)
		{
			SourceCubeVerties[i] = SourceCubeVerties[i] + SourcePosition;
		}

		for (int Collsion_enum = 0; Collsion_enum < COLLISION_END; ++Collsion_enum)
		{

			for (auto& pDestObjects : m_CollisionObjects[Collsion_enum])
			{
				if (pFlexibleObjects == pDestObjects)
					continue;
	
				

				CTransform* DestObject = (CTransform*)pDestObjects->Get_Component(TEXT("Com_Transform")); //Dest�� Ʈ���� ��

				//COLLISION_FLEXIBLE ������Ʈ�� �����Ÿ�(��Ʈ 2) �̻��� ���� �浹������ ���� ����
				_float3&	DestPosition = DestObject->Get_MatrixState(CTransform::STATE_POS);
				if(SourcePosition.Get_Distance(DestPosition) > 1.42f)
					continue;

				//_float3&		SourcePosition = SourceObject->Get_MatrixState(CTransform::STATE_POS); //SourceObject�� ������
				//_float3&		DestPosition = DestObject->Get_MatrixState(CTransform::STATE_POS); //DestPosition�� ������


				//CVIBuffer_Cube* SourceCube = (CVIBuffer_Cube*)pFlexibleObjects->Get_Component(TEXT("Com_CollisionBuffer")); //Sourceť�� ������Ʈ �޴���
				//CVIBuffer_Cube* DestCube = (CVIBuffer_Cube*)pDestObjects->Get_Component(TEXT("Com_CollisionBuffer")); //Destť�� ������Ʈ �޴���

			//	VTXTEX* pSourceVertices = (VTXTEX*)SourceCube->Get_Vtxtex(); //Sourceť�� ���� �޴���
			//	VTXTEX* DestVertices = (VTXTEX*)DestCube->Get_Vtxtex(); //Destť�� ���� �޴���

				//_uint SourceVertices_Number = SourceCube->Get_NumVertices(); //���� ���� �޴���
				//_uint DestVertices_Number = DestCube->Get_NumVertices();

				//m_CCollision_SourceVertices = new VTXTEX[SourceVertices_Number]; //Source ���� ������ �޾Ƽ� �����迭�� ����
				//ZeroMemory(m_CCollision_SourceVertices, sizeof(VTXTEX) * SourceVertices_Number);
			//	memcpy(m_CCollision_SourceVertices, pSourceVertices, sizeof(VTXTEX)*SourceVertices_Number);

			//	m_CCollision_DestVertices = new VTXTEX[DestVertices_Number]; //Dest ���� ������ �޾Ƽ� �����迭�� ����
			//	ZeroMemory(m_CCollision_DestVertices, sizeof(VTXTEX) * DestVertices_Number);
			//	memcpy(m_CCollision_DestVertices, pSourceVertices, sizeof(VTXTEX)*DestVertices_Number);


				///////////////////////////////////////////////////////////////////////��������� �޶����!!!



				//_Matrix& DesteWorldMatrix = DestObject->Get_WorldMatrix(); //Dest�� �������
				_float3 DestCubeVerties[8];
				memcpy(DestCubeVerties, DefaultCubeVerties, sizeof(_float3) * 8);

				for (_uint i = 0; i < 8; ++i)
				{
					DestCubeVerties[i] = DestCubeVerties[i] + DestPosition;
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


					//���� �޴� ������Ʈ���� ������ �˷��ִ� �κ�
					_float3 fCollision_Distance = { fDistanceX ,fDistanceY ,fDistanceZ };

					if (DestPosition.x > SourcePosition.x)
						fCollision_Distance.x *= -1;

					if (DestPosition.y > SourcePosition.y) 
						fCollision_Distance.y *= -1;
		
					if (DestPosition.z > SourcePosition.z) 
						fCollision_Distance.z *= -1;
			

					pFlexibleObjects->Obsever_On_Trigger(pDestObjects, fCollision_Distance				, fDeltaTime);
					pDestObjects->Obsever_On_Trigger(pFlexibleObjects, fCollision_Distance.Get_Inverse(), fDeltaTime);



					/*				if (fDistanceX > fDistanceY)
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
				*/

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
