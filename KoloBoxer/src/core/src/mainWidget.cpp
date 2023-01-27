#include <QApplication>
#include <QtGui>

#include "arthurstyle.h"
#include "mainWidget.h"
#include "mapView.h"
#include "cStsDialog.h"

const QString pass = "ALyA0408";
const QString defName = QString::fromLocal8Bit("Не установлен");

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
{
	isDev = false;
	wasBestUpdated = false;
	best = new QSettings("levels/records.ini", QSettings::IniFormat);

	setupDefaultColors();

	readSettings();

	setupGroupBoxes();
	setupMapView();

	setupSpinBoxes();
	setupLCDNumbers();
	setupLabels();
	setupButtons();
	
	setupLayouts();
	setupWindow();
	setupStyle();

	readBest();
}

void mainWidget::readSettings()
{
	globalConfig = new QSettings("config.ini", QSettings::IniFormat);

	isColorsDefault = globalConfig->value("Application/DefaultColors", "true").toBool();
	stackSize		= globalConfig->value("Application/StackSize", "10000").toInt();
	styleName		= globalConfig->value("Application/Style", "Arthur").toString();

	startLevel = globalConfig->value("Game/StartLevel", "0").toInt();
	levelCount = globalConfig->value("Game/LevelCount", "27").toInt();

if(isColorsDefault == false) {

	globalConfig->beginGroup("Colors");

	frameColor = QColor( globalConfig->value("FrameColorR", "255").toInt(),
		globalConfig->value("FrameColorG", "0").toInt(),
		globalConfig->value("FrameColorB", "0").toInt(),
		globalConfig->value("FrameColorA", "155").toInt() );

	gridColor = QColor( globalConfig->value("GridColorR", "0").toInt(),
		globalConfig->value("GridColorG", "255").toInt(),
		globalConfig->value("GridColorB", "0").toInt(),
		globalConfig->value("GridColorA", "155").toInt() );

	boxColor = QColor( globalConfig->value("BoxColorR", "200").toInt(),
		globalConfig->value("BoxColorG", "200").toInt(),
		globalConfig->value("BoxColorB", "200").toInt(),
		globalConfig->value("BoxColorA", "155").toInt() );

	boxFrameColor = QColor( globalConfig->value("BoxFrameColorR", "155").toInt(),
		globalConfig->value("BoxFrameColorG", "155").toInt(),
		globalConfig->value("BoxFrameColorB", "155").toInt(),
		globalConfig->value("BoxFrameColorA", "155").toInt() );

	placeColor = QColor( globalConfig->value("PlaceColorR", "0").toInt(),
		globalConfig->value("PlaceColorG", "255").toInt(),
		globalConfig->value("PlaceColorB", "100").toInt(),
		globalConfig->value("PlaceColorA", "155").toInt() );

	onPlaceColor = QColor( globalConfig->value("OnPlaceColorR", "0").toInt(),
		globalConfig->value("OnPlaceColorG", "65").toInt(),
		globalConfig->value("OnPlaceColorB", "220").toInt(),
		globalConfig->value("OnPlaceColorA", "155").toInt() );

	onPlaceFrameColor = QColor( globalConfig->value("OnPlaceFrameColorR", "0").toInt(),
		globalConfig->value("OnPlaceFrameColorG", "65").toInt(),
		globalConfig->value("OnPlaceFrameColorB", "220").toInt(),
		globalConfig->value("OnPlaceFrameColorA", "155").toInt() );

	wallColor = QColor( globalConfig->value("WallColorR", "200").toInt(),
		globalConfig->value("WallColorG", "0").toInt(),
		globalConfig->value("WallColorB", "0").toInt(),
		globalConfig->value("WallColorA", "155").toInt() );

	wallFrameColor = QColor( globalConfig->value("WallFrameColorR", "200").toInt(),
		globalConfig->value("WallFrameColorG", "155").toInt(),
		globalConfig->value("WallFrameColorB", "155").toInt(),
		globalConfig->value("WallFrameColorA", "155").toInt() );

	globalConfig->endGroup();

} else
	setDefaultColors();


	globalConfig->beginGroup("Window");

	windowSize = QSize( globalConfig->value("Width", "826").toInt(),
		globalConfig->value("Height", "640").toInt() );
	
	windowPos = QPoint( globalConfig->value("PosX", "100").toInt(),
		globalConfig->value("PosY", "100").toInt() );

	isFullScr = globalConfig->value("FullScreen", "false").toBool();

	globalConfig->endGroup();


	globalConfig->beginGroup("Pages");

	pagesList.append(QString(":/KAS/resources/pages/kas.html"));
	pagesList.append(globalConfig->value("WinPage").toString());

	globalConfig->endGroup();
}

