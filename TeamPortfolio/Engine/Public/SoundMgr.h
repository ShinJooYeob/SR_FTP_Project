#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CSoundMgr :public CBase
{
	DECLARE_SINGLETON(CSoundMgr);

private:
	explicit CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	HRESULT Initialize_FMOD();

public:
	_int  VolumeUp(CHANNELID eID, _float _vol);
	_int  VolumeDown(CHANNELID eID, _float _vol);
	_int  BGMVolumeUp(_float _vol);
	_int  BGMVolumeDown(_float _vol);
	_int  Pause(CHANNELID eID);
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();


private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;

private:
	HRESULT LoadSoundFile();

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[CHANNEL_MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;
	_bool		m_bPause = false;
public:
	virtual void Free() override;
};


END

