#include <iostream>
#include "tetris.h"

const int width = 14;
const int height = 20;

//массив с кодировкой блоков тетрамин
int tetra[7][4] = {
	{2, 3, 4, 5}, //O
	{1, 3, 5, 7}, //I
	{2, 4, 5, 7}, //S
	{3, 4, 5, 6}, //Z
	{2, 4, 6, 7}, //L
	{3, 5, 6, 7}, //J
	{3, 4, 5, 7}, //T
};

Tetramino::Tetramino() {
	m_figure = rand() % 7;
	for (int block = 0; block <= 3; block++) {
		m_x[block] = tetra[m_figure][block] % 2;
		m_y[block] = tetra[m_figure][block] / 2;
	}
}

Tetramino::Tetramino(int figure) {
	figure %= 7;
	m_figure = figure;
	for (int block = 0; block <= 3; block++) {
		m_x[block] = tetra[m_figure][block] % 2;
		m_y[block] = tetra[m_figure][block] / 2;
	}
}

int Tetramino::getX(int block) {
	return m_x[block];
}

int Tetramino::getY(int block) {
	return m_y[block];
}

//метод создания новой тетрамины
void Tetramino::newTetra (int figure) {
	figure %= 7;
	m_figure = figure;
	for (int block = 0; block <= 3; block++) {
		m_x[block] = tetra[m_figure][block] % 2;
		m_y[block] = tetra[m_figure][block] / 2;
	}
}

//метод движения тетромины
void Tetramino::move(int dx, int dy) {
	for (int b = 0; b <= 3; b++) {
		m_x[b] += dx;
		m_y[b] += dy;
	}
}

//метод вращения тетромины
void Tetramino::rotate() {
	int center_x = m_x[2], center_y = m_y[2], x, y;
	int border = 0; //переменна для отслеживания выхода за границы при вращении: 1 - выход за левую границу, 0 - все в порядке, -1 - выход за правую границу
	for (int b = 0; b <= 3; b++) {
		x = m_x[b];
		y = m_y[b];
		m_x[b] = center_x - (y - center_y);
		m_y[b] = center_y + (x - center_x);
		if (m_x[b] < 0)
			border++;
		if (m_x[b] > width - 1)
			border--;
	}
	if (border != 0)
		move(border, 0);
}

Field::Field() {
	field = new int* [width];
	for (int x = 0; x < width; x++) {
		field[x] = new int[height] { 0 };
	}
}

void Field::getTetra(Tetramino& const elem, int color, int & const score) {
	int x, y;
	for (int block = 0; block <= 3; block++) {
		x = elem.getX(block);
		y = elem.getY(block);
		if (y >= 0)
			field[x][y] = color;
	}
	score += 10;
}

//функция удаления ряда и смещение верхних рядов вниз
void Field::deleteLines(int& const score) {
	int num_line = 0, count = 0; //переменные для хранения заполненной линии и их количество
	while ((num_line = checkLine()) != -1) {
		count++;
		for (int x = 0; x < width; x++)
			field[x][num_line] = 0;
		for (int x = 0; x < width; x++) {
			for (int y = num_line - 1; y >= 0; y--) {
				field[x][y + 1] = field[x][y];
			}
		}
		for (int x = 0; x < width; x++)
			field[x][0] = 0;
	}
	if (count == 1)
		score += 100;
	else if (count == 2)
		score += 300;
	else if (count == 3)
		score += 700;
	else if (count == 4)
		score += 1500;

}

int Field::getColor(int x, int y) {
	return field[x][y];
}

int Field::getWidth() {
	return width;
}

int Field::getHeight() {
	return height;
}

//функция проверки свободной клетки
bool Field::checkBlock(Tetramino& const elem) {
	int x, y;
	for (int i = 0; i <= 3; i++) {
		x = elem.getX(i);
		y = elem.getY(i);
		if (x < 0 || x > (width - 1) || y > height - 1)
			return false;
		if (y > 0) {
			if (field[x][y])
				return false;
		}
	}
	return true;
}

//функция проверки переполненности поля
bool Field::overflow() {
	for (int x = 0; x < width; x++) {
		if (field[x][0])
			return true;
	}
	return false;
}

//функция поиска первой, начиная снизу, заполненной линии
int Field::checkLine() {
	int count = 0; //переменная для отслеживания заполненных блоков ряда
	for (int y = height - 1; y >= 0; y--) {
		//блок с определением заполненнности ряда
		count = 0;
		for (int x = 0; count < width && field[x][y]; x++, count++) { }
		if (count == width)
			return y;
	}
	return -1;
}

//функция вручного ввода номера цвета
void Field::setColor(int x, int y, int color) {
	if (x >= 0 && x < width && y >= 0 && y < height)
		field[x][y] = color;
}