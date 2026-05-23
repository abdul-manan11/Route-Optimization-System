#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QFileDialog>
#include <QTextStream>
#include <QElapsedTimer>
#include <QPainter>
#include <QPainterPath>
#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QRegularExpression>
#include <QMessageBox>
#include <QProgressBar>
#include <QTimer>
#include <cmath>
#include <vector>
#include <algorithm>
#include <QRandomGenerator>
#include <random>
#include <queue>
#include <QTextEdit>
#include <QScrollBar>

#include "City.h"
#include "Graph.h"
#include "Algorithms.h"
// ======================================================================

namespace UIConfig {
// ... (unchanged, same as original) ...
const QString DARK_BG = "background-color: #0b0e14;";
const QString SIDEBAR_BG = "background-color: #161b22; border-right: 1px solid #30363d;";
const QString SIDEBAR_BTN =
    "QPushButton { background-color: #21262d; border: 1px solid #30363d; border-radius: 6px;"
    "color: #c9d1d9; padding: 12px; font-weight: 600; font-size: 12px; text-align: left; }"
    "QPushButton:hover { background-color: #30363d; border-color: #8b949e; color: #00f2ff; }";
const QString TOGGLE_BTN =
    "QPushButton { background-color: transparent; border: none; color: #00f2ff; font-size: 24px; font-weight: bold; } "
    "QPushButton:hover { color: #ffffff; }";
const QString STATS_HEADER =
    "background-color: #161b22; border-top: 1px solid #30363d; color: #8b949e; font-weight: bold; font-size: 11px;";

const QString MENU_STYLE =
    "QMenu { background-color: #161b22; border: 1px solid #30363d; color: #c9d1d9; padding: 5px; }"
    "QMenu::item { padding: 8px 25px 8px 20px; border-radius: 4px; background: transparent; }"
    "QMenu::item:selected { background-color: #21262d; color: #00f2ff; }";

const QString DIALOG_STYLE =
    "QDialog { background-color: #0d1117; border: 1px solid #30363d; }"
    "QLabel { color: #c9d1d9; }"
    "QLineEdit { background-color: #161b22; border: 1px solid #30363d; color: #c9d1d9; padding: 5px; border-radius: 4px; }"
    "QPushButton { background-color: #238636; color: white; border-radius: 4px; padding: 8px; font-weight: bold; }";

const QString BACK_BTN_STYLE =
    "QPushButton { background-color: rgba(33, 38, 45, 0.9); border: 1px solid #f85149; color: #f85149; "
    "border-radius: 6px; padding: 8px 16px; font-weight: bold; font-size: 12px; }"
    "QPushButton:hover { background-color: #f85149; color: white; }";

const QString UNDO_BTN_STYLE =
    "QPushButton { background-color: #21262d; border: 1px solid #00f2ff; color: #00f2ff; "
    "border-radius: 6px; padding: 12px; font-weight: bold; font-size: 12px; text-align: left; }"
    "QPushButton:hover { background-color: #00f2ff; color: #0b0e14; }";

const QString RUN_OPTIMIZATION_STYLE =
    "QPushButton { background-color: #238636; border: 1px solid #30363d; color: white; "
    "border-radius: 6px; padding: 12px; font-weight: bold; font-size: 12px; text-align: left; }"
    "QPushButton:hover { background-color: #2ea043; border-color: #8b949e; }";

const QString TABLE_STYLE =
    "QTableWidget { background-color: #161b22; border: 1px solid #30363d; color: #c9d1d9;"
    "gridline-color: #30363d; font-size: 12px; }"
    "QTableWidget::item { padding: 5px; }"
    "QHeaderView::section { background-color: #21262d; color: #8b949e; font-weight: bold;"
    "border: 1px solid #30363d; padding: 4px; }";

const QString RED_CLOSE_BTN =
    "QPushButton { background-color: #f85149; color: white; border: none; border-radius: 6px;"
    "padding: 10px; font-weight: bold; font-size: 12px; }"
    "QPushButton:hover { background-color: #da3633; }";

const QString LOADING_BAR_STYLE =
    "QProgressBar { border: 1px solid #30363d; background-color: #161b22; border-radius: 6px; text-align: center; color: #c9d1d9; font-weight: bold; }"
    "QProgressBar::chunk { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #00f2ff, stop:1 #ff007f); border-radius: 5px; }";
}

void drawTechnicalBackground(QPainter* painter, const QRectF& rect, const QPointF& sceneCenter, double viewportWidth, double viewportHeight, double opacityFactor = 1.0) {
    // ... (exactly same as original) ...
    painter->fillRect(rect, QColor("#0d1117"));

    double cx = sceneCenter.x();
    double cy = sceneCenter.y();
    double w = rect.width();
    double h = rect.height();

    QColor gridColor("#13171e");
    gridColor.setAlphaF(gridColor.alphaF() * opacityFactor);
    painter->setPen(QPen(gridColor, 1.2, Qt::SolidLine));
    double stepY = h / 14.0;
    double stepX = w / 14.0;
    for (int i = -15; i <= 15; ++i) {
        painter->drawLine(rect.left(), cy + (i * stepY), rect.right(), cy + (i * stepY));
        painter->drawLine(cx + (i * stepX), rect.top(), cx + (i * stepX), rect.bottom());
    }

    double baseRadius = qMin(viewportWidth, viewportHeight) * 0.32;
    if (baseRadius < 100) baseRadius = 200;

    QColor orbitColor("#161b22");
    orbitColor.setAlphaF(orbitColor.alphaF() * opacityFactor);
    QPen orbitPen(orbitColor, 1.5, Qt::DashLine);
    painter->setPen(orbitPen);
    painter->setBrush(Qt::NoBrush);

    painter->drawEllipse(QPointF(cx, cy), baseRadius * 0.6, baseRadius * 0.6);
    painter->drawEllipse(QPointF(cx, cy), baseRadius, baseRadius);
    painter->drawEllipse(QPointF(cx, cy), baseRadius * 1.4, baseRadius * 1.4);

    QPainterPath routeMesh;
    routeMesh.moveTo(cx - baseRadius, cy + baseRadius * 0.3);
    routeMesh.cubicTo(cx - baseRadius * 0.4, cy - baseRadius * 0.8, cx + baseRadius * 0.4, cy - baseRadius * 0.7, cx + baseRadius, cy - baseRadius * 0.2);
    routeMesh.moveTo(cx - baseRadius * 1.2, cy - baseRadius * 0.2);
    routeMesh.quadTo(cx, cy + baseRadius * 0.9, cx + baseRadius * 1.2, cy + baseRadius * 0.1);

    QColor meshColor("#1a2332");
    meshColor.setAlphaF(meshColor.alphaF() * opacityFactor);
    painter->setPen(QPen(meshColor, 2.0, Qt::DotLine));
    painter->drawPath(routeMesh);

    QPointF vectorNodes[] = {
        QPointF(cx - baseRadius, cy + baseRadius * 0.3),
        QPointF(cx + baseRadius, cy - baseRadius * 0.2),
        QPointF(cx - baseRadius * 1.2, cy - baseRadius * 0.2),
        QPointF(cx + baseRadius * 1.2, cy + baseRadius * 0.1),
        QPointF(cx - baseRadius * 0.5, cy - baseRadius * 0.5),
        QPointF(cx + baseRadius * 0.4, cy + baseRadius * 0.6)
    };
    QColor nodeBrushColor("#131a26");
    QColor nodePenColor("#222d3d");
    nodeBrushColor.setAlphaF(nodeBrushColor.alphaF() * opacityFactor);
    nodePenColor.setAlphaF(nodePenColor.alphaF() * opacityFactor);

    painter->setBrush(nodeBrushColor);
    painter->setPen(QPen(nodePenColor, 1.2));
    for (const auto& node : vectorNodes) {
        painter->drawEllipse(node, 5, 5);
    }

    painter->save();
    painter->translate(cx - baseRadius * 0.1, cy - baseRadius * 0.55);
    painter->rotate(-15);

    QPainterPath planePath;
    planePath.moveTo(0, -18);
    planePath.lineTo(3, -6);
    planePath.lineTo(22, 2);
    planePath.lineTo(3, 5);
    planePath.lineTo(2, 14);
    planePath.lineTo(8, 19);
    planePath.lineTo(0, 17);
    planePath.lineTo(-8, 19);
    planePath.lineTo(-2, 14);
    planePath.lineTo(-3, 5);
    planePath.lineTo(-22, 2);
    planePath.lineTo(-3, -6);
    planePath.closeSubpath();

    QColor planeBrushColor("#152232");
    QColor planePenColor("#1f2e45");
    planeBrushColor.setAlphaF(planeBrushColor.alphaF() * opacityFactor);
    planePenColor.setAlphaF(planePenColor.alphaF() * opacityFactor);

    painter->setBrush(planeBrushColor);
    painter->setPen(QPen(planePenColor, 1.2));
    painter->drawPath(planePath);
    painter->restore();

    painter->save();
    painter->translate(cx + baseRadius * 0.1, cy + baseRadius * 0.45);

    QPainterPath truckPath;
    truckPath.addRoundedRect(QRectF(-22, -10, 32, 18), 2, 2);
    truckPath.moveTo(10, 8);
    truckPath.lineTo(20, 8);
    truckPath.lineTo(16, -3);
    truckPath.lineTo(10, -3);
    truckPath.closeSubpath();

    QColor truckBrushColor("#14202e");
    QColor truckPenColor("#1e2c3d");
    truckBrushColor.setAlphaF(truckBrushColor.alphaF() * opacityFactor);
    truckPenColor.setAlphaF(truckPenColor.alphaF() * opacityFactor);

    painter->setBrush(truckBrushColor);
    painter->setPen(QPen(truckPenColor, 1.2));
    painter->drawPath(truckPath);

    QColor exhaustColor("#1a2535");
    exhaustColor.setAlphaF(exhaustColor.alphaF() * opacityFactor);
    painter->setPen(QPen(exhaustColor, 2));
    painter->drawLine(-32, -4, -26, -4);
    painter->drawLine(-35, 1, -26, 1);
    painter->drawLine(-31, 6, -26, 6);
    painter->restore();
}

