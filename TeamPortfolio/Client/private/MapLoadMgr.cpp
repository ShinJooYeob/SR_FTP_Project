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

	// 3. �ش� �� ȣ��� �� �����Ͱ� �򸮰� ������.


	// ���⼭ ���� ����� ������ �ڵ带 ����
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

	// 1. �� ��θ� txt ���Ͽ��� �����ͼ� �ش� ���� ���� �ε��Ѵ�.

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

			// '|' ������ ��� ���ڿ� �Է� ó��

			fin.getline(szPadding, MAX_PATH, '|');
			fin.getline(szStateKey, MAX_PATH, '|');
			fin.getline(szCount, MAX_PATH, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			// ���ڿ��� ������ ��ȯ �Լ�
			_int iCount = _ttoi(szCount);

			// �� ������ ���� �о� �� �����Ϳ� ����
			FAILED_CHECK(Insert_MapData(szPath, iCount));

			if (fin.eof())
				break;
		}

		fin.close();		// close �Լ��� ���� ����(��ü Ÿ���̾ �Ҹ� ������ �˾Ƽ� ������ ���� ���� �Ҹ� ����)
	}




	return S_OK;
}

HRESULT CMapLoadMgr::Insert_MapData(const _tchar * filepath, _uint maxcount)
{
	// 2. .dat���� �˻��ϰ� ������ ��� �� ������ �����̳ʿ� ����
	// ���� ť�� �ƹ��ų� �����ؼ� �������

	RemoveMapList();
	m_MaxMapCount = maxcount;
	m_MapObjectList = new list<OUTPUT_OBJECTINFO *>[m_MaxMapCount];


	for (_uint i=0; i< m_MaxMapCount; i++)
	{
		// �� Dat ���� ���� �д´�.
		// Filename ID Objects
		_tchar szFilePath[MAX_PATH] = L"";
		wsprintf(szFilePath, filepath, i);



		HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			continue;

		// Fileheader ��ŭ �˻�
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
				// ������Ʈ ������ ����
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

