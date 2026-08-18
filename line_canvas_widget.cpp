#include "line_canvas_widget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QResizeEvent>
#include <iostream>
constexpr int _width = 1200;
constexpr int _height = 800;
LineCanvasWidget::LineCanvasWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(_width, _height);
    resetCanvas();
}

void LineCanvasWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // QWidget drawing flow:
    // Qt calls paintEvent() whenever the widget must be repainted. All visible
    // drawing for this widget should happen here through QPainter.
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    // The QImage is our software canvas. Future pixel-level line algorithms can
    // update m_canvas, then call update() to ask Qt for a repaint.
    painter.drawImage(0, 0, m_canvas);
}

void LineCanvasWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    // mousePressEvent() provides the click coordinate in widget-local pixels.
    const QPoint clickedPoint = event->pos();
	std::cout<< "Mouse click at: " << clickedPoint.x() << ", " << clickedPoint.y() << std::endl;
    switch (m_clickState) {
    case ClickState::WaitingForFirstPoint:
        m_pointA = clickedPoint;
        m_hasPointA = true;
        m_hasPointB = false;
        m_clickState = ClickState::WaitingForSecondPoint;
        break;

    case ClickState::WaitingForSecondPoint:
        m_pointB = clickedPoint;
        m_hasPointB = true;
        processLine(m_pointA, m_pointB);
        m_clickState = ClickState::WaitingForClear;
        break;

    case ClickState::WaitingForClear:
        m_hasPointA = false;
        m_hasPointB = false;
        m_pointA = QPoint();
        m_pointB = QPoint();
        m_clickState = ClickState::WaitingForFirstPoint;
        resetCanvas();
        break;
    }

    // update() schedules paintEvent(); it keeps repainting centralized instead
    // of drawing directly from input handlers.
    update();
}

void LineCanvasWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (event->size().width() <= m_canvas.width()
        && event->size().height() <= m_canvas.height()) {
        return;
    }

    QImage newCanvas(event->size(), QImage::Format_ARGB32_Premultiplied);
    newCanvas.fill(Qt::white);

    QPainter painter(&newCanvas);
    painter.drawImage(0, 0, m_canvas);
    m_canvas = newCanvas;
}

void LineCanvasWidget::resetCanvas()
{
    const QSize canvasSize = size().isValid() ? size() : QSize(800, 600);
    m_canvas = QImage(canvasSize, QImage::Format_ARGB32_Premultiplied);
    m_canvas.fill(Qt::white);

    // Simple pixel test example:
    // Uncomment this line to turn one point on the white canvas black.
    // drawExamplePixel();
}

void LineCanvasWidget::processLine(const QPoint &a, const QPoint &b)
{
    Q_UNUSED(a);
    Q_UNUSED(b);
    //计算向量
	QPoint vector_line = b - a;
	double radius = 2.0; // 设置线条的半径
	double aawidth = 1.0; // 设置抗锯齿宽度
    for(int i = 0; i < _width; ++i){
        for (int j = 0; j < _height; ++j) {
            QPoint pixel = QPoint(i, j);
            QPoint direct = pixel - a;
            //获取投影比例
            double portion = (double)(direct.x() * vector_line.x() + direct.y() * vector_line.y()) / (vector_line.x() * vector_line.x() + vector_line.y() * vector_line.y());
            portion = std::max(0.0, std::min(1.0, portion));
            QPoint closest_point = a + vector_line * portion;
            //获取距离
            double distance = std::sqrt(std::pow(pixel.x() - closest_point.x(), 2) + std::pow(pixel.y() - closest_point.y(), 2));
            if (distance < radius - aawidth) {
                setPixelColor(i, j, Qt::black);
            }
            else if (distance >= radius + aawidth) {
                
                
            }
            else {
                double alpha = (radius + aawidth - distance) / (2 * aawidth);
                QColor color = QColor(0, 0, 0, static_cast<int>(alpha * 255));
                setPixelColor(i, j, color);
            }
        }

	}

    //QPainter painter(this);
    //drawPixel(painter,)
    // Keep future line generation in a dedicated function so mouse input,
    // repainting, and the line rasterization algorithm remain independently
    // testable and easy to replace.
    //
    // Future implementation will:
    // - Generate a line segment from the two clicked points.
    // - Use SDF or another method to compute each pixel's coverage.
    // - Draw the anti-aliased result onto m_canvas.
}

void LineCanvasWidget::drawPixel(QPainter &painter, int x, int y)
{
    painter.drawPoint(x, y);
}

void LineCanvasWidget::setPixelColor(int x, int y, const QColor &color)
{
    if (!m_canvas.rect().contains(x, y)) {
        return;
    }

    m_canvas.setPixelColor(x, y, color);
}

void LineCanvasWidget::drawExamplePixel()
{
    setPixelColor(20, 20, Qt::black);
}
