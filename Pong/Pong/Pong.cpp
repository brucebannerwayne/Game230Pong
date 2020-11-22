#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>
#include <string>
#include <ctime>

const int HEIGHT = 600;
const int WIDTH = 800;
sf::SoundBuffer snd;
sf::Sound impact;
sf::Vector2f Normalize(sf::Vector2f vector);
sf::Vector2f CollisionCheck(sf::Vector2f center, sf::Vector2f velocity, float padX, float radius);
int random(int a) {
	srand((int)time(0));
	return rand() % a;
}
class Ball {
public:
	sf::CircleShape shape;
	sf::Vector2f velocity;
	float moveSpeed;
	sf::Sprite sprite;

	bool UpdateBall(float deltaTime, sf::Vector2f padPosition, sf::Vector2f padSize, bool isLeft) {
		auto position = this->shape.getPosition();
		auto center = this->shape.getPosition() + sf::Vector2f(this->shape.getRadius(), this->shape.getRadius());
		auto padCenter = sf::Vector2f(padPosition.x + padSize.x / 2, padPosition.y + padSize.y / 2);
		auto dir = center - padCenter;
		this->velocity = Normalize(this->velocity) * this->moveSpeed;
		this->shape.setPosition(position.x + this->velocity.x * deltaTime, position.y + this->velocity.y * deltaTime);
		this->sprite.setPosition(position.x + this->velocity.x * deltaTime, position.y + this->velocity.y * deltaTime);
		position = this->shape.getPosition();
		center = this->shape.getPosition() + sf::Vector2f(this->shape.getRadius(), this->shape.getRadius());
		dir = center - padCenter;
		if (position.y >= HEIGHT - this->shape.getRadius() * 2.0f) {
			this->velocity.y = - abs(this->velocity.y);
			impact.play();
		}
		if (position.y <= 0.0f) {
			this->velocity.y = abs(this->velocity.y);
			impact.play();
		}
		/*if (position.x + velocity.x * deltaTime < padSize.x) {
			sf::Vector2f check = CollisionCheck(center, velocity, padSize.x, this->shape.getRadius());
			if (check.x <= padPosition.y + padSize.y && check.x >= padPosition.y) {
				std::cout << position.x << ", " << this->shape.getPosition().x << std::endl;
				sf::Vector2f impactPoint = sf::Vector2f(padSize.x, check.y + this->shape.getRadius());
				this->shape.setPosition(impactPoint);
				dir = Normalize(impactPoint - padCenter);
				dir.x = abs(dir.x);
				dir.y = -dir.y;
				moveSpeed += 10;
				this->velocity = dir * moveSpeed;
			}
			
		}
		if (position.x + velocity.x * deltaTime >= WIDTH - this->shape.getRadius() * 2.0f - padSize.x) {
			sf::Vector2f check = CollisionCheck(center, velocity, padPosition.x, this->shape.getRadius());
			if (check.x <= padPosition.y + padSize.y && check.x >= padPosition.y) {
				sf::Vector2f impactPoint = sf::Vector2f(padPosition.x - this->shape.getRadius() * 2.0f, check.y + this->shape.getRadius());
				this->shape.setPosition(impactPoint - padCenter);
				dir = Normalize(dir);
				dir.x = -abs(dir.x);
				dir.y = -dir.y;
				moveSpeed += 10;
				this->velocity = dir * moveSpeed;
			}
			
		}*/
		if (position.x >= WIDTH - this->shape.getRadius() * 2.0f - padSize.x ) {
			if (center.y >= padPosition.y && center.y <= padPosition.y + padSize.y) {
				dir = Normalize(dir);
				dir.x = -abs(dir.x);
				dir.y = -dir.y;
				moveSpeed += 10;
				if (moveSpeed >= 200) {
					moveSpeed = 200;
				}
				this->velocity = dir * moveSpeed;
				impact.play();
				std::cout << dir.x <<","<< dir.y << std::endl;
				return true;
			}
			else if(position.x >= WIDTH - this->shape.getRadius() * 2.0f) {
				return false;
			}
		}
		if (position.x <= padSize.x) {
			if (center.y >= padPosition.y && center.y <= padPosition.y + padSize.y) {
				dir = Normalize(dir);
				dir.x = abs(dir.x);
				dir.y = -dir.y;
				moveSpeed += 10;
				if (moveSpeed >= 200) {
					moveSpeed = 200;
				}
				this->velocity = dir * moveSpeed;
				impact.play();
				std::cout << dir.x << "," << dir.y << std::endl;
			}
			else if(position.x < -this->shape.getRadius()){
				return false;
			}
		}
		if (isLeft) {
			if (pow(center.x - padSize.x, 2) + pow(center.y - padPosition.y, 2) <= pow(this->shape.getRadius(), 2)) {
				dir = Normalize(dir);
				dir.x = abs(dir.x);
				dir.y = -dir.y;
				moveSpeed += 10;
				if (moveSpeed >= 200) {
					moveSpeed = 200;
				}
				this->velocity = dir * moveSpeed;
			}
			else if (pow(center.x - padSize.x, 2) + pow(center.y - padPosition.y - padSize.y, 2) <= pow(this->shape.getRadius(), 2)) {
				dir = Normalize(dir);
				dir.x = abs(dir.x);
				dir.y = -dir.y;
				moveSpeed += 10;
				if (moveSpeed >= 200) {
					moveSpeed = 200;
				}
				this->velocity = dir * moveSpeed;
			}
			
		}
		else {
			if (pow(center.x - padPosition.x, 2) + pow(center.y - padPosition.y, 2) <= pow(this->shape.getRadius(), 2)) {
				dir = Normalize(dir);
				dir.x = -abs(dir.x);
				dir.y = -dir.y;
				moveSpeed += 10;
				if (moveSpeed >= 200) {
					moveSpeed = 200;
				}
				this->velocity = dir * moveSpeed;
			}
			else if (pow(center.x - padPosition.x, 2) + pow(center.y - padPosition.y - padSize.y, 2) <= pow(this->shape.getRadius(), 2)) {
				dir = Normalize(dir);
				dir.x = -abs(dir.x);
				dir.y = -dir.y;
				moveSpeed += 10;
				if (moveSpeed >= 200) {
					moveSpeed = 200;
				}
				this->velocity = dir * moveSpeed;
			}
			
		}
		return true;
	}
	void draw(sf::RenderWindow *window) {
		window->draw(this->shape);
		window->draw(this->sprite);
		
	}

};
sf::Vector2f CollisionCheck(sf::Vector2f center, sf::Vector2f velocity, float padX, float radius) {
	float k = velocity.y / velocity.x;
	float b = center.y - k * center.x;
	return sf::Vector2f(k * padX + b, k * (padX + radius) + b);
}
sf::Vector2f Normalize(sf::Vector2f vector) {
	float mag = sqrtf(pow(vector.x, 2) + pow(vector.y, 2));
	return sf::Vector2f(vector.x / mag, vector.y / mag);
}
class Pad {
public:
	sf::RectangleShape shape;
	sf::Vector2f velocity;
	
