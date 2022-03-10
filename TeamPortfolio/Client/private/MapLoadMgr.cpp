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

	Init_LoadMap(); // 모든 맵 데이터 저장

}




HRESULT CMapLoadMgr::LoadMap(SCENEID sceneid, _uint index, list<SPECIALCUBE*>* spciallist)
{
	if (m_MaxMapCount <= index)
		return E_FAIL;

	// 3. 해당 맵 호출시 맵 데이터가 깔리게 만들자.


	// 여기서 맵을 만들어 버리는 코드를 제작
	list<OUTPUT_OBJECTINFO*> currentMap = m_MapObjectList[index];

	// 데이터 대입 부분

	// 1. 오브젝트를 생성함
	// 2. 큐브 생성후 데이터를 대입받음
	// 3, 대입받은 오브젝트에 데이터를 적용

	// 맵 생성부분

	for (auto Infodata : currentMap)
	{

		// 1. 큐브 ID로 각 큐브 생성
		// 생성이 안될때는 그냥 넘김
		bool bSpecialCube = true;
		bSpecialCube = CubeID2Create(sceneid, *Infodata, spciallist);


		if (bSpecialCube == false)
		{
			// 생성 X 위치만 저장하는 특수 큐브

		}
		// 충돌 안하는 큐브 추가
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
		else if(Infodata->CubeID == CUBEID_GRAVITY)
		{

			CGameObject* newObj = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(sceneid, /*TAG_LAY(Layer_Terrain)*/TEXT("Layer_Gravity"))->back();
			if (newObj == nullptr)
				continue;

			CTransform* trans = (CTransform*)newObj->Get_Component(TAG_COM(Com_Transform));
			_float3 vSacle = trans->Get_MatrixScale();
			trans->Set_Matrix(Infodata->WorldMatData);
			trans->Scaled(vSacle);

			// 특수큐브는 내부에서 텍스처 변경
			if (Infodata->CubeID == 0)
			{
				CTexture* tex = (CTexture*)newObj->Get_Component(TAG_COM(Com_Texture));
				tex->Change_TextureLayer(Infodata->TexDesc.szStateKey);
				tex->Set_LoadTexutreNumber(Infodata->TexDesc.StateIndex);
			}
		}
		else
		{

			CGameObject* newObj = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(sceneid, TAG_LAY(Layer_Terrain))->back();
			if (newObj == nullptr)
				continue;

			CTransform* trans = (CTransform*)newObj->Get_Component(TAG_COM(Com_Transform));
			_float3 scale =trans->Get_MatrixScale();
			trans->Set_Matrix(Infodata->WorldMatData);
			trans->Scaled(scale);

			// 특수큐브는 내부에서 텍스처 변경
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
	// OUTPUT_OBJECTINFO 리스트로 저장해서 실제 생성시에는 CUBEID를 해석해서 특수큐브 생성

	RemoveMapList();
	m_MaxMapCount = maxcount;
	m_MapObjectList = new list<OUTPUT_OBJECTINFO *>[m_MaxMapCount];


	for (_uint i = 0; i < m_MaxMapCount; i++)
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
				OUTPUT_OBJECTINFO* pInfoData = new OUTPUT_OBJECTINFO;

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
	// 큐브 아이디로 큐브 개별 생성
	SPECIALCUBE* specialdata = nullptr;

	switch (info.CubeID)
	{
	case CUBEID_NONE:
		GetSingle(CGameInstance)->Add_GameObject_To_Layer(sceneid,
			TAG_LAY(Layer_Terrain), TAG_OP(Prototype_TerrainCube),
			_float3(0, 0, 0));
		break;


	case CUBEID_GRAVITY:
		GetSingle(CGameInstance)->Add_GameObject_To_Layer(sceneid,
			/*TAG_LAY(Layer_Terrain)*/TEXT("Layer_Gravity"), TAG_OP(Prototype_GravityCube),
			_float3(0, 0, 0));
		break;
	case CUBEID_JUMP:
		GetSingle(CGameInstance)->Add_GameObject_To_Layer(sceneid,
			TAG_LAY(Layer_Terrain), TAG_OP(Prototype_FixCube),
			_float3(0, 0, 0));
		break;


		// 포탈 엘리베이터 공전 큐브는 인자값으로 여러가지를 전달해야한다. 
		// 위치만 태그와 위치만 
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
	case CUBEID_STAR:
		if (spciallist)
		{
			specialdata = new SPECIALCUBE(TEXT("ProtoType_GameObject_Object_Star"), info.WorldMatData);
			spciallist->push_back(specialdata);
			return false;
		}
		break;
		
		// 이외 나타나고 사라지는 큐브는 위치만 넘겨도 된다.
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


	return true;
}

void CMapLoadMgr::Free()
{
	RemoveMapList();

}

