#include <QPixmap>
#include <QPlastiqueStyle>
#include <QStyle>
#include <QColor>
#include <QColorDialog>

#include "cStsDialog.h"

cStsDialog::cStsDialog(QWidget *pr, mainWidget *p, bool isD)
	: QDialog(pr)
{
	parent		= p; 
	isDefault	= isD;

	setupColors();
	setupColorsGroup();
	setupDefButton();
	setupButtons();
	setupColorButtons();
	setupSpins();
	setupLabels();
	setupLayout();

	setupWindow();
}

void cStsDialog::setupWindow()
{
	setModal(true);
	setWindowTitle(QString::fromLocal8Bit("Настройки"));
	setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);

	defCheck->setChecked(isDefault);
	resize(380, 290);

	QStyle *curStyle = new QPlastiqueStyle;
	setStyle(curStyle);

	QList<QWidget *> widgets = qFindChildren<QWidget *>(this);
	foreach (QWidget *w, widgets)
		w->setStyle(curStyle);
}

void cStsDialog::setupColors()
{
	wallFrame		= parent->wallFrameColor;
	wallFill		= parent->wallColor;
	boxFrame		= parent->boxFrameColor;
	boxFill			= parent->boxColor;
	onPlaceFrame	= parent->onPlaceFrameColor;
	onPlaceFill		= parent->onPlaceColor;
	place			= parent->placeColor; 
	grid			= parent->gridColor;
	frame			= parent->frameColor;
}

void cStsDialog::setupColorsGroup()
{
	chooseBox = new QGroupBox(QString::fromLocal8Bit("Цветовая палитра"));
}

void cStsDialog::setupDefButton()
{
	defCheck = new QCheckBox();
	defCheck->setText(QString::fromLocal8Bit("Использовать стандартные настройки"));
	connect(defCheck, SIGNAL(toggled(bool)),
		chooseBox, SLOT(setDisabled(bool)));
}

void cStsDialog::setupButtons()
{
	btnOk = new QPushButton("Ok");
	btnCancel = new QPushButton(QString::fromLocal8Bit("Отмена")); 
	btnApply = new QPushButton(QString::fromLocal8Bit("Применить"));

	connect(btnOk, SIGNAL(clicked()), this, SLOT(ok()));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(btnApply, SIGNAL(clicked()), this, SLOT(apply()));
}

void cStsDialog::setupSpins()
{
	sBox = new QSpinBox;
    sBox->setRange(0, 255);
    sBox->setSingleStep(1);
	sBox->setWrapping(true);
    sBox->setValue(boxFill.alpha());
	sBox->setFocusPolicy(Qt::NoFocus);
	connect(sBox, SIGNAL(valueChanged(int )), this, SLOT(zBox(int )));
		
	sWall = new QSpinBox;
    sWall->setRange(0, 255);
    sWall->setSingleStep(1);
	sWall->setWrapping(true);
    sWall->setValue(wallFill.alpha());
	sWall->setFocusPolicy(Qt::NoFocus);
	connect(sWall, SIGNAL(valueChanged(int )), this, SLOT(zWall(int )));
	
	soBox = new QSpinBox;
    soBox->setRange(0, 255);
    soBox->setSingleStep(1);
	soBox->setWrapping(true);
    soBox->setValue(onPlaceFill.alpha());
	soBox->setFocusPolicy(Qt::NoFocus); 
	connect(soBox, SIGNAL(valueChanged(int )), this, SLOT(zoBox(int )));

	sPlace = new QSpinBox;
    sPlace->setRange(0, 255);
    sPlace->setSingleStep(1);
	sPlace->setWrapping(true);
    sPlace->setValue(place.alpha());
	sPlace->setFocusPolicy(Qt::NoFocus);
	connect(sPlace, SIGNAL(valueChanged(int )), this, SLOT(zPlace(int )));

	sGrid = new QSpinBox;
    sGrid->setRange(0, 255);
    sGrid->setSingleStep(1);
	sGrid->setWrapping(true);
    sGrid->setValue(grid.alpha());
	sGrid->setFocusPolicy(Qt::NoFocus); 
	connect(sGrid, SIGNAL(valueChanged(int )), this, SLOT(zGrid(int )));
	
	sFrame = new QSpinBox;
    sFrame->setRange(0, 255);
    sFrame->setSingleStep(1);
	sFrame->setWrapping(true);
    sFrame->setValue(frame.alpha());
	sFrame->setFocusPolicy(Qt::NoFocus);
	connect(sFrame, SIGNAL(valueChanged(int )), this, SLOT(zFrame(int )));
}