void mainWidget::writeSettings()
{
	globalConfig->setValue("Application/DefaultColors", isColorsDefault);
	globalConfig->setValue("Application/StackSize", stackSize);
	globalConfig->setValue("Application/Style", styleName);

	globalConfig->setValue("Game/StartLevel", startLevel);
	globalConfig->setValue("Game/LevelCount", levelCount);


	globalConfig->beginGroup("Colors");

	globalConfig->setValue("FrameColorR", frameColor.red());
	globalConfig->setValue("FrameColorG", frameColor.green());
	globalConfig->setValue("FrameColorB", frameColor.blue());
	globalConfig->setValue("FrameColorA", frameColor.alpha());

	globalConfig->setValue("GridColorR", gridColor.red());
	globalConfig->setValue("GridColorG", gridColor.green());
	globalConfig->setValue("GridColorB", gridColor.blue());
	globalConfig->setValue("GridColorA", gridColor.alpha());

	globalConfig->setValue("BoxColorR", boxColor.red());
	globalConfig->setValue("BoxColorG", boxColor.green());
	globalConfig->setValue("BoxColorB", boxColor.blue());
	globalConfig->setValue("BoxColorA", boxColor.alpha());

	globalConfig->setValue("BoxFrameColorR", boxFrameColor.red());
	globalConfig->setValue("BoxFrameColorG", boxFrameColor.green());
	globalConfig->setValue("BoxFrameColorB", boxFrameColor.blue());
	globalConfig->setValue("BoxFrameColorA", boxFrameColor.alpha());

	globalConfig->setValue("PlaceColorR", placeColor.red());
	globalConfig->setValue("PlaceColorG", placeColor.green());
	globalConfig->setValue("PlaceColorB", placeColor.blue());
	globalConfig->setValue("PlaceColorA", placeColor.alpha());

	globalConfig->setValue("OnPlaceColorR", onPlaceColor.red());
	globalConfig->setValue("OnPlaceColorG", onPlaceColor.green());
	globalConfig->setValue("OnPlaceColorB", onPlaceColor.blue());
	globalConfig->setValue("OnPlaceColorA", onPlaceColor.alpha());

	globalConfig->setValue("OnPlaceFrameColorR", onPlaceFrameColor.red());
	globalConfig->setValue("OnPlaceFrameColorG", onPlaceFrameColor.green());
	globalConfig->setValue("OnPlaceFrameColorB", onPlaceFrameColor.blue());
	globalConfig->setValue("OnPlaceFrameColorA", onPlaceFrameColor.alpha());

	globalConfig->setValue("WallColorR", wallColor.red());
	globalConfig->setValue("WallColorG", wallColor.green());
	globalConfig->setValue("WallColorB", wallColor.blue());
	globalConfig->setValue("WallColorA", wallColor.alpha());

	globalConfig->setValue("WallFrameColorR", wallFrameColor.red());
	globalConfig->setValue("WallFrameColorG", wallFrameColor.green());
	globalConfig->setValue("WallFrameColorB", wallFrameColor.blue());
	globalConfig->setValue("WallFrameColorA", wallFrameColor.alpha());

	globalConfig->endGroup();


	globalConfig->beginGroup("Window");

	globalConfig->setValue("Width", width());
	globalConfig->setValue("Height", height());
	globalConfig->setValue("PosX", x());
	globalConfig->setValue("PosY", y());
	globalConfig->setValue("FullScreen", isFullScreen());

	globalConfig->endGroup();
}

