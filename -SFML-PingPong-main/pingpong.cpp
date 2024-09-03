#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

const int windowWidth = 800;
const int windowHeight = 640;

enum class GameState {
    MainMenu,
    PVP,
    AI
};

class MainMenu {
public:
    MainMenu(Font& font) {
        title.setFont(font);
        title.setString("Ping your Pong?");
        title.setCharacterSize(50);
        title.setFillColor(Color::Black);
        title.setPosition(200.f, 50.f);

        float buttonWidth = 200.f;
        float buttonHeight = 60.f;
        float buttonSpacing = 100.f;
        float buttonStartY = 200.f;

        playButton.setSize(Vector2f(buttonWidth, buttonHeight));
        playButton.setFillColor(Color::Green);
        playButton.setPosition((windowWidth - buttonWidth) / 2, buttonStartY);
        playButton.setOutlineThickness(2);
        playButton.setOutlineColor(Color::Black);

        playText.setFont(font);
        playText.setString("PVP Mode");
        playText.setCharacterSize(30);
        playText.setFillColor(Color::White);
        playText.setPosition((windowWidth - playText.getLocalBounds().width) / 2, buttonStartY + 10.f);

        aibutton.setSize(Vector2f(buttonWidth, buttonHeight));
        aibutton.setFillColor(Color::Blue);
        aibutton.setPosition((windowWidth - buttonWidth) / 2, buttonStartY + buttonSpacing);
        aibutton.setOutlineThickness(2);
        aibutton.setOutlineColor(Color::Black);

        aitext.setFont(font);
        aitext.setString("AI Mode");
        aitext.setCharacterSize(30);
        aitext.setFillColor(Color::White);
        aitext.setPosition((windowWidth - aitext.getLocalBounds().width) / 2, buttonStartY + buttonSpacing + 10.f);

        quitButton.setSize(Vector2f(buttonWidth, buttonHeight));
        quitButton.setFillColor(Color::Red);
        quitButton.setPosition((windowWidth - buttonWidth) / 2, buttonStartY + 2 * buttonSpacing);
        quitButton.setOutlineThickness(2);
        quitButton.setOutlineColor(Color::Black);

        quitText.setFont(font);
        quitText.setString("Quit");
        quitText.setCharacterSize(30);
        quitText.setFillColor(Color::White);
        quitText.setPosition((windowWidth - quitText.getLocalBounds().width) / 2, buttonStartY + 2 * buttonSpacing + 10.f);
    }

    void draw(RenderWindow& window) {
        window.draw(title);
        window.draw(playButton);
        window.draw(playText);
        window.draw(aibutton);
        window.draw(aitext);
        window.draw(quitButton);
        window.draw(quitText);
    }

    bool isPlayButtonClicked(Vector2f mousePosition) {
        return playButton.getGlobalBounds().contains(mousePosition);
    }

    bool isaiButtonClicked(Vector2f mousePosition) {
        return aibutton.getGlobalBounds().contains(mousePosition);
    }

    bool isQuitButtonClicked(Vector2f mousePosition) {
        return quitButton.getGlobalBounds().contains(mousePosition);
    }

private:
    Text title;
    RectangleShape playButton;
    Text playText;
    RectangleShape aibutton;
    Text aitext;
    RectangleShape quitButton;
    Text quitText;
};

void pvp(RenderWindow& window, RectangleShape& p1, RectangleShape& p2, CircleShape& ball, float& ballSpeedX, float& ballSpeedY) {
    int player1Score = 0;
    int player2Score = 0;

    Text scoreText;
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return;
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::White);

    bool quitting = false;

    while (window.isOpen() && !quitting) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                quitting = true;
        }

        // Update paddle movement
        if (Keyboard::isKeyPressed(Keyboard::W) && p1.getPosition().y > 0)
            p1.move(0.f, -1.f);
        if (Keyboard::isKeyPressed(Keyboard::S) && p1.getPosition().y + p1.getSize().y < windowHeight)
            p1.move(0.f, 1.f);
        if (Keyboard::isKeyPressed(Keyboard::Up) && p2.getPosition().y > 0)
            p2.move(0.f, -1.f);
        if (Keyboard::isKeyPressed(Keyboard::Down) && p2.getPosition().y + p2.getSize().y < windowHeight)
            p2.move(0.f, 1.f);

        // Update ball movement
        ball.move(ballSpeedX, ballSpeedY);

        // Ball collisions with paddles
        if (p1.getGlobalBounds().intersects(ball.getGlobalBounds()) || p2.getGlobalBounds().intersects(ball.getGlobalBounds())) {
            ballSpeedX = -ballSpeedX; // Reverse horizontal speed
        }

        // Ball collisions with top and bottom window edges
        if (ball.getPosition().y <= 0 || ball.getPosition().y >= windowHeight - ball.getRadius() * 2) {
            ballSpeedY = -ballSpeedY; // Reverse vertical speed
        }

        // Scoring and resetting ball position
        if (ball.getPosition().x < 0) {
            player2Score++;
            ball.setPosition(Vector2f(windowWidth / 2 - ball.getRadius(), windowHeight / 2 - ball.getRadius()));
        }
        if (ball.getPosition().x > windowWidth - ball.getRadius() * 2) {
            player1Score++;
            ball.setPosition(Vector2f(windowWidth / 2 - ball.getRadius(), windowHeight / 2 - ball.getRadius()));
        }

        // Update score display
        scoreText.setString("Player 1: " + std::to_string(player1Score) + " - Player 2: " + std::to_string(player2Score));
        scoreText.setPosition(Vector2f((windowWidth - scoreText.getLocalBounds().width) / 2, 15.f));

        window.clear();

        // Draw
        window.draw(scoreText);
        window.draw(p1);
        window.draw(p2);
        window.draw(ball);

        window.display();
    }
}

