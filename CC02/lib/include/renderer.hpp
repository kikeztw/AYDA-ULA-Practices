#pragma once

#include <types.hpp>

#ifdef RENDER_MODE_GRAPHIC
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#else
#include <iostream>
#endif

struct RenderData
{
    static size_t rows;
    static size_t cols;
    static std::vector<char>* env;

    #ifdef RENDER_MODE_GRAPHIC
    static constexpr int TILE_SIZE{32};
    static sf::RenderTexture render_texture;
    static sf::Sprite render_sprite;
    static std::shared_ptr<sf::RenderWindow> render_window;
    static std::unordered_map<std::string, sf::Texture> textures;
    static std::unordered_map<std::string, sf::SoundBuffer> sound_buffers;
    static std::unordered_map<std::string, sf::Sound> sounds;
    static sf::Music music;
    #endif
};

void setup_renderer();

void update_renderer(std::vector<char>* env, size_t rows, size_t cols);

void render_env(State state, Action action);

void close_renderer();