void mainWidget::setupDefaultColors()
{
	defWallColor			= QColor(0, 0, 255, 100);
	defWallFrameColor		= QColor(155, 155, 155, 100);
	defBoxColor				= QColor(200, 200, 200, 155);
	defBoxFrameColor		= QColor(155, 155, 155, 155);
	defPlaceColor			= QColor(0, 255, 100, 155);
	defOnPlaceColor			= QColor(255, 255, 0, 255);
	defOnPlaceFrameColor	= QColor(144, 144, 255, 155);
	defGridColor			= QColor(85, 170, 255, 155);
	defFrameColor			= QColor(0, 0, 255, 155);
}

void mainWidget::setupWindow()
{
    setWindowTitle(tr("KoloBoxer"));
	
	setLayout(layoutMain);

	setMinimumHeight(minHeight);
	setMinimumWidth(minWidth);

	if(isFullScr == true)
		resize(minWidth, minHeight);
	else {
		resize(windowSize);
		move(windowPos);
	}
}

void mainWidget::setupMapView()
{	
	map = new mapView(this, stackSize);
	map->setGridColor(gridColor);
	map->setFrameColor(frameColor);
	map->setWallColor(wallColor);
	map->setWallFrameColor(wallFrameColor);
	map->setBoxColor(boxColor);
	map->setBoxFrameColor(boxFrameColor);
	map->setPlaceColor(placeColor);
	map->setOnPlaceColor(onPlaceColor);
	map->setOnPlaceFrameColor(onPlaceFrameColor);
	
	map->setDescList(pagesList);

	isKASShown=false;
	
	// Загружаем только сообщение о выйгрыше, т.к. только
	// он обрабатывается внутри мэпа, остальные - отсюда
	map->loadDescription(pagesList.at(1));


	map->setLevelCount(levelCount);
	map->openLevel(startLevel);
}

void mainWidget::readBest()
{
	best->beginGroup(QString("map%1").arg(map->statLevel()));

	bestM = best->value("Moves", "0").toInt(); 
	bestW = best->value("MovesWith", "0").toInt();
	bestName = best->value("Name", defName).toString();
	bestChecksumm = best->value("Checksumm", 
		qChecksum(defName.toLocal8Bit().constData(), 
			defName.length() ) ).toInt() - bestM%(bestW+1) - map->statLevel();

	best->endGroup();

	if( bestChecksumm != qChecksum(bestName.toLocal8Bit().constData(), bestName.length()) ) {
		bestM = bestW = 0;
		bestName = defName;
		writeBest();
	}

	numberBestMoves->display(bestMoves());
	numberBestWith->display(bestMovesWith());
	labelTheBest->setText("<b>"+bestName+"</b>");
}

void mainWidget::writeBest()
{
	best->beginGroup(QString("map%1").arg(map->statLevel()));

	best->setValue("Moves", bestM); 
	best->setValue("MovesWith", bestW);
	best->setValue("Name", bestName);
	best->setValue("Checksumm", qChecksum(bestName.toLocal8Bit().constData(), 
		bestName.length() ) + bestM%(bestW+1) + map->statLevel() );

	best->endGroup();
}

void mainWidget::setupGroupBoxes()
{
	groupMain = new QGroupBox(this);
    groupMain->setFixedWidth(180);
    groupMain->setTitle("KoloBoxer");

	groupStats = new QGroupBox(groupMain);
	groupStats->setAttribute(Qt::WA_ContentsPropagated);
	groupStats->setTitle(QString::fromLocal8Bit("Статистика"));

	groupMenu = new QGroupBox(groupMain);
	groupMenu->setAttribute(Qt::WA_ContentsPropagated);
	groupMenu->setTitle(QString::fromLocal8Bit("Игровое меню"));

	groupHelp = new QGroupBox(groupMain);
	groupHelp->setAttribute(Qt::WA_ContentsPropagated);
	groupHelp->setTitle(QString::fromLocal8Bit("Горячие клавиши"));

	groupAbout = new QGroupBox(groupMain);
	groupAbout->setAttribute(Qt::WA_ContentsPropagated);
	groupAbout->setTitle(QString::fromLocal8Bit("О программе"));

	groupTheBest = new QGroupBox(groupMain);
	groupTheBest->setAttribute(Qt::WA_ContentsPropagated);
	groupTheBest->setTitle(QString::fromLocal8Bit("Лучший"));
}

