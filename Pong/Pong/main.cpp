#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <random>

const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;
const int FPS = 60;


class Menu {
public:
     Menu();
     void draw(sf::RenderWindow& window) {
         window.draw(title);
         for (int i = 0; i < 2; i++) {
			 window.draw(menuItems[i]);
         }
     }

     void drawMode(sf::RenderWindow& window) {
         window.draw(title);
         for (int i = 0; i < 2; i++) {
             window.draw(gameMode[i]);
         }
     }

     void drawAISelection(sf::RenderWindow& window) {
         window.draw(title);
         for (int i = 0; i < 2; i++) {
             window.draw(aiSelection[i]);
         }
     }

     void MoveUp() {
         if (selectedItemIndex - 1 >= 0 && inModeSelection == false && inAISelection == false) {
             menuItems[selectedItemIndex].setStyle(sf::Text::Regular);
             selectedItemIndex--;
             menuItems[selectedItemIndex].setStyle(sf::Text::Underlined);
         }

         if (selectedItemIndex - 1 >= 0 && inModeSelection && inAISelection == false) {
             gameMode[selectedItemIndex].setStyle(sf::Text::Regular);
             selectedItemIndex--;
             gameMode[selectedItemIndex].setStyle(sf::Text::Underlined);
         }

         if (selectedItemIndex - 1 >= 0 && inAISelection) {
             aiSelection[selectedItemIndex].setStyle(sf::Text::Regular);
             selectedItemIndex--;
             aiSelection[selectedItemIndex].setStyle(sf::Text::Underlined);
         }

     };
     void MoveDown() {
         if (selectedItemIndex + 1 < 2 && inModeSelection == false && inAISelection == false) {
             menuItems[selectedItemIndex].setStyle(sf::Text::Regular);
             selectedItemIndex++;
             menuItems[selectedItemIndex].setStyle(sf::Text::Underlined);
         }

         if (selectedItemIndex + 1 < 2 && inModeSelection && inAISelection == false) {
             gameMode[selectedItemIndex].setStyle(sf::Text::Regular);
             selectedItemIndex++;
             gameMode[selectedItemIndex].setStyle(sf::Text::Underlined);
         }

         if (selectedItemIndex + 1 < 2 && inAISelection) {
             aiSelection[selectedItemIndex].setStyle(sf::Text::Regular);
             selectedItemIndex++;
             aiSelection[selectedItemIndex].setStyle(sf::Text::Underlined);
         }
     };

     int getPressedItem() {
         return selectedItemIndex;
     }

	 bool inModeSelection = false;
     int selectedItemIndex;
     bool inAISelection = false;
private:
    
    
    sf::Font font;
    sf::Text title = sf::Text(font);
    sf::Text menuItems[3] = { sf::Text(font),
                           sf::Text(font),
                           sf::Text(font) };
    
    sf::Text gameMode[2] = { sf::Text(font),
                           sf::Text(font) };

    sf::Text aiSelection[2] = { sf::Text(font),
                               sf::Text(font) };

};