class TitlePageWidget : public QWidget {
    Q_OBJECT
public:
    TitlePageWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setStyleSheet(UIConfig::DARK_BG);
        setupTitleLayout();
    }

signals:
    void loadingComplete();

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        drawTechnicalBackground(&painter, rect(), QPointF(width() / 2.0, height() / 2.0), width(), height(), 0.55);
    }

private:
    QLabel* m_mainHeading;
    QLabel* m_subHeading;
    QProgressBar* m_loadingBar;
    QTimer* m_loadTimer;
    int m_currentProgress = 0;

    void setupTitleLayout() {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->setAlignment(Qt::AlignCenter);
        mainLayout->setSpacing(25);

        m_mainHeading = new QLabel("ROUTE OPTIMIZATION SYSTEM", this);
        m_mainHeading->setAlignment(Qt::AlignCenter);
        m_mainHeading->setStyleSheet("color: #00f2ff; font-family: 'Segoe UI', Arial, sans-serif; font-size: 42px; font-weight: 800; letter-spacing: 2px; background: transparent;");

        m_subHeading = new QLabel("BY ABDUL MANAN", this);
        m_subHeading->setAlignment(Qt::AlignCenter);
        m_subHeading->setStyleSheet("color: #ff007f; font-family: 'Segoe UI', Arial, sans-serif; font-size: 16px; font-weight: 700; letter-spacing: 4px; background: transparent;");

        m_loadingBar = new QProgressBar(this);
        m_loadingBar->setFixedSize(380, 14);
        m_loadingBar->setRange(0, 100);
        m_loadingBar->setValue(0);
        m_loadingBar->setStyleSheet(UIConfig::LOADING_BAR_STYLE);

        mainLayout->addStretch();
        mainLayout->addWidget(m_mainHeading);
        mainLayout->addWidget(m_subHeading);
        mainLayout->addSpacing(40);
        mainLayout->addWidget(m_loadingBar, 0, Qt::AlignCenter);
        mainLayout->addStretch();

        QGraphicsOpacityEffect* mainEff = new QGraphicsOpacityEffect(m_mainHeading);
        QGraphicsOpacityEffect* subEff = new QGraphicsOpacityEffect(m_subHeading);
        QGraphicsOpacityEffect* barEff = new QGraphicsOpacityEffect(m_loadingBar);

        m_mainHeading->setGraphicsEffect(mainEff);
        m_subHeading->setGraphicsEffect(subEff);
        m_loadingBar->setGraphicsEffect(barEff);

        QPropertyAnimation* animMain = new QPropertyAnimation(mainEff, "opacity");
        animMain->setDuration(1400);
        animMain->setStartValue(0.0);
        animMain->setEndValue(1.0);
        animMain->setEasingCurve(QEasingCurve::InQuad);

        QPropertyAnimation* animSub = new QPropertyAnimation(subEff, "opacity");
        animSub->setDuration(1200);
        animSub->setStartValue(0.0);
        animSub->setEndValue(1.0);
        animSub->setEasingCurve(QEasingCurve::InQuad);

        QPropertyAnimation* animBar = new QPropertyAnimation(barEff, "opacity");
        animBar->setDuration(1000);
        animBar->setStartValue(0.0);
        animBar->setEndValue(1.0);
        animBar->setEasingCurve(QEasingCurve::Linear);

        QSequentialAnimationGroup* sequentialGroup = new QSequentialAnimationGroup(this);
        sequentialGroup->addAnimation(animMain);
        sequentialGroup->addAnimation(animSub);
        sequentialGroup->addAnimation(animBar);

        m_loadTimer = new QTimer(this);
        connect(m_loadTimer, &QTimer::timeout, this, &TitlePageWidget::updateProgress);

        sequentialGroup->start(QAbstractAnimation::DeleteWhenStopped);
        m_loadTimer->start(65);
    }

    void updateProgress() {
        m_currentProgress++;
        m_loadingBar->setValue(m_currentProgress);
        if (m_currentProgress >= 100) {
            m_loadTimer->stop();
            emit loadingComplete();
        }
    }
};

class GraphVisualizerView : public QGraphicsView {
public:
    GraphVisualizerView(QGraphicsScene* scene, QWidget* parent = nullptr) : QGraphicsView(scene, parent) {
        setStyleSheet("background: #0d1117; border: none;");
    }

    void setHasGraphData(bool hasData) {
        if (m_hasGraphData != hasData) {
            m_hasGraphData = hasData;
            viewport()->update();
        }
    }

protected:
    void drawBackground(QPainter* painter, const QRectF& rect) override {
        QRect viewportRect = this->viewport()->rect();
        QPoint viewportCenter(viewportRect.width() / 2, viewportRect.height() / 2);
        QPointF targetCenter = mapToScene(viewportCenter);

        double targetOpacity = 1.0;

        if (m_hasGraphData) {
            targetCenter += QPointF(viewportRect.width() * 0.28, viewportRect.height() * 0.22);
            targetOpacity = 0.18;
        }

        drawTechnicalBackground(painter, rect, targetCenter, viewportRect.width(), viewportRect.height(), targetOpacity);
    }

private:
    bool m_hasGraphData = false;
};

class LocationMarkerItem : public QGraphicsItem {
public:
    LocationMarkerItem(QPointF position, QGraphicsItem* parent = nullptr)
        : QGraphicsItem(parent), m_pos(position) {
        setZValue(300);
    }

    QRectF boundingRect() const override {
        return QRectF(m_pos.x() - 20, m_pos.y() - 45, 40, 50);
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        painter->setRenderHint(QPainter::Antialiasing);

        QPainterPath pinPath;
        pinPath.moveTo(m_pos.x(), m_pos.y());
        pinPath.cubicTo(m_pos.x() - 18, m_pos.y() - 22, m_pos.x() - 16, m_pos.y() - 40, m_pos.x(), m_pos.y() - 40);
        pinPath.cubicTo(m_pos.x() + 16, m_pos.y() - 40, m_pos.x() + 18, m_pos.y() - 22, m_pos.x(), m_pos.y());

        pinPath.setFillRule(Qt::WindingFill);

        painter->setBrush(QColor("#ff007f"));
        painter->setPen(QPen(QColor("#0d1117"), 1.5));
        painter->drawPath(pinPath);

        painter->setBrush(QColor("#0d1117"));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPointF(m_pos.x(), m_pos.y() - 27), 5, 5);
    }
private:
    QPointF m_pos;
};

class MeasuredEdgeItem : public QObject, public QGraphicsLineItem {
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
    MeasuredEdgeItem(QLineF line, double weight, QGraphicsItem* parent = nullptr)
        : QGraphicsLineItem(line, parent), m_weight(weight) {
        setPen(QPen(Qt::white, 4, Qt::SolidLine, Qt::RoundCap));
        setZValue(200);

        m_label = new QGraphicsTextItem(QString::number(weight) + " km", this);
        m_label->setDefaultTextColor(Qt::white);
        m_label->setFont(QFont("Segoe UI", 12, QFont::Bold));
        m_label->setPos(line.center() + QPointF(10, -20));

        m_anim = new QPropertyAnimation(this, "opacity", this);
        m_anim->setDuration(600);
        m_anim->setStartValue(1.0);
        m_anim->setEndValue(0.3);
        m_anim->setEasingCurve(QEasingCurve::InOutSine);
        m_anim->setLoopCount(-1);
        m_anim->start();
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        QGraphicsLineItem::paint(painter, option, widget);
        QLineF ln = line();
        double angle = std::atan2(-ln.dy(), ln.dx());
        QPointF arrowHead = ln.p2();
        double arrowSize = 15;
        QPointF p1 = arrowHead - QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
        QPointF p2 = arrowHead - QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);
        painter->setBrush(Qt::white);
        painter->setPen(Qt::NoPen);
        painter->drawPolygon(QPolygonF() << arrowHead << p1 << p2);
    }
