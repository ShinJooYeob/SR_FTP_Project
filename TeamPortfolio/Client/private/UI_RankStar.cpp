#include "stdafx.h"
#include "..\public\UI_RankStar.h"

CUI_RankStar::CUI_RankStar(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_RankStar::CUI_RankStar(const CUI_RankStar & rhs)
	: CUI(rhs)
{
}


HRESULT CUI_RankStar::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_RankStar::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		memcpy(&m_RankStarDesc, pArg, sizeof(RANKSTARDESC));
		vRect.x = m_RankStarDesc.WindowRectPos.x;
		vRect.y = m_RankStarDesc.WindowRectPos.y;
		vRect.z = 60 + m_RankStarDesc.WindowRectPos.x;
		vRect.w = 70 + m_RankStarDesc.WindowRectPos.y;
	}
	else
	{
		MSGBOX("Fail to Clone CUI_RankStar");
	}

	//vRect = { 230,655,230,690 };
	//1��° ����Ʈ,2��° ž,
	//x: ����Ʈ,y: ž, z: ����Ʈ,w: ����,

	//vRect = { 190,300,250,370 };
	//Set_UI_Transform(m_ComTransform, Set_byRectPos(vRect));

	//X������60
	//Y������70

	//m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(45.f, -15.f, 0.f));

	m_fDepth = -6666;
	//850
	//m_vUIDesc = _float4(g_iWinCX >> 1, g_iWinCY >> 1, g_iWinCX, g_iWinCY);
	//m_vUIDesc = _float4(WinPosX, WinPosY, WinCX, WinCY);

	//x = Ltop


	m_ComTexture->Change_TextureLayer(TEXT("RankStar"));

	//m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(280.f, 675.f, 0.f));

	//������ ���� ����
	//TempPos = 350;
	StartPosW = 350;
	StartPosZ = 800;
	TargetPosZ = 60 + m_RankStarDesc.WindowRectPos.x;
	TargetPosW = 70 + m_RankStarDesc.WindowRectPos.y;
	return S_OK;
}

_int CUI_RankStar::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	m_fFrame = fDeltaTime;

	_float TempW;
	_float TempZ;

	m_fTimer += fDeltaTime;
	if (m_RankStarDesc.Time < m_fTimer)
	{
		EasingTime += fDeltaTime;
		TempW = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, StartPosW, TargetPosW, EasingTime, 2.0f);
		TempZ = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, StartPosZ, TargetPosZ, EasingTime, 2.0f);

		//vRect.z = Temptest + m_RankStarDesc.WindowRectPos.x;
		if (EasingTime > 2)
		{
			TempW = TargetPosW;
			TempZ = TargetPosZ;
		}
		vRect.z = TempZ;
		vRect.w = TempW;
		Set_UI_Transform(m_ComTransform, Set_byRectPos(vRect));

	}

	//�̰ɷ� ������ ������ �������� �������� �Ұ�.
	//����ƽ����������
	//static _uint	temp = 350;
	//if (temp > 60)
	//{
	//	--temp;
	//}
	//vRect.z = temp + m_RankStarDesc.WindowRectPos.x;
	//vRect.w = temp+10 + m_RankStarDesc.WindowRectPos.y;
	//Set_UI_Transform(m_ComTransform, Set_byRectPos(vRect));


	return _int();
}

_int CUI_RankStar::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;
	return _int();
}

_int CUI_RankStar::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
		return E_FAIL;

	//if (FAILED(m_ComTexture->Bind_Texture(0)))
	//	return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CUI_RankStar::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_float4 CUI_RankStar::Set_byRectPos(_float4 tRect)
{
	_float4 Result = {};
	Result.z = tRect.z - tRect.x; //x�� ������
	Result.w = tRect.w - tRect.y; //y�� ������
	Result.x = tRect.x + Result.z * 0.5f; //x�� ��ǥ 
	Result.y = tRect.y + Result.w * 0.5f; //y�� ��ǥ


	return Result;
}

HRESULT CUI_RankStar::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	//m_eNowSceneNum = SCENEID::SCENE_LOADING;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_RankStar_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_RankStar::SetUp_RenderState()
{
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); //���ĺ����ϰڴ� ��
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//Sour => ���� �׸������ϴ� �׸��� ��
	//Dest => �������� ȭ�鿡 �׷��� ��
	//
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	return S_OK;
}

HRESULT CUI_RankStar::Release_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

CUI_RankStar * CUI_RankStar::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_RankStar* pInstance = new CUI_RankStar(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CUI_RankStar_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_RankStar::Clone(void * pArg)
{
	CUI_RankStar* pInstance = new CUI_RankStar((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CUI_RankStar_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_RankStar::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
