#pragma once



enum OBJECTPROTOTYPEID
{
	Prototype_Camera_Main,
	Prototype_Player,
	Prototype_BackGround,
	Prototype_WIRECUBE,
	Prototype_TerrainGround,
	Prototype_TerrainCube,
	Prototype_Shop,
	Prototype_Button
};

static const TCHAR* Tag_Object_Prototype(OBJECTPROTOTYPEID eTag)
{
	switch (eTag)
	{
	case Prototype_Camera_Main:
		return TEXT("Prototype_GameObject_Camera_Main");
		break;
	case Prototype_Player:
		return TEXT("Prototype_GameObject_Player");
		break;
	case Prototype_BackGround:
		return TEXT("Prototype_GameObject_BackGround");
		break;
	case Prototype_WIRECUBE:
		return TEXT("Prototype_GameObject_WireCube");
		break;
	case Prototype_TerrainGround:
		return TEXT("Prototype_GameObject_TerrainGround");
		break;
	case Prototype_TerrainCube:
		return TEXT("Prototype_GameObject_TerrainCube");
		break;
	case Prototype_Shop:
		return TEXT("Prototype_GameObject_Shop");
		break;
	case Prototype_Button:
		return TEXT("Prototype_GameObject_Button");
		break;
		//////////////////////////////////////////////////////////////////////////
	default:
		AfxMessageBox(L"Wrong Type Layer", MB_OK);
		return nullptr;
		break;
	}


}
#define  TAG_OP Tag_Object_Prototype

enum LAYERID
{
	Layer_Camera_Main,
	Layer_Player,
	Layer_Terrain,
	Layer_Terrain_Cube,
	Layer_BackGround,
	Layer_WireCube,
	Layer_View,
	Layer_Map,
	Layer_Shop

};

static const TCHAR* Tag_Layer(LAYERID eTag)
{
	switch (eTag)
	{
	case Layer_Camera_Main:
		return TEXT("Layer_Camera_Main");
		break;
	case Layer_Player:
		return TEXT("Layer_Player");
		break;
	case Layer_Terrain:
		return TEXT("Layer_Terrain");
		break;
	case Layer_Terrain_Cube:
		return TEXT("Layer_Terrain_Cube");
		break;
	case Layer_BackGround:
		return TEXT("Layer_BackGround");
		break;
	case Layer_View:
		return TEXT("Layer_View");
		break;
	case Layer_WireCube:
		return TEXT("Layer_WireCube");
		break;
	case Layer_Map:
		return TEXT("Layer_Map");
		break;
	case Layer_Shop:
		return TEXT("Layer_Shop");
		break;

		//////////////////////////////////////////////////////////////////////////
	default:
		AfxMessageBox(L"Wrong Type Layer", MB_OK);
		return nullptr;
		break;
	}
	

}
#define  TAG_LAY Tag_Layer

enum COMPONENTPROTOTYPEID
{
	Prototype_Renderer,
	Prototype_Transform,
	Prototype_Inventory,
	Prototype_Collision,
	//////////////////////////////////////////////////////////////////////////
	Prototype_VIBuffer_Rect,
	Prototype_VIBuffer_Terrain128x128,
	Prototype_VIBuffer_Cube,
	//////////////////////////////////////////////////////////////////////////
	Prototype_Texture_Cube,
	Prototype_Texture_Blank,
	Prototype_Texture_Default,
	Prototype_Texture_Player,
	Prototype_Texture_Terrain,
	Prototype_Texture_Shop,
	Prototype_Texture_Monster,


};
static const TCHAR* Tag_Component_Prototype(COMPONENTPROTOTYPEID eTag)
{
	switch (eTag)
	{
	case Prototype_Renderer:
		return TEXT("Prototype_Component_Renderer");
		break;
	case Prototype_Transform:
		return TEXT("Prototype_Component_Transform");
		break;
	case Prototype_Collision:
		return TEXT("Prototype_Component_Collision");
		break;
	case Prototype_Inventory:
		return TEXT("Prototype_Component_Inventory");
		break;

	case Prototype_VIBuffer_Rect:
		return TEXT("Prototype_Component_VIBuffer_Rect");
		break;
	case Prototype_VIBuffer_Terrain128x128:
		return TEXT("Prototype_Component_VIBuffer_Terrain128x128");
		break;
	case Prototype_VIBuffer_Cube:
		return TEXT("Prototype_Component_VIBuffer_Cube");
		break;

	case Prototype_Texture_Blank:
		return TEXT("Prototype_Component_Texture_Blank");
		break;
	case Prototype_Texture_Default:
		return TEXT("Prototype_Component_Texture_Default");
		break;
	case Prototype_Texture_Cube:
		return TEXT("Prototype_Component_Texture_Cube");
		break;
		
	case Prototype_Texture_Player:
		return TEXT("Prototype_Component_Texture_Player");
		break;
	case Prototype_Texture_Terrain:
		return TEXT("Prototype_Component_Texture_Terrain");
		break;
	case Prototype_Texture_Shop:
		return TEXT("Prototype_Component_Texture_Shop");
		break;
	case Prototype_Texture_Monster:
		return TEXT("Prototype_Component_Texture_Monster");
		break;
		//////////////////////////////////////////////////////////////////////////
	default:
		AfxMessageBox(L"Wrong Type Layer",MB_OK);
		return nullptr;
		break;
	}


}

#define  TAG_CP Tag_Component_Prototype

enum COMPONENTID
{
	Com_Renderer,
	Com_VIBuffer,
	Com_Transform,
	Com_Texture,
	Com_Texture_Cube,
	Com_Collision,
	Com_Inventory

};
static const TCHAR* Tag_Component(COMPONENTID eTag)
{
	switch (eTag)
	{
	case Com_Renderer:
		return TEXT("Com_Renderer");
		break;
	case Com_VIBuffer:
		return TEXT("Com_VIBuffer");
		break;
	case Com_Transform:
		return TEXT("Com_Transform");
		break;
	case Com_Texture:
		return TEXT("Com_Texture");		
		break;
	case Com_Texture_Cube:
		return TEXT("Com_Texture_Cube");
	case Com_Collision:
		return TEXT("Com_Collision");
		break;
	case Com_Inventory:
		return TEXT("Com_Inventory");
		break;

		//////////////////////////////////////////////////////////////////////////
	default:
		AfxMessageBox(L"Wrong Type Layer", MB_OK);
		return nullptr;
		break;
	}


}
#define  TAG_COM Tag_Component

// 맵에대한 정보

enum E_FILETYPE
{
	FILETYPE_PNG,
	FILETYPE_DDS,
	FILETYPE_XML,
	FILETYPE_DAT,
	FILETYPE_END,

};