void cStsDialog::setupColorButtons()
{
	QPixmap pix(10, 10);

	pix.fill(boxFrame);
	colBF = new QPushButton(pix, "");
	colBF->setFocusPolicy(Qt::NoFocus);
	colBF->setFlat(true);
	connect(colBF, SIGNAL(clicked()), this, SLOT(scolBF()));

	pix.fill(boxFill);
	colBB = new QPushButton(pix, "");
	colBB->setFocusPolicy(Qt::NoFocus);
	colBB->setFlat(true);
	connect(colBB, SIGNAL(clicked()), this, SLOT(scolBB()));

	pix.fill(wallFrame);
	colWF = new QPushButton(pix, "");
	colWF->setFocusPolicy(Qt::NoFocus);
	colWF->setFlat(true);
	connect(colWF, SIGNAL(clicked()), this, SLOT(scolWF()));

	pix.fill(wallFill);
	colWB = new QPushButton(pix, "");
	colWB->setFocusPolicy(Qt::NoFocus);
	colWB->setFlat(true);
	connect(colWB, SIGNAL(clicked()), this, SLOT(scolWB()));

	pix.fill(onPlaceFrame);
	colOF = new QPushButton(pix, "");
	colOF->setFocusPolicy(Qt::NoFocus);
	colOF->setFlat(true);
	connect(colOF, SIGNAL(clicked()), this, SLOT(scolOF()));

	pix.fill(onPlaceFill);
	colOB = new QPushButton(pix, "");
	colOB->setFocusPolicy(Qt::NoFocus);
	colOB->setFlat(true);
	connect(colOB, SIGNAL(clicked()), this, SLOT(scolOB()));

	pix.fill(place);
	colP = new QPushButton(pix, "");
	colP->setFocusPolicy(Qt::NoFocus);
	colP->setFlat(true);
	connect(colP, SIGNAL(clicked()), this, SLOT(scolP()));

	pix.fill(grid);
	colG = new QPushButton(pix, "");
	colG->setFocusPolicy(Qt::NoFocus);
	colG->setFlat(true);
	connect(colG, SIGNAL(clicked()), this, SLOT(scolG()));

	pix.fill(frame);
	colF = new QPushButton(pix, "");
	colF->setFocusPolicy(Qt::NoFocus);
	colF->setFlat(true);
	connect(colF, SIGNAL(clicked()), this, SLOT(scolF()));


	pix.fill(Qt::white);

	colDP = new QPushButton(pix, "");
	colDP->setFocusPolicy(Qt::NoFocus);
	colDP->setFlat(true);
	colDP->setDisabled(true);
	
	colDG = new QPushButton(pix, "");
	colDG->setFocusPolicy(Qt::NoFocus);
	colDG->setFlat(true);
	colDG->setDisabled(true);
	
	colDF = new QPushButton(pix, "");
	colDF->setFocusPolicy(Qt::NoFocus);
	colDF->setFlat(true);
	colDF->setDisabled(true);
}

