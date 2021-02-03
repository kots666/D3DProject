#pragma once

enum SoundChannel { BGM, PLAYER, PLAYER_EFFECT, MONSTER, EFFECT, MAXCHANNEL };

class CSoundManager
{
public:
	static CSoundManager* GetInstance();
	static void Destroy();

private:
	CSoundManager();
	~CSoundManager();

public:
	static void Initialize();
	static void Update(const _float& deltaTime);
	static void Release();
public:
	static void PlaySound(TCHAR* pSoundKey, SoundChannel eID);
	static void PlayOverlapSound(TCHAR* pSoundKey, SoundChannel eID, float offsetVolume = 0.1f, float duration = 0.1f);
	static void PlayOverlapSoundWithAmp(TCHAR* pSoundKey, SoundChannel eID);
	static void PlayBGM(TCHAR* pSoundKey);
	static void StopSound(SoundChannel eID);
	static void StopAll();
	static void SetVolume(SoundChannel channel, float per);

private:
	static void LoadSoundFile();

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> soundMap;
	map<TCHAR*, pair<bool, float>> soundTimeMap;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* channels[MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* pSystem;
	float volume;
};