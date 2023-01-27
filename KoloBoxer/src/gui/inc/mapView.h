#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "mainFrame.h"
#include "stack.h"
#include "level.h"

class QPainter;
class QPixmap;
class QImage;
class QSize;
class QString;
class QColor;
class QStringList;

class mapView : public MainFrame
{
    Q_OBJECT

public:
    mapView(QWidget *parent, int stackSize = 10000);
    void paint(QPainter *);
    QSize sizeHint() const { return QSize(600, 600); }

	bool isWin;

    void resizeEvent(QResizeEvent *e);

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void undoMove();
	// void redoMove();

	void showDescription(int );
	void hideDescription();

	QString curLevelName() { return curName; };

	int statLevel() { return curLevel.number; };
	int statMoves() { return moves; };
	int statMovesWith() { return movesWith; };

	void saveLevel(QString fName);
	bool openLevel(int n) { setDescriptionEnabled(false); return startLevel(n); };
	bool openLevel(QString fName) { setDescriptionEnabled(false); return startLevel(fName); };
	void nextLevel();
	void previousLevel();
	void completeLevel();

	void redrawLevel() { setupField(); repaint(); };

	void setDescList(QStringList sl) { pagesList = sl; };

	void setWallColor(QColor c) { wallColor = c; };
	void setWallFrameColor(QColor c) { wallFrameColor = c; };
	void setBoxColor(QColor c) { boxColor = c; };
	void setBoxFrameColor(QColor c) { boxFrameColor = c; };
	void setPlaceColor(QColor c) { placeColor = c; };
	void setOnPlaceColor(QColor c) { onPlaceColor = c; };
	void setOnPlaceFrameColor(QColor c) { onPlaceFrameColor = c; };
	void setGridColor(QColor c) { gridColor = c; };
	void setFrameColor(QColor c) { frameColor = c; };
	void setLevelCount(int n) { levelCount = n; };
	//void setStackSize(int );

public slots:
    void reset();

private:
	int curX, curY;
	int moves, movesWith;
	int boxesNotInPlaces, placesNotWithoutBoxes;
	int levelCount;
	bool mChar, mWBox, toIn, wasIn;
	bool isBlocked;

	//const static int alpha = 155;
	//const static int stackSize = 10000;
	//const static int levelCount = 27;
	const static int width = 600;
	const static int height = 600;
	const static int pixSize = 24;
	const static int boxer = -1;
	const static int empty = 0;
	const static int wall = 1;
	const static int box = 2;
	const static int boxIn = 3;
	const static int toMove = 4;
	const static int charIn = 5;

	enum Napravlenie { left, up, right, down, unLeft, unUp, unRight, unDown};

	void setupStack(int );
	void setupLocals();void setupPainter(QPainter *painter);
	void setupField();

	bool startLevel(int n);
	bool startLevel(QString fileName);
	
	void drawField(QPainter *painter);
	void drawPixmap(QPainter *painter, int pix, int x, int y, int offsetX = 0, int offsetY = 0);
    //void drawText(QPainter *painter, QString text);
	//void loadLevel(QPainter *painter);
	void moveChar(QPainter *painter, Napravlenie napr);
	void moveCharWB(QPainter *painter, Napravlenie napr);

	QPainter *pT;

	QStringList pagesList;

	QImage staticImage;

	QSize S;

	QString path;
	QString curName;

	QColor wallColor;
	QColor wallFrameColor;
	QColor boxColor;
	QColor boxFrameColor;
	QColor placeColor;
	QColor onPlaceColor;
	QColor onPlaceFrameColor;
	QColor gridColor;
	QColor frameColor;

	Level curLevel;
	Napravlenie toW;

	Stack stackMoves;
};

#endif // MAPVIEW_H