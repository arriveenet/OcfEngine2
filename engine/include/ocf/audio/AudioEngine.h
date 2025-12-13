#pragma once
#include <string>
#include <memory>
#include <unordered_map>

namespace ocf {

class AudioEngineImpl;

using AUDIO_ID = int;

class AudioEngine {
public:
	static const int AUDIO_ID_INVALID;
	static const float TIME_UNKNOWN;

	static bool lazyInit();
	static void end();

	static void setLoop(AUDIO_ID audioID, bool loop);

	static bool isLoop(AUDIO_ID audioID);

	static void setVolume(AUDIO_ID audioID, float volume);

	static float getVolume(AUDIO_ID audioID);

	static AUDIO_ID play2d(std::string_view filename, bool loop = false, float volume = 1.0f);

	static void stop(AUDIO_ID audioID);

	static void stopAll();

	static void pause(AUDIO_ID audioID);

	static void pauseAll();

	static void resume(AUDIO_ID audioID);

	static void resumeAll();

	static void uncache(std::string_view filePath);

	static void uncacheAll();

private:
	struct AudioInfo {
		std::string filePath;

		float volume;
		bool loop;
		float duration;

		AudioInfo();
		~AudioInfo();
	};

	static std::unordered_map<AUDIO_ID, AudioInfo> m_audioIdInfoMap;

    static std::unordered_map<std::string, AUDIO_ID> m_audioPathIdMap;

	static AudioEngineImpl* m_pAudioEngineImpl;
};

} // namespace ocf