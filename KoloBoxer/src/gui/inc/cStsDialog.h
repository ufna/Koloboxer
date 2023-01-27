#ifndef CSTSDIALOG_H
#define CSTSDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>

#include "mainWidget.h"

class QGroupBox;
class QColor;
class QPushButton;


class cStsDialog : public QDialog
{
    Q_OBJECT

public:
	cStsDialog(QWidget *p, mainWidget *parent, bool isDefault);

private slots:
	void ok();
	void cancel();
	void apply();

	void scolBF();
	void scolBB();
	void scolWF(); 
	void scolWB();
	void scolOF(); 
	void scolOB();
	void scolP(); 
	void scolG(); 
	void scolF();

	void zBox(int);
	void zWall(int);
	void zoBox(int); 
	void zPlace(int);
	void zGrid(int); 
	void zFrame(int);

private:
	bool isDefault;
	
	void setupWindow();
	void setupColors();
	void setupDefButton();
	void setupButtons();
	void setupColorButtons();
	void setupSpins();
	void setupLabels();
	void setupColorsGroup();
	void setupLayout();

	mainWidget *parent;

	QVBoxLayout	*mainLay, *midLay;
	QHBoxLayout	*butsLay;
	QGridLayout *colsLay;

	QGroupBox *chooseBox;

	QCheckBox *defCheck;

	QPushButton *btnOk,
		*btnCancel, *btnApply;

	QLabel *lBox, *lWall,
		*loBox, *lPlace,
		*lGrid, *lFrame,
		*lInf, *lFill,
		*lFr, *lAlpha;

	QSpinBox *sBox, *sWall,
		*soBox, *sPlace,
		*sGrid, *sFrame;

	QPushButton *colBF, *colBB,
		*colWF, *colWB,
		*colOF, *colOB,
		*colP, *colG, *colF,
		*colDP, *colDG, *colDF;

	QColor wallFrame,
		wallFill,
		boxFrame,
		boxFill,
		onPlaceFrame,
		onPlaceFill,
		place,
		grid,			
		frame;

};

#endif // CSTSDIALOG_H