void mainWidget::setupSpinBoxes()
{
	spinBoxLevel = new QSpinBox;
    spinBoxLevel->setRange(0, levelCount);
    spinBoxLevel->setSingleStep(1);
	spinBoxLevel->setWrapping(true);
    spinBoxLevel->setValue(0);
	spinBoxLevel->setFocusPolicy(Qt::NoFocus);
	spinBoxLevel->setFocusProxy(this);
}

void mainWidget::setupLCDNumbers()
{
	numberStatLevel	= new QLCDNumber();
	numberStatLevel->setSegmentStyle(QLCDNumber::Filled);
	numberStatLevel->display(map->statLevel());

	numberStatMoves	= new QLCDNumber();
	numberStatMoves->setSegmentStyle(QLCDNumber::Filled);
	numberStatMoves->display(map->statMoves());

	numberStatMovesWith	= new QLCDNumber();
	numberStatMovesWith->setSegmentStyle(QLCDNumber::Filled);
	numberStatMovesWith->display(map->statMovesWith());

	numberBestMoves	= new QLCDNumber();
	numberBestMoves->setSegmentStyle(QLCDNumber::Filled);
	
	numberBestWith	= new QLCDNumber();
	numberBestWith->setSegmentStyle(QLCDNumber::Filled);
}

void mainWidget::setupLabels()
{
	labelStatLevel = new QLabel(QString::fromLocal8Bit("<b>Уровень:</b>"));
	labelStatLevel->setBuddy(numberStatLevel);

	labelStatMoves = new QLabel(QString::fromLocal8Bit("<b>Шагов:</b>"));
	labelStatMoves->setBuddy(numberStatMoves);

	labelStatMovesWith = new QLabel(QString::fromLocal8Bit("<b>С грузом:</b>"));
	labelStatMovesWith->setBuddy(numberStatMovesWith);

	labelTheBest = new QLabel();

	labelHelp = new QLabel( QString::fromLocal8Bit(
		//"Press <b>arrows</b> to move <br>"
		//"<b>Ctrl+F</b> - switch between Fullscreen/Normal mode<br>"
		"<b>Ctrl+N</b> - следующий<br>"
		"<b>Ctrl+P</b> - предыдущий<br>"
		"<b>Ctrl+R</b> - перезапуск<br>"
		//"<b>Ctrl+S</b> - save the game<br>"
		"<b>Ctrl+X</b> - покинуть игру<br>" 
		//"<b>Shift+Y</b> - redo move<br>"
		"<b>Shift+Z</b> - отменить ход<br>" ));
		//"<b>Shift+R</b> - show records" 
	labelHelp->setWordWrap(true);

	labelAbout = new QLabel(QString::fromLocal8Bit(
		"<b>NightWind KoloBoxer</b>"
		"<br><b>v.1.0.0.2 2006-Jul-07</b>" ));
		//"<br>web:www.nightwind.ru"
		//"<br>support@nightwind.ru"
		//"<br>"
		//"<br>Alyamkin \"AVA\" Vladimir"
		//"<br>alyamkin@nightwind.ru" ));
}