private:
    double m_weight;
    QGraphicsTextItem* m_label;
    QPropertyAnimation* m_anim;
};

class PathEdgeItem : public QObject, public QGraphicsLineItem {
    Q_OBJECT
    Q_PROPERTY(qreal progress READ progress WRITE setProgress)
public:
    PathEdgeItem(QLineF line, double weight, int durationMs, QGraphicsItem* parent = nullptr)
        : QGraphicsLineItem(line, parent), m_weight(weight), m_progress(0.0) {

        QPen dashPen(QColor("#00f2ff"), 3.5);
        dashPen.setStyle(Qt::DashLine);
        dashPen.setCapStyle(Qt::RoundCap);
        dashPen.setJoinStyle(Qt::RoundJoin);
        setPen(dashPen);
        setZValue(10);

        m_label = new QGraphicsTextItem(QString::number(m_weight) + " km", this);
        m_label->setDefaultTextColor(Qt::white);
        m_label->setFont(QFont("Segoe UI", 16, QFont::Bold));
        m_label->setPos(line.center() + QPointF(-35, -48));
        m_label->setZValue(15);

        m_anim = new QPropertyAnimation(this, "progress", this);
        m_anim->setDuration(durationMs);
        m_anim->setStartValue(0.0);
        m_anim->setEndValue(1.0);
        m_anim->setEasingCurve(QEasingCurve::Linear);
        m_anim->start();
    }

    ~PathEdgeItem() {
        if (m_anim) {
            m_anim->stop();
            delete m_anim;
        }
    }

    qreal progress() const { return m_progress; }
    void setProgress(qreal p) { m_progress = p; update(); }

    void removeLabel() {
        if (m_label) {
            delete m_label;
            m_label = nullptr;
        }
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        painter->setRenderHint(QPainter::Antialiasing, true);
        QGraphicsLineItem::paint(painter, option, widget);
        QLineF ln = line();
        double angle = std::atan2(-ln.dy(), ln.dx());

        QPointF arrowMid = ln.center();
        double arrowSize = 12;
        QPointF p1 = arrowMid - QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
        QPointF p2 = arrowMid - QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);

        painter->setBrush(QColor("#ff007f"));
        painter->setPen(Qt::NoPen);
        painter->drawPolygon(QPolygonF() << arrowMid << p1 << p2);

        QPointF endPoint = ln.p2();
        painter->setBrush(Qt::white);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(endPoint, 5, 5);
        painter->setBrush(QColor("#ff007f"));
        painter->drawEllipse(endPoint, 3, 3);
    }
private:
    double m_weight;
    qreal m_progress;
    QGraphicsTextItem* m_label = nullptr;
    QPropertyAnimation* m_anim = nullptr;
};

class EdgeWeightItem : public QGraphicsRectItem {
public:
    EdgeWeightItem(const QString& text, QPointF pos, QGraphicsItem* parent = nullptr)
        : QGraphicsRectItem(parent), m_text(text) {
        QFont font; font.setPointSize(8);
        QFontMetrics fm(font);
        QRect textRect = fm.boundingRect(text);
        setRect(pos.x() - (textRect.width() / 2) - 2, pos.y() - (textRect.height() / 2),
                textRect.width() + 4, textRect.height());
        setBrush(QColor("#0d1117"));
        setPen(Qt::NoPen);
        setZValue(5);
    }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        painter->setBrush(brush());
        painter->drawRect(rect());
        painter->setPen(QColor("#8b949e"));
        painter->drawText(rect(), Qt::AlignCenter, m_text);
    }
private:
    QString m_text;
};

class LivePerformanceChart : public QWidget {
public:
    LivePerformanceChart(double cost, double timeMs, double memMB, QString routeStr, QWidget* parent = nullptr)
        : QWidget(parent), m_cost(cost), m_timeMs(timeMs), m_mem(memMB), m_routeStr(routeStr) {
        setFixedHeight(120);

        m_routeContainer = new QTextEdit(this);
        m_routeContainer->setReadOnly(true);
        m_routeContainer->setLineWrapMode(QTextEdit::NoWrap);
        m_routeContainer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_routeContainer->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_routeContainer->setFrameShape(QFrame::NoFrame);

        m_routeContainer->setStyleSheet(
            "QTextEdit { background-color: transparent; border: none; padding: 0px; }"
            "QScrollBar:horizontal { background: #161b22; height: 8px; margin: 0px; border-radius: 4px; }"
            "QScrollBar::handle:horizontal { background: #30363d; min-width: 20px; border-radius: 4px; }"
            "QScrollBar::handle:horizontal:hover { background: #8b949e; }"
            "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0px; background: none; }"
            );

        QString richText = QString("<span style='color:#ffffff; font-weight:bold;'>Optimal Route: </span>"
                                   "<span style='color:#00f2ff; font-weight:bold;'>%1</span>").arg(m_routeStr);
        m_routeContainer->setHtml(richText);
        m_routeContainer->setFont(QFont("Segoe UI", 11));
        m_routeContainer->setGeometry(10, 5, width() - 20, 32);
    }

protected:
    void resizeEvent(QResizeEvent* event) override {
        QWidget::resizeEvent(event);
        if (m_routeContainer) {
            m_routeContainer->resize(width() - 20, 32);
        }
    }

    void paintEvent(QPaintEvent*) override {
        QPainter p(this); p.setRenderHint(QPainter::Antialiasing);

        QString labels[] = { "Cost (km)", "Algo Time (ms)", "Memory (MB)" };
        double vals[] = { m_cost, m_timeMs, m_mem };
        QColor colors[] = { QColor("#f85149"), QColor("#00f2ff"), QColor("#3fb950") };
        double maxVal = 0;
        for (double v : vals) if (v > maxVal) maxVal = v;
        double availableWidth = width() - 180;

        p.setFont(QFont("Segoe UI", 9, QFont::Normal));
        for (int i = 0; i < 3; ++i) {
            int w = (maxVal > 0) ? (int)((vals[i] / maxVal) * availableWidth) : 0;
            p.setBrush(colors[i]);
            p.drawRoundedRect(110, 40 + (i * 25), w, 15, 4, 4);
            p.setPen(QColor("#8b949e"));
            p.drawText(10, 52 + (i * 25), labels[i]);

            if (i == 1) {
                p.drawText(120 + w, 52 + (i * 25), QString::number(m_timeMs, 'f', 4) + " ms");
            }
            else if (i == 2) {
                p.drawText(120 + w, 52 + (i * 25), QString::number(m_mem, 'f', 4) + " MB");
            }
            else {
                p.drawText(120 + w, 52 + (i * 25), QString::number(vals[i], 'f', 2));
            }
        }
    }
private:
    double m_cost;
    double m_timeMs;
    double m_mem;
    QString m_routeStr;
    QTextEdit* m_routeContainer = nullptr;
};

class VehicleItem : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    VehicleItem() { setRect(-10, -10, 20, 20); setBrush(Qt::white); setZValue(100); }
};

class Dashboard : public QMainWindow {
    Q_OBJECT
public:
    Dashboard() { setupUI(); this->showFullScreen(); }
private:
    vector<City> m_cities;
    Graph m_graph;
    // ============================================================================================

    vector<QGraphicsEllipseItem*> m_nodeItems;
    bool m_isDynamic = false;
    QString m_selectedAlgo = "Select Algorithm";
    GraphVisualizerView* m_visualizer;
    QVBoxLayout* m_bottomChartArea;
    QLabel* m_modeIndicator;
    QWidget* m_sidebar;
    QWidget* m_bottomSection;
    QPushButton* m_goBackBtn;
    QPushButton* m_undoBtn;
    QPushButton* m_runOptBtn;
    MeasuredEdgeItem* m_currentMeasure = nullptr;
    QSequentialAnimationGroup* m_activeAnimationGroup = nullptr;
    bool m_sidebarVisible = true;
    bool m_statsVisible = true;

    double m_lastCost = 0.0;
    double m_lastTime = 0.0;
    double m_lastMemory = 45.2;
    QString m_lastAlgoName = "None";
    int m_lastInputSize = 0;

