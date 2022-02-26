#include "stdafx.h"
#include "../public/MapLoadMgr.h"


IMPLEMENT_SINGLETON(CMapLoadMgr);

CMapLoadMgr::CMapLoadMgr()
{
	m_MapObjectList = nullptr;
	m_MaxMapCount = 0;

	Init_LoadMap();

}

HRESULT CMapLoadMgr::LoadMap(SCENEID id, _uint index)
{
	if (m_MaxMapCount <= index)
		return E_FAIL;

	// 3. 해당 맵 호출시 맵 데이터가 깔리게 만들자.


	// 여기서 맵을 만들어 버리는 코드를 제작
	list<OUTPUT_OBJECTINFO*> currentMap = m_MapObjectList[index];
	_float3 xyz;

	for (auto obj : currentMap)
	{
		memcpy(xyz, &obj->WorldMatData.m[3][0],sizeof(_float3));
		GetSingle(CGameInstance)->Add_GameObject_To_Layer(id,
			TAG_LAY(Layer_Terrain), TAG_OP(Prototype_TerrainCube),
			&xyz);

	}

	return S_OK;

}

HRESULT CMapLoadMgr::Init_LoadMap()
{

	// #TODO For Client Load

	// 1. 맵 경로를 txt 파일에서 가져와서 해당 맵을 전부 로드한다.

	wifstream		fin;

	_tchar MapTxtFilePath[MAX_PATH] = L"";

	lstrcat(MapTxtFilePath, g_Path_Data_MapTXT);
	fin.open(MapTxtFilePath);

	if (!fin.fail())
	{
		_tchar	szPadding[64] = L"";
		_tchar	szPath[MAX_PATH] = L"";
		_tchar	szStateKey[MAX_PATH] = L"";
		_tchar	szCount[MAX_PATH] = L"";

		while (true)
		{

			// '|' 단위로 끊어서 문자열 입력 처리

			fin.getline(szPadding, MAX_PATH, '|');
			fin.getline(szStateKey, MAX_PATH, '|');
			fin.getline(szCount, MAX_PATH, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			// 문자열을 정수로 변환 함수
			_int iCount = _ttoi(szCount);

			// 맵 정보를 각각 읽어 맵 데이터에 저장
			FAILED_CHECK(Insert_MapData(szPath, iCount));

			if (fin.eof())
				break;
		}

		fin.close();		// close 함수는 생략 가능(객체 타입이어서 소멸 시점에 알아서 개방한 파일 또한 소멸 가능)
	}




	return S_OK;
}

HRESULT CMapLoadMgr::Insert_MapData(const _tchar * filepath, _uint maxcount)
{
	// 2. .dat인지 검사하고 데이터 대로 맵 정보를 컨테이너에 저장
	// 원형 큐브 아무거나 복사해서 사용하자

	RemoveMapList();
	m_MaxMapCount = maxcount;
	m_MapObjectList = new list<OUTPUT_OBJECTINFO *>[m_MaxMapCount];


	for (_uint i=0; i< m_MaxMapCount; i++)
	{
		// 각 Dat 파일 별로 읽는다.
		// Filename ID Objects
		_tchar szFilePath[MAX_PATH] = L"";
		wsprintf(szFilePath, filepath, i);



		HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			continue;

		// Fileheader 만큼 검사
		OUTPUT_ID			infoid;
		DWORD				dwByte = 0;


		ReadFile(hFile, &infoid, sizeof(OUTPUT_ID), &dwByte, nullptr);
		switch (infoid.FILEID)
		{
		case OUTPUT_MAP:
			while (true)
			{
				OUTPUT_OBJECTINFO *pInfoData = new OUTPUT_OBJECTINFO;

				ReadFile(hFile, &(*pInfoData), sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);

				if (0 == dwByte)
				{
					Safe_Delete(pInfoData);
					break;
				}
				// 오브젝트 데이터 저장
				m_MapObjectList[i].push_back(pInfoData);
			}

			break;
		default:
			break;
		}
		CloseHandle(hFile);
	}
	return S_OK;
}

HRESULT CMapLoadMgr::RemoveMapList()
{
	if (m_MapObjectList != nullptr)
	{
		for (int i = 0; i<m_MaxMapCount; i++)
		{
			for (auto pObj : m_MapObjectList[i])
			{
				Safe_Delete(pObj);
			}
			m_MapObjectList[i].clear();
		}
	}
	Safe_Delete_Array(m_MapObjectList);
	return S_OK;
}

void CMapLoadMgr::Free()
{
	RemoveMapList();

}