void mainWidget::setupButtons()
{
	buttonLoadLevel = new QPushButton();
    buttonLoadLevel->setText(QString::fromLocal8Bit("Загрузить"));
	buttonLoadLevel->setFocusPolicy(Qt::NoFocus);

	buttonOpenLevel = new QPushButton();
    buttonOpenLevel->setText(QString::fromLocal8Bit("Открыть уровень"));
	buttonOpenLevel->setFocusPolicy(Qt::NoFocus);

	buttonSaveLevel = new QPushButton();
	buttonSaveLevel->setText(QString::fromLocal8Bit("Сохранить игру"));
	buttonSaveLevel->setFocusPolicy(Qt::NoFocus);

	/*
	buttonShowRecords = new QPushButton();
    buttonShowRecords->setText(QString::fromLocal8Bit("Показать рекорды"));
	buttonShowRecords->setCheckable(true);
	buttonShowRecords->setFocusPolicy(Qt::NoFocus);
	*/

	buttonSettings = new QPushButton();
	buttonSettings->setText(QString::fromLocal8Bit("Настройки"));
	buttonSettings->setFocusPolicy(Qt::NoFocus);

	buttonExit = new QPushButton();
    buttonExit->setText(QString::fromLocal8Bit("Выход"));
	buttonExit->setFocusPolicy(Qt::NoFocus);

	connect(buttonOpenLevel, SIGNAL(clicked()), this, SLOT(openLevel()));
	connect(buttonLoadLevel, SIGNAL(clicked()), this, SLOT(loadLevel()));
	connect(buttonSaveLevel, SIGNAL(clicked()), this, SLOT(saveLevel()));
	connect(buttonSettings, SIGNAL(clicked()), this, SLOT(showSettings()));
	
	//connect(buttonShowRecords, SIGNAL(clicked(bool)), map, SLOT(setDescriptionEnabled(bool)));
	//connect(map, SIGNAL(descriptionEnabledChanged(bool)), buttonShowRecords, SLOT(setChecked(bool)));
	
	connect(buttonExit, SIGNAL(clicked()), this, SLOT(close()));
}

void mainWidget::setupLayouts()
{
	layoutGroupStats = new QGridLayout(groupStats);
	layoutGroupStats->addWidget(labelStatLevel, 1, 1);
	layoutGroupStats->addWidget(numberStatLevel, 1, 2);
	layoutGroupStats->addWidget(labelStatMoves, 2, 1);
	layoutGroupStats->addWidget(numberStatMoves, 2, 2);
	layoutGroupStats->addWidget(labelStatMovesWith, 3, 1);
	layoutGroupStats->addWidget(numberStatMovesWith, 3, 2);

	layoutGroupMenuLoad = new QHBoxLayout();
	layoutGroupMenuLoad->addWidget(spinBoxLevel);
	layoutGroupMenuLoad->addWidget(buttonLoadLevel);

	layoutGroupMenu = new QVBoxLayout(groupMenu);
	layoutGroupMenu->addLayout(layoutGroupMenuLoad);
	layoutGroupMenu->addWidget(buttonOpenLevel);
	layoutGroupMenu->addWidget(buttonSaveLevel);
	layoutGroupMenu->addWidget(buttonSettings);
	layoutGroupMenu->addWidget(buttonExit);

	layoutGroupTheBest = new QGridLayout(groupTheBest);
	layoutGroupTheBest->addWidget(labelTheBest, 1, 1, 1, 2, Qt::AlignCenter);
	layoutGroupTheBest->addWidget(numberBestMoves, 2, 1);
	layoutGroupTheBest->addWidget(numberBestWith, 2, 2);

	layoutGroupHelp = new QVBoxLayout(groupHelp);
	layoutGroupHelp->addWidget(labelHelp);

	layoutGroupAbout = new QVBoxLayout(groupAbout);
	layoutGroupAbout->addWidget(labelAbout);

	layoutGroupMain = new QVBoxLayout(groupMain);
	layoutGroupMain->addWidget(groupStats);
	layoutGroupMain->addWidget(groupMenu);
	layoutGroupMain->addStretch(1);
	layoutGroupMain->addWidget(groupTheBest);
	layoutGroupMain->addWidget(groupHelp);
    layoutGroupMain->addWidget(groupAbout);

	layoutMain = new QHBoxLayout(this);
	layoutMain->addWidget(map);
	layoutMain->addWidget(groupMain);
}

