#include "MainMenu.hpp"
#include "GamePlay.hpp"

#include <SFML/Window/Event.hpp>

namespace
{
std::shared_ptr<Context> PrepareContext(std::shared_ptr<Context> &context)
{
        context->m_assets->AddFont(MAIN_FONT, "assets/fonts/Pacifico-Regular.ttf");
        return context;
}
}

MainMenu::MainMenu(std::shared_ptr<Context> &context)
        : m_context(PrepareContext(context)),
            m_gameTitle(m_context->m_assets->GetFont(MAIN_FONT)),
            m_playButton(m_context->m_assets->GetFont(MAIN_FONT)),
            m_exitButton(m_context->m_assets->GetFont(MAIN_FONT)),
            m_isPlayButtonSelected(true),
      m_isPlayButtonPressed(false), m_isExitButtonSelected(false),
    m_isExitButtonPressed(false)
{
}

MainMenu::~MainMenu()

{
}

void MainMenu::Init()
{
    // Title
    m_gameTitle.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_gameTitle.setString("Snake Game");
    m_gameTitle.setOrigin({m_gameTitle.getLocalBounds().size.x / 2,
                          m_gameTitle.getLocalBounds().size.y / 2});
    m_gameTitle.setPosition({static_cast<float>(m_context->m_window->getSize().x) / 2.f,
                              static_cast<float>(m_context->m_window->getSize().y) / 2.f - 150.f});

    // Play Button
    m_playButton.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_playButton.setString("PLAY");
    m_playButton.setOrigin({m_playButton.getLocalBounds().size.x / 2,
                            m_playButton.getLocalBounds().size.y / 2});
    m_playButton.setPosition({static_cast<float>(m_context->m_window->getSize().x) / 2.f,
                              static_cast<float>(m_context->m_window->getSize().y) / 2.f - 25.f});
    m_playButton.setCharacterSize(20);

    // Exit Button
    m_exitButton.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_exitButton.setString("EXIT");
    m_exitButton.setOrigin({m_exitButton.getLocalBounds().size.x / 2,
                           m_exitButton.getLocalBounds().size.y / 2});
    m_exitButton.setPosition({static_cast<float>(m_context->m_window->getSize().x) / 2.f,
                              static_cast<float>(m_context->m_window->getSize().y) / 2.f + 25.f});
    m_exitButton.setCharacterSize(20);
}

void MainMenu::ProcessInput()
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
            case sf::Keyboard::Key::Up:
            {
                if (!m_isPlayButtonSelected)
                {
                    m_isPlayButtonSelected = true;
                    m_isExitButtonSelected = false;
                }
                break;
            }
            case sf::Keyboard::Key::Down:
            {
                if (!m_isExitButtonSelected)
                {
                    m_isPlayButtonSelected = false;
                    m_isExitButtonSelected = true;
                }
                break;
            }
            case sf::Keyboard::Key::Enter:
            {
                m_isPlayButtonPressed = false;
                m_isExitButtonPressed = false;

                if (m_isPlayButtonSelected)
                {
                    m_isPlayButtonPressed = true;
                }
                else
                {
                    m_isExitButtonPressed = true;
                }

                break;
            }
            default:
            {
                break;
            }
            }
        }
    }
}

void MainMenu::Update(const sf::Time &deltaTime)
{
    if (m_isPlayButtonSelected)
    {
        m_playButton.setFillColor(sf::Color(200, 12, 167));
        m_exitButton.setFillColor(sf::Color::White);
    }
    else
    {
        m_exitButton.setFillColor(sf::Color(200, 12, 167));
        m_playButton.setFillColor(sf::Color::White);
    }

    if (m_isPlayButtonPressed)
    {
        m_context->m_states->Add(std::make_unique<GamePlay>(m_context), true);
    }
    else if (m_isExitButtonPressed)
    {
        m_context->m_states->PopAll();
    }
}

void MainMenu::Draw()
{
    m_context->m_window->clear(sf::Color::Blue);
    m_context->m_window->draw(m_gameTitle);
    m_context->m_window->draw(m_playButton);
    m_context->m_window->draw(m_exitButton);
    m_context->m_window->display();
}