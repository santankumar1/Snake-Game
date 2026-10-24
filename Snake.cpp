#include "Snake.hpp"

Snake::Snake()
{
}

Snake::~Snake()
{
}

void Snake::Init(const sf::Texture &texture)
{
    m_body.clear();
    float x = 16.f;
    for (int i = 0; i < 3; ++i)
    {
        m_body.emplace_back(texture);
        auto &piece = m_body.back();
        piece.setPosition({x, 16.f});
        x += 16.f;
    }
    m_head = --m_body.end();
    m_tail = m_body.begin();
}

void Snake::Move(const sf::Vector2f &direction)
{
    m_tail->setPosition(m_head->getPosition() + direction);
    m_head = m_tail;
    ++m_tail;

    if (m_tail == m_body.end())
    {
        m_tail = m_body.begin();
    }
}

bool Snake::IsOn(const sf::Sprite &other) const
{
    return other.getGlobalBounds().findIntersection(m_head->getGlobalBounds()).has_value();
}

void Snake::Grow(const sf::Vector2f &direction)
{
    sf::Sprite newPiece = *m_body.begin();
    newPiece.setPosition(m_head->getPosition() + direction);

    m_head = m_body.insert(++m_head, newPiece);
}

bool Snake::IsSelfIntersecting() const
{
    bool flag = false;

    for (auto piece = m_body.begin(); piece != m_body.end(); ++piece)
    {
        if (m_head != piece)
        {
            flag = IsOn(*piece);

            if (flag)
            {
                break;
            }
        }
    }

    return flag;
}

void Snake::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto &piece : m_body)
    {
        target.draw(piece);
    }
}