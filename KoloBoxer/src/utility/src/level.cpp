#include <QFile>
#include <QTextstream>

#include "level.h"

Level::Level(QObject *parent, int sizeX, int sizeY)
	: QObject(parent)
{
	length=sizeX*sizeY;
	coord = new Coord[length];
	xSize=sizeX; ySize=sizeY;
	path = "levels/";
}

Level::~Level()
{
	delete[] coord;
}

bool Level::loadLevel(int n)
{
	QFile file(QString(path+"map%1.txt").arg(n));
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QTextStream in(&file);

	QString name;
	in >> name;

	if(name != "KAS")
		return false;

	for(int i=0; i<length; i++) {
		in >> coord[i].z;
		coord[i].x = i%xSize;
		coord[i].y = i/xSize;
	}

	in >> charX >> charY >> moves >> movesWith >> number;

	if( (coord[charX+xSize*(charY-1)-1].z != 0) &&
		(coord[charX+xSize*(charY-1)-1].z != 4) )
		return false;

    return true;
}

bool Level::loadLevel(QString fileName)
{
	QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

	QTextStream in(&file);

	QString name;
	in >> name;

	if(name != "KAS")
		return false;

	for(int i=0; i<xSize*ySize; i++) {
		in >> coord[i].z;
		coord[i].x = i%xSize;
		coord[i].y = i/xSize;
	}

	in >> charX >> charY >> moves >> movesWith >> number;

	if((moves<0) || (movesWith<0) || (movesWith>moves))
		return false;

	if( (coord[charX+xSize*(charY-1)-1].z != 0) &&
		(coord[charX+xSize*(charY-1)-1].z != 4) )
		return false;

    return true;
}

bool Level::saveLevel(QString fileName)
{
	QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }

	QTextStream out(&file);

	QString name = "KAS";
	out << name << '\n';

	for(int i=0; i<xSize*ySize; i++) {
		out << coord[i].z << " ";
		if((i+1)%xSize == 0) out << '\n';
	}

	out << charX << " " << charY << " " 
		<< moves << " " << movesWith << " " 
		<< number;

    return true;
}