    struct AlgoStats {
        bool hasRun = false;
        double cost = 0.0;
        double speedMs = 0.0;
        double memoryMb = 45.2;
    };
    AlgoStats m_bruteStats;
    AlgoStats m_greedyStats;
    AlgoStats m_mananStats;
    AlgoStats m_dijkstraStats;

    double calculateExactMemoryUsage(int nodeCount, const QString& algoName) {
        double baselineOverhead = 45.2;
        double graphStorageOverhead = 0.0;

        if (m_isDynamic) {
            graphStorageOverhead = (nodeCount * sizeof(City) + nodeCount * sizeof(vector<double>)) / (1024.0 * 1024.0);
        }

        double matrixFootprint = (nodeCount * nodeCount * sizeof(double)) / (1024.0 * 1024.0);
        double activeAlgorithmMemory = 0.0;

        if (algoName == "Brute Force") {
            activeAlgorithmMemory = (nodeCount * sizeof(int)) / (1024.0 * 1024.0);
        }
        else if (algoName == "Greedy Search" || algoName == "Manan's Algorithm") {
            activeAlgorithmMemory = (nodeCount * sizeof(bool) + nodeCount * sizeof(int)) / (1024.0 * 1024.0);
        }
        else if (algoName == "Dijkstra (Shortest Path)") {
            double structures = (nodeCount * sizeof(double) + nodeCount * sizeof(int)) / (1024.0 * 1024.0);
            double priorityQueueMaxBound = (nodeCount * sizeof(pair<double, int>)) / (1024.0 * 1024.0);
            activeAlgorithmMemory = structures + priorityQueueMaxBound;
        }

        return baselineOverhead + matrixFootprint + graphStorageOverhead + activeAlgorithmMemory;
    }

    void setupUI() {
        QWidget* central = new QWidget(); central->setStyleSheet(UIConfig::DARK_BG);
        setCentralWidget(central);
        QVBoxLayout* root = new QVBoxLayout(central);
        root->setContentsMargins(0, 0, 0, 0);
        root->setSpacing(0);

        QWidget* headerWidget = new QWidget();
        headerWidget->setFixedHeight(60);
        headerWidget->setStyleSheet("background: #161b22; border-bottom: 1px solid #30363d;");
        QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
        QPushButton* toggleMenuBtn = new QPushButton("≡");
        toggleMenuBtn->setFixedSize(50, 50);
        toggleMenuBtn->setStyleSheet(UIConfig::TOGGLE_BTN);
        connect(toggleMenuBtn, &QPushButton::clicked, this, &Dashboard::toggleSidebar);
        QLabel* headerTitle = new QLabel("ROUTE OPTIMIZATION SYSTEM");
        headerTitle->setAlignment(Qt::AlignCenter);
        headerTitle->setStyleSheet("color: #00f2ff; font-weight: bold; font-size: 20px; border: none;");
        headerLayout->addWidget(toggleMenuBtn);
        headerLayout->addStretch();
        headerLayout->addWidget(headerTitle);
        headerLayout->addStretch();
        headerLayout->addSpacing(50);
        root->addWidget(headerWidget);

        QHBoxLayout* mainLayout = new QHBoxLayout();
        mainLayout->setSpacing(0);
        root->addLayout(mainLayout);

        m_sidebar = new QWidget();
        m_sidebar->setFixedWidth(240);
        m_sidebar->setStyleSheet(UIConfig::SIDEBAR_BG);
        QVBoxLayout* sideLayout = new QVBoxLayout(m_sidebar);
        mainLayout->addWidget(m_sidebar);

        auto addBtn = [&](QString t, auto f) {
            QPushButton* b = new QPushButton(t); b->setStyleSheet(UIConfig::SIDEBAR_BTN);
            sideLayout->addWidget(b); connect(b, &QPushButton::clicked, this, f); return b;
        };

        addBtn("📂 Load Dataset", &Dashboard::onLoadFile);
        addBtn("🎲 Generate Random Graph", &Dashboard::onGenerateRandom);
        addBtn("🔄 Toggle Representation", &Dashboard::toggleMode);
        addBtn("📏 Measure Specific Distance", &Dashboard::onMeasureSpecificDistance);

        QPushButton* algoBtn = new QPushButton("▼ " + m_selectedAlgo);
        algoBtn->setStyleSheet(UIConfig::SIDEBAR_BTN);
        QMenu* menu = new QMenu(this);
        menu->setStyleSheet(UIConfig::MENU_STYLE);

        menu->addAction("Brute Force");
        menu->addAction("Greedy Search");
        menu->addAction("Manan's Algorithm");
        menu->addAction("Dijkstra (Shortest Path)");

        algoBtn->setMenu(menu);
        connect(menu, &QMenu::triggered, [this, algoBtn](QAction* a) {
            m_selectedAlgo = a->text();
            algoBtn->setText("▼ " + m_selectedAlgo);
            updateRunButtonVisibilityState();
        });
        sideLayout->addWidget(algoBtn);

        m_runOptBtn = new QPushButton("🚀 Run Optimization");
        m_runOptBtn->setStyleSheet(UIConfig::RUN_OPTIMIZATION_STYLE);
        m_runOptBtn->hide();
        connect(m_runOptBtn, &QPushButton::clicked, this, &Dashboard::onRunOptimizationTriggered);
        sideLayout->addWidget(m_runOptBtn);

        m_undoBtn = new QPushButton("↩ Undo / Go Back");
        m_undoBtn->setStyleSheet(UIConfig::UNDO_BTN_STYLE);
        m_undoBtn->hide();
        connect(m_undoBtn, &QPushButton::clicked, this, &Dashboard::onUndoAnimationLayout);
        sideLayout->addWidget(m_undoBtn);

        addBtn("📊 Performance Analysis", &Dashboard::onShowPerformanceAnalysis);
        addBtn("⚖️ Compare Algorithms", &Dashboard::onShowCompareAlgorithms);
        addBtn("🧹 Reset View", &Dashboard::resetView);
        addBtn("❌ Exit System", &QApplication::quit);
        sideLayout->addStretch();

        QVBoxLayout* rightContent = new QVBoxLayout();
        rightContent->setContentsMargins(0, 0, 0, 0);
        mainLayout->addLayout(rightContent);

        m_visualizer = new GraphVisualizerView(new QGraphicsScene(), this);
        rightContent->addWidget(m_visualizer, 5);

        m_goBackBtn = new QPushButton("Go Back", m_visualizer);
        m_goBackBtn->setStyleSheet(UIConfig::BACK_BTN_STYLE);
        m_goBackBtn->hide();
        connect(m_goBackBtn, &QPushButton::clicked, this, &Dashboard::clearMeasurement);

        m_modeIndicator = new QLabel(m_visualizer);

        m_modeIndicator->setStyleSheet(
            "color: #00f2ff;"
            "padding: 10px;"
            "background: rgba(22, 27, 34, 0.8);"
            "border-radius: 5px;"
            "font-size: 14px;"
            "font-weight: bold;"
            );

        m_modeIndicator->setText("<span style='color:#ffffff;'>MODE:</span> DYNAMIC");
        m_modeIndicator->adjustSize();
        QWidget* statsHeaderWidget = new QWidget();
        statsHeaderWidget->setFixedHeight(35);
        statsHeaderWidget->setStyleSheet(UIConfig::STATS_HEADER);
        QHBoxLayout* statsHeaderLayout = new QHBoxLayout(statsHeaderWidget);
        statsHeaderLayout->setContentsMargins(15, 0, 15, 0);
        QPushButton* toggleStatsBtn = new QPushButton("≡");
        toggleStatsBtn->setFixedSize(30, 30);
        toggleStatsBtn->setStyleSheet(UIConfig::TOGGLE_BTN + " QPushButton { font-size: 18px; }");
        connect(toggleStatsBtn, &QPushButton::clicked, this, &Dashboard::toggleStats);
        QLabel* statsLabel = new QLabel("STATISTICS");
        statsLabel->setStyleSheet("border: none;");
        statsHeaderLayout->addWidget(toggleStatsBtn);
        statsHeaderLayout->addWidget(statsLabel);
        statsHeaderLayout->addStretch();
        rightContent->addWidget(statsHeaderWidget);

        m_bottomSection = new QWidget();
        m_bottomSection->setFixedHeight(140);
        m_bottomChartArea = new QVBoxLayout(m_bottomSection);
        m_bottomChartArea->setContentsMargins(10, 5, 10, 5);
        rightContent->addWidget(m_bottomSection);
    }