	void Move(float deltaTime, bool goUp) {
		auto position = this->shape.getPosition();
		if (goUp) {
			this->shape.setPosition(position.x, position.y - abs(this->velocity.y) * deltaTime);
			if (position.y <= 0) {
				this->shape.setPosition(position.x, 0);
			}
		}
		else {
			this->shape.setPosition(position.x, position.y + abs(this->velocity.y) * deltaTime);
			if (position.y >= HEIGHT - this->shape.getSize().y) {
				this->shape.setPosition(position.x, HEIGHT - this->shape.getSize().y);
			}
		}
	}
	void draw(sf::RenderWindow* window) {
		window->draw(this->shape);
	}
};
class Holes {
public:
	int type;
	sf::CircleShape shape;
	float moveSpeed;
	sf::Vector2f dir;
	float getTotalRadius() {
		return this->shape.getRadius() + this->shape.getOutlineThickness();
	}
	void Patrol(sf::Vector2f path1, sf::Vector2f path2, float deltaTime) {
		float dis1 = sqrtf(pow(this->shape.getPosition().x - path1.x, 2) + pow(this->shape.getPosition().y - path1.y, 2));
		float dis2 = sqrtf(pow(this->shape.getPosition().x - path2.x, 2) + pow(this->shape.getPosition().y - path2.y, 2));
		if (dis1 <= 10) {
			dir = Normalize(path2 - path1);
		}
		if (dis2 <= 10) {
			dir = Normalize(path1 - path2);
		}
		this->shape.setPosition(this->shape.getPosition() + dir * moveSpeed * deltaTime);
	}
	void effect(Ball* ball, float deltaTime) {
		sf::Vector2f HoleCenter = sf::Vector2f(this->shape.getPosition().x + this->shape.getRadius(), this->shape.getPosition().y + this->shape.getRadius());
		sf::Vector2f ballCenter = sf::Vector2f(ball->shape.getPosition().x + ball->shape.getRadius(), ball->shape.getPosition().y + ball->shape.getRadius());
		sf::Vector2f dir = ballCenter - HoleCenter;
		float dis = sqrtf(pow(dir.x, 2) + pow(dir.y, 2));
		switch (type)
		{
		case 0:
			if (dis <= this->getTotalRadius() + ball->shape.getRadius()) {
				ball->velocity += -Normalize(dir) * 40.0f * deltaTime;
			}
			break;
		case 1:
			if (dis <= this->getTotalRadius() + ball->shape.getRadius()) {
				ball->velocity += Normalize(dir) * 40.0f * deltaTime;
			}
			break;
		case 2:
			if (dis <= this->getTotalRadius() + ball->shape.getRadius()) {
				ball->velocity = Normalize(dir) * ball->moveSpeed;
				impact.play();
			}
			break;
		default:
			break;
		}
	}
	void draw(sf::RenderWindow* window) {
		window->draw(this->shape);
	}
};
void BallCollision(Ball* ball, Ball* ball2) {
	sf::Vector2f ballCenter = sf::Vector2f(ball->shape.getPosition().x + ball->shape.getRadius(), ball->shape.getPosition().y + ball->shape.getRadius());
	sf::Vector2f ball2Center = sf::Vector2f(ball2->shape.getPosition().x + ball2->shape.getRadius(), ball2->shape.getPosition().y + ball2->shape.getRadius());
	sf::Vector2f dir = ballCenter - ball2Center;
	float dis = sqrtf(pow(dir.x, 2) + pow(dir.y, 2));
	if (dis <= ball->shape.getRadius() + ball2->shape.getRadius()) {
		ball->velocity = Normalize(dir) * ball->moveSpeed;
		ball2->velocity = -Normalize(dir) * ball2->moveSpeed;
		impact.play();
	}
}
int AiPadMove(Pad pad, Ball ball) {
	if (pad.shape.getPosition().y >= ball.shape.getPosition().y + ball.shape.getRadius()) {
		return 1;
	}
	else if (pad.shape.getPosition().y + pad.shape.getSize().y <= ball.shape.getPosition().y + ball.shape.getRadius()) {
		return 2;
	}
	else {
		return 3;
	}
	
}

