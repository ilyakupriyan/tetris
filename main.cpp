#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <time.h>
#include "tetris.h"

using namespace sf;

int main()
{
	srand(time(NULL));
	// Объект, который, собственно, является главным окном приложения
	RenderWindow window(VideoMode(400, 450), "Tetris");

	int color = 1 + rand() % 10, color_next = 1 + rand() % 10, color_bl = 0; //цвет тетрамино, следующей тетрамино и клетки поля
	int score = 0; //переменная для хрения очков

	//создание поля
	Field field;
	int width = field.getWidth();
	int height = field.getHeight();

	//подключение шрифта и обьекта текста для вывода очков
	Font font;
	font.loadFromFile("fonts//visitor1.ttf");
	Text scr ("", font, 30);
	scr.setStyle(sf::Text::Bold);
	scr.setFillColor(Color::Black);
	scr.setPosition(310, 300);

	//строка "конец игры"
	sf::Vector2f centerPos = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);
	Text game_over_t("Game over", font, 40);
	game_over_t.setPosition(centerPos.x - game_over_t.getGlobalBounds().width / 2, centerPos.y - game_over_t.getGlobalBounds().height / 2);
	game_over_t.setStyle(sf::Text::Bold);
	game_over_t.setFillColor(Color::Black);

	//создание заднего фона
	Texture frame;
	frame.loadFromFile("img//frame.png");
	Sprite backgroung(frame);

	//подключаем текстуры блоков
	Texture texture;
	texture.loadFromFile("img//blocks.png");

	//задание цвета первой тетрамино
	Sprite block(texture);
	block.setTextureRect(IntRect(color*18, 0, 18, 18));

	//Создание тетрамино и следующая тетрамино
	Tetramino tetra, next_tetra;
	tetra.move(width / 2 - 1, -3);

	//переменные для таймера и задержки
	float timer = 0, delay = 0.4;

	//переменная для хранения истинности продолжения игры
	bool game_over = false;

	Clock clock;

	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen()) {
		//переменные для отслеживания нажатия клавиш влево, вправо и пробел
		int dx = 0;
		bool rot = false;
		delay = 0.4;

		// Получаем время, прошедшее с начала отсчета, и конвертируем его в секунды
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		// Обрабатываем очередь событий в цикле
		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик»
			if (event.type == Event::Closed)
				window.close();

			//Пользователь нажал клавишу клавиатуры
			if (!game_over) {
				if (event.type == Event::KeyPressed) {
					//клавиша влево
					if (event.key.code == Keyboard::Left) {
						dx = -1;
					}
					//клавиша вправо
					if (event.key.code == Keyboard::Right) {
						dx = 1;
					}
					//клавиша пробел = вращение тетрамины
					if (event.key.code == Keyboard::Space)
						rot = true;
				}
			}
		}


		if (!game_over) {
			//клавиша вниз = ускорение
			if (Keyboard::isKeyPressed(Keyboard::Down))
				delay = 0.035;

			//вращаем тетрамино
			if (rot) {
				tetra.rotate();
			}
			//сохраняем изменения по горизонтальному движению
			if (dx != 0) {
				tetra.move(dx, 0);
				if (!field.checkBlock(tetra)) {
					tetra.move(-dx, 0);
				}
			}
			//опускаем тетрамино
			if (timer > delay) {
				tetra.move(0, 1);
				timer = 0;
				//если тетрамино упала вниз или на заполненную клетку
				if (!field.checkBlock(tetra) && !(game_over = field.overflow())) {
					//добавление тетрамино к полю и удаление заполненных линий
					tetra.move(0, -1);
					field.getTetra(tetra, color, score);
					field.deleteLines(score);
					color = color_next;
					tetra = next_tetra;
					tetra.move(width / 2 - 1, -3);
					next_tetra.newTetra(rand() % 7);
					color_next = 1 + rand() % 10;
				}
			}

			scr.setString(std::to_string(score));
		}

		//блок с отрисовкой
		window.clear(Color::White);
		if (!game_over) {
			//отрисовка заднего фона
			window.draw(backgroung);

			//разукрашиваем поле
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					color_bl = field.getColor(x, y);
					if (color_bl == 0)
						continue;
					block.setTextureRect(IntRect(color_bl * 18, 0, 18, 18));
					block.setPosition(x * 18 + 31, y * 18 + 73);
					window.draw(block);
				}
			}

			//отрисовка тетрамино
			if (field.checkBlock(tetra)) {
				block.setTextureRect(IntRect(color * 18, 0, 18, 18));
				for (int i = 0; i <= 3; i++) {
					if (tetra.getY(i) < 0)
						continue;
					block.setPosition(tetra.getX(i) * 18 + 31, tetra.getY(i) * 18 + 73);
					window.draw(block);
				}
			}

			//отрисовка следующей тетрамино
			block.setTextureRect(IntRect(color_next * 18, 0, 18, 18));
			for (int i = 0; i <= 3; i++) {
				block.setPosition(next_tetra.getX(i) * 18 + 329, next_tetra.getY(i) * 18 + 122);
				window.draw(block);
			}

			window.draw(scr);
		} 
		else 
			window.draw(game_over_t);

		// Отрисовка окна	
		window.display();
	}
	return 0;
}