/****************************************************************************
**
** Copyright (C) 2005-2005 Trolltech AS. All rights reserved.
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef ARTHURSTYLE_H
#define ARTHURSTYLE_H

#include <QWindowsStyle>

class ArthurStyle : public QWindowsStyle
{
public:
    ArthurStyle();

    void drawHoverRect(QPainter *painter, const QRect &rect) const;

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = 0) const;
//     void drawControl(ControlElement element, const QStyleOption *option,
//                      QPainter *painter, const QWidget *widget) const;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const;
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const;

    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget) const;

//     SubControl hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option,
// 				     const QPoint &pos, const QWidget *widget = 0) const;

    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;

    void polish(QPalette &palette);
    void polish(QWidget *widget);
    void unpolish(QWidget *widget);
};

#endif
