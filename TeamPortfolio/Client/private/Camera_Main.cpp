#include "stdafx.h"
#include "..\Public\Camera_Main.h"
#include "GameInstance.h"
#include "Player.h"
#include "Inventory.h"
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
	case CCamera_Main::CAM_EFT_ACTION:
		pCamemra->CamAction(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;

	case CCamera_Main::CAM_EFT_VICTORY:
		pCamemra->CamViectoryEft(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
		
	default:
		MSGBOX("worng Cam Eft");
		break;
	}

	return 0;
}

CCamera_Main::CCamera_Main(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CCamera(pGraphicDevice), m_ActionTargetPos(nullptr)
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
	if (FAILED(SetUp_DefaultLookAtAxis(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CCamera_Main::Update(_float fDeltaTime)
{

	if (FAILED(Input_Keyboard(fDeltaTime)))
		return E_FAIL;

	__super::Update(fDeltaTime);


	return _int();
}

_int CCamera_Main::LateUpdate(_float fDeltaTime)
{	
	//렌더링 그룹에 넣어주는 역활

	CGameInstance* pInstace = GetSingle(CGameInstance);
	
	
	CInventory* temp=(CInventory*)pInstace->Get_Commponent_By_LayerIndex(SCENE_STATIC, TEXT("Layer_Player"), TEXT("Com_Inventory"), 0);
	
	if(temp!=nullptr&&(temp->Get_Skill_Level(SKILL_CAMERA))&&pInstace->Get_DIKeyState(DIK_TAB) & DIS_Press)
	{
		CPlayer* Player = (CPlayer*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENEID::SCENE_STATIC, TAG_LAY(LAYERID::Layer_Player), 0));
		/*if (Player->Get_CoolDownStart(SKILL_CAMERA) == false)
			m_bMinimapStart = true;*/
		if (pInstace->Get_DIKeyState(DIK_TAB) & DIS_Down)
		{
			m_MapOpenTime = 0;
			if (Player->Get_CoolDownStart(SKILL_CAMERA) == false)
			{
				m_bMinimapStart = true;
				Player->Set_CoolDownStart_True(SKILL_CAMERA);
			}
			else
				m_bMinimapStart = false;
		}
		if(m_bMinimapStart)
		{
			if (FAILED(m_ComRenderer->Add_MainCamemra(this)))
				return E_FAIL;

			if (FAILED(m_MiniMapUI->LateUpdate(fDeltaTime)))
				return E_FAIL;

			m_MapOpenTime += fDeltaTime;
			_float EsaingValue = pInstace->Easing(TYPE_BounceOut, 0, (_float)g_iWinCY, m_MapOpenTime, 2.f);

			if (m_MapOpenTime > 2.f)
				EsaingValue = g_iWinCY;

			if (FAILED(m_MiniMapUI->Reset_MiniMapSize({ EsaingValue ,EsaingValue })))
				return E_FAIL;
		}
		
	}

	if (pInstace->Get_DIKeyState(DIK_M) & DIS_Down) {
		Change_Camera_Demension();
	}


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


	if(FAILED(m_pGraphicDevice->SetTransform(D3DTS_WORLD, &m_CamEffectMatricx)))
		return E_FAIL;


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

HRESULT CCamera_Main::ReInitialize(_float3* ActionPointArr, _uint iArrSize)
{
	if (ActionPointArr == nullptr)
		return E_FAIL;

	Safe_Delete_Array(m_ActionTargetPos);

	m_ActionTargetPos = new _float3[iArrSize + 1];

	m_ActionTargetPos[0] = m_pTransform->Get_MatrixState(CTransform::STATE_POS);

	memcpy(&(m_ActionTargetPos[1]), ActionPointArr, sizeof(_float3)*iArrSize);
	m_iActionArrSize = iArrSize + 1;

	m_VictoryTurnAxis = NOT_EXIST_BLOCK;
	m_fOrthoZoomInOut = 16.f;

	return S_OK;
}





void CCamera_Main::CameraEffect(CameraEffectID eEffect,_float fTimeDelta, _float fTotalFrame)
{
	if (m_eEffectID != CAM_EFT_VICTORY) {

		if (m_eEffectID != CAM_EFT_END || eEffect >= CAM_EFT_END)
			return;
	}

	m_eEffectID = eEffect;
	m_fTimeDelta = fTimeDelta;
	m_fTotalEftFrame = fTotalFrame;
	GetSingle(CGameInstance)->PlayThread(CameraEffectThread, this);
	
}

HRESULT CCamera_Main::Revolution_Turn_AxisY_CW(_float3 vRevPos, _float fTimeDelta)
{

	m_fPassedTime += fTimeDelta;
	_float3 vOriginCameraPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
	vRevPos.y = 0;
	_float3 vCameraPos = vOriginCameraPos - vRevPos;
	
	vCameraPos.y = 0;

	_float fDegreeAngle = GetSingle(CGameInstance)->Easing(TYPE_ExpoInOut, m_fStartAngle, m_fTargetAngle, m_fPassedTime);

	if (m_fPassedTime >= 1.f) 
	{
		m_IsTurning = false;
		fDegreeAngle = m_fTargetAngle;
		if (m_fTargetAngle < 0)
			m_eLoookState = _uint((_int((m_fTargetAngle + 360) / 90)) % 4);
		else
			m_eLoookState = _uint((_int((m_fTargetAngle) / 90)) % 4);
	}

	_float fDist = vCameraPos.Get_Distance(_float3(0,0,0));

	vCameraPos.x = cosf(D3DXToRadian(fDegreeAngle)) * fDist;
	vCameraPos.z = sinf(D3DXToRadian(fDegreeAngle)) * fDist;

	vCameraPos.y = vOriginCameraPos.y;
	vCameraPos += vRevPos;

	m_pTransform->Set_MatrixState(CTransform::STATE_POS, vCameraPos);
	vRevPos.y = vCameraPos.y;
	m_pTransform->LookAt(vRevPos);

	
	return S_OK;


}

HRESULT CCamera_Main::Revolution_Turn_AxisY_CCW(_float3 vRevPos, _float fTimeDelta)
{
	if (FAILED(Revolution_Turn_AxisY_CW(vRevPos, -fTimeDelta)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CCamera_Main::Reset_LookAtAxis(void * pArg, CameraLookStateID LookDir)
{
	if (pArg == nullptr && m_pGraphicDevice == nullptr && m_pTransform == nullptr)
		return E_FAIL;


	memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	m_eLoookState = LookDir;

	m_pTransform->Set_TransformDesc(m_CameraDesc.TransformDesc);


	m_CameraDesc.vEye = m_CameraDesc.vWorldRotAxis;

	switch (m_eLoookState)
	{
	case Client::CCamera_Main::Look_Left_Axis:
		m_CameraDesc.vEye.x += 20;
		break;
	case Client::CCamera_Main::Look_Back_Axis:
		m_CameraDesc.vEye.z += 20;
		break;
	case Client::CCamera_Main::Look_Right_Axis:
		m_CameraDesc.vEye.x -= 20;
		break;
	case Client::CCamera_Main::Look_Front_Axis:
		m_CameraDesc.vEye.z -= 20;
		break;
	default:
		break;
	}

	m_CameraDesc.vAt = m_CameraDesc.vWorldRotAxis;

	_float3 vRight, vUp, vLook;

	vLook = (m_CameraDesc.vAt - m_CameraDesc.vEye).Get_Nomalize();

	vRight = m_CameraDesc.vAxisY.Get_Cross(vLook).Get_Nomalize();

	vUp = vLook.Get_Cross(vRight).Get_Nomalize();

	m_pTransform->Set_MatrixState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_MatrixState(CTransform::STATE_UP, vUp);
	m_pTransform->Set_MatrixState(CTransform::STATE_LOOK, vLook);
	m_pTransform->Set_MatrixState(CTransform::STATE_POS, m_CameraDesc.vEye);


	//m_eLoookState = CameraLookStateID::Look_Front_Axis;






	return S_OK;
}

void CCamera_Main::FadeIn(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{

	m_ComTexture->Change_TextureLayer(TEXT("blank"));
	CGameInstance* pInstance = GetSingle(CGameInstance);
	m_ARGB[0] = 0;
	m_ARGB[1] = 0;
	m_ARGB[2] = 0;
	m_ARGB[3] = 0;

	_float fPassedTime = 0;
	_uint  interpolationValue = 0;

	DWORD  NowTick = GetTickCount();
	DWORD  OldTick = NowTick;

	while (true)
	{
		if (*_IsClientQuit == true)
			return;

		NowTick = GetTickCount();

		if ((NowTick - OldTick) < g_fDeltaTime * 1000)
			continue;
		fPassedTime += (NowTick - OldTick)*0.001f;
		OldTick = GetTickCount();

		interpolationValue = (_uint)pInstance->Easing(0, 0, 255.f, fPassedTime, m_fTotalEftFrame);

		if (fPassedTime >= m_fTotalEftFrame)
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
	m_ComTexture->Change_TextureLayer(TEXT("blank"));
	CGameInstance* pInstance = GetSingle(CGameInstance);
	m_ARGB[0] = 255;
	m_ARGB[1] = 0;
	m_ARGB[2] = 0;
	m_ARGB[3] = 0;

	_float fPassedTime = 0;
	_uint  interpolationValue = 0;

	DWORD  NowTick = GetTickCount();
	DWORD  OldTick = NowTick;

	while (true)
	{
		if (*_IsClientQuit == true)
			return;

		NowTick = GetTickCount();

		if ((NowTick - OldTick) < g_fDeltaTime * 1000)
			continue;
		fPassedTime += (NowTick - OldTick)*0.001f;
		OldTick = GetTickCount();

		interpolationValue = (_uint)pInstance->Easing(0,255.f, 0, fPassedTime, m_fTotalEftFrame);


		if (fPassedTime >= m_fTotalEftFrame)
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
	Camera_Shaking(g_fDeltaTime, m_fTotalEftFrame);

	EnterCriticalSection(_CriSec);
	m_eEffectID = CCamera_Main::CAM_EFT_END;
	LeaveCriticalSection(_CriSec);
	return;
}

void CCamera_Main::HitEft(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	EnterCriticalSection(_CriSec);
	m_ComTexture->Change_TextureLayer(TEXT("shadow"));
	LeaveCriticalSection(_CriSec);

	Camera_Shaking(g_fDeltaTime, m_fTotalEftFrame);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	m_ARGB[0] = 120;
	m_ARGB[1] = 255;
	m_ARGB[2] = 120;
	m_ARGB[3] = 120;

	_float fPassedTime = 0;
	_uint  interpolationValue = 0;

	DWORD  NowTick = GetTickCount();
	DWORD  OldTick = NowTick;

	while (true)
	{
		if (*_IsClientQuit == true)
			return;

		NowTick = GetTickCount();

		if ((NowTick - OldTick) < g_fDeltaTime * 1000)
			continue;
		fPassedTime += (NowTick - OldTick)*0.001f;
		OldTick = GetTickCount();



		interpolationValue = (_uint)pInstance->Easing(0, 120.f, 0, fPassedTime, m_fTotalEftFrame);


		if (fPassedTime >= m_fTotalEftFrame)
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

void CCamera_Main::CamAction(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	if (m_ActionTargetPos == nullptr)
	{
		EnterCriticalSection(_CriSec);
		m_eEffectID = CCamera_Main::CAM_EFT_END;
		LeaveCriticalSection(_CriSec);
		return;
	}

	_float fPassedTime  = 0; 
	m_ActionTargetPos[0] = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
	DWORD SleepTime = DWORD(m_fTimeDelta * 1000);
	CGameInstance* pInstance = GetSingle(CGameInstance);


	for (_uint i = 0; i < m_iActionArrSize - 1; i++)
	{
		fPassedTime = 0;
		while (fPassedTime < 1.5f)
		{
			Sleep(DWORD(g_fDeltaTime * 1000));
			if (*_IsClientQuit == true)
				return;
			fPassedTime += m_fTimeDelta;

			if (fPassedTime < 1.f)
			{
				_float3 vRenewPos;
				vRenewPos.x = pInstance->Easing(TYPE_ExpoInOut, m_ActionTargetPos[i].x, m_ActionTargetPos[i + 1].x, fPassedTime, 1);
				vRenewPos.y = pInstance->Easing(TYPE_ExpoInOut, m_ActionTargetPos[i].y, m_ActionTargetPos[i + 1].y, fPassedTime, 1);
				vRenewPos.z = pInstance->Easing(TYPE_ExpoInOut, m_ActionTargetPos[i].z, m_ActionTargetPos[i + 1].z, fPassedTime, 1);

				m_pTransform->Set_MatrixState(CTransform::STATE_POS, vRenewPos);

			}

		}
	}


	fPassedTime = 0;
	while (fPassedTime < 1.f)
	{
		if (*_IsClientQuit == true)
			return;
		Sleep(SleepTime);
		fPassedTime += m_fTimeDelta;


		_float3 vRenewPos;
		vRenewPos.x = pInstance->Easing(TYPE_ExpoInOut, m_ActionTargetPos[m_iActionArrSize - 1].x, m_ActionTargetPos[0].x, fPassedTime, 1);
		vRenewPos.y = pInstance->Easing(TYPE_ExpoInOut, m_ActionTargetPos[m_iActionArrSize - 1].y, m_ActionTargetPos[0].y, fPassedTime, 1);
		vRenewPos.z = pInstance->Easing(TYPE_ExpoInOut, m_ActionTargetPos[m_iActionArrSize - 1].z, m_ActionTargetPos[0].z, fPassedTime, 1);

		m_pTransform->Set_MatrixState(CTransform::STATE_POS, vRenewPos);



	}





	m_pTransform->Set_MatrixState(CTransform::STATE_POS, m_ActionTargetPos[0]);

	EnterCriticalSection(_CriSec);
	m_eEffectID = CCamera_Main::CAM_EFT_END;
	LeaveCriticalSection(_CriSec);

}

void CCamera_Main::CamViectoryEft(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	if (m_VictoryTurnAxis == NOT_EXIST_BLOCK || m_vCamPivot == NOT_EXIST_BLOCK)
	{
		EnterCriticalSection(_CriSec);
		m_eEffectID = CCamera_Main::CAM_EFT_END;
		LeaveCriticalSection(_CriSec);
		return;
	}

	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (abs(m_vCamPivot.x) < abs(m_vCamPivot.y) && abs(m_vCamPivot.z) < abs(m_vCamPivot.y))
		m_vCamPivot.y = 0;
	else if (abs(m_vCamPivot.x) < abs(m_vCamPivot.z) && abs(m_vCamPivot.y) < abs(m_vCamPivot.z))
		m_vCamPivot.z = 0;
	else if (abs(m_vCamPivot.z) < abs(m_vCamPivot.x) && abs(m_vCamPivot.y) < abs(m_vCamPivot.x))
		m_vCamPivot.y = 0;

	//m_vCamPivot = m_vCamPivot.Get_Nomalize() * 3.f;
	_float3 vRevPos = m_VictoryTurnAxis;
	_float3 vOriCameraPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
	_float3 vOriCameraLookAtPos = vOriCameraPos + m_pTransform->Get_MatrixState(CTransform::STATE_LOOK);
	_float RevAxisY = vRevPos.y;
	vRevPos.y = vOriCameraPos.y = 0;


	_float fDist = vRevPos.Get_Distance(vOriCameraPos);


	_float3 RevToCamPos = vOriCameraPos - vRevPos;

	_float StartAngle = acosf(RevToCamPos.Get_Nomalize().Get_Dot(_float3(vRevPos + _float3(1, 0, 0)).Get_Nomalize()));

	StartAngle = D3DXToDegree(StartAngle);

	if (vOriCameraPos.z < vRevPos.z)
		StartAngle = 360.f - StartAngle;

	vOriCameraPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
	_float TargetAngle = StartAngle + 1080;
	_float fPassedTime = 0;



	DWORD  NowTick = GetTickCount();
	DWORD  OldTick = NowTick;

	while (true)
	{
		if (*_IsClientQuit == true)
			return;

		NowTick = GetTickCount();

		if((NowTick - OldTick) < g_fDeltaTime * 1000)
			continue;
		fPassedTime += (NowTick - OldTick)*0.001f;
		OldTick = GetTickCount();



		_float fDegreeAngle = pInstance->Easing(TYPE_Linear, StartAngle, TargetAngle, fPassedTime, m_fTotalEftFrame);

		if (fPassedTime >= m_fTotalEftFrame)
		{
			fDegreeAngle = TargetAngle;
			fPassedTime = m_fTotalEftFrame;
			_float3 vNewCamPos;

			vNewCamPos.x = cosf(D3DXToRadian(fDegreeAngle))*fDist + vRevPos.x;
			vNewCamPos.z = sinf(D3DXToRadian(fDegreeAngle))*fDist + vRevPos.z;
			vNewCamPos.y = RevAxisY + (m_vCamPivot * (1 - fPassedTime / m_fTotalEftFrame)).y;


			m_pTransform->Set_MatrixState(CTransform::STATE_POS, vNewCamPos);
			m_pTransform->LookAt(vRevPos + m_vCamPivot * (1 - fPassedTime / m_fTotalEftFrame) + _float3(0, RevAxisY,0));
			m_fOrthoZoomInOut = 3.f;
			break;
		}


		_float3 vNewCamPos;

		vNewCamPos.x = cosf(D3DXToRadian(fDegreeAngle))*fDist + vRevPos.x;
		vNewCamPos.z = sinf(D3DXToRadian(fDegreeAngle))*fDist + vRevPos.z;
		vNewCamPos.y = RevAxisY + (m_vCamPivot * (1 - fPassedTime / m_fTotalEftFrame)).y;


		m_pTransform->Set_MatrixState(CTransform::STATE_POS, vNewCamPos);

		m_pTransform->LookAt(vRevPos + m_vCamPivot * (1- fPassedTime / m_fTotalEftFrame) + _float3(0, RevAxisY, 0));
		m_fOrthoZoomInOut = 3.f + 13.f *  (1 - fPassedTime / m_fTotalEftFrame);

	}

	EnterCriticalSection(_CriSec);
	m_eEffectID = CCamera_Main::CAM_EFT_END;
	LeaveCriticalSection(_CriSec);

}

HRESULT CCamera_Main::SetUp_DefaultLookAtAxis(void* pArg)
{

	if (pArg != nullptr)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	if (m_pGraphicDevice == nullptr)
		return E_FAIL;

	m_pTransform = CTransform::Create(m_pGraphicDevice);

	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pTransform->Set_TransformDesc(m_CameraDesc.TransformDesc);


	m_CameraDesc.vEye = m_CameraDesc.vWorldRotAxis;
	m_CameraDesc.vEye.z -= 20;
	m_CameraDesc.vAt = m_CameraDesc.vWorldRotAxis;

	_float3 vRight, vUp, vLook;

	vLook = (m_CameraDesc.vAt - m_CameraDesc.vEye).Get_Nomalize();

	vRight = m_CameraDesc.vAxisY.Get_Cross(vLook).Get_Nomalize();

	vUp = vLook.Get_Cross(vRight).Get_Nomalize();

	m_pTransform->Set_MatrixState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_MatrixState(CTransform::STATE_UP, vUp);
	m_pTransform->Set_MatrixState(CTransform::STATE_LOOK, vLook);
	m_pTransform->Set_MatrixState(CTransform::STATE_POS, m_CameraDesc.vEye);


	m_eLoookState = CameraLookStateID::Look_Front_Axis;




	return S_OK;
}

HRESULT CCamera_Main::SetUp_Components()
{

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Blank), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	m_MiniMapUI = CMiniMapUI::Create(m_pGraphicDevice, &_float4(g_iWinCX - 100.f, 100.f, 100.f,100.f));
	if (m_MiniMapUI == nullptr)
		return E_FAIL;

	//카메라 이팩트용 이미지 월드 세팅
	m_CamEffectMatricx =
	{
		(_float)g_iWinCX,	0,					0,					0,
		0,					(_float)g_iWinCY,	0,					0,
		0,					0,					1,					0,
		0,					0,					0,					1,
	};













	//m_ParticleDesc.eParticleID = Particle_Fixed;
	//m_ParticleDesc.TotalParticleTime = 0.f;
	//m_ParticleDesc.EachParticleLifeTime = 1000;
	//m_ParticleDesc.ParticleSize = _float3(3.f, 3.f, 3.f);
	//m_ParticleDesc.Particle_Power = 1;
	//m_ParticleDesc.PowerRandomRange = _float2(0.5f, 1.5f);
	//m_ParticleDesc.MaxParticleCount = 1;
	//m_ParticleDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	//m_ParticleDesc.szTextureLayerTag = TEXT("Turning_Effect");
	//m_ParticleDesc.m_bIsTextureAutoFrame = true;
	//m_ParticleDesc.fAutoFrameMul = 3.f;
	//m_ParticleDesc.MaxBoundary = _float3(10, 10, 10);
	//m_ParticleDesc.ParticleColorChage = false;
	//m_ParticleDesc.TargetColor = _float3(237, 28, 36);
	//m_ParticleDesc.TargetColor2 = _float3(53, 255.f, 11);
	//m_ParticleDesc.m_bIsUI = false;
	//m_ParticleDesc.ParticleStartRandomPosMin = _float3(0.0f, 0.7f, 0.0f);
	//m_ParticleDesc.ParticleStartRandomPosMax = _float3(0.f, 0.7f, 0.0f);
	//m_ParticleDesc.MustDraw = true;
	//m_ParticleDesc.IsParticleFameEndtoDie = true;

	m_ParticleDesc.eParticleID = Particle_Fixed;
	m_ParticleDesc.TotalParticleTime = 0.f;
	m_ParticleDesc.EachParticleLifeTime = 1000;

	m_ParticleDesc.FixedTarget = _float3(g_iWinCX*0.5f, g_iWinCY*0.4f+10.f, 0.05);
	m_ParticleDesc.ParticleSize = _float3(g_iWinCX*1.3f, g_iWinCY*1.3f, 1.f);

	m_ParticleDesc.Particle_Power = 1;
	m_ParticleDesc.PowerRandomRange = _float2(0.5f, 1.5f);
	m_ParticleDesc.MaxParticleCount = 1;
	m_ParticleDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	m_ParticleDesc.szTextureLayerTag = TEXT("Turning_Effect_A");

	m_ParticleDesc.m_bIsTextureAutoFrame = true;
	m_ParticleDesc.fAutoFrameMul = 3.f;


	m_ParticleDesc.MaxBoundary = _float3(10, 10, 10);

	m_ParticleDesc.ParticleColorChage = false;
	m_ParticleDesc.TargetColor = _float3(237, 28, 36);
	m_ParticleDesc.TargetColor2 = _float3(53, 255.f, 11);

	m_ParticleDesc.m_bIsUI = true;
	m_ParticleDesc.m_bUIDepth = 200;

	m_ParticleDesc.ParticleStartRandomPosMin = _float3(0.0f, 0.f, 0.0f);
	m_ParticleDesc.ParticleStartRandomPosMax = _float3(0.f, 0.f, 0.0f);
	m_ParticleDesc.MustDraw = true;
	m_ParticleDesc.IsParticleFameEndtoDie = true;
	m_ParticleDesc.AlphaBlendON = true;





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

HRESULT CCamera_Main::Input_Keyboard(_float fDeltaTime)
{

	CGameInstance* pInstance = GetSingle(CGameInstance);

	

		//if (pInstance->Get_DIKeyState(DIK_1) & DIS_Down)
		//{
		//	CameraEffect(CCamera_Main::CAM_EFT_FADE_IN, fDeltaTime);
		//}
		//if (pInstance->Get_DIKeyState(DIK_2) & DIS_Down)
		//{
		//	CameraEffect(CCamera_Main::CAM_EFT_FADE_OUT, fDeltaTime);
		//}
		//if (pInstance->Get_DIKeyState(DIK_3) & DIS_Down)
		//{
		//	CameraEffect(CCamera_Main::CAM_EFT_SHAKE, fDeltaTime);
		//}
		//if (pInstance->Get_DIKeyState(DIK_4) & DIS_Down)
		//{
		//	CameraEffect(CCamera_Main::CAM_EFT_HIT, fDeltaTime);
		//}	
	if (pInstance->Get_DIKeyState(DIK_5) & DIS_Down)
	{
		CameraEffect(CCamera_Main::CAM_EFT_ACTION, fDeltaTime);
	}


	if (m_eNowSceneNum != SCENE_LOBY) 
	{

		if (!m_IsTurning && pInstance->Get_DIKeyState(DIK_E) & DIS_Down)
		{

			GetSingle(CQuest)->Set_QuestIndexIncrease(QUEST_1, 1);
			m_CameraDesc.vWorldRotAxis = _float3(0, 0, 16.f).PosVector_Matrix(m_pTransform->Get_WorldMatrix());


			m_fStartAngle = (m_eLoookState) * 90.f;
			m_fTargetAngle = (m_eLoookState + 1) * 90.f;

			m_fPassedTime = 0;
			m_IsTurning = true;



			//if (m_ParticleDesc.FollowingTarget == nullptr)
			//	m_ParticleDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_Commponent_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

			m_ParticleDesc.szTextureLayerTag = TEXT("Turning_Effect_A");
			GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_ParticleDesc);

			GetSingle(CGameInstance)->PlaySound(TEXT("JY_rotateleft.wav"), CHANNEL_UI);

		}

		if (!m_IsTurning && pInstance->Get_DIKeyState(DIK_Q) & DIS_Down)
		{
			GetSingle(CQuest)->Set_QuestIndexIncrease(QUEST_1, 1);
			m_CameraDesc.vWorldRotAxis = _float3(0, 0, 16.f).PosVector_Matrix(m_pTransform->Get_WorldMatrix());

			m_fStartAngle = (m_eLoookState) * 90.f;


			m_fTargetAngle = (m_eLoookState - 1) * 90.f;

			m_fPassedTime = 0;
			m_IsTurning = true;

			//if (m_ParticleDesc.FollowingTarget == nullptr)
			//	m_ParticleDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_Commponent_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

			m_ParticleDesc.szTextureLayerTag = TEXT("Turning_Effect_B");
			GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_ParticleDesc);

			GetSingle(CGameInstance)->PlaySound(TEXT("JY_rotateright.wav"), CHANNEL_UI);

		}
	}
	if (m_IsTurning) {

		Revolution_Turn_AxisY_CW(m_CameraDesc.vWorldRotAxis, fDeltaTime);

	}


	if (pInstance->Get_DIKeyState(DIK_W)& DIS_Press)
	{
		if (pInstance->Get_DIKeyState(DIK_W)& DIS_Down)
		{
			m_fZoomInTime = 0;
			GetSingle(CGameInstance)->PlaySound(TEXT("JY_trixellogoin.wav"), CHANNEL_UI);
		}
		else if (pInstance->Get_DIKeyState(DIK_W)& DIS_Up) 
		{

			m_fOrthoZoomInOut = 16.0f;
		}
		else {
			m_fZoomInTime += fDeltaTime;
			m_fOrthoZoomInOut = pInstance->Easing(TYPE_QuinticOut, 16, 3, m_fZoomInTime, 2.5f);


			if (m_fZoomInTime > 2.5f) 
			{
				m_fZoomInTime = 2.5f;
				m_fOrthoZoomInOut = 3.f;
			}
		}
	}




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

	Safe_Delete_Array(m_ActionTargetPos);
	Safe_Release(m_MiniMapUI);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComRenderer);
}
