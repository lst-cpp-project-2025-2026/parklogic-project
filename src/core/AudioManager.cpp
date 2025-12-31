#include "core/AudioManager.hpp"
#include "core/AssetManager.hpp"
#include "core/Logger.hpp"

#include "ui/UIButton.hpp"
#include "config.hpp"

AudioManager::AudioManager() = default;
AudioManager::~AudioManager() = default;

void AudioManager::InitUI(std::shared_ptr<EventBus> bus) {
    muteButton = std::make_unique<UIButton>(
        Vector2{(float)Config::LOGICAL_WIDTH - 75.0f, (float)Config::LOGICAL_HEIGHT - 75.0f}, 
        Vector2{55.0f, 55.0f}, "", bus);
    
    muteButton->setOnClick([this]() { this->ToggleMute(); });
    Logger::Info("AudioManager: Mute button initialized.");
}

void AudioManager::DrawUI() {
    if (muteButton) {
        muteButton->draw();
        DrawVolumeIcon({(float)Config::LOGICAL_WIDTH - 75.0f, (float)Config::LOGICAL_HEIGHT - 75.0f}, isMuted);
    }
}

void AudioManager::DrawVolumeIcon(Vector2 pos, bool muted) {
    auto& AM = AssetManager::Get();
    std::string texName = muted ? "sound_off" : "sound_on";
    Texture2D tex = AM.GetTexture(texName);

    if (tex.id > 0) {
        float iconSize = 32.0f;
        float centerX = pos.x + (55.0f - iconSize) / 2.0f;
        float centerY = pos.y + (55.0f - iconSize) / 2.0f;

        Rectangle source = {0.0f, 0.0f, (float)tex.width, (float)tex.height};
        Rectangle dest = {centerX, centerY, iconSize, iconSize};

        ::DrawTexturePro(tex, source, dest, {0, 0}, 0.0f, WHITE);
    }
}

void AudioManager::PlayMusic(const std::string& name) {
    auto& AM = AssetManager::Get();
    Music mus = AM.GetMusic(name);
    if (mus.stream.buffer != nullptr) {
        currentMusicName = name;
        ::PlayMusicStream(mus);
        ::SetMusicVolume(mus, isMuted ? 0.0f : 0.1f);
        Logger::Info("Playing music: {}", name);
    }
}

void AudioManager::Update() {
    if (currentMusicName.empty()) return;

    auto& AM = AssetManager::Get();
    Music mus = AM.GetMusic(currentMusicName);
    if (mus.stream.buffer != nullptr) {
        ::UpdateMusicStream(mus);
    }
}

void AudioManager::ToggleMute() {
    isMuted = !isMuted;
    float volume = isMuted ? 0.0f : masterVolume;
    ::SetMasterVolume(volume);
    
    if (!currentMusicName.empty()) {
        auto& AM = AssetManager::Get();
        Music mus = AM.GetMusic(currentMusicName);
        if (mus.stream.buffer != nullptr) {
             ::SetMusicVolume(mus, isMuted ? 0.0f : 0.1f);
        }
    }

    Logger::Info("Audio state: {}", isMuted ? "Muted" : "Unmuted");
}

void AudioManager::SetVolume(float volume) {
    masterVolume = volume;
    if (!isMuted) {
        ::SetMasterVolume(masterVolume);
    }
}

void AudioManager::PlaySound(const std::string& name, float pitch) {
    if (isMuted) return;

    auto& AM = AssetManager::Get();
    Sound snd = AM.GetSound(name);
    if (snd.frameCount > 0) {
        ::SetSoundPitch(snd, pitch);
        ::PlaySound(snd);
    }
}
