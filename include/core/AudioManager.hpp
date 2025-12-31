#pragma once
#include "raylib.h"
#include <string>
#include <memory>

/**
 * @file AudioManager.hpp
 * @brief Manages global music and sound effects, and the global mute UI.
 */
class UIButton;
class EventBus;

class AudioManager {
public:
    static AudioManager& Get() {
        static AudioManager instance;
        return instance;
    }

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    /**
     * @brief Initializes the global mute button and UI.
     * @param bus The event bus for the button.
     */
    void InitUI(std::shared_ptr<EventBus> bus);

    /**
     * @brief Renders the global mute button and volume icon.
     */
    void DrawUI();

    /**
     * @brief Plays background music.
     * @param name Name of the music asset in AssetManager.
     */
    void PlayMusic(const std::string& name);

    /**
     * @brief Updates the current music stream. Should be called every frame.
     */
    void Update();

    /**
     * @brief Toggles the mute state.
     */
    void ToggleMute();

    /**
     * @brief Sets the master volume.
     * @param volume Volume level (0.0 to 1.0).
     */
    void SetVolume(float volume);

    /**
     * @brief Plays a sound effect.
     * @param name Name of the sound asset in AssetManager.
     * @param pitch Optional pitch shift.
     */
    void PlaySound(const std::string& name, float pitch = 1.0f);

    bool IsMuted() const { return isMuted; }

private:
    AudioManager();
    ~AudioManager();
    void DrawVolumeIcon(Vector2 pos, bool muted);
    
    bool isMuted = false;
    std::string currentMusicName;
    float masterVolume = 0.5f;

    std::unique_ptr<UIButton> muteButton;
};
