#include <SFML/Graphics/CircleShape.hpp>

#include "Game.hpp"
#include "MainMenu.hpp"

Game::Game() : m_context(std::make_shared<Context>())
{
    m_context->m_window->create(sf::VideoMode({640, 352}), "Snake Game", sf::Style::Default);
    m_context->m_states->Add(std::make_unique<MainMenu>(m_context));
    m_context->m_window->setFramerateLimit(60);
    m_context->m_window->setVerticalSyncEnabled(true);
}

Game::~Game()
{
}

void Game::Run()
{
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock clock;

    while (m_context->m_window->isOpen())
    {       
        sf::Time deltaTime = clock.restart();
        sf::Time timeSinceLastUpdate = sf::Time::Zero;
        timeSinceLastUpdate += deltaTime;

        m_context->m_states->ProcessStateChange();

        if (!m_context->m_states->IsEmpty())
        {
                m_context->m_states->GetCurrent()->ProcessInput();
                m_context->m_states->GetCurrent()->Update(deltaTime);
                m_context->m_states->GetCurrent()->Draw();
        }
        
        
        else
        {
                m_context->m_window->close();
        }
    }
}
