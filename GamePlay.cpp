#include <GamePlay.hpp>
#include <GameOver.hpp>
#include <PauseGame.hpp>

#include <SFML/Window/Event.hpp>

#include <stdlib.h>
#include <time.h>

namespace
{
std::shared_ptr<Context> &LoadGameplayTextures(std::shared_ptr<Context> &context)
{
    context->m_assets->AddTexture(GRASS, "assets/textures/grass.png", true);
    context->m_assets->AddTexture(FOOD, "assets/textures/food.png");
    context->m_assets->AddTexture(WALL, "assets/textures/wall.png", true);
    context->m_assets->AddTexture(SNAKE, "assets/textures/snake.png");
    return context;
}
}

GamePlay::GamePlay(std::shared_ptr<Context> &context)
    : m_context(LoadGameplayTextures(context)),
      m_grass(m_context->m_assets->GetTexture(GRASS)),
      m_walls{
          sf::Sprite(m_context->m_assets->GetTexture(WALL)),
          sf::Sprite(m_context->m_assets->GetTexture(WALL)),
          sf::Sprite(m_context->m_assets->GetTexture(WALL)),
          sf::Sprite(m_context->m_assets->GetTexture(WALL))},
      m_food(m_context->m_assets->GetTexture(FOOD)),
      m_score(0),
      m_scoreText(m_context->m_assets->GetFont(MAIN_FONT)),
      m_snakeDirection({16.f, 0.f}),
      m_elapsedTime(sf::Time::Zero),
      m_isPaused(false)
{
    srand(time(nullptr));
}

GamePlay::~GamePlay()
{
}

void GamePlay::Init()
{
    m_grass.setTexture(m_context->m_assets->GetTexture(GRASS));
    m_grass.setTextureRect(m_context->m_window->getViewport(m_context->m_window->getDefaultView()));

    for (auto &wall : m_walls)
    {
        wall.setTexture(m_context->m_assets->GetTexture(WALL));
    }

    m_walls[0].setTextureRect({{0, 0}, {static_cast<int>(m_context->m_window->getSize().x), 16}});
    m_walls[1].setTextureRect({{0, 0}, {static_cast<int>(m_context->m_window->getSize().x), 16}});
    m_walls[1].setPosition({0, static_cast<float>(m_context->m_window->getSize().y - 16.f)});

    m_walls[2].setTextureRect({{0, 0}, {16, static_cast<int>(m_context->m_window->getSize().y)}});
    m_walls[3].setTextureRect({{0, 0}, {16, static_cast<int>(m_context->m_window->getSize().y)}});
    m_walls[3].setPosition({static_cast<float>(m_context->m_window->getSize().x - 16.f), 0});

    m_food.setTexture(m_context->m_assets->GetTexture(FOOD));
    m_food.setPosition({static_cast<float>(m_context->m_window->getSize().x) / 2.f, static_cast<float>(m_context->m_window->getSize().y) / 2.f});

    m_snake.Init(m_context->m_assets->GetTexture(SNAKE));

    m_scoreText.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_scoreText.setString("Score : " + std::to_string(m_score));
    m_scoreText.setCharacterSize(15);
}

void GamePlay::ProcessInput()
{
    while (const auto event = m_context->m_window->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_context->m_states->PopAll();
        }
        
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            sf::Vector2f newDirection = m_snakeDirection;
            switch (keyPressed->code)
            {
            case sf::Keyboard::Key::Up:
                newDirection = {0.f, -16.f};
                break;
            case sf::Keyboard::Key::Down:
                newDirection = {0.f, 16.f};
                break;
            case sf::Keyboard::Key::Left:
                newDirection = {-16.f, 0.f};
                break;
            case sf::Keyboard::Key::Right:
                newDirection = {16.f, 0.f};
                break;
            case sf::Keyboard::Key::Space:
                m_context->m_states->Add(std::make_unique<PauseGame>(m_context));
                break;

            default:
                break;
            }

            if (std::abs(m_snakeDirection.x) != std::abs(newDirection.x) ||
                std::abs(m_snakeDirection.y) != std::abs(newDirection.y))
            {
                m_snakeDirection = newDirection;
            }
        }
    }
}

void GamePlay::Update(const sf::Time &deltaTime)
{
    if (!m_isPaused)
    {
        m_elapsedTime += deltaTime;

        if (m_elapsedTime.asSeconds() > 0.2f)
        {
            for (auto &wall : m_walls)
            {
                if (m_snake.IsOn(wall))
                {
                    m_context->m_states->Add(std::make_unique<GameOver>(m_context), true);
                    break;
                }
            }

            if (m_snake.IsOn(m_food))
            {
                m_snake.Grow(m_snakeDirection);

                int x = 0, y = 0;
                x = std::clamp<int>(rand() % m_context->m_window->getSize().x, 16, m_context->m_window->getSize().x - 2 * 16);
                y = std::clamp<int>(rand() % m_context->m_window->getSize().y, 16, m_context->m_window->getSize().y - 2 * 16);

                m_food.setPosition({static_cast<float>(x), static_cast<float>(y)});
                m_score += 1;
                m_scoreText.setString(std::string("Score : ") + std::to_string(m_score));
            }
            else
            {
                m_snake.Move(m_snakeDirection);
            }

            if (m_snake.IsSelfIntersecting())
            {
                m_context->m_states->Add(std::make_unique<GameOver>(m_context), true);
            }

            m_elapsedTime = sf::Time::Zero;
        }
    }
}

void GamePlay::Draw()
{
    m_context->m_window->clear();
    m_context->m_window->draw(m_grass);

    for (auto &wall : m_walls)
    {
        m_context->m_window->draw(wall);
    }
    m_context->m_window->draw(m_food);
    m_context->m_window->draw(m_snake);
    m_context->m_window->draw(m_scoreText);

    m_context->m_window->display();
}

void GamePlay::Pause()
{
    m_isPaused = true;
}

void GamePlay::Start()
{
    m_isPaused = false;
}