void mainWidget::setupStyle()
{
	QStyle *curStyle = 0;

	if(styleName == "Arthur") {
		curStyle = new ArthurStyle();
	}
	else if(styleName == "Plastique") {
		curStyle = new QPlastiqueStyle();
	}
	else if(styleName == "WinXP") {
		//curStyle = new QWindowsXPStyle();
	}
	else if(styleName == "Default") {
		curStyle = new QWindowsStyle();
	}
	else
		curStyle = new QWindowsStyle();

	setStyle(curStyle);

	QList<QWidget *> widgets = qFindChildren<QWidget *>(this);
	foreach (QWidget *w, widgets)
		w->setStyle(curStyle);
}

void mainWidget::openLevel()
{
	QString dir = QDir::currentPath();
	QString fileName = QFileDialog::getOpenFileName(this,
						QString::fromLocal8Bit("Открыть уровень..."),
						dir, QString::fromLocal8Bit(
						"Текстовые файлы (*.txt);;"
						"Все файлы (*)"));

	if (!fileName.isEmpty())
	{
		if(!map->openLevel(fileName)) {
			openFailed();
		}
		else {
			readBest();
			numberStatLevel->display(map->statLevel());
			numberStatMoves->display(map->statMoves());
			numberStatMovesWith->display(map->statMovesWith());
			wasBestUpdated = false;
		}
	}
}

void mainWidget::loadLevel()
{
	if(!map->openLevel(spinBoxLevel->value())) {
		openFailed();
	} 
	else {
		readBest();
		spinBoxLevel->setValue(map->statLevel());
		numberStatLevel->display(map->statLevel());
		numberStatMoves->display(map->statMoves());
		numberStatMovesWith->display(map->statMovesWith());
		wasBestUpdated = false;
	}
}

void mainWidget::saveLevel()
{
	QString dir = QDir::currentPath();
	QString fileName = QFileDialog::getSaveFileName(this,
						QString::fromLocal8Bit("Сохранить игру..."),
						dir, QString::fromLocal8Bit(
						"Текстовый файл (*.txt);;"
						"Все файлы (*)" ));

	if (!fileName.isEmpty())
		map->saveLevel(fileName+".txt");
}

void mainWidget::showKAS()
{
	isKASShown=!isKASShown;
	if(isKASShown == true)
		map->showDescription(0);
	else
		map->hideDescription();
}

void mainWidget::showSettings()
{
	cStsDialog *settingsD = new cStsDialog(this, this, isColorsDefault);
	settingsD->exec();
	delete settingsD;
}

void mainWidget::setDefaultColors()
{
	wallColor			= defWallColor;
	wallFrameColor		= defWallFrameColor;
	boxColor			= defBoxColor;
	boxFrameColor		= defBoxFrameColor;
	placeColor			= defPlaceColor;
	onPlaceColor		= defOnPlaceColor;
	onPlaceFrameColor	= defOnPlaceFrameColor;
	gridColor			= defGridColor;
	frameColor			= defFrameColor;

	isColorsDefault = true;
}

void mainWidget::updateColors()
{
	map->setGridColor(gridColor);
	map->setFrameColor(frameColor);
	map->setWallColor(wallColor);
	map->setWallFrameColor(wallFrameColor);
	map->setBoxColor(boxColor);
	map->setBoxFrameColor(boxFrameColor);
	map->setPlaceColor(placeColor);
	map->setOnPlaceColor(onPlaceColor);
	map->setOnPlaceFrameColor(onPlaceFrameColor);

	map->redrawLevel();
}

void mainWidget::openFailed()
{
	QPixmap ic(":/icons/resources/img/caution_win.png");
	QMessageBox *openFailed = new QMessageBox;
    openFailed->setText(QString::fromLocal8Bit(
		"При загрузке уровня возникла неопознанная ошибка."
		"<br>"
		"<br>"
		"Удостоверьтесь в правильности введенных"
		" данных и свяжитесь с разработчиками."));
    openFailed->setIconPixmap(ic);
	openFailed->setButtonText(1, "Ok");
	openFailed->setWindowTitle(QString::fromLocal8Bit("Загрузка не удалась"));
	openFailed->exec();
	delete openFailed;
}

