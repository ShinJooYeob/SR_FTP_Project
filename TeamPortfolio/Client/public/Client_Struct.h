#pragma once

// Ư��ť���� ��ġ�� ������ �����صд�.
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
