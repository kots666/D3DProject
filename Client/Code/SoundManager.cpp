#include "stdafx.h"
#include "SoundManager.h"
#include <algorithm>

CSoundManager* pSoundManager = nullptr;

CSoundManager* CSoundManager::GetInstance()
{
	if (pSoundManager == nullptr)
	{
		pSoundManager = new CSoundManager;
	}
	return pSoundManager;
}

void CSoundManager::Destroy()
{
	if (pSoundManager)
	{
		delete pSoundManager;
		pSoundManager = nullptr;
	}
}

CSoundManager::CSoundManager()
{
	pSystem = nullptr; 
	volume = 0.5f;
}

CSoundManager::~CSoundManager()
{
	Release();
}

void CSoundManager::Initialize()
{
	FMOD_System_Create(&pSoundManager->pSystem);
	
	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(pSoundManager->pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile(); 
}

void CSoundManager::Update(const _float& deltaTime)
{
	for (auto& pair : pSoundManager->soundTimeMap)
	{
		if (pair.second.first)
		{
			pair.second.second -= deltaTime;

			if (0.f > pair.second.second)
			{
				pair.second.first = false;
				pair.second.second = 0.f;
			}
		}
	}
}

void CSoundManager::Release()
{
	for (auto& Mypair : pSoundManager->soundMap)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	pSoundManager->soundMap.clear();
	FMOD_System_Release(pSoundManager->pSystem);
	FMOD_System_Close(pSoundManager->pSystem);
}

void CSoundManager::PlaySound(TCHAR * pSoundKey, SoundChannel eID)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter; 

	iter = find_if(pSoundManager->soundMap.begin(), pSoundManager->soundMap.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == pSoundManager->soundMap.end())
		return;

	FMOD_BOOL bPlay = FALSE; 
	if (FMOD_Channel_IsPlaying(pSoundManager->channels[eID], &bPlay))
	{
		FMOD_System_PlaySound(pSoundManager->pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &pSoundManager->channels[eID]);
		FMOD_Channel_SetVolume(pSoundManager->channels[eID], pSoundManager->volume);
	}
	FMOD_System_Update(pSoundManager->pSystem);
}

void CSoundManager::PlayOverlapSound(TCHAR * pSoundKey, SoundChannel eID, float offsetVolume, float duration)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(pSoundManager->soundMap.begin(), pSoundManager->soundMap.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == pSoundManager->soundMap.end())
		return;

	auto timeIter = find_if(pSoundManager->soundTimeMap.begin(), pSoundManager->soundTimeMap.end(), [&](auto& elem)
	{
		return !lstrcmp(pSoundKey, elem.first);
	});

	if (timeIter->second.first) return;

	FMOD_System_PlaySound(pSoundManager->pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &pSoundManager->channels[eID]);
	FMOD_Channel_SetVolume(pSoundManager->channels[eID], pSoundManager->volume * offsetVolume);

	timeIter->second.first = true;
	timeIter->second.second = duration;

	FMOD_System_Update(pSoundManager->pSystem);
}

void CSoundManager::PlayOverlapSoundWithAmp(TCHAR * pSoundKey, SoundChannel eID)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(pSoundManager->soundMap.begin(), pSoundManager->soundMap.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == pSoundManager->soundMap.end())
		return;

	FMOD_System_PlaySound(pSoundManager->pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &pSoundManager->channels[eID]);
	FMOD_Channel_SetVolume(pSoundManager->channels[eID], pSoundManager->volume);

	FMOD_System_Update(pSoundManager->pSystem);
}

void CSoundManager::PlayBGM(TCHAR * pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(pSoundManager->soundMap.begin(), pSoundManager->soundMap.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == pSoundManager->soundMap.end())
		return;

	FMOD_System_PlaySound(pSoundManager->pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &pSoundManager->channels[BGM]);
	FMOD_Channel_SetMode(pSoundManager->channels[BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(pSoundManager->channels[BGM], 0.1f);
	FMOD_System_Update(pSoundManager->pSystem);
}

void CSoundManager::StopSound(SoundChannel eID)
{
	FMOD_Channel_Stop(pSoundManager->channels[eID]);
}

void CSoundManager::StopAll()
{
	for (int i = 0 ; i < MAXCHANNEL ; ++i)
		FMOD_Channel_Stop(pSoundManager->channels[i]);
}

void CSoundManager::SetVolume(SoundChannel channel, float per)
{
	FMOD_Channel_SetVolume(pSoundManager->channels[channel], per);
}

void CSoundManager::LoadSoundFile()
{
	_finddata_t fd; 

	long handle = _findfirst("../Sound/*.*", &fd);

	if (handle == 0)
		return; 
	if (handle == -1)
		return;

	int iResult = 0; 

	char szCurPath[128] = "../Sound/";
	char szFullPath[128] = ""; 

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath); 
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr; 

		FMOD_RESULT eRes = FMOD_System_CreateSound(pSoundManager->pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1; 

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			pSoundManager->soundMap.emplace(pSoundKey, pSound);
			pSoundManager->soundTimeMap.emplace(pSoundKey, make_pair(false, 0.f));
		}
		iResult = _findnext(handle, &fd);
	}
	FMOD_System_Update(pSoundManager->pSystem);
	_findclose(handle);
}