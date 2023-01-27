#include "MainFrame.h"

#include <QPainter>
#include <QPainterPath>
#include <QPixmapCache>
#include <QtEvents>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QFile>
#include <QTextBrowser>
#include <QBoxLayout>
#include <QTextCodec>

extern QPixmap cached(const QString &img);

MainFrame::MainFrame(QWidget *parent)
    : QWidget(parent), m_prefer_image(false)
{
    m_document = 0;
    m_show_doc = false;

    m_tile = QPixmap(100, 100);
    m_tile.fill(Qt::white);
    //QPainter pt(&m_tile);
    //QColor color(240, 240, 240);
    //pt.fillRect(0, 0, 50, 50, color);
    //pt.fillRect(50, 50, 50, 50, color);
    //pt.end();

//     QPalette pal = palette();
//     pal.setBrush(backgroundRole(), m_tile);
//     setPalette(pal);

#ifdef Q_WS_X11
    QPixmap xRenderPixmap(1, 1);
    m_prefer_image = !xRenderPixmap.x11PictureHandle();
#endif
}


void MainFrame::paintEvent(QPaintEvent *e)
{
    static QImage *static_image = 0;
    QPainter painter;
    if (preferImage()) {
        if (!static_image) {
            static_image = new QImage(size(), QImage::Format_RGB32);
        } else if (static_image->size() != size()) {
            delete static_image;
            static_image = new QImage(size(), QImage::Format_RGB32);
        }
        painter.begin(static_image);

        int o = 10;

        QBrush bg = palette().brush(QPalette::Background);
        painter.fillRect(0, 0, o, o, bg);
        painter.fillRect(width() - o, 0, o, o, bg);
        painter.fillRect(0, height() - o, o, o, bg);
        painter.fillRect(width() - o, height() - o, o, o, bg);
    } else {
        painter.begin(this);
    }

    painter.setClipRect(e->rect());

    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath clipPath;

    QRect r = rect();
    double left = r.x() + 1;
    double top = r.y() + 1;
    double right = r.right();
    double bottom = r.bottom();
    double radius2 = 8 * 2;

    clipPath.moveTo(right - radius2, top);
    clipPath.arcTo(right - radius2, top, radius2, radius2, 90, -90);
    clipPath.arcTo(right - radius2, bottom - radius2, radius2, radius2, 0, -90);
    clipPath.arcTo(left, bottom - radius2, radius2, radius2, 270, -90);
    clipPath.arcTo(left, top, radius2, radius2, 180, -90);
    clipPath.closeSubpath();

    painter.save();
    painter.setClipPath(clipPath, Qt::IntersectClip);

    painter.drawTiledPixmap(rect(), m_tile);

    // client painting

    paint(&painter);

    painter.restore();

    painter.save();
    if (m_show_doc)
        paintDescription(&painter);
    painter.restore();

    int level = 180;
    painter.setPen(QPen(QColor(level, level, level), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(clipPath);

    if (preferImage()) {
        painter.end();
        painter.begin(this);
        painter.drawImage(e->rect(), *static_image, e->rect());
    }
}

void MainFrame::setDescriptionEnabled(bool enabled)
{
    if (m_show_doc != enabled) {
        m_show_doc = enabled;
        emit descriptionEnabledChanged(m_show_doc);
        update();
    }
}

void MainFrame::loadDescription(const QString &fileName)
{
    QFile textFile(fileName);
	QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
	QString text;

    if (!textFile.open(QFile::ReadOnly))
		text = QString("Unable to load resource file: '%1'").arg(fileName);
	else
        text = codec->toUnicode(textFile.readAll());

	setDescription(text);
}

void MainFrame::setDescription(const QString &text)
{
    m_document = new QTextDocument(this);
	m_document->setHtml(text);
}

void MainFrame::paintDescription(QPainter *painter)
{
    if (!m_document)
        return;

    int pageWidth = qMax(width() - 100, 100);
    int pageHeight = qMax(height() - 100, 100);
    if (pageWidth != m_document->pageSize().width()) {
        m_document->setPageSize(QSize(pageWidth, pageHeight));
    }

    QRect textRect(width() / 2 - pageWidth / 2,
                   height() / 2 - pageHeight / 2,
                   pageWidth,
                   pageHeight);
    int pad = 10;
    QRect clearRect = textRect.adjusted(-pad, -pad, pad, pad);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 63));
    int shade = 10;
    painter->drawRect(clearRect.x() + clearRect.width() + 1,
                      clearRect.y() + shade,
                      shade,
                      clearRect.height() + 1);
    painter->drawRect(clearRect.x() + shade,
                      clearRect.y() + clearRect.height() + 1,
                      clearRect.width() - shade + 1,
                      shade);

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setBrush(QColor(255, 255, 255, 220));
    painter->setPen(Qt::black);
    painter->drawRect(clearRect);

    painter->setClipRegion(textRect, Qt::IntersectClip);
    painter->translate(textRect.topLeft());

    QAbstractTextDocumentLayout::PaintContext ctx;

    QLinearGradient g(0, 0, 0, textRect.height());
    g.setColorAt(0, Qt::black);
    g.setColorAt(0.9, Qt::black);
    g.setColorAt(1, Qt::transparent);

    QPalette pal = palette();
    pal.setBrush(QPalette::Text, g);

    ctx.palette = pal;
    ctx.clip = QRect(0, 0, textRect.width(), textRect.height());
    m_document->documentLayout()->draw(painter, ctx);
}
