#include "MusicPlayer.h"
#include <stdexcept>


MusicPlayer::MusicPlayer() {
    m_filenames["gameTheme"] = "../assets/Music/GAMEPLAY-MUSIC.ogg";
    m_filenames["menuTheme"] = "../assets/Music/MENU-MUSIC.ogg";
    m_filenames["winTheme"] = "../assets/Music/VICTORY-MUSIC.ogg";
    m_filenames["loseTheme"] = "../assets/Music/DEATH-MUSIC.ogg";

}


MusicPlayer& MusicPlayer::getInstance() {
    static MusicPlayer instance;
    return instance;
}


void MusicPlayer::play(String theme) {
    if (!m_music.openFromFile(m_filenames[theme]))
        throw std::runtime_error("Music could not open file");

    m_music.setVolume(m_volume);
    m_music.setLoop(true);
    m_music.play();
}


void MusicPlayer::stop() {
    m_music.stop();
}


void MusicPlayer::setPaused(bool paused) {
    if (paused)
        m_music.pause();
    else
        m_music.play();
}


void MusicPlayer::setVolume(float volume) {
    m_volume = volume;
    m_music.setVolume(m_volume);
}