void cStsDialog::setupLabels()
{
	lBox	= new QLabel(QString::fromLocal8Bit("<b>Ящик:</b>"));
	lWall	= new QLabel(QString::fromLocal8Bit("<b>Стенка:</b>"));
	loBox	= new QLabel(QString::fromLocal8Bit("<b>На месте:</b>"));
	lPlace	= new QLabel(QString::fromLocal8Bit("<b>Место:</b>"));
	lGrid	= new QLabel(QString::fromLocal8Bit("<b>Сетка:</b>"));
	lFrame	= new QLabel(QString::fromLocal8Bit("<b>Контур:</b>"));
	lInf	= new QLabel(QString::fromLocal8Bit(
		"Здесь вы можете выбрать цвета для большинства элментов"
		" игровой палитры. Цвет заливки и контура выбираются"
		" отдельно. Числовое значение - множитель альфа-смешивания." ));
	lInf->setWordWrap(true);
	lFill	= new QLabel(QString::fromLocal8Bit("<b>Заливка</b>"));
	lFill->setAlignment(Qt::AlignCenter);
	lFr	= new QLabel(QString::fromLocal8Bit("<b>Контур</b>"));
	lFr->setAlignment(Qt::AlignCenter);
	lAlpha	= new QLabel(QString::fromLocal8Bit("<b>Альфа</b>"));
	lAlpha->setAlignment(Qt::AlignCenter);
}

void cStsDialog::setupLayout()
{
	colsLay = new QGridLayout();
	colsLay->addWidget(lFill, 0, 2);
	colsLay->addWidget(lFr, 0, 3);
	colsLay->addWidget(lAlpha, 0, 4);

	colsLay->addWidget(lWall, 1, 1);
	colsLay->addWidget(colWB, 1, 2);
	colsLay->addWidget(colWF, 1, 3);
	colsLay->addWidget(sWall, 1, 4);

	colsLay->addWidget(lBox, 2, 1);
	colsLay->addWidget(colBB, 2, 2);
	colsLay->addWidget(colBF, 2, 3);
	colsLay->addWidget(sBox, 2, 4);

	colsLay->addWidget(loBox, 3, 1);
	colsLay->addWidget(colOB, 3, 2);
	colsLay->addWidget(colOF, 3, 3);
	colsLay->addWidget(soBox, 3, 4);

	colsLay->addWidget(lPlace, 4, 1);
	colsLay->addWidget(colDP, 4, 2);
	colsLay->addWidget(colP, 4, 3);
	colsLay->addWidget(sPlace, 4, 4);

	colsLay->addWidget(lGrid, 5, 1);
	colsLay->addWidget(colDG, 5, 2);
	colsLay->addWidget(colG, 5, 3);
	colsLay->addWidget(sGrid, 5, 4);

	colsLay->addWidget(lFrame, 6, 1);
	colsLay->addWidget(colDF, 6, 2);
	colsLay->addWidget(colF, 6, 3);
	colsLay->addWidget(sFrame, 6, 4);

	midLay = new QVBoxLayout(chooseBox);
	midLay->addWidget(lInf);
	midLay->addLayout(colsLay);

	butsLay = new QHBoxLayout();
	butsLay->addStretch(1);
	butsLay->addWidget(btnOk);
	butsLay->addWidget(btnCancel);
	butsLay->addWidget(btnApply);
	
	mainLay = new QVBoxLayout(this);
	mainLay->addWidget(defCheck);
	mainLay->addWidget(chooseBox);
	mainLay->addLayout(butsLay);
}

void cStsDialog::scolBF()
{
	QColor col = QColorDialog::getColor(boxFrame, this);
	if (!col.isValid())
		return;
	col.setAlpha(sBox->value());
	boxFrame = col;
	QPixmap pix(10, 10);
	pix.fill(col);
	colBF->setIcon(pix);
}

void cStsDialog::scolBB()
{
	QColor col = QColorDialog::getColor(boxFill, this);
	if (!col.isValid())
		return;
	col.setAlpha(sBox->value());
	boxFill = col;
	QPixmap pix(10, 10);
	pix.fill(col);
	colBB->setIcon(pix);
}

void cStsDialog::scolWF()
{
	QColor col = QColorDialog::getColor(wallFrame, this);
	if (!col.isValid())
		return;
	col.setAlpha(sWall->value());
	wallFrame = col;
	QPixmap pix(10, 10);
	pix.fill(col);
	colWF->setIcon(pix);
}

