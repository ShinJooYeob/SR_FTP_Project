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
	SCENE_TUTORIAL,
	SCENE_BOSS,
	SCENE_IMGUISCENE,
	SCENE_END
};



enum OBJECTPROTOTYPEID
{
	Prototype_Camera_Main, 
	Prototype_Player,
	Prototype_Boss,
	Prototype_Bullet,
	Prototype_BackGround,
	Prototype_TerrainGround,
	Prototype_TerrainCube,
	Prototype_FixCube,
	Prototype_GravityCube,
	Prototype_PortalCube_A,
	Prototype_PortalCube_B,
	Prototype_EscalatorCube,
	Prototype_OrbitButtonCube,
	Prototype_OrbitCube,
	Prototype_VanishCube,
	Prototype_AppearCube,
	Prototype_Shop,
	Prototype_Quest,
	Prototype_UI_Loby,
	Prototype_UI_Common
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
	case Prototype_Boss:
		return TEXT("Prototype_Boss");
		break;
	case Prototype_Bullet:
		return TEXT("Prototype_Bullet");
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
	case Prototype_FixCube:
		return TEXT("Prototype_GameObject_Object_FixCube");
		break;
	case Prototype_GravityCube:
		return TEXT("Prototype_GameObject_Object_GravityCube");
		break;
	case Prototype_PortalCube_A:
		return TEXT("Prototype_GameObject_Object_PortalCube_A");
		break;
	case Prototype_PortalCube_B:
		return TEXT("Prototype_GameObject_Object_PortalCube_B");
		break;
	case Prototype_EscalatorCube:
		return TEXT("Prototype_GameObject_Object_EscalatorCube");
		break;
	case Prototype_OrbitButtonCube:
		return TEXT("Prototype_GameObject_Object_OrbitButton");
		break;
	case Prototype_OrbitCube:
		return TEXT("Prototype_GameObject_Object_OrbitCube");
		break;
	case Prototype_VanishCube:		
		return TEXT("Prototype_GameObject_Object_VanishCube");
		break;
	case Prototype_AppearCube:
		return TEXT("Prototype_GameObject_Object_AppearCube");
		break;
	case Prototype_Shop:
		return TEXT("Prototype_GameObject_Shop");
		break;
	case Prototype_Quest:
		return TEXT("Prototype_GameObject_Quest");
		break;
	case Prototype_UI_Loby:
		return TEXT("Prototype_GameObject_UI_Loby");
		break;
	case Prototype_UI_Common:
		return TEXT("Prototype_GameObject_UI_Common");
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
	Layer_Monster,
	Layer_Bullet,
	Layer_Terrain,
	Layer_Terrain_Cube,
	Layer_BackGround,
	Layer_Shop,
	Layer_Quest,
	Layer_UI_Loby,
	Layer_UI_Common,
	Layer_ArticleObject
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
	case Layer_Monster:
		return TEXT("Layer_Monster");
		break;
	case Layer_Bullet:
		return TEXT("Layer_Bullet");
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
	case Layer_Quest:
		return TEXT("Layer_Quest");
		break;
	case Layer_UI_Loby:
		return TEXT("Layer_Loby");
		break;
	case Layer_UI_Common:
		return TEXT("Layer_Common");
		break;
		
	case Layer_ArticleObject:
		return TEXT("Layer_ArticleObject");
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
	Prototype_CollisionView,
	Prototype_Shader_Test,
	Prototype_Shader_Cube,
	Prototype_Gun,
	//////////////////////////////////////////////////////////////////////////
	Prototype_VIBuffer_Rect,
	Prototype_VIBuffer_Terrain128x128,
	Prototype_VIBuffer_Cube,
	//////////////////////////////////////////////////////////////////////////
	Prototype_Texture_Blank,
	Prototype_Texture_MiniMap,
	Prototype_Texture_Default,
	Prototype_Texture_Player, 
	Prototype_Texture_Terrain,
	Prototype_Texture_UI,
	Prototype_Texture_Monster

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
	case Prototype_CollisionView:
		return TEXT("Prototype_Component_CollisionView");
		break;
	case Prototype_Inventory:
		return TEXT("Prototype_Component_Inventory");
		break;
	case Prototype_Shader_Test:
		return TEXT("Prototype_Component_Shader_Test");
	case Prototype_Shader_Cube:
		return TEXT("Prototype_Component_Shader_Cube");
		break;
	case Prototype_Gun:
		return TEXT("Prototype_Component_Gun");
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

	case Prototype_Texture_MiniMap:
		return TEXT("Prototype_Component_Texture_MiniMap");
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
	case Prototype_Texture_UI:
		return TEXT("Prototype_Component_Texture_UI");
		break;
	case Prototype_Texture_Monster:
		return TEXT("Prototype_Component_Texture_Monster");

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
	Com_CollisionView,
	Com_Inventory,
	Com_Shader,
	Com_Gun

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
	case Com_CollisionView:
		return TEXT("Com_CollisionView");
		break;
	case Com_Inventory:
		return TEXT("Com_Inventory");
		break;
	case Com_Shader:
		return TEXT("Com_Shader");
		break;
	case Com_Gun:
		return TEXT("Com_Gun");
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
	SKILL_CAMERA,
	SKILL_POTION,
	SKILL_END
};

enum SHOPCASE
{
	SHOP_SPEEDUP=10,
	SHOP_DUBBLEJUMP,
	SHOP_DASH,
	SHOP_POTION,
	SHOP_BUY,
	SHOP_EXIT,
	SHOP_SELL,
	SHOP_END
};

enum RESULTCASE
{
	RESULT_START = 10000,
	RESULT_CANCEL,
	RESULT_END
};

enum QUEST
{
	QUEST_1,
	QUEST_2,
	QUEST_3,
	QUEST_4,
	QUEST_END
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

enum E_CUBEID
{
	CUBEID_NONE,
	CUBEID_GRAVITY,
	CUBEID_JUMP,
	CUBEID_POTAL,
	CUBEID_ELEVETOR,
	CUBEID_ORBIT,
	CUBEID_VANISH, // 사라짐
	CUBEID_APPEAR, // 나타남
	CUBEID_ARTICLEOBJECT, //주엽
	CUBEID_STAR=10, //별생성
	CUBEID_FIXED,

	CUBEID_END,

};

enum MOUSETYPEID
{
	MOUSE_DEFAULT,
	MOUSE_CLICK,
	MOUSE_GRAB,
	MOUSE_END
};

// 날아가는 타입
enum E_BulletType_MOVE
{
	BULLETTYPE_MOVE_NOMAL,
	BULLETTYPE_MOVE_END

};