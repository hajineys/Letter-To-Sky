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

	// ���� ���ϵ� ��� ������
	for (auto it = m_SoundFileMap.begin(); it != m_SoundFileMap.end(); it++)
	{
		SafeDelete(it->second);
	}

	// ä���� �����Ѵ�.
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

	// ä���� �������ش�.
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

		// Ȯ���� �˻�
		if (fileExt == _fileFilter)
		{
			std::string fileName = filePath.substr(filePath.rfind("\\") + 1, filePath.rfind("."));
				for (int i = 0; i < 4; i++)
					fileName.pop_back();    // ���� Ȯ���� ����

			m_SoundFilePathAndName[filePath] = fileName;
		}
	}

	// ���带 ����
	CreateAllSound(m_SoundFilePathAndName);
}

void SoundManager::CreateAllSound(std::unordered_map<std::string, std::string>& filePathAndName)
{
	for (auto it : filePathAndName)
	{
		// FMOD ���� ���� ����
		// BGM����(Loop�� ������) üũ
		std::string _fileAttr;
		std::string _IsBGM = "BGM";
		_fileAttr = it.second.substr(0, 3);

		// ���� ������ �����ϰ� map�� �ִ´�.
		if (_fileAttr == _IsBGM)
			CreateSound(it.first, it.second, SoundInfo::Loop);
		else
			CreateSound(it.first, it.second, SoundInfo::None);
	}
}

void SoundManager::CreateSound(std::string path, std::string soundName, SoundInfo info)
{
	// ������ �ƴ϶��
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

// �� ���� �÷��̵Ǵ� ���� ������ �Ѵ�. ��� �÷��� �Ϸ��� Stop() -> Play()
void SoundManager::Play(int channelNum, std::string soundName)
{
	// ä���� ����� �Է��ߴٸ�
	if (channelNum < m_ChannelVec.size())
	{
		// �̸��� �´� ���尡 �ִٸ� 
		if (m_SoundFileMap[soundName])
		{
			// ���尡 ���� �÷��̵��� �ʾҴٸ�
			if (m_SoundFileMap[soundName]->IsPlayed == false)
			{
				FMOD_System_PlaySound(FmodSoundSystem, m_SoundFileMap[soundName]->Sound, NULL, false, &m_ChannelVec[channelNum]->FmodChannel);
				m_SoundFileMap[soundName]->IsPlayed = true;
			}
		}
		else
		{
			// �߸��� �� ����
			m_SoundFileMap.erase(soundName);
		}
	}
}

void SoundManager::Pause(int channelNum, std::string soundName)
{
	// ä���� ����� �Է��ߴٸ�
	if (channelNum < m_ChannelVec.size())
	{
		// �̸��� �´� ���尡 �ִٸ� 
		if (m_SoundFileMap[soundName])
		{
			// ���尡 �̹� �÷��� �Ǿ��ٸ�
			if (m_SoundFileMap[soundName]->IsPlayed == true)
			{
				FMOD_Channel_SetPaused(m_ChannelVec[channelNum]->FmodChannel, true);
			}
		}
		else
		{
			// �߸��� �� ����
			m_SoundFileMap.erase(soundName);
		}
	}
}

void SoundManager::Resume(int channelNum, std::string soundName)
{
	// ä���� ����� �Է��ߴٸ�
	if (channelNum < m_ChannelVec.size())
	{
		// �̸��� �´� ���尡 �ִٸ� 
		if (m_SoundFileMap[soundName])
		{
			// ���尡 �̹� �÷��� �Ǿ��ٸ�
			if (m_SoundFileMap[soundName]->IsPlayed == true)
			{
				FMOD_Channel_SetPaused(m_ChannelVec[channelNum]->FmodChannel, false);
			}
		}
		else
		{
			// �߸��� �� ����
			m_SoundFileMap.erase(soundName);
		}
	}
}

void SoundManager::Stop(int channelNum, std::string soundName)
{
	// ä���� ����� �Է��ߴٸ�
	if (channelNum < m_ChannelVec.size())
	{
		// �̸��� �´� ���尡 �ִٸ� 
		if (m_SoundFileMap[soundName])
		{
			// ���尡 �̹� �÷��� �Ǿ��ٸ�
			if (m_SoundFileMap[soundName]->IsPlayed == true)
			{
				FMOD_Channel_Stop(m_ChannelVec[channelNum]->FmodChannel);
				m_SoundFileMap[soundName]->IsPlayed = false;
			}
		}
		else
		{
			// �߸��� �� ����
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
	// ä���� �÷��� ���� ��츸 �ش� ä���� ������Ʈ���ش�.
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