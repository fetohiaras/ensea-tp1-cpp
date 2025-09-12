#include "SoundManager.hpp"
#include <iostream>

SoundManager::SoundManager() : isLoaded(false) {}

bool SoundManager::loadAndPlay(const std::string& filePath) {
    if (buffer.loadFromFile(filePath)) {
        sound.setBuffer(buffer);
        isLoaded = true;
        sound.play(); // Joue automatiquement !
        std::cout << "Son de démarrage joué: " << filePath << std::endl;
        return true;
    }

    std::cerr << "Erreur: Impossible de charger le son: " << filePath << std::endl;
    return false;
}

void SoundManager::stop() {
    sound.stop();
}

void SoundManager::setVolume(float volume) {
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;
    sound.setVolume(volume);
}

bool SoundManager::isPlaying() const {
    return sound.getStatus() == sf::Sound::Playing;
}