    void updateRunButtonVisibilityState() {
        if (!m_cities.empty() && m_selectedAlgo != "Select Algorithm" && m_undoBtn->isHidden()) {
            if (m_runOptBtn->isHidden()) {
                m_runOptBtn->show();

                QPoint originalPos = m_runOptBtn->pos();
                QPropertyAnimation* shakeAnim = new QPropertyAnimation(m_runOptBtn, "pos", this);
                shakeAnim->setDuration(300);
                shakeAnim->setKeyValueAt(0.0, originalPos);
                shakeAnim->setKeyValueAt(0.2, originalPos + QPoint(-7, 0));
                shakeAnim->setKeyValueAt(0.4, originalPos + QPoint(7, 0));
                shakeAnim->setKeyValueAt(0.6, originalPos + QPoint(-5, 0));
                shakeAnim->setKeyValueAt(0.8, originalPos + QPoint(5, 0));
                shakeAnim->setKeyValueAt(1.0, originalPos);
                shakeAnim->start(QAbstractAnimation::DeleteWhenStopped);
            }
        }
        else {
            m_runOptBtn->hide();
        }
    }

    void onRunOptimizationTriggered() {
        if (m_cities.empty() || m_selectedAlgo == "Select Algorithm") {
            return;
        }
        runOptimization();
    }

    void resizeEvent(QResizeEvent* event) override {
        QMainWindow::resizeEvent(event);
        updateGoBackButtonPosition();
        m_visualizer->viewport()->update();
    }

    void updateGoBackButtonPosition() {
        if (m_goBackBtn) {
            int x = m_visualizer->width() - m_goBackBtn->width() - 20;
            int y = m_visualizer->height() - m_goBackBtn->height() - 20;
            m_goBackBtn->move(x, y);
            m_goBackBtn->raise();
        }
    }

    void toggleSidebar() { m_sidebarVisible = !m_sidebarVisible; m_sidebar->setVisible(m_sidebarVisible); m_visualizer->viewport()->update(); }
    void toggleStats() { m_statsVisible = !m_statsVisible; m_bottomSection->setVisible(m_statsVisible); m_visualizer->viewport()->update(); }

    void toggleMode() {
        m_isDynamic = !m_isDynamic;
        m_modeIndicator->setText(m_isDynamic ? "<span style='color:#ffffff;'>MODE:</span> DYNAMIC" : "<span style='color:#ffffff;'>MODE:</span> STATIC");
        computeAllMetrics();
    }

    void onUndoAnimationLayout() {
        m_undoBtn->hide();
        updateRunButtonVisibilityState();
        if (m_activeAnimationGroup) {
            m_activeAnimationGroup->stop();
            delete m_activeAnimationGroup;
            m_activeAnimationGroup = nullptr;
        }

        for (auto* nodeItem : m_nodeItems) {
            if (nodeItem) {
                nodeItem->setPen(QPen(QColor("#ff0000"), 4));
            }
        }

        QList<QGraphicsItem*> items = m_visualizer->scene()->items();
        for (auto* item : items) {
            if (dynamic_cast<PathEdgeItem*>(item) || dynamic_cast<VehicleItem*>(item) || dynamic_cast<LocationMarkerItem*>(item)) {
                m_visualizer->scene()->removeItem(item);
                delete item;
            }
            if (auto* staticWeight = dynamic_cast<EdgeWeightItem*>(item)) {
                staticWeight->show();
            }
        }
        while (m_bottomChartArea->count() > 0) {
            delete m_bottomChartArea->takeAt(0)->widget();
        }
    }

    void onShowPerformanceAnalysis() {
        if (m_cities.empty() || m_lastAlgoName == "None") {
            QDialog errorDialog(this);
            errorDialog.setWindowTitle("No Metrics Available");
            errorDialog.setStyleSheet(UIConfig::DIALOG_STYLE);

            QVBoxLayout* layout = new QVBoxLayout(&errorDialog);
            layout->setContentsMargins(20, 20, 20, 20);
            layout->setSpacing(15);

            QLabel* errorMsg = new QLabel(
                "<b>No Performance Metrics Found!</b><br><br>"
                "Please load or generate a network graph, and successfully run an "
                "optimization algorithm before checking performance metrics evaluation indices."
                );
            errorMsg->setStyleSheet("color: #c9d1d9; font-size: 13px; line-height: 1.4;");
            layout->addWidget(errorMsg);

            QPushButton* closeBtn = new QPushButton("Close");
            closeBtn->setStyleSheet(UIConfig::RED_CLOSE_BTN);
            connect(closeBtn, &QPushButton::clicked, &errorDialog, &QDialog::accept);
            layout->addWidget(closeBtn, 0, Qt::AlignRight);

            errorDialog.exec();
            return;
        }

        QDialog dialog(this);
        dialog.setWindowTitle("Performance Metrics - " + m_lastAlgoName);
        dialog.setMinimumSize(500, 320);
        dialog.setStyleSheet("QDialog { background-color: #0d1117; border: 1px solid #30363d; }");

        QVBoxLayout* layout = new QVBoxLayout(&dialog);
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setSpacing(15);

        QLabel* title = new QLabel("Performance Summary (" + m_lastAlgoName + ")");
        title->setStyleSheet("color: #00f2ff; font-weight: bold; font-size: 15px;");
        layout->addWidget(title);

        QTableWidget* table = new QTableWidget(4, 2);
        table->setStyleSheet(UIConfig::TABLE_STYLE);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionMode(QAbstractItemView::NoSelection);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->verticalHeader()->setVisible(false);

        table->setHorizontalHeaderLabels(QStringList() << "Metric" << "Description");

        auto setTableRow = [table](int row, QString metric, QString desc) {
            QTableWidgetItem* mItem = new QTableWidgetItem(metric);
            QTableWidgetItem* dItem = new QTableWidgetItem(desc);
            mItem->setForeground(QBrush(QColor("#c9d1d9")));
            dItem->setForeground(QBrush(QColor("#8b949e")));
            table->setItem(row, 0, mItem);
            table->setItem(row, 1, dItem);
        };

        QString compStr = "O(V + E log V)";
        if (m_lastAlgoName == "Brute Force") compStr = QString("O(N!) [Nodes: %1]").arg(m_lastInputSize);
        else if (m_lastAlgoName == "Greedy Search" || m_lastAlgoName == "Manan's Algorithm") compStr = QString("O(N²) [Nodes: %1]").arg(m_lastInputSize);

        setTableRow(0, "Algorithm Execution Time", QString("%1 ms").arg(QString::number(m_lastTime, 'f', 4)));
        setTableRow(1, "Path Cost", QString("%1 km").arg(m_lastCost, 0, 'f', 2));
        setTableRow(2, "Memory Usage", QString("%1 MB").arg(m_lastMemory, 0, 'f', 4));
        setTableRow(3, "Complexity", compStr);

        layout->addWidget(table);

        QPushButton* closeBtn = new QPushButton("Close");
        closeBtn->setStyleSheet(UIConfig::RED_CLOSE_BTN);
        connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(closeBtn, 0, Qt::AlignRight);

        dialog.exec();
    }

    void computeAllMetrics() {
        if (m_cities.empty()) return;
        int n = m_cities.size();

        QElapsedTimer timer;

        // Brute Force (simple cyclic)
        timer.start();
        auto bruteRes = Algorithms::bruteForce(m_graph, 0);
        double bruteTime = timer.nsecsElapsed() / 1000000.0;
        m_bruteStats = { true, bruteRes.second, bruteTime, calculateExactMemoryUsage(n, "Brute Force") };

        // Greedy Search
        timer.restart();
        auto greedyRes = Algorithms::greedySearch(m_graph, 0);
        double greedyTime = timer.nsecsElapsed() / 1000000.0;
        m_greedyStats = { true, greedyRes.second, greedyTime, calculateExactMemoryUsage(n, "Greedy Search") };

        // Manan's Algorithm
        timer.restart();
        auto mananRes = Algorithms::mananAlgorithm(m_graph, 0);
        double mananTime = timer.nsecsElapsed() / 1000000.0;
        m_mananStats = { true, mananRes.second, mananTime, calculateExactMemoryUsage(n, "Manan's Algorithm") };

        m_dijkstraStats = { false, 0.0, 0.0, 45.2 };
    }
    // ==============================================================================================

