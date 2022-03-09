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
	

	if (GetSingle(CGameInstance)->Get_DIMouseButtonState(Engine::CInput_Device::MBS_LBUTTON) & DIS_Press)
	{
		GetSingle(CGameInstance)->PlaySound(L"beep.mp3", CHANNEL_UI);
		m_eMouseType = MOUSETYPEID::MOUSE_CLICK;
		//m_ComTexture->Change_TextureLayer(L"cursor_clicker");

		m_fFrame = 1;
	}

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	Set_UI_Transform(m_ComTransform, _float4((_float)ptMouse.x, (_float)ptMouse.y, 45.f, 45.f));

	if (GetSingle(CGameInstance)->Get_DIMouseMoveState(CInput_Device::MMS_X) || GetSingle(CGameInstance)->Get_DIMouseMoveState(CInput_Device::MMS_Y))
	{
		PARTICLEDESC tDesc;
		//��ƼŬ�� �𳯸��� ���� ����
		tDesc.eParticleID = Particle_Ball;

		//�� ��ƼŬ�� ���ʵ��� �𳯸� ������ ����
		tDesc.TotalParticleTime = 0.1f;

		//��ƼŬ �ϳ� �ϳ��� ���ʵ��� �𳯸� ������ ����
		tDesc.EachParticleLifeTime = 0.1f;

		//��ƼŬ�� ����� ����
		tDesc.ParticleSize = _float3(15.f, 15.f, 15.f);
		//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ����
		tDesc.Particle_Power = 30;
		//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ���� ������ ����
		tDesc.PowerRandomRange = _float2(0.8f, 1.f);
		//��ƼŬ�� �ѹ��� �ִ� ����� ���� ������ ����
		tDesc.MaxParticleCount = 1;

		//��ƼŬ �ؽ�ó ������Ʈ �̸��� ���� (�⺻������ �ڱ� ���� ������Ʈ�� �ִ��� �˻��ϰ� ����ƽ���ִ����� �˻���)
		tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_UI_Result_Texture");
		//��ƼŬ �ؽ�ó ���̾� ������ƮŰ�� ������ �� ����
		tDesc.szTextureLayerTag = TEXT("Object_Star");
		//�ؽ�ó ������������ ����� ������ �� ������ ����
		tDesc.m_bIsTextureAutoFrame = false;


		//FixedTarget �� ����ϸ� ������ ��ġ���� ����ؼ� ������
		//FollowingTarget�� ����ϸ� �ش� ������Ʈ�� ����ٴϸ鼭 ��ƼŬ�� �𳯷���
		//�� ���� �ϳ��� ��� ����
		//�Ѵ� ����ϰ� ���� ��쿡�� ��ƼŬ�� 2�� ���� ����� ��
		//FollowingTarget�� ��� ����ٳ���� ������Ʈ�� CTransform ������Ʈ�� �־��ָ� ��
		tDesc.FollowingTarget = m_ComTransform;
		//tDesc.FixedTarget = _float3(1000,600,0);


		//��ƼŬ�� �ִ� ��Ż ����(range)�� ������ �� 
		//FollowingTarget �̳� FixedTarget �� ��ǥ �������� �ش� ����(+, -)�� ����� ����
		tDesc.MaxBoundary = _float3(5, 5, 5);


		//�ؽ�ó�� ������ ������ �� �ִ� ��� �¿���
		//���� true�� ����� ��� �ؽ�ó�� ���� ������ ���õǰ� Ÿ�� �������� ��¦��¦ �Ÿ��� ������
		//true�� ����� ��� �ݵ�� Ÿ�� �÷��� ������ �� ��
		tDesc.ParticleColorChage = true;
		tDesc.TargetColor = _float3(120, 30, 80);
		tDesc.TargetColor2 = _float3(255.f, 255.f, 255.f);


		//���� UI�� �׷������Ѵٸ� true ���忡 �׷������Ѵٸ� false �� ������ ��
		//UI �� �׸��� �� ��� ���� ��� ��ǥ�� API ��ǥ �������� ������ ��
		//World�� �׸��� �� ��� ���� ��� ��ǥ�� ���� ��ǥ �������� ������ ��
		tDesc.m_bIsUI = true;
		//UI Depth ���� (UI �������°� ���� �����Ҷ� ���� ����)
		tDesc.m_bUIDepth = -9998.f;

		//������ �����ϰ� ���� �� ����ϴ� �ɼ�
		//ex) straight�� ����ϴµ� ������Ʈ�� ���������� �Ѹ��� ������ ������Ʈ�� right�� �־��ָ� ��
		//Ȥ�� x���� ���� �������� �Ѹ��� ������ _float3(1,0,0); �̷������� �־��ָ� ��;

		//tDesc.vUp = _float3(1, 1, 0);


		//Create_ParticleObject�� ȣ���Ͽ� �������� ���̵�� ���ݱ��� ������ desc�� �־��ָ� ��
		GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	}

	if (GetSingle(CGameInstance)->Get_DIMouseButtonState(Engine::CInput_Device::MBS_LBUTTON) & DIS_Down) {
		m_ComTexture->Change_TextureLayer_Wait(L"cursor_clicker", 12.f);
	}
	if (GetSingle(CGameInstance)->Get_DIMouseButtonState(Engine::CInput_Device::MBS_LBUTTON) & DIS_Up) {
		m_ComTexture->Change_TextureLayer(L"cursor_pointer");
		PARTICLEDESC tDesc;
		tDesc.eParticleID = Particle_Fixed;
		tDesc.TotalParticleTime = 0.5f;
		tDesc.EachParticleLifeTime = 0.5f;
		tDesc.ParticleSize = _float3(45.f, 45.f, 45.f);
		tDesc.Particle_Power = 0.0001f;
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
	//Sour => ���� �׸������ϴ� �׸��� ��
	//Dest => �������� ȭ�鿡 �׷��� ��
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





	//������ �׷쿡 �־��ִ� ��Ȱ
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
