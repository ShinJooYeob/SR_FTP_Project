#pragma once

// 특수큐브의 위치와 정보를 저장해둔다.
typedef struct tag_SpecialCube_DATA
{
	tag_SpecialCube_DATA(wstring w, _Matrix worldmat)
	{
		Tagname = w;
		WorldMat = worldmat;
	}
	wstring Tagname;
	_Matrix WorldMat;
}SPECIALCUBE;
