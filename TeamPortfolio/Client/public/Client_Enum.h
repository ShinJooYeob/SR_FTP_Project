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


// 원본객체를 복사해야되는 경우 미리 ENUM 값을 정의하고 동일한 Tag를 사용하기 위함
// 이름은 다시 회의해서 정함

// 레이어 Tag TEXT
enum E_LAYERTAG
{
	LAYER_DEFAULT,
	LAYER_BACKGROUND,
	LAYER_PLAYER,
	LAYER_UI,
	LAYER_END,
};

static const wchar_t* StrLayer(E_LAYERTAG tag)
{
	switch (tag)
	{
	case LAYER_DEFAULT:
		return TEXT("LAYER_DEFAULT");
	case LAYER_BACKGROUND:
		return TEXT("LAYER_BACKGROUND");
	case LAYER_PLAYER:
		return TEXT("LAYER_PLAYER");
	case LAYER_UI:
		return TEXT("LAYER_UI");
	case LAYER_END:
		return TEXT("LAYER_END");

	default:
		break;
	}
}

// 컴포넌트 Tag
enum E_COMTAG
{
	COMTAG_RENDERER,
	COMTAG_TRANSFORM,
	COMTAG_RECT,
	COMTAG_TERRAIN,
	COMTAG_TEXTURE,
	COMTAG_END,
};

static const wchar_t* StrCompoent(E_COMTAG tag)
{
	switch (tag)
	{
	case COMTAG_RENDERER:
		return TEXT("COMTAG_RENDERER");
		break;
	case COMTAG_TRANSFORM:
		return TEXT("COMTAG_TRANSFORM");
		break;
	case COMTAG_RECT:
		return TEXT("COMTAG_RECT");
		break;
	case COMTAG_TERRAIN:
		return TEXT("COMTAG_TERRAIN");
		break;
	case COMTAG_TEXTURE:
		return TEXT("COMTAG_TEXTURE");
		break;
	case COMTAG_END:
		return TEXT("COMTAG_END");
		break;
	default:
		break;
	}
}