int main()
{
	bool gameOver = false;
	bool leftWin = true;
	int scoreL = 0;
	int scoreR = 0;
	sf::Font font;
	sf::String s1;
	sf::String s2;
	if (!font.loadFromFile("Data/Lobster/Lobster-Regular.ttf")) {
		return 1;
	}
	sf::Text Pong;
	Pong.setFont(font);
	Pong.setCharacterSize(64);
	Pong.setFillColor(sf::Color::White);
	Pong.setPosition(200, 300);
	Pong.setString("Pong");
	sf::Text AiMode;
	AiMode.setFont(font);
	AiMode.setCharacterSize(24);
	AiMode.setFillColor(sf::Color::White);
	AiMode.setPosition(400, 300);
	AiMode.setString("P VS AI");
	sf::Text Pvp;
	Pvp.setFont(font);
	Pvp.setCharacterSize(24);
	Pvp.setFillColor(sf::Color::White);
	Pvp.setPosition(400, 400);
	Pvp.setString("P VS P");
	sf::CircleShape choice;
	choice.setRadius(10);
	choice.setFillColor(sf::Color::Green);
	choice.setPosition(350, 300);
	sf::Text scoreLeft;
	scoreLeft.setFont(font);
	scoreLeft.setCharacterSize(24);
	scoreLeft.setFillColor(sf::Color::Red);
	scoreLeft.setPosition(10, 0);
	sf::Text scoreRight;
	scoreRight.setFont(font);
	scoreRight.setCharacterSize(24);
	scoreRight.setFillColor(sf::Color::Red);
	scoreRight.setPosition(WIDTH - 100, 0);
	sf::Text gameoverText;
	gameoverText.setFont(font);
	gameoverText.setCharacterSize(24);
	gameoverText.setFillColor(sf::Color::Red);
	gameoverText.setPosition(WIDTH/2 - 110, HEIGHT/2);
	sf::Text restart;
	restart.setFont(font);
	restart.setCharacterSize(24);
	restart.setFillColor(sf::Color::Red);
	restart.setPosition(WIDTH / 2 - 100, HEIGHT / 2 + 100);
	sf::Texture ballTexture;
	if (!ballTexture.loadFromFile("Data/Sprites/ball.png")) {
		return 1;
	}
	ballTexture.setSmooth(true);
	if (!snd.loadFromFile("Data/Sound/sound.wav")) {
		return 1;
	}
	
	impact.setBuffer(snd);
	
	sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT), "Pong");
	window.setVerticalSyncEnabled(true);

	Ball ball;
	ball.shape = sf::CircleShape(32.0f);
	ball.shape.setFillColor(sf::Color::Green);
	ball.shape.setPosition(300.0f, 300.0f);
	ball.velocity = sf::Vector2f(1.0, 1.0);
	ball.moveSpeed = 100.0f;
	ball.sprite.setTexture(ballTexture);
	Ball ball2;
	ball2.shape = sf::CircleShape(32.0f);
	ball2.shape.setFillColor(sf::Color::Green);
	ball2.shape.setPosition(1500.0f, 1500.0f);
	ball2.velocity = sf::Vector2f(0.0, 0.0);
	ball2.moveSpeed = 120.0f;
	ball2.sprite.setTexture(ballTexture);
	Pad pad;
	pad.shape = sf::RectangleShape(sf::Vector2f(10.0, 100.0));
	pad.shape.setPosition(0.0f, HEIGHT / 2);
	pad.shape.setFillColor(sf::Color::Red);
	pad.velocity = sf::Vector2f(0.0, 200.0);

	Pad pad2;
	pad2.shape = sf::RectangleShape(sf::Vector2f(10.0, 100.0));
	pad2.shape.setPosition(WIDTH - pad2.shape.getSize().x, HEIGHT / 2);
	pad2.shape.setFillColor(sf::Color::Red);
	pad2.velocity = sf::Vector2f(0.0, 200.0);

	Holes hole;
	hole.shape = sf::CircleShape(20.0f);
	hole.type = random(3);
	std::cout << hole.type << std::endl;
	hole.shape.setFillColor(sf::Color(150, 50, 250));
	hole.shape.setOutlineThickness(50);
	hole.shape.setOutlineColor(sf::Color::Blue);
	hole.moveSpeed = 30;
	sf::Vector2f path1 = sf::Vector2f(300, 300);
	sf::Vector2f path2 = sf::Vector2f(500, 500);
	hole.shape.setPosition(path1);

	sf::Clock clock;
	float deltaTime = 0.0f;
	bool offCheck;
	bool isLeft;
	bool ball2Existed = false;
	bool gameStart = true;
	bool AiEnabled;

	while (window.isOpen())
	{
		deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (gameStart) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				choice.setPosition(350, 300);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				choice.setPosition(350, 400);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				if (choice.getPosition().y == 300) {
					AiEnabled = true;
					gameStart = false;
				}
				else if (choice.getPosition().y == 400) {
					AiEnabled = false;
					gameStart = false;
				}
			}
			window.clear();
			window.draw(Pong);
			window.draw(AiMode);
			window.draw(Pvp);
			window.draw(choice);
			window.display();
		}
		else {
			if (!gameOver) {
				hole.Patrol(path1, path2, deltaTime);
				hole.effect(&ball, deltaTime);
				if (ball2Existed) {
					hole.effect(&ball2, deltaTime);
					BallCollision(&ball, &ball2);
				}
				if (ball.velocity.x > 0) {
					isLeft = false;
					offCheck = ball.UpdateBall(deltaTime, pad2.shape.getPosition(), pad2.shape.getSize(), isLeft);
					if (!offCheck) {
						scoreL++;
					}
				}
				else {
					isLeft = true;
					offCheck = ball.UpdateBall(deltaTime, pad.shape.getPosition(), pad.shape.getSize(), isLeft);
					if (!offCheck) {
						scoreR++;
					}
				}
				if (!offCheck) {
					ball.shape = sf::CircleShape(32.0f);
					ball.shape.setFillColor(sf::Color::Green);
					ball.shape.setPosition(300.0f, 300.0f);
					ball.velocity = sf::Vector2f(1.0, 1.0);
					ball.moveSpeed = 100.0f;
					hole.type = random(3);
					std::cout << hole.type << std::endl;
				}
				if (scoreL >= 2 || scoreR >= 2) {
					if (!ball2Existed) {
						ball2.shape.setPosition(500.0f, 500.0f);
						ball2.velocity = sf::Vector2f(1.0, -1.0);
						ball2Existed = true;
					}
					else {
						if (ball2.velocity.x > 0) {
							isLeft = false;
							offCheck = ball2.UpdateBall(deltaTime, pad2.shape.getPosition(), pad2.shape.getSize(), isLeft);
							if (!offCheck) {
								scoreL++;
							}
						}
						else {
							isLeft = true;
							offCheck = ball2.UpdateBall(deltaTime, pad.shape.getPosition(), pad.shape.getSize(), isLeft);
							if (!offCheck) {
								scoreR++;
							}
						}
						if (!offCheck) {
							ball2.shape = sf::CircleShape(32.0f);
							ball2.shape.setFillColor(sf::Color::Green);
							ball2.shape.setPosition(500.0f, 500.0f);
							ball2.velocity = sf::Vector2f(1.0, -1.0);
							ball2.moveSpeed = 120.0f;
							hole.type = random(3);
							std::cout << hole.type << std::endl;
						}
					}
				}
				if (scoreL == 5) {
					gameOver = true;
					leftWin = true;
				}
				else if (scoreR == 5) {
					gameOver = true;
					leftWin = false;
				}
				s1 = sf::String("Scores: " + std::to_string(scoreL));
				scoreLeft.setString(s1);
				s2 = sf::String("Scores: " + std::to_string(scoreR));
				scoreRight.setString(s2);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					pad.Move(deltaTime, true);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					pad.Move(deltaTime, false);
				}
				if (AiEnabled) {
					if (!ball2Existed) {
						if (AiPadMove(pad2, ball) == 1) {
							pad2.Move(deltaTime, true);
						}
						else if (AiPadMove(pad2, ball) == 2) {
							pad2.Move(deltaTime, false);
						}
					}
					else {
						if (ball.shape.getPosition().x > ball2.shape.getPosition().x) {
							if (AiPadMove(pad2, ball) == 1) {
								pad2.Move(deltaTime, true);
							}
							else if (AiPadMove(pad2, ball) == 2) {
								pad2.Move(deltaTime, false);
							}
						}
						else {
							if (AiPadMove(pad2, ball2) == 1) {
								pad2.Move(deltaTime, true);
							}
							else if (AiPadMove(pad2, ball2) == 2) {
								pad2.Move(deltaTime, false);
							}
						}
					}
				}
				else {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
						pad2.Move(deltaTime, true);
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
						pad2.Move(deltaTime, false);
					}
				}
				/*pad.shape.setPosition(0, ball.shape.getPosition().y + pad.shape.getSize().y / 2);
				pad2.shape.setPosition(WIDTH - pad2.shape.getSize().x, ball.shape.getPosition().y + pad2.shape.getSize().y / 2);*/
				window.clear();
				window.draw(scoreLeft);
				window.draw(scoreRight);
				hole.draw(&window);
				ball.draw(&window);
				if (ball2Existed) {
					ball2.draw(&window);
				}
				pad.draw(&window);
				pad2.draw(&window);
				window.display();
			}
			else {
				if (leftWin) {
					gameoverText.setString("Player on the left wins!");
				}
				else {
					gameoverText.setString("Player on the right wins!");
				}
				restart.setString("Press space bar to play again");
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					gameOver = false;
					ball.shape = sf::CircleShape(32.0f);
					ball.shape.setFillColor(sf::Color::Green);
					ball.shape.setPosition(300.0f, 300.0f);
					ball.velocity = sf::Vector2f(1.0, 1.0);
					ball.moveSpeed = 100.0f;
					scoreL = 0;
					scoreR = 0;
					ball2Existed = false;
					ball2.shape.setPosition(500.0f, 500.0f);
					ball2.velocity = sf::Vector2f(1.0, -1.0);
					gameStart = true;
				}
				window.clear();
				window.draw(gameoverText);
				window.draw(restart);
				window.display();
			}
		}
	}

	return 0;
}