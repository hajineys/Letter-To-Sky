#include "SoundManager.h"
#include "DirectXDefine.h"
#include <filesystem>

FMOD_SYSTEM* SoundManager::FmodSoundSystem;

SoundManager* SoundManager::m_Instance = nullptr;

SoundManager::SoundManager()
	:m_ChannelCount(0)
{

}

SoundManager::~SoundManager()
{
	FMOD_System_Close(FmodSoundSystem);
	FMOD_System_Release(FmodSoundSystem);

	// 사운드 파일들 모두 릴리즈
	for (auto it = m_SoundFileMap.begin(); it != m_SoundFileMap.end(); it++)
	{
		SafeDelete(it->second);
	}

	// 채널을 삭제한다.
	for (int i = 0; i < m_ChannelCount; i++)
	{
		delete m_ChannelVec[i];
		m_ChannelVec[i] = nullptr;
	}

	m_Instance->DeleteInstance();
}

void SoundManager::Initialize(int channelCount)
{
	FMOD_System_Create(&FmodSoundSystem);
	FMOD_System_Init(FmodSoundSystem, 32, FMOD_INIT_NORMAL, NULL);

	m_ChannelCount = channelCount;

	// 채널을 생성해준다.
	for (int i = 0; i < m_ChannelCount; i++)
	{
		CreateChannel();
	}
}

void SoundManager::CreateChannel()
{
	JChannel* _channel = new JChannel();

	m_ChannelVec.push_back(_channel);
}

void SoundManager::LoadSoundFile(std::string folderPath)
{
	const std::filesystem::path _folderPath{ folderPath.c_str() };
	std::filesystem::create_directories(_folderPath);

	std::string _fileFilter = ".mp3";

	int order = 0;
	for (const auto& file : std::filesystem::directory_iterator{ _folderPath })
	{
		std::wstring filePathW = file.path();

		// wstring to string
		std::string filePath;
		filePath.assign(filePathW.begin(), filePathW.end());

		std::string fileExt = filePath.substr(filePath.size() - 4, 4);

		// 확장자 검사
		if (fileExt == _fileFilter)
		{
			std::string fileName = filePath.substr(filePath.rfind("\\") + 1, filePath.rfind("."));
				for (int i = 0; i < 4; i++)
					fileName.pop_back();    // 파일 확장자 제거

			m_SoundFilePathAndName[filePath] = fileName;
		}
	}

	// 사운드를 생성
	CreateAllSound(m_SoundFilePathAndName);
}

void SoundManager::CreateAllSound(std::unordered_map<std::string, std::string>& filePathAndName)
{
	for (auto it : filePathAndName)
	{
		// FMOD 사운드 파일 생성
		// BGM인지(Loop할 것인지) 체크
		std::string _fileAttr;
		std::string _IsBGM = "BGM";
		_fileAttr = it.second.substr(0, 3);

		// 사운드 파일을 생성하고 map에 넣는다.
		if (_fileAttr == _IsBGM)
			CreateSound(it.first, it.second, SoundInfo::Loop);
		else
			CreateSound(it.first, it.second, SoundInfo::None);
	}
}

void SoundManager::CreateSound(std::string path, std::string soundName, SoundInfo info)
{
	// 공백이 아니라면
	if (1 == path.compare("") && 1 == soundName.compare(""))
	{
		SoundData* _newSound = nullptr;
		_newSound = new SoundData;

		if (info == SoundInfo::Loop)
		{
			FMOD_System_CreateSound(FmodSoundSystem, path.c_str(), FMOD_LOOP_NORMAL, 0, &_newSound->Sound);
		}
		else
		{
			FMOD_System_CreateSound(FmodSoundSystem, path.c_str(), FMOD_LOOP_OFF, 0, &_newSound->Sound);
		}

		if (_newSound->Sound != nullptr)
			m_SoundFileMap[soundName] = _newSound;
	}
}

