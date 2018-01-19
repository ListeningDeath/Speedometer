/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "view.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QGraphicsTextItem>
#include "callout.h"
#include <QtGui/QMouseEvent>
#include <QDebug>

View::View(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_coordX(0),
      m_coordY(0),
      m_chart(0),
      m_tooltip(0)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // chart
    m_chart = new QChart;
    m_chart->legend()->hide();
    m_chart->setTitle("深度-声速");
    m_chart->setTheme(QChart::ChartThemeDark);
    m_series = new QLineSeries(m_chart);
    m_chart->addSeries(m_series);
    // 坐标轴初始化
    QValueAxis *axisX = new QValueAxis(m_chart);
    QValueAxis *axisY = new QValueAxis(m_chart);
    axisX->setRange(0, 50);
    axisX->setTitleText("温度/℃");
    axisX->setLabelFormat("%.2f");
    axisX->setReverse();
    axisY->setRange(1000, 2000);
    axisY->setTitleText("声速/(m/s)");
    axisY->setLabelFormat("%.2f");

    m_chart->addAxis(axisX, Qt::AlignLeft);
    m_chart->addAxis(axisY, Qt::AlignTop);
    m_series->attachAxis(axisX);
    m_series->attachAxis(axisY);

    m_chart->setAcceptHoverEvents(true);

    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height());
    m_coordX->setText("声速: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height());
    m_coordY->setText("温度: ");

    connect(m_series, &QLineSeries::clicked, this, &View::keepCallout);
    connect(m_series, &QLineSeries::hovered, this, &View::tooltip);

    this->setMouseTracking(true);
}

QLineSeries *View::series()
{
    return m_series;
}

void View::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         m_chart->resize(event->size());
         m_coordX->setPos(20, 20);
         m_coordY->setPos(150, 20);
         const auto callouts = m_callouts;
         for (Callout *callout : callouts)
             callout->updateGeometry();
    }
    QGraphicsView::resizeEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    setCursor(Qt::CrossCursor);
    qreal cur_x = event->pos().x();
    qreal cur_y = event->pos().y();
    m_coordX->setText(QString("声速: %1 m/s").arg(m_chart->mapToValue(event->pos()).x()));
    m_coordY->setText(QString("温度: %1 ℃").arg(m_chart->mapToValue(event->pos()).y()));
    QGraphicsView::mouseMoveEvent(event);
    if(event->buttons() & Qt::RightButton)
    {
        setCursor(Qt::SizeAllCursor);
        qreal scroll_x = m_pre_x - cur_x;
        qreal scroll_y = cur_y - m_pre_y;
        m_chart->scroll(scroll_x, scroll_y);
        const auto callouts = m_callouts;
        for (Callout *callout : callouts)
            callout->updateGeometry();
    }
    m_pre_x = cur_x;
    m_pre_y = cur_y;
}

void View::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
    {
        m_chart->zoomIn();
    }
    else
    {
        m_chart->zoomOut();
    }
    const auto callouts = m_callouts;
    for (Callout *callout : callouts)
        callout->updateGeometry();
}

void View::keepCallout()
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_chart);
    connect(m_tooltip, &Callout::calloutPressed, this, &View::pressCallout);
}

void View::pressCallout(Callout *callout, Qt::MouseButton mButton)
{
    if(mButton == Qt::LeftButton)
    {
        if(!m_callouts.contains(callout))
        {
            keepCallout();
        }
    }
    else if(mButton == Qt::RightButton)
    {
        callout->hide();
        if(m_callouts.contains(callout))
        {
            m_callouts.removeOne(callout);
        }
    }
}

void View::tooltip(QPointF point, bool state)
{
    if (m_tooltip == 0) {
        m_tooltip = new Callout(m_chart);
        connect(m_tooltip, &Callout::calloutPressed, this, &View::pressCallout);
    }
    if (state) {
        m_tooltip->setText(QString("声速: %1 m/s \n温度: %2 ℃").arg(point.x()).arg(point.y()));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}
