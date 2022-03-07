#pragma once

#include "Scene.h"

BEGIN(Client)

class CScene_Tutorial final : public CScene
{
private:
	explicit CScene_Tutorial(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual ~CScene_Tutorial() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual HRESULT Scene_InGame_Chage(_bool Scene_Chage_Switch, _uint _INextScene)override;

private:
	HRESULT Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain(list<SPECIALCUBE*>* listdata);
	HRESULT Ready_Layer_PauseUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_PlayerStatusUI(const _tchar* pLayerTag);


	HRESULT Ready_Layer_UI_Start(const _tchar * pLayerTag);

	HRESULT Ready_Layer_UI_Result(const _tchar* pLayerTag);							//==>���� ���� ��ġ ��ǥ ����������� �͵�
	HRESULT Ready_Layer_Object_Star(const _tchar* pLayerTag);//�� ȹ��				//==>���� ���� ��ġ ��ǥ ����������� �͵�
	HRESULT Ready_Layer_StageEndCollsionObject(const _tchar* pLayerTag);			//==>���� ���� ��ġ ��ǥ ����������� �͵�
																					//HRESULT Ready_Layer_OrbitButton_And_Cube(const _tchar* pLayerTag);				//==>���ٸ� ���� ���� ��ġ ��ǥ ����������� �͵�

																					////////////// Ʃ�丮�� ����
	HRESULT Ready_Layer_OrbitButton_And_Cube(const _tchar* pLayerTag);				//==>���ٸ� ���� ���� ��ġ ��ǥ ����������� �͵�
	HRESULT Ready_Layer_GravityCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object_PortalCube(const _tchar* pLayerTag);
	/////////////


	HRESULT Ready_Layer_Object_BossEntry(const _tchar* pLayerTag);


	D3DLIGHT9		m_Light;

public:
	static CScene_Tutorial* Create(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual void Free() override;
};

END