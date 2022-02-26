#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "..\Public\FontMgr.h"

IMPLEMENT_SINGLETON(CFontMgr)


CFontMgr::CFontMgr()
{
}

HRESULT CFontMgr::Initialize_FontMgr(LPDIRECT3DDEVICE9 pGraphicDevice, const _tchar * szFonttxtFile, _float2 WindowSize)
{
	if (pGraphicDevice == nullptr || szFonttxtFile == nullptr)
		return E_FAIL;

	m_pGraphicDevice = pGraphicDevice;
	Safe_AddRef(m_pGraphicDevice);

	m_vWindowSize = WindowSize;

	CTexture::TEXTUREDESC TextureDesc{};
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = szFonttxtFile;

	m_texFont = CTexture::Create(m_pGraphicDevice, &TextureDesc);
	m_VIBuffer = CVIBuffer_Rect::Create(pGraphicDevice);

	return S_OK;
}

HRESULT CFontMgr::Render_Font(wstring szString, _float2 vOnWindowPos, _float2 vFontSize, _float3 Color_RGB, _uint UntilDrawIndex)
{

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 130);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_XRGB((_uint)Color_RGB.x, (_uint)Color_RGB.y, (_uint)Color_RGB.z));
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);

	_Matrix matTransform =
	{
		vFontSize.x,								0,													0,			0,
		0,											vFontSize.y,										0,			0,
		0,											0,													1,			0,
		//vOnWindowPos.x,								vOnWindowPos.y ,									0.f,		1
		vOnWindowPos.x - m_vWindowSize.x * 0.5f,	-vOnWindowPos.y + m_vWindowSize.y * 0.5f,			0.f,		1
	};

	const _tchar* pString	= szString.c_str();

	_uint UntilDraw = szString.length();

	if (UntilDrawIndex > 0)
		UntilDraw = UntilDrawIndex;

	for (_uint i = 0 ; i < UntilDrawIndex; i ++)
	{
		FAILED_CHECK(m_pGraphicDevice->SetTransform(D3DTS_WORLD, &matTransform));


		if (pString[i] == L'\n')
		{
			matTransform.m[3][0] = vOnWindowPos.x - m_vWindowSize.x * 0.5f;
			matTransform.m[3][1] = matTransform.m[3][1] - vFontSize.y;

			continue;
		}
		else if (pString[i]>= 97 && pString[i] <=122)//소문자
		{
			m_texFont->Change_TextureLayer(L"lower");

			FAILED_CHECK(m_texFont->Bind_Texture(pString[i] - 97));

			matTransform.m[3][0] = matTransform.m[3][0] + vFontSize.x;
			FAILED_CHECK(m_VIBuffer->Render());
			continue;
		}
		else if (pString[i] >= 48 && pString[i] <= 57)//숫자
		{
			m_texFont->Change_TextureLayer(L"number");

			FAILED_CHECK(m_texFont->Bind_Texture(pString[i] - 48));

			matTransform.m[3][0] = matTransform.m[3][0] + vFontSize.x;
			FAILED_CHECK(m_VIBuffer->Render());
			continue;
		}
		else if (pString[i] >= 65 && pString[i] <= 90)//대문자
		{
			m_texFont->Change_TextureLayer(L"upper");

			FAILED_CHECK(m_texFont->Bind_Texture(pString[i] - 65));

			matTransform.m[3][0] = matTransform.m[3][0] + vFontSize.x;
			FAILED_CHECK(m_VIBuffer->Render());
			continue;
		}
		else //나머지 문자는 그냥 띄워쓰기로 대채
		{
			matTransform.m[3][0] = matTransform.m[3][0] + vFontSize.x;
			continue;
		}
	}

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	return S_OK;
}

void CFontMgr::Free()
{
	Safe_Release(m_pGraphicDevice);
	
	Safe_Release(m_VIBuffer);
	Safe_Release(m_texFont);
}

