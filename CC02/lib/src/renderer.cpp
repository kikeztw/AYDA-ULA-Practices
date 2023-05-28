#include <renderer.hpp>

size_t RenderData::rows{};
size_t RenderData::cols{};
std::vector<char>*RenderData::env{nullptr};

#ifdef RENDER_MODE_GRAPHIC
sf::RenderTexture RenderData::render_texture{};
sf::Sprite RenderData::render_sprite{};
std::shared_ptr<sf::RenderWindow> RenderData::render_window{nullptr};
std::unordered_map<std::string, sf::Texture> RenderData::textures{};
std::unordered_map<std::string, sf::SoundBuffer> sound_buffers{};
std::unordered_map<std::string, sf::SoundBuffer> RenderData::sound_buffers{};
std::unordered_map<std::string, sf::Sound> RenderData::sounds{};
sf::Music RenderData::music{};
#endif

void setup_renderer()
{
    #ifdef RENDER_MODE_GRAPHIC
    fs::path resources_path{"resources"};

    sf::Texture texture{};
    texture.loadFromFile(resources_path / "textures" / "cracked_hole.png");
    RenderData::textures["cracked_hole"] = texture;

    texture.loadFromFile(resources_path / "textures" / "elf_down.png");
    RenderData::textures["elf_down"] = texture;

    texture.loadFromFile(resources_path / "textures" / "elf_left.png");
    RenderData::textures["elf_left"] = texture;

    texture.loadFromFile(resources_path / "textures" / "elf_right.png");
    RenderData::textures["elf_right"] = texture;

    texture.loadFromFile(resources_path / "textures" / "elf_up.png");
    RenderData::textures["elf_up"] = texture;

    texture.loadFromFile(resources_path / "textures" / "goal.png");
    RenderData::textures["goal"] = texture;

    texture.loadFromFile(resources_path / "textures" / "hole.png");
    RenderData::textures["hole"] = texture;

    texture.loadFromFile(resources_path / "textures" / "ice.png");
    RenderData::textures["ice"] = texture;

    texture.loadFromFile(resources_path / "textures" / "stool.png");
    RenderData::textures["stool"] = texture;

    sf::SoundBuffer buffer;
    sf::Sound sound;

    buffer.loadFromFile(resources_path / "sounds" / "ice_cracking.ogg");
    auto result = RenderData::sound_buffers.emplace("ice_cracking", buffer);
    sound.setBuffer(result.first->second);
    RenderData::sounds["ice_cracking"] = sound;

    buffer.loadFromFile(resources_path / "sounds" / "water_splash.ogg");
    result = RenderData::sound_buffers.emplace("ice_viwater_splashllage", buffer);
    sound.setBuffer(result.first->second);
    RenderData::sounds["water_splash"] = sound;

    buffer.loadFromFile(resources_path / "sounds" / "win.ogg");
    result = RenderData::sound_buffers.emplace("win", buffer);
    sound.setBuffer(result.first->second);
    RenderData::sounds["win"] = sound;

    RenderData::music.openFromFile(resources_path / "sounds" / "ice_village.ogg");
    RenderData::music.setLoop(true);
    RenderData::music.play();
    #endif
}

void update_renderer(std::vector<char>* env, size_t rows, size_t cols)
{
    RenderData::rows = rows;
    RenderData::cols = cols;
    RenderData::env = env;
    
    #ifdef RENDER_MODE_GRAPHIC
    sf::Vector2f scale_factors{2.f, 2.f};
    unsigned int virtual_width = cols * RenderData::TILE_SIZE;
    unsigned int virtual_height = rows * RenderData::TILE_SIZE;
    unsigned int window_width = virtual_width * scale_factors.x;
    unsigned int window_height = virtual_height * scale_factors.y;

    if (RenderData::render_window != nullptr)
    {
        RenderData::render_window->close();
    }

    RenderData::render_window = std::make_shared<sf::RenderWindow>(sf::VideoMode{window_width, window_height}, "Frozen Lake Environment");

    RenderData::render_texture.clear();
    RenderData::render_texture.create(virtual_width, virtual_height);
    RenderData::render_sprite = sf::Sprite{};
    RenderData::render_sprite.setTexture(RenderData::render_texture.getTexture());
    RenderData::render_sprite.setScale(scale_factors);
    #endif
}

