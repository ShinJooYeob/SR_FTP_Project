#pragma once


enum SCENEID
{
	SCENE_STATIC,
	SCENE_LOBY,
	SCENE_LOADING,
	SCENE_STAGESELECT,
	SCENE_STAGE1,
	SCENE_STAGE2,
	SCENE_STAGE3,
	SCENE_IMGUISCENE,
	SCENE_END
};

enum OBJECTPROTOTYPEID
{
	Prototype_Camera_Main, 
	Prototype_Player,
	Prototype_BackGround,
	Prototype_TerrainGround,
	Prototype_TerrainCube,
	Prototype_Shop,
	Prototype_Button
};

static const _tchar* Tag_Object_Prototype(OBJECTPROTOTYPEID eTag)
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
		MSGBOX("Wrong Type Object Prototype");
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
	Layer_Shop

};

static const _tchar* Tag_Layer(LAYERID eTag)
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
	case Layer_Shop:
		return TEXT("Layer_Shop");
		break;

		//////////////////////////////////////////////////////////////////////////
	default:
		MSGBOX("Wrong Type Layer");
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
	Prototype_Texture_Blank,
	Prototype_Texture_Default,
	Prototype_Texture_Player, 
	Prototype_Texture_Terrain,
	Prototype_Texture_Shop,

};
static const _tchar* Tag_Component_Prototype(COMPONENTPROTOTYPEID eTag)
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
	case Prototype_Texture_Player:
		return TEXT("Prototype_Component_Texture_Player");
		break;
	case Prototype_Texture_Terrain:
		return TEXT("Prototype_Component_Texture_Terrain");
		break;
	case Prototype_Texture_Shop:
		return TEXT("Prototype_Component_Texture_Shop");
		break;
		//////////////////////////////////////////////////////////////////////////
	default:
		MSGBOX("Wrong Type Layer");
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
	Com_Collision,
	Com_Inventory

};
static const _tchar* Tag_Component(COMPONENTID eTag)
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
	case Com_Collision:
		return TEXT("Com_Collision");
		break;
	case Com_Inventory:
		return TEXT("Com_Inventory");
		break;

		//////////////////////////////////////////////////////////////////////////
	default:
		MSGBOX("Wrong Type Component");
		return nullptr;
		break;
	}


}
#define  TAG_COM Tag_Component

enum SKILL
{
	SKILL_SPEEDUP,
	SKILL_DUBBLEJUMP,
	SKILL_DASH,
	SKILL_POTION,
	SKILL_END,
};

enum EasingTypeID
{
	TYPE_Linear = 0,

	TYPE_QuadIn = 1,
	TYPE_QuadOut,
	TYPE_QuadInOut,

	TYPE_CubicIn = 4,
	TYPE_CubicOut,
	TYPE_CubicInOut,

	TYPE_QuarticIn = 7,
	TYPE_QuarticOut,
	TYPE_QuarticInOut,

	TYPE_QuinticIn = 10,
	TYPE_QuinticOut,
	TYPE_QuinticInOut,

	TYPE_SinIn = 13,
	TYPE_SinOut,
	TYPE_SinInOut,

	TYPE_ExpoIn = 16,
	TYPE_ExpoOut,
	TYPE_ExpoInOut,

	TYPE_CircularIn = 19,
	TYPE_CircularOut,
	TYPE_CircularInOut,

	TYPE_ElasticIn = 22,
	TYPE_ElasticOut,
	TYPE_ElasticInOut,

	TYPE_BounceIn = 25,
	TYPE_BounceOut,


	TYPE_End
};