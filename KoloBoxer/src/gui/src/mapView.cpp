#include "mapView.h"

#include <QLayout>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>

mapView::mapView(QWidget *parent, int stackSize)
    : MainFrame(parent)
{
	setupStack(stackSize);
	setupLocals();

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void mapView::setupStack(int stackSize)
{
	Stack stackMoves(stackSize);
}

void mapView::setupLocals()
{
	curName = ""; isWin = isBlocked = false;
	toW = up; boxesNotInPlaces = 0;
	placesNotWithoutBoxes = 0;
	path = ":/items/resources/img/";
}

void mapView::setupField()
{
	int fSize = 6;
	staticImage = QImage(width+fSize, height+fSize, QImage::Format_RGB32);
	QPainter painter;

	painter.begin(&staticImage);

	painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

	//int level = 241;
	//painter.fillRect(0, 0, width+fSize, height+fSize, QBrush(QColor(level,level,level)));
	painter.fillRect(0, 0, width+fSize, height+fSize, QBrush(Qt::white));

	painter.translate(fSize/2, fSize/2);

	//painter.setPen(QPen(QColor(255, 0, 0, alpha), 1, Qt::DotLine, Qt::FlatCap, Qt::BevelJoin));
	painter.setPen(QPen(frameColor, 1, Qt::DotLine, Qt::FlatCap, Qt::BevelJoin));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRectF(0, 0, width, height).adjusted(-2, -2, 2, 2));

	//painter.setPen(QPen(QColor(0, 255, 0, alpha), 1, Qt::DotLine, Qt::FlatCap, Qt::BevelJoin));
	painter.setPen(QPen(gridColor, 1, Qt::DotLine, Qt::FlatCap, Qt::BevelJoin));

	for(int i=0; i<curLevel.length; i++)
	{
		if(curLevel.coord[i].z == 2)
			drawPixmap(&painter, 0, curLevel.coord[i].x, curLevel.coord[i].y);
		else if(curLevel.coord[i].z == 3)
			drawPixmap(&painter, 4, curLevel.coord[i].x, curLevel.coord[i].y);
		else
			drawPixmap(&painter, curLevel.coord[i].z, curLevel.coord[i].x, curLevel.coord[i].y);
	}

	painter.end();
}

bool mapView::startLevel(int n)
{
	Level level(this, (width/pixSize), (height/pixSize));
	if(!level.loadLevel(n))
		return false;
	else
	{
		curLevel.xSize = level.xSize;
		curLevel.ySize = level.ySize;
		curLevel.length = level.length;
		curLevel.charX = level.charX;
		curLevel.charY = level.charY;
		curLevel.number = level.number;
		for(int i=0; i<curLevel.length; i++) {
			curLevel.coord[i].x = level.coord[i].x;
			curLevel.coord[i].y = level.coord[i].y;
			curLevel.coord[i].z = level.coord[i].z;
		}

		movesWith = level.movesWith; moves = level.moves;
		mChar = false; mWBox = false;
		toIn = false; wasIn = false;
		isWin = isBlocked = false;
		stackMoves.clean(); 
		setupField();
		repaint(); 
	}
	return true;
}

bool mapView::startLevel(QString fileName)
{
	Level level(this, width/pixSize, height/pixSize);
	if(!level.loadLevel(fileName))
		return false;
	else 
	{
		curLevel.xSize = level.xSize;
		curLevel.ySize = level.ySize;
		curLevel.length = level.length;
		curLevel.charX = level.charX;
		curLevel.charY = level.charY;
		curLevel.number = level.number;
		for(int i=0; i<curLevel.length; i++) {
			curLevel.coord[i].x = level.coord[i].x;
			curLevel.coord[i].y = level.coord[i].y;
			curLevel.coord[i].z = level.coord[i].z;
		}
	
		movesWith = level.movesWith; moves = level.moves;
		mChar = false; mWBox = false;
		toIn = false; wasIn = false;
		isWin = isBlocked = false;
		curName = fileName;
		stackMoves.clean(); 
		setupField();
		repaint(); 
	}	
	return true;
}