    void onShowCompareAlgorithms() {
        if (m_cities.empty()) {
            QDialog errorDialog(this);
            errorDialog.setWindowTitle("No Data");
            errorDialog.setStyleSheet(UIConfig::DIALOG_STYLE);

            QVBoxLayout* layout = new QVBoxLayout(&errorDialog);
            layout->setContentsMargins(20, 20, 20, 20);
            layout->setSpacing(15);

            QLabel* errorMsg = new QLabel("<b>No Graph Data Loaded!</b><br><br>Please load a dataset matrix configuration or generate a random graph network first before checking comparison details.");
            errorMsg->setStyleSheet("color: #c9d1d9; font-size: 13px; line-height: 1.4;");
            layout->addWidget(errorMsg);

            QPushButton* closeBtn = new QPushButton("Close");
            closeBtn->setStyleSheet(UIConfig::RED_CLOSE_BTN);
            connect(closeBtn, &QPushButton::clicked, &errorDialog, &QDialog::accept);
            layout->addWidget(closeBtn, 0, Qt::AlignRight);

            errorDialog.exec();
            return;
        }

        QDialog dialog(this);
        dialog.setWindowTitle("Algorithm Comparison");
        dialog.setMinimumSize(740, 280);
        dialog.setStyleSheet("QDialog { background-color: #0d1117; border: 1px solid #30363d; }");

        QVBoxLayout* layout = new QVBoxLayout(&dialog);
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setSpacing(15);

        QLabel* title = new QLabel("Algorithm Comparison");
        title->setStyleSheet("color: #00f2ff; font-weight: bold; font-size: 16px;");
        layout->addWidget(title);

        int totalRows = m_dijkstraStats.hasRun ? 4 : 3;

        QTableWidget* table = new QTableWidget(totalRows, 6);
        table->setStyleSheet(UIConfig::TABLE_STYLE);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionMode(QAbstractItemView::NoSelection);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->verticalHeader()->setVisible(false);

        table->setHorizontalHeaderLabels(QStringList() << "Algorithm" << "Accuracy" << "Memory (MB)" << "Complexity" << "Size" << "Representation");

        QString graphSizeStr = QString::number(m_cities.size());
        QString repModeStr = m_isDynamic ? "Dynamic" : "Static";

        auto setCompareRow = [table, graphSizeStr, repModeStr](int row, QString algo, AlgoStats stats, QString complexity) {
            table->setItem(row, 0, new QTableWidgetItem(algo));
            table->setItem(row, 1, new QTableWidgetItem(QString("%1 km (Cost)").arg(stats.cost, 0, 'f', 1)));
            table->setItem(row, 2, new QTableWidgetItem(QString("%1 MB").arg(stats.memoryMb, 0, 'f', 4)));
            table->setItem(row, 3, new QTableWidgetItem(complexity));
            table->setItem(row, 4, new QTableWidgetItem(graphSizeStr));
            table->setItem(row, 5, new QTableWidgetItem(repModeStr));
            table->item(row, 4)->setTextAlignment(Qt::AlignCenter);
            table->item(row, 5)->setTextAlignment(Qt::AlignCenter);

            for (int col = 0; col < 6; ++col) {
                table->item(row, col)->setForeground(QBrush(col == 0 ? QColor("#00f2ff") : QColor("#c9d1d9")));
            }
        };

        setCompareRow(0, "Brute Force", m_bruteStats, "O(N!)");
        setCompareRow(1, "Greedy Search", m_greedyStats, "O(N²)");
        setCompareRow(2, "Manan's Algorithm", m_mananStats, "O(N²)");
        if (m_dijkstraStats.hasRun) {
            setCompareRow(3, "Dijkstra (Shortest Path)", m_dijkstraStats, "O(V + E log V)");
        }

        layout->addWidget(table);

        QPushButton* closeBtn = new QPushButton("Close");
        closeBtn->setStyleSheet(UIConfig::RED_CLOSE_BTN);
        connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(closeBtn, 0, Qt::AlignRight);

        dialog.exec();
    }

    void onMeasureSpecificDistance() {
        if (m_cities.empty()) {
            QDialog errorDialog(this);
            errorDialog.setWindowTitle("Measurement Error");
            errorDialog.setStyleSheet(UIConfig::DIALOG_STYLE);

            QVBoxLayout* layout = new QVBoxLayout(&errorDialog);
            layout->setContentsMargins(20, 20, 20, 20);
            layout->setSpacing(15);

            QLabel* errorMsg = new QLabel("<b>Error! No graph Loaded</b>");
            errorMsg->setStyleSheet("color: #c9d1d9; font-size: 13px; line-height: 1.4;");
            layout->addWidget(errorMsg);

            QPushButton* closeBtn = new QPushButton("Close");
            closeBtn->setStyleSheet(UIConfig::RED_CLOSE_BTN);
            connect(closeBtn, &QPushButton::clicked, &errorDialog, &QDialog::accept);
            layout->addWidget(closeBtn, 0, Qt::AlignRight);

            errorDialog.exec();
            return;
        }

        QDialog dialog(this);
        dialog.setWindowTitle("Measure Distance");
        dialog.setStyleSheet(UIConfig::DIALOG_STYLE);
        QFormLayout form(&dialog);
        QLineEdit* sIn = new QLineEdit();
        QLineEdit* dIn = new QLineEdit();
        form.addRow("Source Node:", sIn);
        form.addRow("Destination Node:", dIn);
        QPushButton* ok = new QPushButton("Okay");
        form.addRow(ok);
        connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);

