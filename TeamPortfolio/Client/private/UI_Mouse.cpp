#include "stdafx.h"
#include "..\public\UI_Mouse.h"





CUI_Mouse::CUI_Mouse(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_Mouse::CUI_Mouse(const CUI_Mouse & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Mouse::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Mouse::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	if (pArg != nullptr)
	{
		_float4 vUIDesc;
		vUIDesc = *(_float4*)pArg;
		m_rcRect.top = LONG(vUIDesc.y - vUIDesc.w *0.5f);
		m_rcRect.bottom = LONG(vUIDesc.y + vUIDesc.w *0.5f);
		m_rcRect.right = LONG(vUIDesc.x + vUIDesc.z*0.5f);
		m_rcRect.left = LONG(vUIDesc.x - vUIDesc.z*0.5f);
	}
	m_fDepth = -9999.f;
	m_eMouseType = MOUSETYPEID::MOUSE_DEFAULT;
	m_ComTexture->Change_TextureLayer(L"cursor_pointer");
	return S_OK;
}

_int CUI_Mouse::Update(_float fDeltaTime)
{
	
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	

	//if (GetSingle(CGameInstance)->Get_DIMouseButtonState(Engine::CInput_Device::MBS_LBUTTON) & DIS_Press)
	//{
	//	GetSingle(CGameInstance)->PlaySound(L"JW_beep1.wav", CHANNEL_UI,0.4f);
	//	m_eMouseType = MOUSETYPEID::MOUSE_CLICK;
	//	//m_ComTexture->Change_TextureLayer(L"cursor_clicker");

	//	m_fFrame = 1;
	//}

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	Set_UI_Transform(m_ComTransform, _float4((_float)ptMouse.x, (_float)ptMouse.y, 45.f, 45.f));

	if (GetSingle(CGameInstance)->Get_DIMouseMoveState(CInput_Device::MMS_X) || GetSingle(CGameInstance)->Get_DIMouseMoveState(CInput_Device::MMS_Y))
	{
		PARTICLEDESC tDesc;
		//파티클이 흩날리는 종류 설정
		tDesc.eParticleID = Particle_Ball;

		//총 파티클이 몇초동안 흩날릴 것인지 설정
		tDesc.TotalParticleTime = 0.1f;

		//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
		tDesc.EachParticleLifeTime = 0.1f;

		//파티클의 사이즈를 설정
		tDesc.ParticleSize = _float3(15.f, 15.f, 15.f);
		//파티클의 파워(이동속도)를 결정
		tDesc.Particle_Power = 30;
		//파티클의 파워(이동속도)의 랜덤 범위를 결정
		tDesc.PowerRandomRange = _float2(0.8f, 1.f);
		//파티클이 한번에 최대 몇개까지 보일 것인지 설정
		tDesc.MaxParticleCount = 1;

		//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
		tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_UI_Result_Texture");
		//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
		tDesc.szTextureLayerTag = TEXT("Object_Star");
		//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
		tDesc.m_bIsTextureAutoFrame = false;


		//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
		//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
		//단 둘중 하나만 사용 가능
		//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
		//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
		tDesc.FollowingTarget = m_ComTransform;
		//tDesc.FixedTarget = _float3(1000,600,0);


		//파티클의 최대 이탈 범위(range)를 설정해 줌 
		//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
		tDesc.MaxBoundary = _float3(5, 5, 5);


		//텍스처의 색상을 변경할 수 있는 기능 온오프
		//만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
		//true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
		tDesc.ParticleColorChage = true;
		tDesc.TargetColor = _float3(120, 30, 80);
		tDesc.TargetColor2 = _float3(255.f, 255.f, 255.f);


		//만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
		//UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
		//World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
		tDesc.m_bIsUI = true;
		//UI Depth 설정 (UI 가려지는거 순서 결정할때 쓰는 변수)
		tDesc.m_bUIDepth = -9998.f;

		//방향을 설정하고 싶을 때 사용하는 옵션
		//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
		//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;

		//tDesc.vUp = _float3(1, 1, 0);


		//Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
		GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	}

	if (GetSingle(CGameInstance)->Get_DIMouseButtonState(Engine::CInput_Device::MBS_LBUTTON) & DIS_Down) {
		m_ComTexture->Change_TextureLayer_Wait(L"cursor_clicker", 12.f);
	}
	if (GetSingle(CGameInstance)->Get_DIMouseButtonState(Engine::CInput_Device::MBS_LBUTTON) & DIS_Up) {
		GetSingle(CGameInstance)->PlaySound(L"JW_beep1.wav", CHANNEL_UI, 0.25f);
		m_ComTexture->Change_TextureLayer(L"cursor_pointer");
		PARTICLEDESC tDesc;
		tDesc.eParticleID = Particle_Fixed;
		tDesc.TotalParticleTime = 0.5f;
		tDesc.EachParticleLifeTime = 0.5f;
		tDesc.ParticleSize = _float3(45.f, 45.f, 45.f);
		tDesc.Particle_Power = 0.00001f;
		tDesc.PowerRandomRange = _float2(1.f, 1.f);
		tDesc.MaxParticleCount = 1;
		tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
		tDesc.szTextureLayerTag = TEXT("fire");
		tDesc.m_bIsTextureAutoFrame = true;
		/*tDesc.FollowingTarget = m_ComTransform;*/
		tDesc.FixedTarget = _float3((_float)ptMouse.x, (_float)ptMouse.y, 0.5f);
		tDesc.MaxBoundary = _float3(5, 5, 5);
		tDesc.ParticleColorChage = false;
		tDesc.TargetColor = _float3(174, 174, 0);
		tDesc.TargetColor2 = _float3(255.f, 255.f, 206.f);
		tDesc.m_bIsUI = true;
		tDesc.m_bUIDepth = -9998.5f;
		tDesc.fAutoFrameMul = 1.5f;
		GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);
	}

	return _int();

}

HRESULT CUI_Mouse::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//Sour => 현재 그리려고하는 그림의 색
	//Dest => 직전까지 화면에 그려진 색
	//
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(120, 255, 255, 255));
	//
	//
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 20);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	return S_OK;
}

HRESULT CUI_Mouse::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}


_int CUI_Mouse::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;
	return _int();
}

_int CUI_Mouse::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(g_fDeltaTime)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CUI_Mouse::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}


HRESULT CUI_Mouse::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Texture_Mouse"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;


	return S_OK;
}





CUI_Mouse * CUI_Mouse::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_Mouse* pInstance = new CUI_Mouse(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CShop_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_Mouse::Clone(void * pArg)
{
	CUI_Mouse* pInstance = new CUI_Mouse((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CShop_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_Mouse::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
