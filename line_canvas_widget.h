#pragma once

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class QMouseEvent;
class QPaintEvent;
class QPainter;

class LineCanvasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LineCanvasWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    enum class ClickState
    {
        WaitingForFirstPoint,
        WaitingForSecondPoint,
        WaitingForClear
    };

    void resetCanvas();
    void processLine(const QPoint &a, const QPoint &b);

    // Pixel-level drawing test interface reserved for future line experiments.
    void drawPixel(QPainter &painter, int x, int y);
    void setPixelColor(int x, int y, const QColor &color);
    void drawExamplePixel();

    QImage m_canvas;
    QPoint m_pointA;
    QPoint m_pointB;
    bool m_hasPointA = false;
    bool m_hasPointB = false;
    ClickState m_clickState = ClickState::WaitingForFirstPoint;
};
