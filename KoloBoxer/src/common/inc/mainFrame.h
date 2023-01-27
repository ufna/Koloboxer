#ifndef MainFrame_H
#define MainFrame_H

#include <QBitmap>
#include <QPushButton>
#include <QGroupBox>

class QTextDocument;
class QTextEdit;
class QVBoxLayout;

class MainFrame : public QWidget
{
    Q_OBJECT
public:
    MainFrame(QWidget *parent);
    virtual void paint(QPainter *) {}


    void paintDescription(QPainter *p);

    void loadDescription(const QString &filename);
    void setDescription(const QString &htmlDesc);

    bool preferImage() const { return m_prefer_image; }

public slots:
    void setPreferImage(bool pi) { m_prefer_image = pi; }
    void setDescriptionEnabled(bool enabled);

signals:
    void descriptionEnabledChanged(bool);

protected:
    void paintEvent(QPaintEvent *);

    QPixmap m_tile;

    bool m_show_doc;
    bool m_prefer_image;
    QTextDocument *m_document;

    QString m_sourceFileName;

};

#endif
