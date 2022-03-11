#pragma once

#include "Scene.h"

BEGIN(Client)
class CScene_StageSelect final : public CScene
{
private:
	explicit CScene_StageSelect(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual ~CScene_StageSelect() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	//은혁이가 건듬
	virtual HRESULT Scene_InGame_Chage(_bool Scene_Chage_Switch, _uint _INextScene)override;

private:
	HRESULT Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT Ready_Layer_NPC(const _tchar * pLayerTag);
	HRESULT Ready_Layer_UI_Complete(const _tchar * pLayerTag);
	HRESULT Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Shop(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Quest(const _tchar * pLayerTag);
	HRESULT Ready_Layer_UI_Common(const _tchar * pLayerTag);
	HRESULT Ready_Layer_UI_Result(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object_Star(const _tchar* pLayerTag);//별 획득
	void	Make_Particle();
	HRESULT Ready_Layer_Terrain(list<SPECIALCUBE*>* listdata);

	HRESULT Ready_Layer_StageEntryCollsionObject(const _tchar* pLayerTag);




	HRESULT Ready_Layer_GravityCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_JumpCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object_PortalCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_ElevetorCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_OrbitButton_And_Cube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object_VanishCube(const _tchar* pLayerTag);//사라지는 큐브
	HRESULT Ready_Layer_Object_AppearCube(const _tchar* pLayerTag);//나타나는 큐브
	HRESULT Ready_Layer_Object_QrcodeCube(const _tchar* pLayerTag);



private:
	D3DLIGHT9		m_Light;
	static _float3			m_StageReturnBlock;
public:
	static CScene_StageSelect* Create(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual void Free() override;
};
END