Menu::Menu() {
	if (!font.openFromFile("EvilEmpire.otf")) {
        std::cout << "Failed to load font!" << std::endl;
    }
    else {

        title.setFont(font);
        title.setString("Pong Clone");
        title.setCharacterSize(70);
        title.setPosition({(WIDTH / 2.f) - 130 , HEIGHT / 4.f });



        menuItems[0].setFont(font);
        menuItems[0].setString("Start");
        menuItems[0].setCharacterSize(30);
        menuItems[0].setFillColor(sf::Color::White);
        menuItems[0].setPosition({ (WIDTH / 2.f) -25 , 2*HEIGHT / 4.f });
		menuItems[0].setStyle(sf::Text::Underlined);

        menuItems[1].setFont(font);
        menuItems[1].setString("Quit");
        menuItems[1].setCharacterSize(30);
        menuItems[1].setFillColor(sf::Color::White);
        menuItems[1].setPosition({ (WIDTH / 2.f) -20 , (2*HEIGHT / 4.f) + 100 });

        gameMode[0].setFont(font);
        gameMode[0].setString("Best of 5");
        gameMode[0].setCharacterSize(30);
        gameMode[0].setFillColor(sf::Color::White);
        gameMode[0].setPosition({ (WIDTH / 2.f) - 50 , 2 * HEIGHT / 4.f });
        gameMode[0].setStyle(sf::Text::Underlined);

        gameMode[1].setFont(font);
        gameMode[1].setString("Best of 10");
        gameMode[1].setCharacterSize(30);
        gameMode[1].setFillColor(sf::Color::White);
        gameMode[1].setPosition({ (WIDTH / 2.f) - 50 , (2 * HEIGHT / 4.f) + 100});
        
        aiSelection[0].setFont(font);
        aiSelection[0].setString("1 Player");
        aiSelection[0].setCharacterSize(30);
        aiSelection[0].setFillColor(sf::Color::White);
        aiSelection[0].setPosition({ (WIDTH / 2.f) - 70 , 2 * HEIGHT / 4.f });
        aiSelection[0].setStyle(sf::Text::Underlined);

        aiSelection[1].setFont(font);
        aiSelection[1].setString("2 Players");
        aiSelection[1].setCharacterSize(30);
        aiSelection[1].setFillColor(sf::Color::White);
        aiSelection[1].setPosition({ (WIDTH / 2.f) - 50 , (2 * HEIGHT / 4.f) + 100 });

    }
    
	selectedItemIndex = 0;
}

class startGame {
public: 

    startGame(sf::RenderWindow& window , int cap, bool aiMode = false);
};

