#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cmath>
#include <string>

namespace Config
{
    constexpr unsigned int width = 800;
    constexpr unsigned int height = 600;

    constexpr float fieldWidth = static_cast<float>(width);
    constexpr float fieldHeight = static_cast<float>(height);

    constexpr float paddleWidth = 16.f;
    constexpr float paddleHeight = 100.f;
    constexpr float paddleMargin = 40.f;

    constexpr float playerSpeed = 340.f;
    constexpr float computerSpeed = 190.f;

    constexpr float ballRadius = 10.f;
    constexpr float serveSpeedX = 260.f;
    constexpr float serveSpeedY = 180.f;

    constexpr float fixedStep = 1.f / 120.f;
    constexpr float maximumFrameTime = 0.1f;
}

void movePaddle(sf::RectangleShape& paddle, float distance)
{
    sf::Vector2f position = paddle.getPosition();

    position.y = std::clamp(
        position.y + distance,
        0.f,
        Config::fieldHeight - paddle.getSize().y
    );

    paddle.setPosition(position);
}

float readPlayerDirection()
{
    float direction = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        direction -= 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        direction += 1.f;
    }

    return direction;
}

void updateComputer(
    sf::RectangleShape& paddle,
    const sf::CircleShape& ball,
    float deltaTime
)
{
    const float paddleCenter =
        paddle.getPosition().y + paddle.getSize().y / 2.f;

    const float difference =
        ball.getPosition().y - paddleCenter;

    const float maximumMove =
        Config::computerSpeed * deltaTime;

    movePaddle(
        paddle,
        std::clamp(difference, -maximumMove, maximumMove)
    );
}

sf::FloatRect getBallBounds(const sf::CircleShape& ball)
{
    const sf::Vector2f center = ball.getPosition();
    const float radius = ball.getRadius();

    return sf::FloatRect(
        { center.x - radius, center.y - radius },
        { radius * 2.f, radius * 2.f }
    );
}

// Returns 1 if the player scores, -1 if the computer scores,
// and 0 if the rally continues.
int updateBall(
    sf::CircleShape& ball,
    sf::Vector2f& velocity,
    const sf::RectangleShape& player,
    const sf::RectangleShape& computer,
    float deltaTime
)
{
    sf::Vector2f position = ball.getPosition();

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    const float radius = ball.getRadius();

    // Top and bottom walls.
    if (position.y - radius <= 0.f && velocity.y < 0.f)
    {
        position.y = radius;
        velocity.y = std::abs(velocity.y);
    }
    else if (
        position.y + radius >= Config::fieldHeight
        && velocity.y > 0.f
        )
    {
        position.y = Config::fieldHeight - radius;
        velocity.y = -std::abs(velocity.y);
    }

    ball.setPosition(position);

    // Left paddle.
    if (
        velocity.x < 0.f
        && getBallBounds(ball).findIntersection(
            player.getGlobalBounds()
        ).has_value()
        )
    {
        position.x =
            player.getPosition().x
            + player.getSize().x
            + radius;

        velocity.x = std::abs(velocity.x);
        ball.setPosition(position);
    }

    // Right paddle.
    if (
        velocity.x > 0.f
        && getBallBounds(ball).findIntersection(
            computer.getGlobalBounds()
        ).has_value()
        )
    {
        position.x = computer.getPosition().x - radius;

        velocity.x = -std::abs(velocity.x);
        ball.setPosition(position);
    }

    if (position.x + radius < 0.f)
    {
        return -1;
    }

    if (position.x - radius > Config::fieldWidth)
    {
        return 1;
    }

    return 0;
}

