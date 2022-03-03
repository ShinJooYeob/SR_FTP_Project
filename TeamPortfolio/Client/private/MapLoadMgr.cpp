#include "stdafx.h"
#include "../public/MapLoadMgr.h"
#include "Gameobject.h"
#include "Object_AppearCube.h"
#include "Object_VanishCube.h"


IMPLEMENT_SINGLETON(CMapLoadMgr);

CMapLoadMgr::CMapLoadMgr()
{
	m_MapObjectList = nullptr;
	m_MaxMapCount = 0;

	Init_LoadMap(); // ��� �� ������ ����

}




HRESULT CMapLoadMgr::LoadMap(SCENEID sceneid, _uint index, list<SPECIALCUBE*>* spciallist)
{
	if (m_MaxMapCount <= index)
		return E_FAIL;

	// 3. �ش� �� ȣ��� �� �����Ͱ� �򸮰� ������.


	// ���⼭ ���� ����� ������ �ڵ带 ����
	list<OUTPUT_OBJECTINFO*> currentMap = m_MapObjectList[index];

	// ������ ���� �κ�

	// 1. ������Ʈ�� ������
	// 2. ť�� ������ �����͸� ���Թ���
	// 3, ���Թ��� ������Ʈ�� �����͸� ����

	// �� �����κ�

	for (auto Infodata : currentMap)
	{

		// 1. ť�� ID�� �� ť�� ����
		// ������ �ȵɶ��� �׳� �ѱ�
		bool bSpecialCube = true;
		bSpecialCube = CubeID2Create(sceneid, *Infodata, spciallist);


		if (bSpecialCube == false)
		{
			// ���� X ��ġ�� �����ϴ� Ư�� ť��

		}
		else if (Infodata->CubeID == CUBEID_ARTICLEOBJECT)
		{
			CGameObject* newObj = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(sceneid, TAG_LAY(Layer_ArticleObject))->back();
			if (newObj == nullptr)
				continue;

			CTransform* trans = (CTransform*)newObj->Get_Component(TAG_COM(Com_Transform));
			trans->Set_Matrix(Infodata->WorldMatData);

			CTexture* tex = (CTexture*)newObj->Get_Component(TAG_COM(Com_Texture));
			tex->Change_TextureLayer(Infodata->TexDesc.szStateKey);
			tex->Set_LoadTexutreNumber(Infodata->TexDesc.StateIndex);

		}
		else if (Infodata->CubeID == CUBEID_APPEAR)
		{
			CObject_AppearCube* newObj = (CObject_AppearCube*)(GetSingle(CGameInstance)->Get_ObjectList_from_Layer(sceneid, TAG_LAY(Layer_Terrain))->back());
			if (newObj == nullptr)
				continue;

			CTransform* trans = (CTransform*)newObj->Get_Component(TAG_COM(Com_Transform));
			trans->Set_Matrix(Infodata->WorldMatData);

			newObj->Set_AppearDesc(&(Infodata->WorldMatData.m[3][0]));

			CTexture* tex = (CTexture*)newObj->Get_Component(TAG_COM(Com_Texture));
			tex->Change_TextureLayer(Infodata->TexDesc.szStateKey);
			tex->Set_LoadTexutreNumber(Infodata->TexDesc.StateIndex);

		}
		else if (Infodata->CubeID == CUBEID_VANISH)
		{
			CObject_VanishCube* newObj = (CObject_VanishCube*)(GetSingle(CGameInstance)->Get_ObjectList_from_Layer(sceneid, TAG_LAY(Layer_Terrain))->back());
			if (newObj == nullptr)
				continue;

			CTransform* trans = (CTransform*)newObj->Get_Component(TAG_COM(Com_Transform));
			trans->Set_Matrix(Infodata->WorldMatData);

			newObj->Set_VenishDesc(&(Infodata->WorldMatData.m[3][0]));

			CTexture* tex = (CTexture*)newObj->Get_Component(TAG_COM(Com_Texture));
			tex->Change_TextureLayer(Infodata->TexDesc.szStateKey);
			tex->Set_LoadTexutreNumber(Infodata->TexDesc.StateIndex);

		}
		else
		{

			CGameObject* newObj = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(sceneid, TAG_LAY(Layer_Terrain))->back();
			if (newObj == nullptr)
				continue;

			CTransform* trans = (CTransform*)newObj->Get_Component(TAG_COM(Com_Transform));
			trans->Set_Matrix(Infodata->WorldMatData);

			// Ư��ť�� �� �׳� �����ص� �Ǵ� ��
			if (Infodata->CubeID == 0)
			{
				CTexture* tex = (CTexture*)newObj->Get_Component(TAG_COM(Com_Texture));
				tex->Change_TextureLayer(Infodata->TexDesc.szStateKey);
				tex->Set_LoadTexutreNumber(Infodata->TexDesc.StateIndex);
			}
		}
	}

	return S_OK;

}

