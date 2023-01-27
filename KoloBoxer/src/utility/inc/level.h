#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>

class QString;

struct Coord {
	int x,y,z;
};

class Level : public QObject
{
    Q_OBJECT

public:
	Level(QObject *parent = 0, int sizeX = 25, int sizeY = 25);
	~Level();

	bool saveLevel(QString fileName);
	bool loadLevel(int n);
	bool loadLevel(QString fileName);

	Coord *coord;

	int length, number;
	int xSize,ySize;
	int charX, charY;
	int moves, movesWith;

private:
	QString path;

};

#endif // LEVEL_H