void refreshTitle(
    sf::RenderWindow& window,
    int playerScore,
    int computerScore,
    bool waitingForServe
)
{
    std::string title =
        "Pong | Player: " + std::to_string(playerScore)
        + " Computer: " + std::to_string(computerScore)
        + " | Up/Down: Move | Esc: Exit";

    title += waitingForServe
        ? " | Space: Serve"
        : " | Rally in progress";

    window.setTitle(title);
}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ Config::width, Config::height }),
        "Pong",
        sf::Style::Titlebar | sf::Style::Close
    );

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    const float initialPaddleY =
        (Config::fieldHeight - Config::paddleHeight) / 2.f;

    sf::RectangleShape player(
        { Config::paddleWidth, Config::paddleHeight }
    );

    player.setPosition(
        { Config::paddleMargin, initialPaddleY }
    );
    player.setFillColor(sf::Color(70, 190, 240));

    sf::RectangleShape computer(
        { Config::paddleWidth, Config::paddleHeight }
    );

    computer.setPosition({
        Config::fieldWidth
            - Config::paddleMargin
            - Config::paddleWidth,
        initialPaddleY
        });
    computer.setFillColor(sf::Color(240, 150, 80));

    sf::RectangleShape centerLine(
        { 2.f, Config::fieldHeight }
    );

    centerLine.setPosition(
        { Config::fieldWidth / 2.f - 1.f, 0.f }
    );
    centerLine.setFillColor(sf::Color(55, 65, 80));

    sf::CircleShape ball(Config::ballRadius);
    ball.setOrigin(
        { Config::ballRadius, Config::ballRadius }
    );
    ball.setFillColor(sf::Color::White);

    const sf::Vector2f center{
        Config::fieldWidth / 2.f,
        Config::fieldHeight / 2.f
    };

    ball.setPosition(center);

    sf::Vector2f ballVelocity{ 0.f, 0.f };

    int playerScore = 0;
    int computerScore = 0;

    bool waitingForServe = true;

    float nextServeX = -Config::serveSpeedX;
    float nextServeY = Config::serveSpeedY;

    float accumulator = 0.f;
    sf::Clock frameClock;

    refreshTitle(
        window, playerScore, computerScore, waitingForServe
    );

    while (window.isOpen())
    {
        const float frameTime = std::min(
            frameClock.restart().asSeconds(),
            Config::maximumFrameTime
        );

        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* key =
                event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
                else if (
                    key->code == sf::Keyboard::Key::Space
                    && waitingForServe
                    && window.hasFocus()
                    )
                {
                    ballVelocity = { nextServeX, nextServeY };
                    nextServeY = -nextServeY;
                    waitingForServe = false;

                    // Begin the new rally without old time debt.
                    accumulator = 0.f;

                    refreshTitle(
                        window,
                        playerScore,
                        computerScore,
                        waitingForServe
                    );
                }
            }
        }

        if (!window.isOpen())
        {
            break;
        }

        if (window.hasFocus())
        {
            accumulator += frameTime;

            const float playerDirection =
                readPlayerDirection();

            while (accumulator >= Config::fixedStep)
            {
                accumulator -= Config::fixedStep;

                movePaddle(
                    player,
                    playerDirection
                    * Config::playerSpeed
                    * Config::fixedStep
                );

                updateComputer(
                    computer, ball, Config::fixedStep
                );

                if (!waitingForServe)
                {
                    const int point = updateBall(
                        ball,
                        ballVelocity,
                        player,
                        computer,
                        Config::fixedStep
                    );

                    if (point != 0)
                    {
                        if (point > 0)
                        {
                            ++playerScore;
                            nextServeX = Config::serveSpeedX;
                        }
                        else
                        {
                            ++computerScore;
                            nextServeX = -Config::serveSpeedX;
                        }

                        ball.setPosition(center);
                        ballVelocity = { 0.f, 0.f };
                        waitingForServe = true;

                        refreshTitle(
                            window,
                            playerScore,
                            computerScore,
                            waitingForServe
                        );

                        accumulator = 0.f;
                        break;
                    }
                }
            }
        }
        else
        {
            accumulator = 0.f;
        }

        window.clear(sf::Color(20, 30, 45));
        window.draw(centerLine);
        window.draw(player);
        window.draw(computer);
        window.draw(ball);
        window.display();
    }

    return 0;
}
