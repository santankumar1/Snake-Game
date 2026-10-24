#include "PauseGame.hpp"

#include <SFML/Window/Event.hpp>

PauseGame::PauseGame(std::shared_ptr<Context> &context)
        : m_context(context),
          m_pauseTitle(context->m_assets->GetFont(MAIN_FONT))
{
}

void PauseGame::Init()
{
    // Title
    m_pauseTitle.setString("Paused");
    m_pauseTitle.setOrigin({m_pauseTitle.getLocalBounds().size.x / 2.f,
                           m_pauseTitle.getLocalBounds().size.y / 2.f});
    m_pauseTitle.setPosition({static_cast<float>(m_context->m_window->getSize().x) / 2.f,
                              static_cast<float>(m_context->m_window->getSize().y) / 2.f});
}

void PauseGame::ProcessInput()
{
    while (const auto event = m_context->m_window->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_context->m_states->PopAll();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            switch (keyPressed->code)
            {
            case sf::Keyboard::Key::Space:
                m_context->m_states->PopCurrent();
                break;
            default:
                break;
            }
        }
    }
}

void PauseGame::Update(const sf::Time &deltaTime)
{
}

void PauseGame::Draw()
{
    m_context->m_window->draw(m_pauseTitle);
    m_context->m_window->display();
}