#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "mapView.h"

class QPushButton;
class QGroupBox;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLCDNumber;
class QSpinBox;
class QSettings;
class QColor;
class QSize;
class QPoint;
class QStringList;
class QString;

class mainWidget : public QWidget
{
    Q_OBJECT
protected:
	void closeEvent(QCloseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

public:
    mainWidget(QWidget *parent);

	void updateColors();
	void setDefaultColors();

	bool isColorsDefault;

	QColor wallColor;
	QColor wallFrameColor;
	QColor boxColor;
	QColor boxFrameColor;
	QColor placeColor;
	QColor onPlaceColor;
	QColor onPlaceFrameColor;
	QColor gridColor;
	QColor frameColor;

private:
	const static int minWidth = 826;
	const static int minHeight = 640;

	int levelCount;
	int startLevel, stackSize;
	bool isFullScr, isDev;
	bool isKASShown;
	bool wasBestUpdated;

	int bestM, bestW;
	int bestChecksumm;
	QString bestName;

	int bestMoves() { return bestM; };
	int bestMovesWith() { return bestW; };
	QString bestN() { return bestName; };
	void updateBest();

	void readBest();
	void writeBest();

	void showKAS();
	void enterPass();

	void readSettings();
	void writeSettings();

	void setupDefaultColors();
	void setupWindow();
	void setupLayouts();
	void setupGroupBoxes();
	void setupButtons();
	void setupLabels();
	void setupLCDNumbers();
	void setupMapView();
	void setupSpinBoxes();
	void setupStyle();

	void openFailed();

	QStringList pagesList;

	QString styleName;

	QSize windowSize;

	QPoint windowPos;

	QColor defWallColor;
	QColor defWallFrameColor;
	QColor defBoxColor;
	QColor defBoxFrameColor;
	QColor defPlaceColor;
	QColor defOnPlaceColor;
	QColor defOnPlaceFrameColor;
	QColor defGridColor;
	QColor defFrameColor;

	QSettings *globalConfig;
	QSettings *best;

	QGroupBox *groupMain;
	QGroupBox *groupStats;
	QGroupBox *groupMenu;
	QGroupBox *groupHelp;
	QGroupBox *groupAbout;
	QGroupBox *groupTheBest;

	QLabel *labelStatLevel;
	QLabel *labelStatMoves;
	QLabel *labelStatMovesWith;
	QLabel *labelHelp;
	QLabel *labelAbout;
	QLabel *labelTheBest;

	QSpinBox *spinBoxLevel;

	QLCDNumber *numberStatLevel;
	QLCDNumber *numberStatMoves;
	QLCDNumber *numberStatMovesWith;
	QLCDNumber *numberBestMoves;
	QLCDNumber *numberBestWith;

	QPushButton *buttonLoadLevel;
	QPushButton *buttonOpenLevel;
	QPushButton *buttonSaveLevel;
	// QPushButton *buttonShowRecords;
	QPushButton *buttonExit;
	QPushButton *buttonSettings;

	QHBoxLayout *layoutMain;
	QHBoxLayout *layoutGroupMenuLoad;

	QVBoxLayout *layoutGroupMain;
	QVBoxLayout *layoutGroupHelp;
	QVBoxLayout *layoutGroupMenu;
	QVBoxLayout *layoutGroupAbout;

	QGridLayout *layoutGroupStats;
	QGridLayout *layoutGroupTheBest;

	mapView *map;

private slots:
	void loadLevel();
	void openLevel();
	void saveLevel();
	void showSettings();
};

#endif // MAINWIDGET_H