void render_env(State state, Action action)
{
    #ifdef RENDER_MODE_GRAPHIC
    
    sf::Event event;

    // Dummy event handler to avoid window force quit
    while (RenderData::render_window->pollEvent(event)){}

    RenderData::render_texture.clear(sf::Color::Black);

    for (size_t i = 0; i < RenderData::rows; ++i)
    {
        for (size_t j = 0; j < RenderData::cols; ++j)
        {
            size_t index = i * RenderData::cols + j;

            float x = j * RenderData::TILE_SIZE;
            float y = i * RenderData::TILE_SIZE;

            switch ((*RenderData::env)[index])
            {
                case 'C':
                {
                    sf::Sprite sp{RenderData::textures["cracked_hole"]};
                    sp.setPosition(x, y);
                    RenderData::render_texture.draw(sp);
                    RenderData::sounds["ice_cracking"].play();
                    RenderData::sounds["water_splash"].play();
                    break;
                }
                case 'G':
                {
                    sf::Sprite sp1{RenderData::textures["ice"]};
                    sp1.setPosition(x, y);
                    RenderData::render_texture.draw(sp1);

                    if (state != index)
                    {
                        sf::Sprite sp2{RenderData::textures["goal"]};
                        sp2.setPosition(x, y);
                        RenderData::render_texture.draw(sp2);
                    }
                    else
                    {
                        RenderData::sounds["win"].play();
                    }

                    break;
                }
                case 'H':
                {
                    sf::Sprite sp{RenderData::textures["hole"]};
                    sp.setPosition(x, y);
                    RenderData::render_texture.draw(sp);
                    break;
                }
                case 'I':
                {
                    sf::Sprite sp{RenderData::textures["ice"]};
                    sp.setPosition(x, y);
                    RenderData::render_texture.draw(sp);
                    break;
                }
                case 'S':
                {
                    sf::Sprite sp1{RenderData::textures["ice"]};
                    sp1.setPosition(x, y);
                    RenderData::render_texture.draw(sp1);
                    sf::Sprite sp2{RenderData::textures["stool"]};
                    sp2.setPosition(x, y);
                    RenderData::render_texture.draw(sp2);
                    break;
                }
            }

            if (state == index && (*RenderData::env)[state] != 'C')
            {
                std::string texture_name{};

                switch (action)
                {
                    case Left:
                        texture_name = "elf_left";
                        break;
                    case Down:
                        texture_name = "elf_down";
                        break;
                    case Right:
                        texture_name = "elf_right";
                        break;
                    case Up:
                        texture_name = "elf_up";
                        break;
                    default: break;
                }

                sf::Sprite sp{RenderData::textures[texture_name]};
                sp.setPosition(x, y);
                RenderData::render_texture.draw(sp);
            }
        }
    }

    RenderData::render_texture.display();
    RenderData::render_window->draw(RenderData::render_sprite);
    RenderData::render_window->display();
    #else
    for (size_t i = 0; i < RenderData::rows; ++i)
    {
        for (size_t j = 0; j < RenderData::cols; ++j)
        {
            size_t index = i * RenderData::cols + j;
            if (index == state && (*RenderData::env)[index] != 'C')
            {
                std::cout << 'P';
            }
            else
            {
                std::cout << (*RenderData::env)[index];
            }
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
    #endif
}

void close_renderer()
{
    #ifdef RENDER_MODE_GRAPHIC
    RenderData::music.stop();
    RenderData::render_window->close();
    #endif
}