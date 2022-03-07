#pragma once
#include "Component.h"

BEGIN(Engine)

END


BEGIN(Client)

// �Ѿ������� ����� �� ������Ʈ
class CCom_Gun final: public CComponent
{

private:
	explicit CCom_Gun(LPDIRECT3DDEVICE9	pGraphicDevice);
	explicit CCom_Gun(const CCom_Gun& rhs);
	virtual	~CCom_Gun() = default;

public:
	// �Ѿ� ���� ���� ���


public:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);


private:

	


public:
	static CCom_Gun* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;

};
END