startGame::startGame(sf::RenderWindow& window, int cap, bool aiMode) {

    // Add AI flag at the beginning
    bool aiEnabled = aiMode;
    float aiDifficulty = 0.7f;  // 0.0 = easy, 1.0 = hard
    sf::Clock aiReactionTimer;


    //Clock settings
    sf::Clock clock;
    sf::Clock roundTimer;
    float deltaTime = 0.0f;
    float roundTime = 0.0f;



    //Crating Ball
    sf::Vector2f ballVelocity(4.0f, -4.0f);
    float ballSpeed = 500;

    sf::CircleShape ball(10.f);
    ball.setPosition({ WIDTH / 2, HEIGHT / 2 });
    ball.setFillColor(sf::Color::White);


    // CREATING PLAYERS
    sf::RectangleShape p1({ 20.f, 80.f });
    p1.setPosition({ 50.f, HEIGHT / 2 - p1.getSize().y / 2 });

    sf::RectangleShape p2({ 20.f,80.f });
    p2.setPosition({ WIDTH - 50.f - p2.getSize().x, HEIGHT / 2 - p2.getSize().y / 2 });


    // CREATING SCOREBOARD
    int p1Score = 0; int p2Score = 0;

    sf::Font font("EvilEmpire.otf");
    sf::Text scoreboard(font);
    scoreboard.setString(std::to_string(p1Score) + " - " + std::to_string(p2Score));
    scoreboard.setCharacterSize(50);
    scoreboard.setPosition({ WIDTH / 2.f - 30,0.f });
   
	sf::Text countdownText(font);
	countdownText.setCharacterSize(70);
	countdownText.setPosition({ (WIDTH / 2.f) - 5 , (HEIGHT / 2.f) - 90});

    //SOUNDS

    sf::SoundBuffer hitBuffer("hit.wav");
    sf::Sound hitSound(hitBuffer);

	sf::SoundBuffer wallBuffer("wall.wav");
	sf::Sound wallSound(wallBuffer);
    
	sf::SoundBuffer scoreBuffer("score.wav");
	sf::Sound scoreSound(scoreBuffer);


    //Pause menu
    bool isPaused = false;
    sf::Text pauseText(font);
    pauseText.setString("PAUSED\nPress ESC to resume");
    pauseText.setCharacterSize(50);
    pauseText.setPosition({ WIDTH / 2.f - 150, HEIGHT / 2.f - 50 });
    pauseText.setFillColor(sf::Color::Yellow);
    



    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        roundTime = roundTimer.getElapsedTime().asSeconds();

        while (const std::optional event = window.pollEvent())
        {

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
               
            if (event->is <sf::Event::KeyReleased>()) {
                sf::Keyboard::Key releasedKey = event->getIf<sf::Event::KeyReleased>()->code;
                if (releasedKey == sf::Keyboard::Key::Escape) {

                    isPaused = !isPaused;  // Toggle pause state

                    // If unpausing during countdown, don't let the timer jump
                    if (!isPaused && roundTime < 3.0f) {
                        // Adjust timer so countdown continues correctly
                        roundTimer.restart();
                        roundTime = roundTimer.getElapsedTime().asSeconds();
                    }
				}
            }
        }

        if (isPaused) {
            // Draw pause screen
            window.clear();

            // Draw the game state (frozen)
            window.draw(ball);
            window.draw(p1);
            window.draw(p2);
            window.draw(scoreboard);

            // Draw semi-transparent overlay
            sf::RectangleShape overlay({ static_cast<float>(WIDTH), static_cast<float>(HEIGHT) });
            overlay.setFillColor(sf::Color(0, 0, 0, 150));  // Semi-transparent black
            window.draw(overlay);

            // Draw pause text
            window.draw(pauseText);

            window.display();

            // Skip game logic while paused
            continue;  // Go to next iteration of while loop
        }

        //Player1 control
        sf::Vector2f pos1 = p1.getPosition();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            if (pos1.y - 5 >= 0) {
                p1.setPosition(pos1 + sf::Vector2f(0.f, -10.f));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            if (pos1.y + 75 <= HEIGHT) {
                p1.setPosition(pos1 + sf::Vector2f(0.f, 10.f));
            }
        }


        //PLAYER 2 OR AI CONTROLS
        if (!aiEnabled) {

            // player 2 controls
            sf::Vector2f pos2 = p2.getPosition();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)) {
                if (pos2.y - 5 >= 0) {
                    p2.setPosition(pos2 + sf::Vector2f(0.f, -10.f));
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
                if (pos2.y + 75 <= HEIGHT) {
                    p2.setPosition(pos2 + sf::Vector2f(0.f, 10.f));
                }
            }
        }
        else {
            if (aiReactionTimer.getElapsedTime().asSeconds() >= 0.05f) {
                aiReactionTimer.restart();

                float paddleCenter = p2.getPosition().y + p2.getSize().y / 2;
                float balllCenter = ball.getPosition().y + 10;

                float predict = 0.0f;

                if (ballVelocity.x > 0) {
                    float timeToReach = (WIDTH - 70 - ball.getPosition().x) / (ballVelocity.x / deltaTime);
                    predict = ballVelocity.y * timeToReach * aiDifficulty * 0.5f;
                }

                float targetY = balllCenter + predict;

                // Making the bot easier based on difficulty

                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_real_distribution<float> errorDist(-30.0f * (1.0f - aiDifficulty), 30.0f * (1.0f - aiDifficulty));
                targetY += errorDist(gen);

                // Move toward target
                float aiSpeed = 8.0f + 4.0f * aiDifficulty;

                //Dead zone (no movimentation if close to target)

                float deadZone = 15.0f * (1.0f - aiDifficulty * 0.5f);

                if (targetY < paddleCenter - deadZone) {
                    if (p2.getPosition().y > 0)
                        p2.move({ 0.f, -aiSpeed });
                }
                else if (targetY > paddleCenter + deadZone) {
					if (p2.getPosition().y + p2.getSize().y < HEIGHT)
                        p2.move({ 0.f, aiSpeed });
                }
            }

        }

        
        


        //Ball commands
        if (roundTime < 3.0f) {
            ballVelocity = { 0.f, 0.f };
            countdownText.setString(std::to_string(3 - static_cast<int>(roundTime)));
        }
        else {
            float timeInPlay = roundTime - 5;
            float speedMultiplier = 1.0f + (timeInPlay * 0.1f);
            float currentBallSpeed = ballSpeed * speedMultiplier;

            float maxSpeed = 1000.0f;
            if (currentBallSpeed > maxSpeed) {
                currentBallSpeed = maxSpeed;
            }

            // Normalize and scale current velocity
            if (ballVelocity.x != 0 || ballVelocity.y != 0) {
                float length = std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y);
                if (length > 0) {
                    float dirX = ballVelocity.x / length;
                    float dirY = ballVelocity.y / length;
                    ballVelocity.x = dirX * currentBallSpeed * deltaTime;
                    ballVelocity.y = dirY * currentBallSpeed * deltaTime;
                }
            }
            else {
                // First launch
                static std::random_device rd;
                static std::mt19937 gen(rd());
                static std::uniform_real_distribution<float> angleDist(-60.0f, 60.0f);
                std::uniform_real_distribution<float> dirDist(0.0f, 1.0f);

                float randomAngle = angleDist(gen) * 3.14159f / 180.0f;
                bool goRight = dirDist(gen) > 0.5f;

                if (goRight) {
                    ballVelocity = {
                        std::cos(randomAngle) * currentBallSpeed * deltaTime,
                        std::sin(randomAngle) * currentBallSpeed * deltaTime
                    };
                }
                else {
                    ballVelocity = {
                        -std::cos(randomAngle) * currentBallSpeed * deltaTime,
                        std::sin(randomAngle) * currentBallSpeed * deltaTime
                    };
                }
            }

            // Wall collisions
            if (ball.getPosition().y <= 0) {
                wallSound.play();
                ballVelocity.y = std::abs(ballVelocity.y);  // Bounce down
            }
            else if (ball.getPosition().y + 20 >= HEIGHT) {  // Ball diameter is 20
                wallSound.play();
                ballVelocity.y = -std::abs(ballVelocity.y); // Bounce up
            }
            else if (ball.getPosition().x <= 30) {
                p2Score++;
                scoreboard.setString(std::to_string(p1Score) + " - " + std::to_string(p2Score));
                scoreSound.play();
                roundTimer.restart();
                ball.setPosition({ WIDTH / 2, HEIGHT / 2 });
                ballVelocity = { 0.f, 0.f };  // Will be set to random on next frame
            }
            else if (ball.getPosition().x >= WIDTH - 30) {
                p1Score++;
                scoreboard.setString(std::to_string(p1Score) + " - " + std::to_string(p2Score));
                scoreSound.play();
                roundTimer.restart();
                ball.setPosition({ WIDTH / 2, HEIGHT / 2 });
                ballVelocity = { 0.f, 0.f };  // Will be set to random on next frame
            }

            // Paddle collisions with angle physics
            sf::FloatRect ballBounds = ball.getGlobalBounds();
            sf::FloatRect p1Bounds = p1.getGlobalBounds();
            sf::FloatRect p2Bounds = p2.getGlobalBounds();

            auto calculateBounce = [&](float paddleCenterY, bool rightPaddle) {
                float ballCenterY = ball.getPosition().y + 10;  // Ball radius
                float relativeIntersectY = paddleCenterY - ballCenterY;
                float normalizedY = relativeIntersectY / 35.0f;  // Half paddle height (70/2)
                normalizedY = std::max(-1.0f, std::min(1.0f, normalizedY));

                float maxAngle = 75.0f * 3.14159f / 180.0f;
                float bounceAngle = normalizedY * maxAngle;

                float speed = std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y);
                if (speed == 0) speed = currentBallSpeed * deltaTime;

                float newVelX, newVelY;
                if (rightPaddle) {
                    newVelX = -std::cos(bounceAngle) * speed;
                    newVelY = -std::sin(bounceAngle) * speed;
                }
                else {
                    newVelX = std::cos(bounceAngle) * speed;
                    newVelY = -std::sin(bounceAngle) * speed;
                }

                return sf::Vector2f(newVelX, newVelY);
                };

            if (ballBounds.findIntersection(p2Bounds)) {
                hitSound.play();
                float paddleCenterY = p2.getPosition().y + p2.getSize().y / 2;
                ballVelocity = calculateBounce(paddleCenterY, true);
            }

            if (ballBounds.findIntersection(p1Bounds)) {
                hitSound.play();
                float paddleCenterY = p1.getPosition().y + p1.getSize().y / 2;
                ballVelocity = calculateBounce(paddleCenterY, false);
            }
        }

  
        ball.move(ballVelocity);
        
        window.clear(); //CLEAR 

        // =-=-=-=-=-=-=-=-=DRAW-=-=-=-=-=-=-=-
		
        if (p1Score < cap && p2Score < cap) {

            if (roundTime < 3.0f) {
                window.draw(countdownText);
            }
            window.draw(ball);
            window.draw(p1);
            window.draw(p2);
            window.draw(scoreboard);

        }
        else {
            window.clear();
            sf::Text winText(font);
            winText.setCharacterSize(70);
            winText.setPosition({ (WIDTH / 2.f) - 150 , (HEIGHT / 2.f) - 90 });
            if (p1Score > p2Score) {
                winText.setString("Player 1 Wins!");
            }
            else {
                winText.setString("Player 2 Wins!");
            }
			window.draw(winText);


			sf::Text exitText(font);
			exitText.setCharacterSize(30);
			exitText.setPosition({ (WIDTH / 2.f) - 100 , (HEIGHT / 2.f) + 10 });
			exitText.setString("Press Z to return to game mode");
			window.draw(exitText);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
                // Return control to main menu
                return;  // Exit the startGame constructor
            }

        }
        
        

        window.display();

    }
    
}



