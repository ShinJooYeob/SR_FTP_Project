#pragma once

#include "Component.h"

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_DEFAULT, TYPE_CUBEMAP, TYPE_END };

	typedef struct tagTextureDesc
	{

		TYPE			eTextureType = TYPE_DEFAULT;
		const _tchar*	szFilePath = nullptr;
		_uint			m_iNumTexture = 1;

	}TEXTUREDESC;

public:
	HRESULT Bind_Texture(_uint iTextureIndex = 0);

private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Protoype(void * pArg) override;
	virtual HRESULT Initialize_Clone(void * pArg) override;



	vector<LPDIRECT3DBASETEXTURE9>			m_vecTexture;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDevice, TEXTUREDESC* pArg);
	virtual CComponent* Clone(void* pArg = nullptr)override;
	virtual void  Free() override;
};