void cStsDialog::scolWB()
{
	QColor col = QColorDialog::getColor(wallFill, this);
	if (!col.isValid())
		return;
	col.setAlpha(sWall->value());
	wallFill = col;
	QPixmap pix(10, 10);
	pix.fill(col);
	colWB->setIcon(pix);
}

void cStsDialog::scolOF()
{
	QColor col = QColorDialog::getColor(onPlaceFrame, this);
	if (!col.isValid())
		return;
	col.setAlpha(soBox->value());
	onPlaceFrame = col;
	QPixmap pix(10, 10);
	pix.fill(col);
	colOF->setIcon(pix);
}

void cStsDialog::scolOB()
{
	QColor col = QColorDialog::getColor(onPlaceFill, this);
	if (!col.isValid())
		return;

	onPlaceFill = col;
	QPixmap pix(10, 10);
	pix.fill(col);
	colOB->setIcon(pix);
}

void cStsDialog::scolP()
{
	QColor col = QColorDialog::getColor(place, this);
	if (!col.isValid())
		return;
	col.setAlpha(sPlace->value());
	place = col;
	QPixmap pix(10, 10);
	pix.fill(col);
	colP->setIcon(pix);
}

void cStsDialog::scolG()
{
	QColor col = QColorDialog::getColor(grid, this);
	if (!col.isValid())
		return;
	col.setAlpha(sGrid->value());
	grid = col;
	QPixmap pix(10, 10);
	pix.fill(col);
	colG->setIcon(pix);
}

void cStsDialog::scolF()
{
	QColor col = QColorDialog::getColor(frame, this);
	if (!col.isValid())
		return;
	col.setAlpha(sFrame->value());
	frame = col;
	QPixmap pix(10, 10);
	pix.fill(col);
	colF->setIcon(pix);
}

void cStsDialog::ok() 
{
	apply();
	close();
}

void cStsDialog::cancel() 
{
	close();
}

void cStsDialog::apply() 
{
	if(defCheck->checkState() == Qt::Checked) {
		parent->setDefaultColors();
		parent->isColorsDefault = true;
	}
	else {
		parent->wallFrameColor		= wallFrame;
		parent->wallColor			= wallFill;
		parent->boxFrameColor		= boxFrame;
		parent->boxColor			= boxFill;
		parent->onPlaceFrameColor	= onPlaceFrame;
		parent->onPlaceColor		= onPlaceFill;
		parent->placeColor			= place; 
		parent->gridColor			= grid;
		parent->frameColor			= frame;
		parent->isColorsDefault = false;
	}
	parent->updateColors();
}

void cStsDialog::zBox(int a)
{
	QPixmap pix(10, 10);

	boxFill.setAlpha(a);
	pix.fill(boxFill);
	colBB->setIcon(pix);

	boxFrame.setAlpha(a);
	pix.fill(boxFrame);
	colBF->setIcon(pix);
}

void cStsDialog::zWall(int a)
{
	QPixmap pix(10, 10);

	wallFill.setAlpha(a);
	pix.fill(wallFill);
	colWB->setIcon(pix);

	wallFrame.setAlpha(a);
	pix.fill(wallFrame);
	colWF->setIcon(pix);
}

void cStsDialog::zoBox(int a)
{
	QPixmap pix(10, 10);

	onPlaceFrame.setAlpha(a);
	pix.fill(onPlaceFrame);
	colOF->setIcon(pix);
}

void cStsDialog::zPlace(int a)
{
	QPixmap pix(10, 10);

	place.setAlpha(a);
	pix.fill(place);
	colP->setIcon(pix);
}

void cStsDialog::zGrid(int a)
{
	QPixmap pix(10, 10);

	grid.setAlpha(a);
	pix.fill(grid);
	colG->setIcon(pix);
}

void cStsDialog::zFrame(int a)
{
	QPixmap pix(10, 10);

	frame.setAlpha(a);
	pix.fill(frame);
	colF->setIcon(pix);
}