        if (dialog.exec() == QDialog::Accepted) {
            int u = -1, v = -1;
            for (const auto& c : m_cities) {
                if (QString::fromStdString(c.getName()).toUpper() == sIn->text().trimmed().toUpper()) u = c.getId();
                if (QString::fromStdString(c.getName()).toUpper() == dIn->text().trimmed().toUpper()) v = c.getId();
            }
            if (u != -1 && v != -1 && m_graph.hasEdge(u, v)) {
                clearMeasurement();
                double weight = m_graph.getWeight(u, v);
                QLineF line(QPointF(m_cities[u].getX(), m_cities[u].getY()), QPointF(m_cities[v].getX(), m_cities[v].getY()));
                m_currentMeasure = new MeasuredEdgeItem(line, weight);
                m_visualizer->scene()->addItem(m_currentMeasure);
                m_goBackBtn->show();
                updateGoBackButtonPosition();
            }
        }
    }
    // ==============================================================================================

    void clearMeasurement() {
        if (m_currentMeasure) {
            m_visualizer->scene()->removeItem(m_currentMeasure);
            delete m_currentMeasure;
            m_currentMeasure = nullptr;
        }
        m_goBackBtn->hide();
    }

    void onGenerateRandom() {
        resetView();
        random_device rd;
        mt19937 g(rd());

        int n = m_isDynamic ? QRandomGenerator::global()->bounded(3, 51) : QRandomGenerator::global()->bounded(3, 21);

        m_graph.setVertices(n);
        vector<vector<double>> newAdj(n, vector<double>(n, 0.0));

        vector<string> names;
        for (int i = 0; i < n; ++i) {
            if (i < 26) {
                names.push_back(string(1, char('A' + i)));
            }
            else {
                names.push_back("A" + string(1, char('A' + (i - 26))));
            }
        }
        shuffle(names.begin(), names.end(), g);

        double radius = 250;
        m_cities.clear();
        for (int i = 0; i < n; ++i) {
            double angle = (2 * M_PI * i) / n;
            double x = 500 + radius * cos(angle);
            double y = 350 + radius * sin(angle);
            m_cities.push_back(City(i, names[i], x, y));
        }
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double weight = QRandomGenerator::global()->bounded(5, 150);
                m_graph.addEdge(i, j, weight);
                m_graph.addEdge(j, i, weight);
            }
        }
        drawGraph();
        computeAllMetrics();
        updateRunButtonVisibilityState();
    }
    // ==============================================================================================

    void showFormattingAlert(const QString& detailMessage) {
        QDialog errDialog(this);
        errDialog.setWindowTitle("File Parsing Error");
        errDialog.setStyleSheet(UIConfig::DIALOG_STYLE);

        QVBoxLayout* layout = new QVBoxLayout(&errDialog);
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setSpacing(15);

        QLabel* label = new QLabel(QString("<b>Invalid Format!</b><br><br>%1").arg(detailMessage));
        label->setStyleSheet("color: #c9d1d9; font-size: 13px; line-height: 1.4;");
        layout->addWidget(label);

        QPushButton* closeBtn = new QPushButton("Got It");
        closeBtn->setStyleSheet(UIConfig::RED_CLOSE_BTN);
        connect(closeBtn, &QPushButton::clicked, &errDialog, &QDialog::accept);
        layout->addWidget(closeBtn, 0, Qt::AlignRight);

        errDialog.exec();
    }

    void onLoadFile() {
        QString path = QFileDialog::getOpenFileName(this, "Select Dataset", "", "Text Files (*.txt)");
        if (path.isEmpty()) return;
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            showFormattingAlert("Unable to open the selected dataset file template.");
            return;
        }

        QTextStream in(&file);
        QString firstLine = in.readLine().trimmed();
        bool ok = false;
        int n = firstLine.toInt(&ok);

        if (!ok || n <= 0) {
            showFormattingAlert("The file must start with a valid total number of cities integer row.");
            return;
        }

        if (!m_isDynamic && n > 20) {
            QDialog errorDialog(this);
            errorDialog.setWindowTitle("Dataset Cap Exceeded");
            errorDialog.setStyleSheet(UIConfig::DIALOG_STYLE);

            QVBoxLayout* layout = new QVBoxLayout(&errorDialog);
            layout->setContentsMargins(20, 20, 20, 20);
            layout->setSpacing(15);

            QLabel* errorMsg = new QLabel(
                "<b>Limit Exceeded!</b><br><br>"
                "Static Mode supports up to <b>20 cities</b>.<br>"
                "Please toggle to <b>Dynamic Mode</b> to handle larger datasets seamlessly."
                );
            errorMsg->setStyleSheet("color: #c9d1d9; font-size: 13px; line-height: 1.4;");
            layout->addWidget(errorMsg);

            QPushButton* closeBtn = new QPushButton("Got It");
            closeBtn->setStyleSheet(UIConfig::RED_CLOSE_BTN);
            connect(closeBtn, &QPushButton::clicked, &errorDialog, &QDialog::accept);
            layout->addWidget(closeBtn, 0, Qt::AlignRight);

            errorDialog.exec();
            return;
        }

        vector<City> parsedCities;
        for (int i = 0; i < n; ++i) {
            if (in.atEnd()) {
                showFormattingAlert(QString("Expected %1 city labels, but hit end of file early at node row index %2.").arg(n).arg(i));
                return;
            }
            QString nameQt = in.readLine().trimmed();
            if (nameQt.isEmpty()) {
                showFormattingAlert("City labels cannot be blank lines or empty text strings.");
                return;
            }
            double angle = (2 * M_PI * i) / n;
            double x = 500 + 250 * cos(angle);
            double y = 350 + 250 * sin(angle);
            parsedCities.push_back(City(i, nameQt.toStdString(), x, y));
        }

        vector<vector<double>> parsedMatrix(n, vector<double>(n, 0.0));
        for (int i = 0; i < n; ++i) {
            if (in.atEnd()) {
                showFormattingAlert(QString("Incomplete distance adjacency grid. Missing weight matrix string configurations at row index %1.").arg(i));
                return;
            }
            QString line = in.readLine().trimmed();
            QStringList weights = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            if (weights.size() < n) {
                showFormattingAlert(QString("Adjacency layout mismatch at row %1. Expected at least %2 column weight values, found %3.")
                                        .arg(i).arg(n).arg(weights.size()));
                return;
            }
            for (int j = 0; j < n; ++j) {
                bool parseWeightOk = false;
                double val = weights[j].toDouble(&parseWeightOk);
                if (!parseWeightOk || val < 0) {
                    showFormattingAlert(QString("Non-numeric or negative edge values found at weight item intersection point grid index [%1][%2].").arg(i).arg(j));
                    return;
                }
                parsedMatrix[i][j] = val;
            }
        }

        resetView();
        m_cities = std::move(parsedCities);
        m_graph.setVertices(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (parsedMatrix[i][j] > 0) {
                    m_graph.addEdge(i, j, parsedMatrix[i][j]);
                }
            }
        }
        drawGraph();
        computeAllMetrics();
        updateRunButtonVisibilityState();
    }
    // ==============================================================================================

    void drawGraph() {
        m_visualizer->scene()->clear();
        m_visualizer->setHasGraphData(true);
        m_nodeItems.assign(m_cities.size(), nullptr);

        QPen edgePen(QColor(255, 255, 255, 40), 1);
        QPointF center(500, 350);

        for (size_t i = 0; i < m_cities.size(); ++i) {
            for (size_t j = i + 1; j < m_cities.size(); ++j) {
                if (m_graph.hasEdge(i, j)) {
                    double weight = m_graph.getWeight(i, j);
                    QPointF p1(m_cities[i].getX(), m_cities[i].getY());
                    QPointF p2(m_cities[j].getX(), m_cities[j].getY());
                    m_visualizer->scene()->addLine(QLineF(p1, p2), edgePen);
                    QPointF mid = (p1 + p2) / 2.0;
                    m_visualizer->scene()->addItem(new EdgeWeightItem(QString::number(weight), mid));
                }
            }
        }
        for (const auto& city : m_cities) {
            QPointF pos(city.getX(), city.getY());
            auto* nodeEllipse = m_visualizer->scene()->addEllipse(pos.x() - 15, pos.y() - 15, 30, 30, QPen(QColor("#ff0000"), 4), QBrush(QColor("#000000")));
            m_nodeItems[city.getId()] = nodeEllipse;

            QLineF radialLine(center, pos);
            radialLine.setLength(radialLine.length() + 25);
            QPointF textPos = radialLine.p2();

            auto* t = m_visualizer->scene()->addText(QString::fromStdString(city.getName()));
            t->setDefaultTextColor(Qt::white);
            t->setFont(QFont("Segoe UI", 10, QFont::Bold));
            t->setPos(textPos.x() - (t->boundingRect().width() / 2), textPos.y() - (t->boundingRect().height() / 2));
        }
    }

    void runOptimization() {
        if (m_cities.empty()) return;
        if (m_selectedAlgo == "Select Algorithm") return;

        int n = m_cities.size();
        int startNodeId = 0;

        if (m_selectedAlgo != "Dijkstra (Shortest Path)") {
            QDialog startNodeDialog(this);
            startNodeDialog.setWindowTitle("Select Start Node");
            startNodeDialog.setStyleSheet(UIConfig::DIALOG_STYLE);
            QFormLayout startForm(&startNodeDialog);

            QLineEdit* nodeIn = new QLineEdit();
            nodeIn->setPlaceholderText("e.g. A, B, C");
            startForm.addRow("Starting Node Name:", nodeIn);

            QPushButton* nodeOkBtn = new QPushButton("Okay");
            startForm.addRow(nodeOkBtn);
            connect(nodeOkBtn, &QPushButton::clicked, &startNodeDialog, &QDialog::accept);

            if (startNodeDialog.exec() != QDialog::Accepted) {
                return;
            }

            QString inputNode = nodeIn->text().trimmed().toUpper();
            bool foundNode = false;
            for (const auto& city : m_cities) {
                if (QString::fromStdString(city.getName()).toUpper() == inputNode) {
                    startNodeId = city.getId();
                    foundNode = true;
                    break;
                }
            }

            if (!foundNode) {
                QMessageBox::warning(this, "Invalid Node", "The specified starting node was not found inside the loaded dataset graph layout. Aborting execution.");
                return;
            }
        }

        QDialog speedDialog(this);
        speedDialog.setWindowTitle("Animation Speed");
        speedDialog.setStyleSheet(UIConfig::DIALOG_STYLE);
        QFormLayout speedForm(&speedDialog);

        QLineEdit* speedIn = new QLineEdit();
        speedIn->setText("500");
        speedForm.addRow("Step Duration (ms):", speedIn);

        QPushButton* speedOkBtn = new QPushButton("Okay");
        speedForm.addRow(speedOkBtn);
        connect(speedOkBtn, &QPushButton::clicked, &speedDialog, &QDialog::accept);

        if (speedDialog.exec() != QDialog::Accepted) {
            return;
        }

        bool speedParseOk = false;
        int animationSpeedMs = speedIn->text().trimmed().toInt(&speedParseOk);
        if (!speedParseOk || animationSpeedMs <= 0) {
            animationSpeedMs = 500;
        }

        m_undoBtn->hide();
        m_runOptBtn->hide();

        if (m_activeAnimationGroup) {
            m_activeAnimationGroup->stop();
            delete m_activeAnimationGroup;
            m_activeAnimationGroup = nullptr;
        }

        for (auto* nodeItem : m_nodeItems) {
            if (nodeItem) {
                nodeItem->setPen(QPen(QColor("#ff0000"), 4));
            }
        }

        QList<QGraphicsItem*> items = m_visualizer->scene()->items();
        for (auto* item : items) {
            if (dynamic_cast<PathEdgeItem*>(item) || dynamic_cast<VehicleItem*>(item) || dynamic_cast<LocationMarkerItem*>(item)) {
                m_visualizer->scene()->removeItem(item);
                delete item;
            }
            if (auto* staticWeight = dynamic_cast<EdgeWeightItem*>(item)) {
                staticWeight->hide();
            }
        }

        vector<int> path;
        QElapsedTimer algoTimer;
        algoTimer.start();

        if (m_selectedAlgo == "Dijkstra (Shortest Path)") {
            QDialog dialog(this);
            dialog.setWindowTitle("Dijkstra");
            dialog.setStyleSheet(UIConfig::DIALOG_STYLE);
            QFormLayout form(&dialog);
            QLineEdit* sIn = new QLineEdit();
            QLineEdit* dIn = new QLineEdit();
            form.addRow("Start Node Name:", sIn);
            form.addRow("Destination Node Name:", dIn);
            QPushButton* ok = new QPushButton("Compute Path");
            form.addRow(ok);
            connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);

            if (dialog.exec() != QDialog::Accepted) {
                updateRunButtonVisibilityState();
                return;
            }

            algoTimer.restart();

            int startId = -1, destId = -1;
            for (const auto& c : m_cities) {
                if (QString::fromStdString(c.getName()).toUpper() == sIn->text().trimmed().toUpper()) startId = c.getId();
                if (QString::fromStdString(c.getName()).toUpper() == dIn->text().trimmed().toUpper()) destId = c.getId();
            }
            if (startId == -1 || destId == -1) {
                updateRunButtonVisibilityState();
                return;
            }

            startNodeId = startId;
            auto dijkstraRes = Algorithms::dijkstra(m_graph, startId, destId);
            if (dijkstraRes.second >= 1e8) {
                updateRunButtonVisibilityState();
                return;
            }
            path = dijkstraRes.first;
        }
        else if (m_selectedAlgo == "Greedy Search") {
            auto res = Algorithms::greedySearch(m_graph, startNodeId);
            path = res.first;
        }
        else if (m_selectedAlgo == "Manan's Algorithm") {
            auto res = Algorithms::mananAlgorithm(m_graph, startNodeId);
            path = res.first;
        }
        else { // Brute Force
            auto res = Algorithms::bruteForce(m_graph, startNodeId);
            path = res.first;
        }

        LocationMarkerItem* marker = new LocationMarkerItem(QPointF(m_cities[startNodeId].getX(), m_cities[startNodeId].getY()));
        m_visualizer->scene()->addItem(marker);

        double calculationTimeMs = algoTimer.nsecsElapsed() / 1000000.0;
        double currentCalculatedMemory = calculateExactMemoryUsage(n, m_selectedAlgo);

        m_lastTime = calculationTimeMs;
        m_lastMemory = currentCalculatedMemory;
        m_lastAlgoName = m_selectedAlgo;
        m_lastInputSize = n;

        double totalCost = 0;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            totalCost += m_graph.getWeight(path[i], path[i + 1]);
        }

        if (m_selectedAlgo == "Brute Force") {
            m_bruteStats = { true, totalCost, calculationTimeMs, currentCalculatedMemory };
        }
        else if (m_selectedAlgo == "Greedy Search") {
            m_greedyStats = { true, totalCost, calculationTimeMs, currentCalculatedMemory };
        }
        else if (m_selectedAlgo == "Manan's Algorithm") {
            m_mananStats = { true, totalCost, calculationTimeMs, currentCalculatedMemory };
        }
        else if (m_selectedAlgo == "Dijkstra (Shortest Path)") {
            m_dijkstraStats = { true, totalCost, calculationTimeMs, currentCalculatedMemory };
        }

        QStringList routeNames;
        for (int node : path) {
            routeNames.append(QString::fromStdString(m_cities[node].getName()));
        }
        QString routeString = routeNames.join(" → ");

        VehicleItem* v = new VehicleItem();
        m_visualizer->scene()->addItem(v);

        m_activeAnimationGroup = new QSequentialAnimationGroup(this);

        struct MoveStep { int from; int to; double weight; PathEdgeItem* edgeItem; };
        auto* allocatedSteps = new vector<MoveStep>();

        if (startNodeId < (int)m_nodeItems.size() && m_nodeItems[startNodeId]) {
            m_nodeItems[startNodeId]->setPen(QPen(QColor("#00ff00"), 4));
        }

        for (size_t i = 0; i < path.size() - 1; ++i) {
            int from = path[i]; int to = path[i + 1];
            double w = m_graph.getWeight(from, to);

            QPropertyAnimation* a = new QPropertyAnimation(v, "pos");
            a->setDuration(animationSpeedMs);
            a->setEasingCurve(QEasingCurve::Linear);
            a->setStartValue(QPointF(m_cities[from].getX(), m_cities[from].getY()));
            a->setEndValue(QPointF(m_cities[to].getX(), m_cities[to].getY()));

            connect(a, &QPropertyAnimation::stateChanged, [this, from, to, w, animationSpeedMs, allocatedSteps](QAbstractAnimation::State newState, QAbstractAnimation::State oldState) {
                if (newState == QAbstractAnimation::Running) {
                    QLineF baseLine(QPointF(m_cities[from].getX(), m_cities[from].getY()), QPointF(m_cities[to].getX(), m_cities[to].getY()));
                    QLineF clippedLine = baseLine;
                    clippedLine.setLength(baseLine.length() - 15);
                    clippedLine.setLine(clippedLine.p2().x(), clippedLine.p2().y(), clippedLine.p1().x(), clippedLine.p1().y());
                    clippedLine.setLength(clippedLine.length() - 15);
                    clippedLine.setLine(clippedLine.p2().x(), clippedLine.p2().y(), clippedLine.p1().x(), clippedLine.p1().y());

                    auto* pathEdge = new PathEdgeItem(clippedLine, w, animationSpeedMs);
                    m_visualizer->scene()->addItem(pathEdge);
                    allocatedSteps->push_back({ from, to, w, pathEdge });
                }
                else if (newState == QAbstractAnimation::Stopped && oldState == QAbstractAnimation::Running) {
                    if (to < (int)m_nodeItems.size() && m_nodeItems[to]) {
                        m_nodeItems[to]->setPen(QPen(QColor("#00ff00"), 4));
                    }
                    for (auto& step : *allocatedSteps) {
                        if (step.from == from && step.to == to && step.edgeItem) {
                            step.edgeItem->removeLabel();
                        }
                    }
                }
            });
            m_activeAnimationGroup->addAnimation(a);
        }

        connect(m_activeAnimationGroup, &QSequentialAnimationGroup::finished, [this, totalCost, routeString, allocatedSteps]() {
            while (m_bottomChartArea->count() > 0) { delete m_bottomChartArea->takeAt(0)->widget(); }
            m_bottomChartArea->addWidget(new LivePerformanceChart(totalCost, m_lastTime, m_lastMemory, routeString));

            for (auto& step : *allocatedSteps) {
                if (step.edgeItem) {
                    step.edgeItem->removeLabel();
                }
            }
            delete allocatedSteps;

            m_lastCost = totalCost;
            m_activeAnimationGroup = nullptr;

            m_undoBtn->show();

            QPoint originalPos = m_undoBtn->pos();
            QPropertyAnimation* shakeAnim = new QPropertyAnimation(m_undoBtn, "pos", this);
            shakeAnim->setDuration(300);
            shakeAnim->setKeyValueAt(0.0, originalPos);
            shakeAnim->setKeyValueAt(0.2, originalPos + QPoint(-7, 0));
            shakeAnim->setKeyValueAt(0.4, originalPos + QPoint(7, 0));
            shakeAnim->setKeyValueAt(0.6, originalPos + QPoint(-5, 0));
            shakeAnim->setKeyValueAt(0.8, originalPos + QPoint(5, 0));
            shakeAnim->setKeyValueAt(1.0, originalPos);
            shakeAnim->start(QAbstractAnimation::DeleteWhenStopped);
        });

        m_activeAnimationGroup->start(QAbstractAnimation::DeleteWhenStopped);
    }
    // ==============================================================================================

    void resetView() {
        m_undoBtn->hide();
        m_runOptBtn->hide();
        if (m_activeAnimationGroup) {
            m_activeAnimationGroup->stop();
            delete m_activeAnimationGroup;
            m_activeAnimationGroup = nullptr;
        }
        m_visualizer->scene()->clear();
        m_visualizer->setHasGraphData(false);

        m_cities.clear();
        m_graph.clear();
        m_nodeItems.clear();
        m_lastCost = 0.0; m_lastTime = 0.0; m_lastMemory = 45.2; m_lastAlgoName = "None"; m_lastInputSize = 0;
        m_bruteStats.hasRun = m_greedyStats.hasRun = m_mananStats.hasRun = false;
        m_dijkstraStats.hasRun = false;
        while (m_bottomChartArea->count() > 0) { delete m_bottomChartArea->takeAt(0)->widget(); }

        updateRunButtonVisibilityState();
    }
};

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    Dashboard* systemDashboard = new Dashboard();
    TitlePageWidget* titlePage = new TitlePageWidget();

    titlePage->showFullScreen();

    QObject::connect(titlePage, &TitlePageWidget::loadingComplete, [systemDashboard, titlePage]() {
        systemDashboard->showFullScreen();
        titlePage->close();
        titlePage->deleteLater();
    });

    return a.exec();
}

#include "main.moc"