// 한 번만 플레이되는 것을 전제로 한다. 계속 플레이 하려면 Stop() -> Play()
void SoundManager::Play(int channelNum, std::string soundName)
{
	// 채널을 제대로 입력했다면
	if (channelNum < m_ChannelVec.size())
	{
		// 이름에 맞는 사운드가 있다면 
		if (m_SoundFileMap[soundName])
		{
			// 사운드가 아직 플레이되지 않았다면
			if (m_SoundFileMap[soundName]->IsPlayed == false)
			{
				FMOD_System_PlaySound(FmodSoundSystem, m_SoundFileMap[soundName]->Sound, NULL, false, &m_ChannelVec[channelNum]->FmodChannel);
				m_SoundFileMap[soundName]->IsPlayed = true;
			}
		}
		else
		{
			// 잘못들어간 것 제거
			m_SoundFileMap.erase(soundName);
		}
	}
}

void SoundManager::Pause(int channelNum, std::string soundName)
{
	// 채널을 제대로 입력했다면
	if (channelNum < m_ChannelVec.size())
	{
		// 이름에 맞는 사운드가 있다면 
		if (m_SoundFileMap[soundName])
		{
			// 사운드가 이미 플레이 되었다면
			if (m_SoundFileMap[soundName]->IsPlayed == true)
			{
				FMOD_Channel_SetPaused(m_ChannelVec[channelNum]->FmodChannel, true);
			}
		}
		else
		{
			// 잘못들어간 것 제거
			m_SoundFileMap.erase(soundName);
		}
	}
}

void SoundManager::Resume(int channelNum, std::string soundName)
{
	// 채널을 제대로 입력했다면
	if (channelNum < m_ChannelVec.size())
	{
		// 이름에 맞는 사운드가 있다면 
		if (m_SoundFileMap[soundName])
		{
			// 사운드가 이미 플레이 되었다면
			if (m_SoundFileMap[soundName]->IsPlayed == true)
			{
				FMOD_Channel_SetPaused(m_ChannelVec[channelNum]->FmodChannel, false);
			}
		}
		else
		{
			// 잘못들어간 것 제거
			m_SoundFileMap.erase(soundName);
		}
	}
}

void SoundManager::Stop(int channelNum, std::string soundName)
{
	// 채널을 제대로 입력했다면
	if (channelNum < m_ChannelVec.size())
	{
		// 이름에 맞는 사운드가 있다면 
		if (m_SoundFileMap[soundName])
		{
			// 사운드가 이미 플레이 되었다면
			if (m_SoundFileMap[soundName]->IsPlayed == true)
			{
				FMOD_Channel_Stop(m_ChannelVec[channelNum]->FmodChannel);
				m_SoundFileMap[soundName]->IsPlayed = false;
			}
		}
		else
		{
			// 잘못들어간 것 제거
			m_SoundFileMap.erase(soundName);
		}
	}
}

void SoundManager::VolumeUp(int channelNum)
{
	if (channelNum < m_ChannelVec.size())
	{
		m_ChannelVec[channelNum]->Volume += 0.1f;

		FMOD_Channel_SetVolume(m_ChannelVec[channelNum]->FmodChannel, m_ChannelVec[channelNum]->Volume);
	}
}

void SoundManager::VolumeDown(int channelNum)
{
	if (channelNum < m_ChannelVec.size())
	{
		if (m_ChannelVec[channelNum]->Volume > SOUND_MIN)
		{
			m_ChannelVec[channelNum]->Volume -= 0.1f;
		}

		FMOD_Channel_SetVolume(m_ChannelVec[channelNum]->FmodChannel, m_ChannelVec[channelNum]->Volume);
	}
}

void SoundManager::Update()
{
	// 채널이 플레이 중일 경우만 해당 채널을 업데이트해준다.
	for (int i = 0; i < m_ChannelVec.size(); i++)
	{
		FMOD_Channel_IsPlaying(m_ChannelVec[i]->FmodChannel, &m_ChannelVec[i]->IsPlaying);

		if (m_ChannelVec[i]->IsPlaying)
		{
			FMOD_System_Update(FmodSoundSystem);
			FMOD_Channel_SetVolume(m_ChannelVec[i]->FmodChannel, m_ChannelVec[i]->Volume);
		}
	}
}