void mapView::saveLevel(QString fName)
{
	curLevel.moves = moves;
	curLevel.movesWith = movesWith;
	curLevel.saveLevel(fName);
}

void mapView::paint(QPainter *p)
{
	setupPainter(p);
	//setupField(p);
	drawField(p);

	if(isBlocked != true)
	{
		if(mChar == true)
			moveChar(p, toW);
		else if(mWBox == true)
			moveCharWB(p, toW);
		else
			drawPixmap(p, boxer, curLevel.charX-1, curLevel.charY-1);
	} else
		drawPixmap(p, boxer, curLevel.charX-1, curLevel.charY-1);

	if( (boxesNotInPlaces == 0) || (placesNotWithoutBoxes == 0) ) { 
		completeLevel();
	}

	placesNotWithoutBoxes = 0;
	boxesNotInPlaces = 0;

	p->restore();
}

void mapView::setupPainter(QPainter *painter)
{
	painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
	painter->setPen(QPen(gridColor, 1, Qt::DotLine, Qt::FlatCap, Qt::BevelJoin));

	S = size();
	QPointF corner((S.width()-width)/2, (S.height()-height)/2);
    painter->translate(corner);
}

void mapView::drawField(QPainter *painter)
{
	painter->drawImage(QPointF(-3, -3), staticImage);

	for(int i=0; i<curLevel.length; i++)
	{
		//drawPixmap(painter, curLevel.coord[i].z, curLevel.coord[i].x, curLevel.coord[i].y);
		if(curLevel.coord[i].z == 2)
			drawPixmap(painter, 2, curLevel.coord[i].x, curLevel.coord[i].y);
		else if(curLevel.coord[i].z == 3)
			drawPixmap(painter, 3, curLevel.coord[i].x, curLevel.coord[i].y);
		else if(curLevel.coord[i].z == 4)
			placesNotWithoutBoxes++;
	}
}

