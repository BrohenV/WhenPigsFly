#include <map>
#include <string>
#include <SFML/Audio/Music.hpp>

using String = std::string;
class MusicPlayer
{

private:
    MusicPlayer();
    ~MusicPlayer() = default;

public:
    static MusicPlayer& getInstance();

    // no copy or move for singleton
    MusicPlayer(const MusicPlayer&) = delete;
    MusicPlayer(MusicPlayer&&) = delete;
    MusicPlayer& operator=(const MusicPlayer&) = delete;
    MusicPlayer& operator=(MusicPlayer&&) = delete;

    void							play(String theme);
    void							stop();
    void							setPaused(bool paused);
    void							setVolume(float volume);


private:
    sf::Music						m_music;
    std::map<String, String>	    m_filenames;
    float							m_volume{ 25 };
};