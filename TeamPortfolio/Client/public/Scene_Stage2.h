#pragma once

#include "Scene.h"

BEGIN(Client)

class CScene_Stage2 final : public CScene
{
private:
	explicit CScene_Stage2(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual ~CScene_Stage2() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;


private:
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MainCamera(const _tchar* pLayerTag);


	HRESULT Ready_Layer_Cube(const _tchar* pLayerTag); //움직이는 큐브 상호작용 확인용
	HRESULT Ready_Layer_FixCube(const _tchar* pLayerTag); // 움직이지 않는 큐브 충돌 및 옮기기
	HRESULT Ready_Layer_PushCube(const _tchar* pLayerTag); //미는 큐브
	HRESULT Ready_Layer_GravityCube(const _tchar* pLayerTag); // 중력 큐브
	HRESULT Ready_Layer_SelfRotationCube(const _tchar* pLayerTag); // 스스로 자전하는 큐브

	HRESULT Ready_Layer_Object_ButtonCube(const _tchar* pLayerTag); //InteractiveCube와 상호작용하는 큐브로 변수명 변경 가능
	HRESULT Ready_Layer_Object_InteractiveCube(const _tchar* pLayerTag); //버튼과 상호작용하는 큐브

	HRESULT Ready_Layer_Object_PortalCube_A(const _tchar* pLayerTag); //포탈 A
	HRESULT Ready_Layer_Object_PortalCube_B(const _tchar* pLayerTag); //포탈 B

	//움직이는 큐브들
	HRESULT Ready_Layer_Object_RisingCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object_DescentCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object_LeftCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object_RightCube(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Object_VanishCube(const _tchar* pLayerTag);//사라지는 큐브
	HRESULT Ready_Layer_Object_AppearCube(const _tchar* pLayerTag);//나타나는 큐브

	HRESULT Ready_Layer_Object_BlockCube(const _tchar* pLayerTag);//접근을 막는 큐브


	HRESULT Ready_Layer_OrbitCube(const _tchar* pLayerTag);// 공전을 위한 큐브

	D3DLIGHT9		m_Light;

public:
	static CScene_Stage2* Create(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual void Free() override;
};

END