void mapView::drawPixmap(QPainter *painter, int pix, int x, int y, int offsetX, int offsetY)
{
	painter->save();

	QLinearGradient linearGradient(QPointF(x*pixSize+offsetX, y*pixSize+offsetY), 
		QPointF((x+1)*pixSize, (y+1)*pixSize));
	switch(pix) {
		case -1:
			painter->drawPixmap(QPointF(x*pixSize+offsetX+3, y*pixSize+offsetY+3), 
				QPixmap(QString(path+"kolobok.png")));
			break;

		case 0: 
			painter->drawRect(QRectF(x*pixSize, y*pixSize, pixSize, pixSize).adjusted(2, 2, -2, -2));
			break;

		case 1:
			linearGradient.setColorAt(0.0, Qt::white);
			linearGradient.setColorAt(0.2, wallColor); 
			//linearGradient.setColorAt(0.2, QColor(200, 0, 0, alpha)); 
            linearGradient.setColorAt(1.0, Qt::black);

			//painter->setPen(QPen(QColor(155, 155, 155, alpha), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
			painter->setPen(QPen(wallFrameColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
            painter->setBrush(linearGradient);

			painter->drawRoundRect(QRectF(x*pixSize, y*pixSize, pixSize, pixSize).adjusted(2, 2, -2, -2));
			break;

		case 2:
			//painter->drawRect(QRectF(x*pixSize, y*pixSize, pixSize, pixSize).adjusted(2, 2, -2, -2));

			linearGradient.setColorAt(0.0, Qt::white);
            linearGradient.setColorAt(0.2, boxColor);
            linearGradient.setColorAt(1.0, Qt::black);

			//painter->setPen(QPen(QColor(155, 155, 155, alpha), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
			painter->setPen(QPen(boxFrameColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
            painter->setBrush(linearGradient);

			painter->drawRoundRect(QRectF(x*pixSize, y*pixSize, pixSize, pixSize).adjusted(5, 5, -5, -5));

			boxesNotInPlaces++;
			break;

		case 3:
			//painter->setPen(QPen(QColor(0, 255, 100, alpha), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
			//painter->setPen(QPen(placeColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
			//painter->drawRect(QRectF(x*pixSize, y*pixSize, pixSize, pixSize).adjusted(2, 2, -2, -2));

			linearGradient.setColorAt(0.0, Qt::white);
            linearGradient.setColorAt(0.2, onPlaceColor);
			//linearGradient.setColorAt(0.2, QColor(0, 65, 220, alpha));
            linearGradient.setColorAt(1.0, Qt::black);

			//painter->setPen(QPen(QColor(0, 255, 0, alpha), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
			painter->setPen(QPen(onPlaceFrameColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
            painter->setBrush(linearGradient);

			painter->drawRoundRect(QRectF(x*pixSize, y*pixSize, pixSize, pixSize).adjusted(5, 5, -5, -5));
			break;

		case 4:
			//painter->setPen(QPen(QColor(0, 255, 0, alpha), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
			painter->setPen(QPen(placeColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
			painter->drawRect(QRectF(x*pixSize, y*pixSize, pixSize, pixSize).adjusted(2, 2, -2, -2));

			linearGradient.setColorAt(0.0, Qt::white);
            //linearGradient.setColorAt(0.2, QColor(0, 255, 0, alpha));
			linearGradient.setColorAt(0.2, placeColor);
            linearGradient.setColorAt(1.0, Qt::black);

			//painter->setPen(QPen(QColor(0, 255, 0, alpha), 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
            painter->setBrush(linearGradient);

			painter->drawEllipse(QRectF(x*pixSize, y*pixSize, pixSize, pixSize).adjusted(10, 10, -10, -10));

			placesNotWithoutBoxes++;
			break;

		//default:
			//painter->drawPixmap(QPointF(x*pixSize, y*pixSize), QPixmap(QString(path+"map%1.png").arg(pix)));
			//break;
	}

	//painter->setPen(QPen(QColor(0, 255, 0, alpha), 1, Qt::DotLine, Qt::FlatCap, Qt::BevelJoin));
	//painter->setBrush(Qt::NoBrush);

	painter->restore();
}

void mapView::moveChar(QPainter *painter, Napravlenie napr)
{
	switch(napr) {
		case up:
			drawPixmap(painter, boxer, curLevel.charX-1, curLevel.charY-2);
			break;

		case down:
			drawPixmap(painter, boxer, curLevel.charX-1, curLevel.charY);
			break;

		case left:
			drawPixmap(painter, boxer, curLevel.charX-2, curLevel.charY-1);
			break;

		case right:
			drawPixmap(painter, boxer, curLevel.charX, curLevel.charY-1);
			break;
	}
}

void mapView::moveCharWB(QPainter *painter, Napravlenie napr)
{
	switch(napr) {
		case up:
			if(toIn == false) {
				curLevel.coord[(curLevel.charY-3)*curLevel.xSize + curLevel.charX -1].z = box;
				drawPixmap(painter, boxer, curLevel.charX-1, curLevel.charY-2);
				drawPixmap(painter, box, curLevel.charX-1, curLevel.charY-3);
			} else 
			if(toIn == true) {
				curLevel.coord[(curLevel.charY-3)*curLevel.xSize + curLevel.charX -1].z = boxIn;
				drawPixmap(painter, boxer, curLevel.charX-1, curLevel.charY-2);
				drawPixmap(painter, boxIn, curLevel.charX-1, curLevel.charY-3);
			}
			break;

		case down:
			if(toIn == false) {
				curLevel.coord[(curLevel.charY+1)*curLevel.xSize + curLevel.charX -1].z = box;
				drawPixmap(painter, boxer, curLevel.charX-1, curLevel.charY);
				drawPixmap(painter, box, curLevel.charX-1, curLevel.charY+1);
			} else 
			if(toIn == true) {
				curLevel.coord[(curLevel.charY+1)*curLevel.xSize + curLevel.charX -1].z = boxIn;
				drawPixmap(painter, boxer, curLevel.charX-1, curLevel.charY);
				drawPixmap(painter, boxIn, curLevel.charX-1, curLevel.charY+1);
			}
			break;

		case left:
			if(toIn == false) {
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -3].z = box;
				drawPixmap(painter, boxer, curLevel.charX-2, curLevel.charY-1);
				drawPixmap(painter, box, curLevel.charX-3, curLevel.charY-1);
			} else 
			if(toIn == true) {
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -3].z = boxIn;
				drawPixmap(painter, boxer, curLevel.charX-2, curLevel.charY-1);
				drawPixmap(painter, boxIn, curLevel.charX-3, curLevel.charY-1);
			}
			break;

		case right:
			if(toIn == false) {
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX +1].z = box;
				drawPixmap(painter, boxer, curLevel.charX, curLevel.charY-1);
				drawPixmap(painter, box, curLevel.charX+1, curLevel.charY-1);	
			} else 
			if(toIn == true) {
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX +1].z = boxIn;
				drawPixmap(painter, boxer, curLevel.charX, curLevel.charY-1);
				drawPixmap(painter, boxIn, curLevel.charX+1, curLevel.charY-1);	
			}
			break;

		case unDown:
			drawPixmap(painter, boxer, curLevel.charX-1, curLevel.charY-2);
			if(curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z == toMove)
			{
				drawPixmap(painter, boxIn, curLevel.charX-1, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z = boxIn;
			} else
			{
				drawPixmap(painter, box, curLevel.charX-1, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z = box;
			}

			if(curLevel.coord[(curLevel.charY)*curLevel.xSize + curLevel.charX -1].z == boxIn)
			{
				drawPixmap(painter, toMove, curLevel.charX-1, curLevel.charY);
				curLevel.coord[(curLevel.charY)*curLevel.xSize + curLevel.charX -1].z = toMove;
			} else
			{
				drawPixmap(painter, empty, curLevel.charX-1, curLevel.charY);
				curLevel.coord[(curLevel.charY)*curLevel.xSize + curLevel.charX -1].z = empty;
			}
			break;

		case unUp:
			drawPixmap(painter, boxer, curLevel.charX-1, curLevel.charY);
			if(curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z == toMove)
			{
				drawPixmap(painter, boxIn, curLevel.charX-1, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z = boxIn;
			} else
			{
				drawPixmap(painter, box, curLevel.charX-1, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z = box;
			}

			if(curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z == boxIn)
			{
				drawPixmap(painter, toMove, curLevel.charX-1, curLevel.charY-2);
				curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z = toMove;
			} else
			{
				drawPixmap(painter, empty, curLevel.charX-1, curLevel.charY-2);
				curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z = empty;
			}
			break;

		case unLeft:
			drawPixmap(painter, boxer, curLevel.charX, curLevel.charY-1);
			if(curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z == toMove)
			{
				drawPixmap(painter, boxIn, curLevel.charX-1, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z = boxIn;
			} else
			{
				drawPixmap(painter, box, curLevel.charX-1, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z = box;
			}

			if(curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z == boxIn)
			{
				drawPixmap(painter, toMove, curLevel.charX-2, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z = toMove;
			} else
			{
				drawPixmap(painter, empty, curLevel.charX-2, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z = empty;
			}
			break;

		case unRight:
			drawPixmap(painter, boxer, curLevel.charX-2, curLevel.charY-1);
			if(curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z == toMove)
			{
				drawPixmap(painter, boxIn, curLevel.charX-1, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z = boxIn;
			} else
			{
				drawPixmap(painter, box, curLevel.charX-1, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -1].z = box;
			}

			if(curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z == boxIn)
			{
				drawPixmap(painter, toMove, curLevel.charX, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z = toMove;
			} else
			{
				drawPixmap(painter, empty, curLevel.charX, curLevel.charY-1);
				curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z = empty;
			}
			break;
	}
}

void mapView::moveUp()
{
	if(isBlocked != false) return;

	if( ( curLevel.charY-2 < 0 ) || ( curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z == wall ) )
	{

	} else
	if( ( curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z == empty ) ||
		( curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z == toMove ) )
	{
		mChar = true;
	} else 
	if(	( curLevel.coord[(curLevel.charY-3)*curLevel.xSize + curLevel.charX -1].z == box ) 
		|| ( curLevel.coord[(curLevel.charY-3)*curLevel.xSize + curLevel.charX -1].z == boxIn ) )
	{

	} else
	if( ( curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z == box ) &&
		( curLevel.charY-2 > 0 ) && ( curLevel.coord[(curLevel.charY-3)*curLevel.xSize + curLevel.charX -1].z == empty ) )
	{
		curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z = empty;
		mWBox = true;
	} else
	if( ( curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z == box ) &&
		( curLevel.charY-2 > 0 ) && ( curLevel.coord[(curLevel.charY-3)*curLevel.xSize + curLevel.charX -1].z == toMove ) )
	{
		curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z = empty;
		mWBox = true; toIn = true;
	} else
	if( ( curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z == boxIn ) &&
		( curLevel.charY-2 > 0 ) && ( curLevel.coord[(curLevel.charY-3)*curLevel.xSize + curLevel.charX -1].z == empty ) )
	{
		curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z = toMove;
		mWBox = true;  wasIn = true;
	} else
	if( ( curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z == boxIn ) &&
		( curLevel.charY-2 > 0 ) && ( curLevel.coord[(curLevel.charY-3)*curLevel.xSize + curLevel.charX -1].z == toMove ) )
	{
		curLevel.coord[(curLevel.charY-2)*curLevel.xSize + curLevel.charX -1].z = toMove;
		mWBox = true;  wasIn = true; toIn = true;
	}

	if((mChar == true) || (mWBox == true))
	{
		toW = up; moves++;
		if(mWBox == true) {
			movesWith++;
			repaint(); 
		}
		update();
		curLevel.charY--;
		stackMoves.push(0,-1,mWBox);
	}

	mChar = false; mWBox = false;
	wasIn = false; toIn = false;
}

void mapView::moveDown()
{
	if(isBlocked != false) return;

	if( ( curLevel.charY == height/pixSize ) || 
		( curLevel.coord[curLevel.charY*curLevel.xSize + curLevel.charX -1].z == wall ) )
	{

	} else
	if( ( curLevel.coord[(curLevel.charY)*curLevel.xSize + curLevel.charX -1].z == empty ) ||
		( curLevel.coord[(curLevel.charY)*curLevel.xSize + curLevel.charX -1].z == toMove ) )
	{
		mChar = true;
	} else 
	if(	( curLevel.coord[(curLevel.charY+1)*curLevel.xSize + curLevel.charX -1].z == box ) 
		|| ( curLevel.coord[(curLevel.charY+1)*curLevel.xSize + curLevel.charX -1].z == boxIn ) )
	{

	} else
	if( ( curLevel.coord[curLevel.charY*curLevel.xSize + curLevel.charX -1].z == box ) &&
		( curLevel.charY < height/pixSize ) && 
		( curLevel.coord[(curLevel.charY+1)*curLevel.xSize + curLevel.charX -1].z == empty ) )
	{
		curLevel.coord[curLevel.charY*curLevel.xSize + curLevel.charX -1].z = empty;
		mWBox = true;
	} else
	if( ( curLevel.coord[curLevel.charY*curLevel.xSize + curLevel.charX -1].z == box ) &&
		( curLevel.charY < height/pixSize ) && 
		( curLevel.coord[(curLevel.charY+1)*curLevel.xSize + curLevel.charX -1].z == toMove ) )
	{
		curLevel.coord[curLevel.charY*curLevel.xSize + curLevel.charX -1].z = empty;
		mWBox = true; toIn = true;
	} else
	if( ( curLevel.coord[curLevel.charY*curLevel.xSize + curLevel.charX -1].z == boxIn ) &&
		( curLevel.charY < height/pixSize ) && 
		( curLevel.coord[(curLevel.charY+1)*curLevel.xSize + curLevel.charX -1].z == empty ) )
	{
		curLevel.coord[curLevel.charY*curLevel.xSize + curLevel.charX -1].z = toMove;
		mWBox = true;  wasIn = true;
	} else
	if( ( curLevel.coord[curLevel.charY*curLevel.xSize + curLevel.charX -1].z == boxIn ) &&
		( curLevel.charY < height/pixSize ) && 
		( curLevel.coord[(curLevel.charY+1)*curLevel.xSize + curLevel.charX -1].z == toMove ) )
	{
		curLevel.coord[curLevel.charY*curLevel.xSize + curLevel.charX -1].z = toMove;
		mWBox = true;  wasIn = true; toIn = true;
	}

	if((mChar == true) || (mWBox == true))
	{
		toW = down; moves++;
		if(mWBox == true) {
			movesWith++;
			repaint(); 
		}
		update();
		curLevel.charY++;
		stackMoves.push(0,1,mWBox);
	}

	mChar = false; mWBox = false;
	wasIn = false; toIn = false;
}

void mapView::moveLeft()
{
	if(isBlocked != false) return;

	if( ( curLevel.charX-2 < 0 ) || ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z == wall ) )
	{

	} else
	if( ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z == empty ) ||
		( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z == toMove ) )
	{
		mChar = true;
	} else 
	if(	( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -3].z == box ) 
		|| ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -3].z == boxIn ) )
	{

	} else
	if( ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z == box ) &&
		( curLevel.charX-2 > 0 ) && ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -3].z == empty ) )
	{
		curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z = empty;
		mWBox = true;
	} else
	if( ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z == box ) &&
		( curLevel.charX-2 > 0 ) && ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -3].z == toMove ) )
	{
		curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z = empty;
		mWBox = true; toIn = true;
	} else
	if( ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z == boxIn ) &&
		( curLevel.charX-2 > 0 ) && ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -3].z == empty ) )
	{
		curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z = toMove;
		mWBox = true;  wasIn = true;
	} else
	if( ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z == boxIn ) &&
		( curLevel.charX-2 > 0 ) && ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -3].z == toMove ) )
	{
		curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX -2].z = toMove;
		mWBox = true;  wasIn = true; toIn = true;
	}

	if((mChar == true) || (mWBox == true))
	{
		toW = left; moves++;
		if(mWBox == true) {
			movesWith++;
			repaint(); 
		}
		update();
		curLevel.charX--;
		stackMoves.push(-1,0,mWBox);
	}

	mChar = false; mWBox = false;
	wasIn = false; toIn = false;
}

