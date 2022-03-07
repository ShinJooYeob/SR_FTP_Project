#pragma once

#include "Base.h"

BEGIN(Engine)

class CTextureLayer;


class CTextureLayer final :public CBase
{

public:
	typedef struct tagTextureLayerDesc
	{
		_uint			eTextureType  = 0;
		_uint			iNumTexture = 1;
		const _tchar*	szFilePath = nullptr;
	}TEXTURELAYERDESC;


private:
	explicit CTextureLayer(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CTextureLayer() = default;


public:
	HRESULT Bind_Texture(_uint iTextureIndex = 0, _uint iStageIndex = 0);
	HRESULT Bind_ShaderTexture(class CShader * pShader, D3DXHANDLE param, _uint iTextureIndex=0);
	HRESULT ClearTexture();

	_uint Get_TextureNum() {return (_uint)(m_vecTexture.size() - 1);	}
	HRESULT Add_Another_Texture(TEXTURELAYERDESC* pArg);

private:
	vector<LPDIRECT3DBASETEXTURE9>			m_vecTexture;
	LPDIRECT3DDEVICE9						m_pGraphicDevice = nullptr;

private:
	HRESULT Initialize_Prototype(void * pArg);

public:
	static CTextureLayer* Create(LPDIRECT3DDEVICE9 pGraphicDevice, TEXTURELAYERDESC* pArg);
	virtual void  Free() override;
};

END
