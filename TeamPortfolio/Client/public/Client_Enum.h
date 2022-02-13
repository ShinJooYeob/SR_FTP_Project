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


// ������ü�� �����ؾߵǴ� ��� �̸� ENUM ���� �����ϰ� ������ Tag�� ����ϱ� ����
// �̸��� �ٽ� ȸ���ؼ� ����

// ���̾� Tag TEXT
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

// ������Ʈ Tag
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