void mapView::moveRight()
{
	if(isBlocked != false) return;

	if( ( curLevel.charX == width/pixSize ) || 
		( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z == wall ) )
	{

	} else
	if( ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z == empty ) ||
		( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z == toMove ) )
	{
		mChar = true;
	} else 
	if(	( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX +1].z == box ) 
		|| ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX +1].z == boxIn ) )
	{

	} else
	if( ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z == box ) &&
		( curLevel.charX+1 < width/pixSize ) && 
		( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX +1].z == empty ) )
	{
		curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z = empty;
		mWBox = true;
	} else
	if( ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z == box ) &&
		( curLevel.charX+1 < width/pixSize ) && 
		( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX +1].z == toMove ) )
	{
		curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z = empty;
		mWBox = true; toIn = true;
	} else
	if( ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z == boxIn ) &&
		( curLevel.charX+1 < width/pixSize ) && 
		( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX +1].z == empty ) )
	{
		curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z = toMove;
		mWBox = true;  wasIn = true;
	} else
	if( ( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z == boxIn ) &&
		( curLevel.charX+1 < width/pixSize ) && 
		( curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX +1].z == toMove ) )
	{
		curLevel.coord[(curLevel.charY-1)*curLevel.xSize + curLevel.charX].z = toMove;
		mWBox = true;  wasIn = true; toIn = true;
	}

	if((mChar == true) || (mWBox == true))
	{
		toW = right; moves++;
		if(mWBox == true) {
			movesWith++;
			repaint(); 
		}
		update();
		curLevel.charX++;
		stackMoves.push(1,0,mWBox);
	}

	mChar = false; mWBox = false;
	wasIn = false; toIn = false;
}