void mainWidget::enterPass()
{
	bool ok;
	QString text = QInputDialog::getText(this, QString::fromLocal8Bit("Введите пароль"),
		QString::fromLocal8Bit("Введите пароль администратора:"), QLineEdit::Password, "", &ok);
	
	if(ok && (text == pass))
		isDev = true;
	else
		isDev = false;
}

void mainWidget::updateBest()
{
	if( (bestM >= numberStatMoves->value()) || 
		( (bestName == defName) && (bestM == bestW) && (bestM == 0) ) ) 
	{
		bestM = numberStatMoves->value();
		bestW = numberStatMovesWith->value();
		bestName.clear();

		bestName = QInputDialog::getText(this, QString::fromLocal8Bit("Ваше имя?"),
			QString::fromLocal8Bit(
			"Мои поздравления! Вы справились"
			"<br>"
			"лучше всех и будете занесены "
			"<br>"
			"в таблицу рекордов." 
			"<br>"
			"<br>"
			"Как Вас представить, герой?"), 
			QLineEdit::Normal);
			
		if(bestName.isEmpty() == true) 
			bestName = QString::fromLocal8Bit("Аноним");

		labelTheBest->setText("<b>"+bestName+"</b>");

		numberBestMoves->display(bestMoves());
		numberBestWith->display(bestMovesWith());

		writeBest();
	}
}

void mainWidget::keyPressEvent(QKeyEvent *e)
{
	bool modified = false;
	if(e->modifiers() == Qt::ControlModifier) 
	{
		if(e->key() == Qt::Key_X) {
			this->close();
		} 
		else if(e->key() == Qt::Key_S) {
			saveLevel(); 
		} 
		else if(e->key() == Qt::Key_F) {
			if(isFullScreen() == false) {
				showFullScreen(); }
			else
				showNormal(); 
		}
		else if(e->key() == Qt::Key_R) {
			if(map->curLevelName() != "")
				map->openLevel(map->curLevelName()); 
			else
				map->openLevel(map->statLevel());
			wasBestUpdated = false;
			modified = true;
		}
		else if(e->key() == Qt::Key_N) {
			map->nextLevel();
			readBest();
			wasBestUpdated = false;
			modified = true;
		}
		else if(e->key() == Qt::Key_P) {
			map->previousLevel();
			readBest();
			wasBestUpdated = false;
			modified = true;
		}
	}
	else if(e->modifiers() == Qt::ShiftModifier)
	{
		if(e->key() == Qt::Key_Z) {
			map->undoMove(); 
			modified = true;
		} /*else 
		if(e->key() == Qt::Key_Y) {
			map->redoMove(); 
		}*/
	}
	else if(e->modifiers() == Qt::AltModifier)
	{
		if(e->key() == Qt::Key_D) {
			setDefaultColors();
			updateColors();
		}
		else if(e->key() == Qt::Key_P) {
			enterPass();
		}
	}

	if(isDev == true) {
		if(e->modifiers() == Qt::AltModifier) {
			if(e->key() == Qt::Key_W) {
				map->completeLevel(); 
				updateBest();
				wasBestUpdated = true;
			} 
			else if(e->key() == Qt::Key_A) {
				showKAS();
			}
		}
	}

	if(modified == true) {
		spinBoxLevel->setValue(map->statLevel());
		numberStatLevel->display(map->statLevel());
		numberStatMoves->display(map->statMoves());
		numberStatMovesWith->display(map->statMovesWith());
	}
}

void mainWidget::keyReleaseEvent(QKeyEvent *e)
{
	if(e->key() == Qt::Key_Left) {
		map->moveLeft(); }
	else if(e->key() == Qt::Key_Up) {
		map->moveUp(); }
	else if(e->key() == Qt::Key_Right) {
		map->moveRight(); }
	else if(e->key() == Qt::Key_Down) {
		map->moveDown(); }

	numberStatMoves->display(map->statMoves());
	numberStatMovesWith->display(map->statMovesWith()); 

	if( (map->isWin == true) && (wasBestUpdated == false) ) {
		updateBest();
		wasBestUpdated = true;
	}
}

void mainWidget::closeEvent(QCloseEvent *event)
{
	writeSettings();
	event->accept();
}
