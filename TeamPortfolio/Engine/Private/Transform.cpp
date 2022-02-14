#include "..\Public\Transform.h"




CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

CTransform::CTransform(const CTransform & rhs)
	:CComponent(rhs),
	m_WorldMatrix(rhs.m_WorldMatrix)
{
}

void CTransform::Move_Forward(_float fDeltaTime)
{
	_float3 vPos = Get_MatrixState(STATE_POS);
	vPos += Get_MatrixState(STATE_LOOK).Get_Nomalize()* m_TransforDesc.fMovePerSec *fDeltaTime;
	Set_MatrixState(STATE_POS, vPos);
}

void CTransform::Move_Backward(_float fDeltaTime)
{
	_float3 vPos = Get_MatrixState(STATE_POS);

	vPos -= Get_MatrixState(STATE_LOOK).Get_Nomalize()* m_TransforDesc.fMovePerSec *fDeltaTime;

	Set_MatrixState(STATE_POS, vPos);
}

void CTransform::Move_Right(_float fDeltaTime)
{
	_float3 vPos = Get_MatrixState(STATE_POS);
	vPos += Get_MatrixState(STATE_RIGHT).Get_Nomalize()* m_TransforDesc.fMovePerSec *fDeltaTime;
	Set_MatrixState(STATE_POS, vPos);
}

void CTransform::Move_Left(_float fDeltaTime)
{
	_float3 vPos = Get_MatrixState(STATE_POS);
	vPos -= Get_MatrixState(STATE_RIGHT).Get_Nomalize()* m_TransforDesc.fMovePerSec *fDeltaTime;
	Set_MatrixState(STATE_POS, vPos);
}

void CTransform::MovetoTarget(_float3 vTarget, _float fDeltaTime)
{

	_float3 vPos = Get_MatrixState(STATE_POS);

	vPos += (vTarget - vPos).Get_Nomalize() * m_TransforDesc.fMovePerSec * fDeltaTime;

	Set_MatrixState(STATE_POS, vPos);



}

void CTransform::LookAt(_float3 vTarget)
{
	_float3 vPos = Get_MatrixState(STATE_POS);
	_float3 vScale = Get_MatrixScale();

	_float3 vLook, vRight, vUp;

	vLook = (vTarget - vPos).Get_Nomalize() * vScale.z;

	if (vLook.Get_Nomalize() == _float3(0, 1, 0)) 
	{
		MSGBOX("����ȭ �� ���Ͱ� _float3(0,1,0)�� ��� ������ �Ұ����մϴ�.");
	}
	else
		vRight = _float3(0, 1, 0).Get_Cross(vLook).Get_Nomalize() * vScale.x;

	vUp = vLook.Get_Cross(vRight).Get_Nomalize() * vScale.y;


	Set_MatrixState(CTransform::STATE_RIGHT, vRight);
	Set_MatrixState(CTransform::STATE_UP, vUp);
	Set_MatrixState(CTransform::STATE_LOOK, vLook);
}

void CTransform::Turn_CW( _float3 vAxis, _float fDeltaTime)
{
	_Matrix AxisRotateMatrix;

	D3DXMatrixRotationAxis(&AxisRotateMatrix, &vAxis, m_TransforDesc.fRotationPerSec * fDeltaTime);


	Set_MatrixState(CTransform::STATE_RIGHT, Get_MatrixState(STATE_RIGHT).DirVector_Matrix(AxisRotateMatrix));
	Set_MatrixState(CTransform::STATE_UP, Get_MatrixState(STATE_UP).DirVector_Matrix(AxisRotateMatrix));
	Set_MatrixState(CTransform::STATE_LOOK, Get_MatrixState(STATE_LOOK).DirVector_Matrix(AxisRotateMatrix));
}

void CTransform::Turn_CCW( _float3 vAxis, _float fDeltaTime)
{
	Turn_CW(vAxis.Get_Inverse(), fDeltaTime);
}