void mapView::undoMove()
{
	if(isBlocked == false)
	{
		bool wasAviable = false;

		Vect last = stackMoves.pop();

		if(last.t > 0)
		{
			if(last.withBox == false)
			{
				if(last.y == 1) {
					toW = up; mChar = true;
					//repaint(); 
					update();
					curLevel.charY--; //moveUp();
					wasAviable = true; 
				} else
				if(last.y == -1) {
					toW = down; mChar = true;
					//repaint(); 
					update();
					curLevel.charY++; //moveDown();
					wasAviable = true; 
				} else
				if(last.x == -1) {
					toW = right; mChar = true;
					//repaint();
					update();
					curLevel.charX++; //moveRight();
					wasAviable = true;
				} else
				if(last.x == 1) {
					toW = left; mChar = true;
					//repaint(); 
					update();
					curLevel.charX--; //moveLeft();
					wasAviable = true; 
				}
			} else
			{
				if(last.y == 1) {
					toW = unDown; mWBox = true;
					repaint(); update();
					curLevel.charY--; //moveUp();
					wasAviable = true; 
				} else
				if(last.y == -1) {
					toW = unUp; mWBox = true;
					repaint(); update();
					curLevel.charY++; //moveDown();
					wasAviable = true; 
				} else
				if(last.x == -1) {
					toW = unLeft; mWBox = true;
					repaint(); update();
					curLevel.charX++; //moveRight();
					wasAviable = true;
				} else
				if(last.x == 1) {
					toW = unRight; mWBox = true;
					repaint(); update();
					curLevel.charX--; //moveLeft();
					wasAviable = true; 
				}
			}

			if(wasAviable == true) {
				mChar = false; mWBox = false; moves--;
				if(last.withBox == true) movesWith--;
			}
		}
	}
}

