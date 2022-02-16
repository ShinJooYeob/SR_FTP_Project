#include "stdafx.h"
#include "..\Public\Camera_Main.h"
#include "GameInstance.h"


_uint CALLBACK CameraEffectThread(void* _Prameter)
{
	THREADARG tThreadArg{};
	memcpy(&tThreadArg, _Prameter, sizeof(THREADARG));
	delete _Prameter;


	CCamera_Main* pCamemra = (CCamera_Main*)(tThreadArg.pArg);

	switch (pCamemra->Get_EffectID())
	{
	case CCamera_Main::CAM_EFT_FADE_IN:
		pCamemra->FadeIn(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case CCamera_Main::CAM_EFT_FADE_OUT:
		pCamemra->FadeOut(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case CCamera_Main::CAM_EFT_SHAKE:
		pCamemra->CamShake(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case CCamera_Main::CAM_EFT_HIT:
		pCamemra->HitEft(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;

	default:
		break;
	}

	return 0;
}

CCamera_Main::CCamera_Main(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CCamera(pGraphicDevice)
{
}

CCamera_Main::CCamera_Main(const CCamera_Main & rhs)
	:CCamera(rhs)
{
}
//1
HRESULT CCamera_Main::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCamera_Main::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CCamera_Main::Update(_float fDeltaTime)
{
	CGameInstance* pInstance = GetSingle(CGameInstance);



	if (pInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		m_pTransform->Move_Forward(fDeltaTime);
	}
	if (pInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		m_pTransform->Move_Backward(fDeltaTime);

	}
	if (pInstance->Get_DIKeyState(DIK_X) & DIS_Press)
	{
		m_pTransform->MovetoTarget(m_pTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.f, 0), fDeltaTime);
	}
	if (pInstance->Get_DIKeyState(DIK_Z) & DIS_Press)
	{
		m_pTransform->MovetoTarget(m_pTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.f, 0), -fDeltaTime);
	}

	if (pInstance->Get_DIKeyState(DIK_1) & DIS_Down)
	{
		CameraEffect(CCamera_Main::CAM_EFT_FADE_IN,fDeltaTime);
	}
	if (pInstance->Get_DIKeyState(DIK_2) & DIS_Down)
	{
		CameraEffect(CCamera_Main::CAM_EFT_FADE_OUT, fDeltaTime);
	}
	if (pInstance->Get_DIKeyState(DIK_3) & DIS_Down)
	{
		CameraEffect(CCamera_Main::CAM_EFT_SHAKE, fDeltaTime);
	}
	if (pInstance->Get_DIKeyState(DIK_4) & DIS_Down)
	{
		CameraEffect(CCamera_Main::CAM_EFT_HIT, fDeltaTime);
	}


	if (!m_IsTurning && pInstance->Get_DIKeyState(DIK_E) & DIS_Down)
	{

		_float3 vOriginCameraPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
		_float3 vRotAxis = m_CameraDesc.vWorldRotAxis;
		vRotAxis.y = 0;
		_float3 vCameraPos = vOriginCameraPos - vRotAxis;
		vCameraPos.y = 0;

		_float fRadianAngle = acosf(vCameraPos.Get_Nomalize().Get_Dot(_float3(1, 0, 0)));

		if (0 > vCameraPos.z)
			fRadianAngle = 2 * D3DX_PI - fRadianAngle;

		m_fStartAngle = fRadianAngle;
		m_fTargetAngle = fRadianAngle + D3DXToRadian(90);
		m_fPassedTime = 0;
		m_IsTurning = true;

	}
	
	if (!m_IsTurning && pInstance->Get_DIKeyState(DIK_Q) & DIS_Down)
	{
		_float3 vOriginCameraPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);		
		_float3 vRotAxis = m_CameraDesc.vWorldRotAxis;
		vRotAxis.y = 0;
		_float3 vCameraPos = vOriginCameraPos - vRotAxis;
		vCameraPos.y = 0;

		_float fRadianAngle = acosf(vCameraPos.Get_Nomalize().Get_Dot(_float3(1, 0, 0)));

		if (0 > vCameraPos.z)
			fRadianAngle = 2 * D3DX_PI - fRadianAngle;

		m_fStartAngle = fRadianAngle;
		m_fTargetAngle = fRadianAngle - D3DXToRadian(90);
		m_fPassedTime = 0;
		m_IsTurning = true;

	}
	if (m_IsTurning) {

		Revolution_Turn_AxisY_CW(m_CameraDesc.vWorldRotAxis, fDeltaTime);

	}



	if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_DoubleDown)
	{
		Change_Camera_Demension();
	}

	//_long MoveDist;

	//if (MoveDist = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
	//{
	//	m_pTransform->Turn_CW({0,1,0}, fDeltaTime * MoveDist * 0.1f);

	//}
	//if (MoveDist = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
	//{
	//	m_pTransform->Turn_CW(m_pTransform->Get_MatrixState(CTransform::STATE_RIGHT), fDeltaTime * MoveDist * 0.1f);
	//}


	__super::Update(fDeltaTime);


	return _int();
}

_int CCamera_Main::LateUpdate(_float fDeltaTime)
{	
	//렌더링 그룹에 넣어주는 역활

	if (m_eEffectID <= CCamera_Main::CAM_EFT_HIT )
	{
		if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return E_FAIL;
	}


	return _int();
}

_int CCamera_Main::Render()
{
	if (m_pGraphicDevice == nullptr)
		return E_FAIL;

	_float3 vCamPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
	_float3 vCamLook = m_pTransform->Get_MatrixState(CTransform::STATE_LOOK);
	_float3 vPos = vCamPos + vCamLook * (m_CameraDesc.fNear + 0.001f);
	

	_Matrix matUI = m_pTransform->Get_WorldMatrix();

	memcpy(&(matUI.m[3][0]), &vPos, sizeof(_float3));
	matUI.m[0][0] *= 30;
	matUI.m[0][1] *= 30;
	matUI.m[0][2] *= 30;
	matUI.m[1][0] *= 20;
	matUI.m[1][1] *= 20;
	matUI.m[1][2] *= 20;


	m_pGraphicDevice->SetTransform(D3DTS_WORLD, &matUI);


	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CCamera_Main::LateRender()
{


	return _int();
}

void CCamera_Main::CameraEffect(CameraEffectID eEffect,_float fTimeDelta)
{
	if (m_eEffectID != CAM_EFT_END || eEffect >= CAM_EFT_END)
		return;

	m_eEffectID = eEffect;
	m_fTimeDelta = fTimeDelta;
	GetSingle(CGameInstance)->PlayThread(CameraEffectThread, this);

}

HRESULT CCamera_Main::Revolution_Turn_AxisY_CW(_float3 vRevPos, _float fTimeDelta)
{

	m_fPassedTime += fTimeDelta;
	_float3 vOriginCameraPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
	vRevPos.y = 0;
	_float3 vCameraPos = vOriginCameraPos - vRevPos;
	
	vCameraPos.y = 0;

	_float fRadianAngle = GetSingle(CGameInstance)->Easing(21, m_fStartAngle, m_fTargetAngle, m_fPassedTime);

	if (m_fPassedTime >= 1.f) 
	{
		m_IsTurning = false;
		m_fStartAngle = m_fTargetAngle;
		fRadianAngle = m_fTargetAngle;
	}

	_float fDist = vCameraPos.Get_Distance(_float3(0,0,0));

	vCameraPos.x = cosf(fRadianAngle) * fDist;
	vCameraPos.z = sinf(fRadianAngle) * fDist;

	vCameraPos.y = vOriginCameraPos.y;
	vCameraPos += vRevPos;

	m_pTransform->Set_MatrixState(CTransform::STATE_POS, vCameraPos);
	vRevPos.y = vCameraPos.y;
	m_pTransform->LookAt(vRevPos);


	return S_OK;


	//_float fRadianAngle = acosf(vCameraPos.Get_Nomalize().Get_Dot(_float3(1,0,0)));
	//

	//if (0 > vCameraPos.z)
	//	fRadianAngle = 2 * D3DX_PI - fRadianAngle;

	//fRadianAngle += D3DXToRadian(90) * fTimeDelta;

}

HRESULT CCamera_Main::Revolution_Turn_AxisY_CCW(_float3 vRevPos, _float fTimeDelta)
{
	if (FAILED(Revolution_Turn_AxisY_CW(vRevPos, -fTimeDelta)))
		return E_FAIL;
	return S_OK;
}

void CCamera_Main::FadeIn(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{

	CGameInstance* pInstance = GetSingle(CGameInstance);
	m_ARGB[0] = 0;
	m_ARGB[1] = 0;
	m_ARGB[2] = 0;
	m_ARGB[3] = 0;

	_float fPassedTime = 0;
	_float fTotalFrameTime = 1.5;
	_uint  interpolationValue = 0;
	DWORD SleepTime = DWORD(m_fTimeDelta * 1000);

	while (true)
	{
		if (*_IsClientQuit == true)
			return;

		Sleep(SleepTime);
		fPassedTime += m_fTimeDelta;
		interpolationValue = (_uint)pInstance->Easing(0, 0, 255.f, fPassedTime, fTotalFrameTime);

		if (fPassedTime >= fTotalFrameTime)
		{
			interpolationValue = 255;
			EnterCriticalSection(_CriSec);
			m_ARGB[0] = interpolationValue;
			LeaveCriticalSection(_CriSec);
			break;
		}

		EnterCriticalSection(_CriSec);
		m_ARGB[0] = interpolationValue;
		LeaveCriticalSection(_CriSec);
	}


	m_eEffectID = CCamera_Main::CAM_EFT_END;
	return;
}

void CCamera_Main::FadeOut(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pInstance = GetSingle(CGameInstance);
	m_ARGB[0] = 255;
	m_ARGB[1] = 0;
	m_ARGB[2] = 0;
	m_ARGB[3] = 0;

	_float fPassedTime = 0;
	_float fTotalFrameTime = 1.5;
	_uint  interpolationValue = 0;
	DWORD SleepTime = DWORD(m_fTimeDelta * 1000);

	while (true)
	{
		if (*_IsClientQuit == true)
			return;

		Sleep(SleepTime);
		fPassedTime += m_fTimeDelta;
		interpolationValue = (_uint)pInstance->Easing(0,255.f, 0, fPassedTime, fTotalFrameTime);


		if (fPassedTime >= fTotalFrameTime)
		{
			interpolationValue = 0;
			EnterCriticalSection(_CriSec);
			m_ARGB[0] = interpolationValue;
			LeaveCriticalSection(_CriSec);
			break;
		}

		EnterCriticalSection(_CriSec);
		m_ARGB[0] = interpolationValue;
		LeaveCriticalSection(_CriSec);
	}


	EnterCriticalSection(_CriSec);
	m_eEffectID = CCamera_Main::CAM_EFT_END;
	LeaveCriticalSection(_CriSec);
	return;
}

void CCamera_Main::CamShake(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	Camera_Shaking(m_fTimeDelta);

	EnterCriticalSection(_CriSec);
	m_eEffectID = CCamera_Main::CAM_EFT_END;
	LeaveCriticalSection(_CriSec);
	return;
}

void CCamera_Main::HitEft(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{

	Camera_Shaking(m_fTimeDelta);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	m_ARGB[0] = 120;
	m_ARGB[1] = 255;
	m_ARGB[2] = 120;
	m_ARGB[3] = 120;

	_float fPassedTime = 0;
	_float fTotalFrameTime = 0.5;
	_uint  interpolationValue = 0;
	DWORD SleepTime = DWORD(m_fTimeDelta * 1000);

	while (true)
	{
		if (*_IsClientQuit == true)
			return;

		Sleep(SleepTime);
		fPassedTime += m_fTimeDelta;
		interpolationValue = (_uint)pInstance->Easing(0, 120.f, 0, fPassedTime, fTotalFrameTime);


		if (fPassedTime >= fTotalFrameTime)
		{
			interpolationValue = 0;
			EnterCriticalSection(_CriSec);
			m_ARGB[0] = interpolationValue;
			LeaveCriticalSection(_CriSec);
			break;
		}

		EnterCriticalSection(_CriSec);
		m_ARGB[0] = interpolationValue;
		LeaveCriticalSection(_CriSec);
	}

	EnterCriticalSection(_CriSec);
	m_eEffectID = CCamera_Main::CAM_EFT_END;
	LeaveCriticalSection(_CriSec);
	return;
}

HRESULT CCamera_Main::SetUp_Components()
{

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Blank), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Main::SetUp_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_ARGB[0], m_ARGB[1], m_ARGB[2], m_ARGB[3]));
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);

	return S_OK;
}

HRESULT CCamera_Main::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	return S_OK;
}

CCamera_Main * CCamera_Main::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CCamera_Main* pInstance = new CCamera_Main(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg))) {
		MSGBOX("Failed to Create CCamera_Main");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject * CCamera_Main::Clone(void * pArg)
{
	CCamera_Main* pInstance = new CCamera_Main(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg))) {
		MSGBOX("Failed to Create CCamera_Main");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCamera_Main::Free()
{
	__super::Free();
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComRenderer);
}
