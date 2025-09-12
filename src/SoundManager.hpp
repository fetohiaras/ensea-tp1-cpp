#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <SFML/Audio.hpp>
#include <string>

class SoundManager {
private:
    sf::SoundBuffer buffer;
    sf::Sound sound;
    bool isLoaded;

public:
    SoundManager();

    // Charger et jouer automatiquement
    bool loadAndPlay(const std::string& filePath);

    void stop();
    void setVolume(float volume);
    bool isPlaying() const;
};

#endif // SOUNDMANAGER_HPP