int main()
{
    
    //Setting window
	sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Pong clone");
    window.setFramerateLimit(FPS);

    sf::View view;
    view.setCenter({ WIDTH / 2.0f, HEIGHT / 2.0f });
    view.setSize({ WIDTH, HEIGHT });
    window.setView(view);

    Menu menu;

	bool selectedAIMode = false;
	int selectedCap = 5;
	bool startingGame = false;

    // ==-=-=-=-=-=-=-=-=-=-=UPDATE=-=-=-=-=-=-=-=-=-=-=
    while (window.isOpen())
    {
        //deltaTime = clock.restart().asSeconds();
        //roundTime = roundTimer.getElapsedTime().asSeconds();

        while (const std::optional event = window.pollEvent())
        {

            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is <sf::Event::KeyReleased>()) {
                sf::Keyboard::Key releasedKey = event->getIf<sf::Event::KeyReleased>()->code;
                if (releasedKey == sf::Keyboard::Key::W || releasedKey == sf::Keyboard::Key::Up) {
                    menu.MoveUp();
                }
                else if (releasedKey == sf::Keyboard::Key::S || releasedKey == sf::Keyboard::Key::Down) {
                    menu.MoveDown();
                }
                else if (releasedKey == sf::Keyboard::Key::Enter) {
                    if (menu.getPressedItem() == 0 && menu.inModeSelection == false && menu.inAISelection == false) {
                        window.clear();
                        menu.selectedItemIndex = 0;
                        menu.inAISelection = true;
                        
                    }
                    else if (menu.getPressedItem() == 1 && !menu.inModeSelection && !menu.inAISelection) {
                        window.close();
                    }
                    // AI Selection Menu
                    else if (menu.getPressedItem() == 0 && menu.inAISelection) {
                        selectedAIMode = true;
                        menu.inAISelection = false;
                        menu.inModeSelection = true;
                        menu.selectedItemIndex = 0;
                    }
                    else if (menu.getPressedItem() == 1 && menu.inAISelection) {
                        selectedAIMode = false;
                        menu.inAISelection = false;
                        menu.inModeSelection = true;
                        menu.selectedItemIndex = 0;
                    }
                    // Game Mode Selection
                    else if (menu.getPressedItem() == 0 && menu.inModeSelection) {
                        selectedCap = 5;
                        window.clear();
                        startGame game(window, selectedCap, selectedAIMode);
                    }
                    else if (menu.getPressedItem() == 1 && menu.inModeSelection) {
                        selectedCap = 10;
                        window.clear();
                        startGame game(window, selectedCap, selectedAIMode);
                    }
				}
                else if (releasedKey == sf::Keyboard::Key::Z) {
                    // Back button
                    if (menu.inModeSelection) {
                        menu.selectedItemIndex = 0;
                        menu.inModeSelection = false;
                        menu.inAISelection = true;
                    }
                    else if (menu.inAISelection) {
                        menu.selectedItemIndex = 0;
                        menu.inAISelection = false;
                    }
                }
            }
        }

        
        
        window.clear(); //CLEAR 
        
        // =-=-=-=-=-=-=-=-=DRAW-=-=-=-=-=-=-=-
        if (menu.inAISelection) {
            menu.drawAISelection(window);
        }
        else if (menu.inModeSelection) {
            menu.drawMode(window);
        }
        else {
            menu.draw(window);
        }
        window.display();
    }
}