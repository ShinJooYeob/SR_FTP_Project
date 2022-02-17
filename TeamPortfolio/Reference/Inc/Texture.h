#pragma once

#include "Component.h"

BEGIN(Engine)

class CTextureLayer;

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_DEFAULT, TYPE_CUBEMAP, TYPE_END };
	
	typedef struct tagTextureDesc
	{
		TYPE			eTextureType = TYPE_DEFAULT;
		const _tchar*	szTextFilePath = nullptr;

	}TEXTUREDESC;


public:
	virtual HRESULT Initialize_Prototype(void * pArg) override;
	virtual HRESULT Initialize_Clone(void * pArg) override;

private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	HRESULT Change_TextureLayer(const _tchar* tagTexureLayer);
	HRESULT Bind_Texture_AutoFrame(_float fTimeDelta);
	HRESULT Bind_Texture(_uint iTextureIndex = 0);
	HRESULT ClearTexture();

private:
	//얘는 다른 클론들과 공유 X
	CTextureLayer*								m_pBindedTextureLayer = nullptr;

	//얘는 다른 클론들과 공유 0
	map<wstring, CTextureLayer*>			m_mapTextureLayers;
	typedef	map<wstring, CTextureLayer*>  TEXTURELAYERS;


	_float										m_fFrameTime = 0;
	_uint										m_iNumMaxTexture = 0;

private:
	HRESULT Read_TextFile(TYPE eTextureType, const _tchar* szFilePath);
	HRESULT Insert_TextureLayer(TYPE eType, _tchar* szFilePath, _tchar* szStateKey, _int iNumTextureCount);





public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDevice, TEXTUREDESC* pArg);
	virtual CComponent* Clone(void* pArg = nullptr)override;
	virtual void  Free() override;
};

END