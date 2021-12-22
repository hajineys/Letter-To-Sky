#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "fmod.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

enum class SoundInfo
{
	None,
	Loop,
};

class JChannel
{
public:
	JChannel() : FmodChannel(nullptr), Volume(SOUND_DEFAULT), IsPlaying(false) {};
	~JChannel() {};

public:
	FMOD_CHANNEL* FmodChannel;
	float Volume;
	FMOD_BOOL IsPlaying;
};

/// <summary>
/// ���带 �ε��ϰ� �����ϴ� ���� �Ŵ���
/// 2021.02.21
/// ���̺귯�� ���� - �̿���, �ձ��� / ����, ������Ʈ - ������
/// 2021.08.23 �ڵ� ���� �� 64��Ʈ ���� ������Ʈ - ������
/// </summary>
class SoundManager
{
private:
	static SoundManager* m_Instance;
public:
	static SoundManager* GetInstance()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new SoundManager();
		}
		return m_Instance;
	}

private:
	SoundManager();
	~SoundManager();
	static FMOD_SYSTEM* FmodSoundSystem;

private:
	struct SoundData
	{
		SoundData() : Sound(nullptr), IsPlayed(false) {}
		~SoundData() { FMOD_Sound_Release(Sound); }

		FMOD_SOUND* Sound;
		bool IsPlayed;
	};

private:
	std::unordered_map<std::string, SoundData*> m_SoundFileMap;		// ����
	std::vector<JChannel*> m_ChannelVec;
	int m_ChannelCount;

	std::unordered_map<std::string, std::string> m_SoundFilePathAndName;

public:
	void LoadSoundFile(std::string folderPath);
	void CreateAllSound(std::unordered_map<std::string, std::string>& filePathAndName);

public:
	void Initialize(int channelCount);

	void Play(int channelNum, std::string soundName);
	void Pause(int channelNum, std::string soundName);
	void Resume(int channelNum, std::string soundName);
	void Stop(int channelNum, std::string soundName);
	void VolumeUp(int channelNum);
	void VolumeDown(int channelNum);
	void Update();

private:
	void CreateSound(std::string path, std::string soundName, SoundInfo info);
	void CreateChannel();

public:
	static void DeleteInstance()
	{
		if (m_Instance != nullptr)
		{
			delete m_Instance;
			m_Instance = nullptr;
		}
	}
};