void CTransform::Rotation_CW(_float3 vAxis, _float fRadian)
{
	_float3		vRight = _float3(1, 0, 0) * Get_MatrixScale().x;
	_float3		vUp = _float3(0, 1, 0)* Get_MatrixScale().y;
	_float3		vLook = _float3(0, 0, 1)* Get_MatrixScale().z;

	_Matrix AxisRotateMatrix;

	D3DXMatrixRotationAxis(&AxisRotateMatrix, &vAxis, fRadian);


	Set_MatrixState(CTransform::STATE_RIGHT, vRight.DirVector_Matrix(AxisRotateMatrix));
	Set_MatrixState(CTransform::STATE_UP, vUp.DirVector_Matrix(AxisRotateMatrix));
	Set_MatrixState(CTransform::STATE_LOOK, vLook.DirVector_Matrix(AxisRotateMatrix));
}

void CTransform::Rotation_CCW(_float3 vAxis, _float fRadian)
{
	Rotation_CW(vAxis.Get_Inverse(), fRadian);
}

void CTransform::Scaled(_float3 vScale)
{

	Set_MatrixState(CTransform::STATE_RIGHT, Get_MatrixState(STATE_RIGHT).Get_Nomalize() * vScale.x);
	Set_MatrixState(CTransform::STATE_UP, Get_MatrixState(STATE_UP).Get_Nomalize() * vScale.y);
	Set_MatrixState(CTransform::STATE_LOOK, Get_MatrixState(STATE_LOOK).Get_Nomalize() * vScale.z);
}

void CTransform::Scaling(_float3 vScale, _float fDeltaTime)
{
	_float3 NowScale = Get_MatrixScale();
	_float3 FinalScale = (NowScale + (vScale * fDeltaTime));

	Set_MatrixState(CTransform::STATE_RIGHT, Get_MatrixState(STATE_RIGHT).Get_Nomalize() * FinalScale.x);
	Set_MatrixState(CTransform::STATE_UP, Get_MatrixState(STATE_UP).Get_Nomalize() * FinalScale.y);
	Set_MatrixState(CTransform::STATE_LOOK, Get_MatrixState(STATE_LOOK).Get_Nomalize() * FinalScale.z);
}



HRESULT CTransform::Bind_WorldMatrix()
{
	if (m_pGraphicDevice == nullptr)
		return E_FAIL;

	_Matrix WorldMatrixSetbyPivot = m_WorldMatrix;

	WorldMatrixSetbyPivot._41 -= m_TransforDesc.vPivot.x;
	WorldMatrixSetbyPivot._42 -= m_TransforDesc.vPivot.y;
	WorldMatrixSetbyPivot._43 -= m_TransforDesc.vPivot.z;

	m_pGraphicDevice->SetTransform(D3DTS_WORLD, &WorldMatrixSetbyPivot);

	return S_OK;
}

HRESULT CTransform::Bind_WorldMatrix_Look_Camera(_float3 vCameraPos)
{
	if (m_pGraphicDevice == nullptr)
		return E_FAIL;

	_float3 vPos = Get_MatrixState(STATE_POS);
	_float3 vScale = Get_MatrixScale();

	_float3 vLook, vRight, vUp;

	vLook = (vPos - vCameraPos).Get_Nomalize() * vScale.z;

	if (vLook.Get_Nomalize() == _float3(0, 1, 0))
	{
		MSGBOX("����ȭ �� ���Ͱ� _float3(0,1,0)�� ��� ������ �Ұ����մϴ�.");
	}
	else
		vRight = _float3(0, 1, 0).Get_Cross(vLook).Get_Nomalize() * vScale.x;

	vUp = vLook.Get_Cross(vRight).Get_Nomalize() * vScale.y;


	_Matrix LookCameraStateMatrix
	{
		vRight.x,	vRight.y,	vRight.z,	0,
		vUp.x,		vUp.y,		vUp.z,		0,
		vLook.x,	vLook.y,	vLook.z,	0,
		vPos.x- m_TransforDesc.vPivot.x,		vPos.y - m_TransforDesc.vPivot.y,		vPos.z - m_TransforDesc.vPivot.z,		1
	};



	m_pGraphicDevice->SetTransform(D3DTS_WORLD, &LookCameraStateMatrix);

	return S_OK;
}

HRESULT CTransform::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK; 
}

HRESULT CTransform::Initialize_Clone(void * pArg)
{
	if (FAILED( __super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
		memcpy(&m_TransforDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{

	CTransform* pInstance = new CTransform(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg))) {
		MSGBOX("Failed to Create Transform Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform* pInstance = new CTransform((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create Transform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
