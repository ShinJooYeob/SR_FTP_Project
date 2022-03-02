#pragma once
#include "Base.h"

// 맵 데이터를 관리하는 싱글턴 클래스

class CMapLoadMgr : public CBase
{

	DECLARE_SINGLETON(CMapLoadMgr);

	explicit CMapLoadMgr();
	virtual ~CMapLoadMgr() = default;

public:
	// 맵 정보를 불러온다.
	HRESULT  LoadMap(SCENEID id, _uint index, list<SPECIALCUBE*>* spciallist);


private:
	// 맵 txt 파일을 불러온다.
	HRESULT Init_LoadMap();
	// 맵의 정보를 해석해 컨테이너에 저장한다.
	HRESULT Insert_MapData(const _tchar* filepath, _uint count);
	HRESULT RemoveMapList();

	bool CubeID2Create(_uint sceneid, OUTPUT_OBJECTINFO& info, list<SPECIALCUBE*>* spciallist);

private:
	list<OUTPUT_OBJECTINFO*>* m_MapObjectList;
	int m_MaxMapCount;

public:
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

};