HRESULT CMapLoadMgr::Init_LoadMap()
{

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
	// OUTPUT_OBJECTINFO ����Ʈ�� �����ؼ� ���� �����ÿ��� CUBEID�� �ؼ��ؼ� Ư��ť�� ����

	RemoveMapList();
	m_MaxMapCount = maxcount;
	m_MapObjectList = new list<OUTPUT_OBJECTINFO *>[m_MaxMapCount];


	for (_uint i = 0; i < m_MaxMapCount; i++)
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
				OUTPUT_OBJECTINFO* pInfoData = new OUTPUT_OBJECTINFO;

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
		for (int i = 0; i < m_MaxMapCount; i++)
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

bool CMapLoadMgr::CubeID2Create(_uint sceneid, OUTPUT_OBJECTINFO& info, list<SPECIALCUBE*>* spciallist)
{
	// ť�� ���̵�� ť�� ���� ����
	SPECIALCUBE* specialdata = nullptr;

	switch (info.CubeID)
	{
	case CUBEID_NONE:
		GetSingle(CGameInstance)->Add_GameObject_To_Layer(sceneid,
			TAG_LAY(Layer_Terrain), TAG_OP(Prototype_TerrainCube),
			_float3(0, 0, 0));
		break;

		// #STOP ��� ť�� static�� �̸� ���� �Ŀ� �׽�Ʈ ����
	case CUBEID_GRAVITY:
		GetSingle(CGameInstance)->Add_GameObject_To_Layer(sceneid,
			TAG_LAY(Layer_Terrain), TAG_OP(Prototype_GravityCube),
			_float3(0, 0, 0));
		break;
	case CUBEID_JUMP:
		GetSingle(CGameInstance)->Add_GameObject_To_Layer(sceneid,
			TAG_LAY(Layer_Terrain), TAG_OP(Prototype_FixCube),
			_float3(0, 0, 0));
		break;


		// ��Ż ���������� ���� ť��� ���ڰ����� ���������� �����ؾ��Ѵ�. 
		// ��ġ�� �±׿� ��ġ�� 
	case CUBEID_POTAL:
		if (spciallist)
		{
			specialdata = new SPECIALCUBE(TAG_OP(Prototype_PortalCube_A), info.WorldMatData);
			spciallist->push_back(specialdata);
			return false;
		}
		break;
	case CUBEID_ELEVETOR:
		if (spciallist)
		{
			specialdata = new SPECIALCUBE(TAG_OP(Prototype_EscalatorCube), info.WorldMatData);
			spciallist->push_back(specialdata);
			return false;
		}
		break;
	case CUBEID_ORBIT:
		if (spciallist)
		{
			specialdata = new SPECIALCUBE(TAG_OP(Prototype_OrbitButtonCube), info.WorldMatData);
			spciallist->push_back(specialdata);
			return false;
		}
		break;

		// �̿� ��Ÿ���� ������� ť��� ��ġ�� �Ѱܵ� �ȴ�.
	case CUBEID_VANISH:
		GetSingle(CGameInstance)->Add_GameObject_To_Layer(sceneid,
			TAG_LAY(Layer_Terrain), TAG_OP(Prototype_VanishCube),
			_float3(0, 0, 0));
		break;
	case CUBEID_APPEAR:
		GetSingle(CGameInstance)->Add_GameObject_To_Layer(sceneid,
			TAG_LAY(Layer_Terrain), TAG_OP(Prototype_AppearCube),
			_float3(0, 0, 0));
		break;



	case CUBEID_ARTICLEOBJECT:

			GetSingle(CGameInstance)->Add_GameObject_To_Layer(sceneid,
				TAG_LAY(Layer_ArticleObject), TAG_OP(Prototype_TerrainCube),
				_float3(0, 0, 0));
		
		break;
	case CUBEID_END:
		return false;
		break;
	default:
		break;
	}

	// #Tag �� �ڵ尡 �ϼ��Ǹ� ����
	//GetSingle(CGameInstance)->Add_GameObject_To_Layer(sceneid,
	//	TAG_LAY(Layer_Terrain), TAG_OP(Prototype_TerrainCube),
	//	_float3(0, 0, 0));
	return true;
}

void CMapLoadMgr::Free()
{
	RemoveMapList();

}

