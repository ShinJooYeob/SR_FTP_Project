#pragma once
#include "Base.h"

// �� �����͸� �����ϴ� �̱��� Ŭ����

class CMapLoadMgr : public CBase
{

	DECLARE_SINGLETON(CMapLoadMgr);

	explicit CMapLoadMgr();
	virtual ~CMapLoadMgr() = default;

public:
	// �� ������ �ҷ��´�.
	HRESULT  LoadMap(SCENEID id, _uint index, list<SPECIALCUBE*>* spciallist);


private:
	// �� txt ������ �ҷ��´�.
	HRESULT Init_LoadMap();
	// ���� ������ �ؼ��� �����̳ʿ� �����Ѵ�.
	HRESULT Insert_MapData(const _tchar* filepath, _uint count);
	HRESULT RemoveMapList();

	HRESULT CubeID2Create(_uint sceneid, OUTPUT_OBJECTINFO& info, list<SPECIALCUBE*>* spciallist);

private:
	list<OUTPUT_OBJECTINFO*>* m_MapObjectList;
	int m_MaxMapCount;

public:
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;

};

