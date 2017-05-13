#pragma once

class Grid {
public:
	Grid(size_t dim);
	~Grid();

	void reset();

	size_t getDim() const;

	int getHeight(int x, int y) const;
	int getColour(int x, int y) const;

	void incHeight(int x, int y);
	void decHeight(int x, int y);
	void setHeight(int x, int y, int h);

	void setColour(int x, int y, int c);

private:
	size_t m_dim;
	int *m_heights;
	int *m_cols;

	int getIndex(int x, int y) const;
};