void ai(RenderWindow& window, RectangleShape& p1, RectangleShape& p2, CircleShape& ball, float& ballSpeedX, float& ballSpeedY) {
    int playerScore = 0;
    ballSpeedX=ballSpeedY=0.6f;

    Text scoreText;
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return;
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::White);

    bool quitting = false;

    while (window.isOpen() && !quitting) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                quitting = true;
        }

        // Update paddle movement for Player 1
        if (Keyboard::isKeyPressed(Keyboard::W) && p1.getPosition().y > 0)
            p1.move(0.f, -1.f);
        if (Keyboard::isKeyPressed(Keyboard::S) && p1.getPosition().y + p1.getSize().y < windowHeight)
            p1.move(0.f, 1.f);

        // Update paddle movement for AI (Player 2)
        // AI follows the ball
        if (ball.getPosition().y + ball.getRadius() < p2.getPosition().y + p2.getSize().y / 2)
            p2.move(0.f, -1.f);
        if (ball.getPosition().y + ball.getRadius() > p2.getPosition().y + p2.getSize().y / 2)
            p2.move(0.f, 1.f);

        // Update ball movement
        ball.move(ballSpeedX, ballSpeedY);

        // Ball collisions with paddles
        if (p1.getGlobalBounds().intersects(ball.getGlobalBounds()) || p2.getGlobalBounds().intersects(ball.getGlobalBounds())) {
            ballSpeedX = -ballSpeedX; // Reverse horizontal speed
            playerScore++;
        }

        // Ball collisions with top and bottom window edges
        if (ball.getPosition().y <= 0 || ball.getPosition().y >= windowHeight - ball.getRadius() * 2) {
            ballSpeedY = -ballSpeedY; // Reverse vertical speed
        }

        // Scoring and resetting ball position
        if (ball.getPosition().x < 0 || ball.getPosition().x > windowWidth - ball.getRadius() * 2) {
            playerScore++;
            ball.setPosition(Vector2f(windowWidth / 2 - ball.getRadius(), windowHeight / 2 - ball.getRadius()));
        }

        // Update score display
        scoreText.setString("Score: " + std::to_string(playerScore));
        scoreText.setPosition(Vector2f((windowWidth - scoreText.getLocalBounds().width) / 2, 15.f));

        window.clear();

        // Draw
        window.draw(scoreText);
        window.draw(p1);
        window.draw(p2);
        window.draw(ball);

        window.display();
    }
}

int main() {
    RenderWindow window(VideoMode(windowWidth, windowHeight), "PingPong");
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return 1;
    }

    MainMenu mainMenu(font);

    RectangleShape p1(Vector2f(10.f, 69.f));
    RectangleShape p2(Vector2f(10.f, 69.f));
    p1.setFillColor(Color::White);
    p2.setFillColor(Color::White);
    p1.setPosition(Vector2f(10.f, windowHeight / 2 - p1.getSize().y / 2));
    p2.setPosition(Vector2f(windowWidth - p2.getSize().x - 10.f, windowHeight / 2 - p2.getSize().y / 2));

    CircleShape ball(25.f);
    ball.setFillColor(Color::Yellow);
    ball.setPosition(Vector2f(windowWidth / 2 - ball.getRadius(), windowHeight / 2 - ball.getRadius()));

    float ballSpeedX = 0.3f; // Horizontal speed of the ball
    float ballSpeedY = 0.3f; // Vertical speed of the ball

    GameState gameState = GameState::MainMenu;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
                    switch (gameState) {
                        case GameState::MainMenu:
                            if (mainMenu.isPlayButtonClicked(mousePosition)) {
                                gameState = GameState::PVP;
                            } else if (mainMenu.isaiButtonClicked(mousePosition)) {
                                gameState = GameState::AI;
                            } else if (mainMenu.isQuitButtonClicked(mousePosition)) {
                                window.close();
                            }
                            break;
                        case GameState::PVP:
                            break;
                        case GameState::AI:
                            break;
                    }
                }
            }
        }

        window.clear(Color::White);

        switch (gameState) {
            case GameState::MainMenu:
                mainMenu.draw(window);
                break;
            case GameState::PVP:
                pvp(window, p1, p2, ball, ballSpeedX, ballSpeedY);
                gameState = GameState::MainMenu; // After the game finishes, return to main menu
                break;
            case GameState::AI:
                ai(window, p1, p2, ball, ballSpeedX, ballSpeedY);
                gameState = GameState::MainMenu; // After the game finishes, return to main menu
                break;
        }

        window.display();
    }

    return 0;
}