/* void mapView::redoMove()
{
	Vect last = stackMoves.up();

	if(last.y == -1) {
		moveUp();
	} else
	if(last.y == 1) {
		moveDown();
	} else
	if(last.x == 1) {
		moveRight();
	} else
	if(last.x == -1) {
		moveLeft();
	}
} */

void mapView::reset()
{

}

void mapView::nextLevel()
{
	curName = ""; isWin = isBlocked = false;
	startLevel((curLevel.number+1)%(levelCount+1));
	setDescriptionEnabled(false);
}

void mapView::previousLevel() 
{
	curName = ""; isWin = isBlocked = false;
	if(curLevel.number == 0)
		startLevel(levelCount);
	else
		startLevel((curLevel.number-1)%(levelCount+1));

	setDescriptionEnabled(false);
}

void mapView::showDescription(int n)
{
	loadDescription(pagesList.at(n));
	setDescriptionEnabled(true);
	isBlocked = true; repaint();
}

void mapView::hideDescription()
{
	loadDescription(pagesList.at(1));
	setDescriptionEnabled(false);
	isBlocked = false;
}

void mapView::completeLevel()
{
	loadDescription(pagesList.at(1));
	setDescriptionEnabled(true);
	isWin = isBlocked = true; 
}

void mapView::resizeEvent(QResizeEvent *)
{
    
}
