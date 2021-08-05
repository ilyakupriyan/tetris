#ifndef TETRIS_H
#define TETRIS_H

class Tetramino {
private:
	int m_figure;
	int m_x[4];
	int m_y[4];
public:
	Tetramino();
	Tetramino(int figure);
	void newTetra (int figure);
	void rotate();
	int getX(int block);
	int getY(int block);
	void move(int dx, int dy);
};

class Field {
private:
	int** field;
	int checkLine();
public:
	Field();
	void getTetra(Tetramino& const elem, int color, int & const score);
	int getColor(int x, int y);
	bool checkBlock(Tetramino& const elem);
	bool overflow();
	void setColor(int x, int y, int color);
	int getWidth();
	int getHeight();
	void deleteLines(int& const score);
};

#endif