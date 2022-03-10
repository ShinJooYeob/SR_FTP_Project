#pragma once
#include "Component.h"

BEGIN(Engine)

END


BEGIN(Client)

// ÃÑ¾ËÆÐÅÏÀ» ¸¸µå´Â ÃÑ ÄÄÆ÷³ÍÆ®
class CCom_Gun final: public CComponent
{
public:
	typedef struct tagGunDesc	
	{
		_uint mSceneID;

	}GUNDESC;
		

private:
	explicit CCom_Gun(LPDIRECT3DDEVICE9	pGraphicDevice);
	explicit CCom_Gun(const CCom_Gun& rhs);
	virtual	~CCom_Gun() = default;

public:
   // #TODO ÃÑ¾Ë ÆÐÅÏ¼³Á¤

	// Å¸°ÙÅº
	HRESULT CreateBullet_Target(_float3 startPos, _float3 targetPos, _float speed=1.f, E_BulletType_MOVE type = BULLETTYPE_MOVE_NOMAL);
	// ¹æÇâÅº
	HRESULT CreateBullet_Dir(_float3 startPos, _float3 moveidr, _float speed = 1.f,E_BulletType_MOVE type = BULLETTYPE_MOVE_NOMAL);

	// ¿øÇüÅº

	
	HRESULT DestoryBullet_All();


public:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);

private:
	tagGunDesc mDesc;
	


public:
	static CCom_Gun* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;

};
END
