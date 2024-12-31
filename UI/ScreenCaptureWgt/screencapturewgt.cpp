#include "screencapturewgt.h"


ScreenCaptureWgt::ScreenCaptureWgt(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(Qt::Tool /*| Qt::WindowStaysOnTopHint*/ | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    setMouseTracking(true); // 启用鼠标追踪
    m_maxRect = QGuiApplication::primaryScreen()->geometry();

    m_pMenu = new QWidget(this);
    m_pMenu->setFocusPolicy(Qt::NoFocus);
    m_pMenu->setFixedHeight(50);
    m_pMenu->setObjectName("SCMenu");
    m_pMenu->setStyleSheet("QWidget#SCMenu\
    {\
        background-color: white;\
        border-radius: 4px;\
    }");

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(5, 5, 5, 5);
    m_pBtnClose = new QPushButton("X");
    m_pBtnClose->setFixedSize(40, 40);
    m_pBtnClose->setFocusPolicy(Qt::NoFocus);
    pLayout->addWidget(m_pBtnClose);
    m_pBtnOk = new QPushButton("√");
    m_pBtnOk->setFixedSize(40, 40);
    m_pBtnOk->setFocusPolicy(Qt::NoFocus);
    pLayout->addWidget(m_pBtnOk);
    m_pMenu->setLayout(pLayout);
    m_pMenu->setCursor(Qt::ArrowCursor);
    m_pMenu->move(10000, 10000);
    m_pMenu->hide();

    connect(m_pBtnOk, SIGNAL(clicked(bool)), this, SLOT(onOk()));
    connect(m_pBtnClose, SIGNAL(clicked(bool)), this, SLOT(onClose()));

    showFullScreen();
}

QPixmap ScreenCaptureWgt::pixmap()
{
    return m_currPixmap;
}

void ScreenCaptureWgt::paintEvent(QPaintEvent *)
{
    QRect rect = scRect();
    int x = rect.x();
    int y = rect.y();
    int w = rect.width();
    int h = rect.height();

    QPoint tlPos = rect.topLeft();
    QPoint trPos = rect.topRight();
    QPoint tPos = (tlPos + trPos)/2.0;
    QPoint blPos = rect.bottomLeft();
    QPoint brPos = rect.bottomRight();
    QPoint bPos = (blPos + brPos)/2.0;
    QPoint lPos = (blPos + tlPos)/2.0;
    QPoint rPos = (trPos + brPos)/2.0;

    QList<QPoint> opPoints;
    opPoints << tlPos << trPos << tPos << blPos << brPos << bPos << lPos << rPos;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    QPen pen;
    pen.setColor(QColor(0x0099ff));
    pen.setWidth(2);
    // pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawPixmap(0, 0, m_pBgScreen);

    if (qAbs(m_startPos.x() - m_endPos.x()) < 1 || qAbs(m_startPos.y() - m_endPos.y()) < 1)
    {
        return;
    }
    QPainterPath cPath;
    cPath.addRoundedRect(x - 1, y - 1, w + 2, h + 2, m_roundedRadius, m_roundedRadius);
    painter.setClipPath(cPath);

    painter.drawPixmap(x, y, m_pFullScreen.copy(x, y, w, h));

    painter.drawRoundedRect(x, y, w, h, m_roundedRadius, m_roundedRadius);
    painter.setClipping(false);
    painter.setBrush(Qt::white);
    pen.setWidth(1);
    for(const auto &p: opPoints)
    {
        painter.drawEllipse(p, m_opRadius, m_opRadius);
    }

    pen.setColor(Qt::black);
    int off = 0;
    if(y < 30)
    {
        off = 35;
    }
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(x + 5, y - 30 + off, 230, 25, 4, 4);
    painter.setPen(pen);
    painter.drawText(x + 10, y - 13 + off, tr("位置( %1 x %2 )  大小：( %3 x %4 )").arg(x).arg(y).arg(w).arg(h));
}

void ScreenCaptureWgt::showEvent(QShowEvent *)
{
    QPoint point(-1, -1);
    m_startPos = point;
    m_endPos = point;
    QScreen *pscreen = QApplication::primaryScreen();
    m_pFullScreen = pscreen->grabWindow(0, 0, 0, m_maxRect.width(), m_maxRect.height());


    //设置透明度实现模糊背景
    QPixmap pix(m_maxRect.width(), m_maxRect.height());
    pix.fill(QColor(160, 160, 160, 100));
    m_pBgScreen = QPixmap(m_pFullScreen);
    QPainter p(&m_pBgScreen);
    p.drawPixmap(0, 0, pix);
}

double ScreenCaptureWgt::distanceBetweenPoints(const QPoint &p1, const QPoint &p2)
{
    int dx = p1.x() - p2.x();
    int dy = p1.y() - p2.y();
    return sqrt(pow(dx, 2) + pow(dy, 2));
}

void ScreenCaptureWgt::calculateStatus(QPoint pos)
{
    if(m_status == SELECT)
    {
        this->setCursor(Qt::ArrowCursor);
        return;
    }
    if(m_status == DRAW)
    {
        this->setCursor(Qt::CrossCursor);
        return;
    }
    QRect rect = scRect();
    QPoint tlPos = rect.topLeft();
    QPoint trPos = rect.topRight();
    QPoint tPos = (tlPos + trPos)/2.0;
    QPoint blPos = rect.bottomLeft();
    QPoint brPos = rect.bottomRight();
    QPoint bPos = (blPos + brPos)/2.0;
    QPoint lPos = (blPos + tlPos)/2.0;
    QPoint rPos = (trPos + brPos)/2.0;
    QCursor cursor = Qt::ArrowCursor;
    m_status = NONE;
    if(distanceBetweenPoints(pos, tlPos) < m_opRadius)
    {
        m_status = TL;
        cursor = Qt::SizeFDiagCursor;
    }
    else if(distanceBetweenPoints(pos, trPos) < m_opRadius)
    {
        m_status = TR;
        cursor = Qt::SizeBDiagCursor;
    }
    else if(distanceBetweenPoints(pos, tPos) < m_opRadius)
    {
        m_status = TOP;
        cursor = Qt::SizeVerCursor;
    }
    else if(distanceBetweenPoints(pos, blPos) < m_opRadius)
    {
        m_status = BL;
        cursor = Qt::SizeBDiagCursor;
    }
    else if(distanceBetweenPoints(pos, brPos) < m_opRadius)
    {
        m_status = BR;
        cursor = Qt::SizeFDiagCursor;
    }
    else if(distanceBetweenPoints(pos, bPos) < m_opRadius)
    {
        m_status = BOTTON;
        cursor = Qt::SizeVerCursor;
    }
    else if(distanceBetweenPoints(pos, lPos) < m_opRadius)
    {
        m_status = LEFT;
        cursor = Qt::SizeHorCursor;
    }
    else if(distanceBetweenPoints(pos, rPos) < m_opRadius)
    {
        m_status = RIGHT;
        cursor = Qt::SizeHorCursor;
    }
    else if(rect.contains(pos))
    {
        m_status = CENTER;
        cursor = Qt::SizeAllCursor;
    }
    this->setCursor(cursor);
}

QRect ScreenCaptureWgt::scRect()
{
    return QRect(QPoint(qMin(m_startPos.x(), m_endPos.x()), qMin(m_startPos.y(), m_endPos.y())),
               QPoint(qMax(m_startPos.x(), m_endPos.x()), qMax(m_startPos.y(), m_endPos.y())));
}

void ScreenCaptureWgt::onOk()
{
    QRect rect = scRect();
    int x = rect.x();
    int y = rect.y();
    int w = rect.width();
    int h = rect.height();
    // 创建一个新的 QPixmap 对象，尺寸与 rect 相同
    QPixmap roundedRectPixmap(w, h);
    roundedRectPixmap.fill(Qt::transparent); // 填充透明背景

    // 使用 QPainter 在新的 QPixmap 上绘制圆角矩形
    QPainter painter(&roundedRectPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    QPainterPath path;
    path.addRoundedRect(QRect(0, 0, w, h), m_roundedRadius, m_roundedRadius); // 添加圆角矩形，半径为 20
    painter.setClipPath(path); // 设置裁剪路径

    // 将原始 QPixmap 的指定区域复制到新的 QPixmap 上
    painter.drawPixmap(0, 0, m_pFullScreen.copy(x, y, w, h));
    painter.setClipping(false); // 关闭裁剪

    // 完成绘制，m_currPixmap 现在包含了圆角效果的区域
    m_currPixmap = roundedRectPixmap;

    // m_currPixmap = m_pFullScreen.copy(x, y, w, h);

    // 获取剪贴板对象
    QClipboard *clipboard = QApplication::clipboard();
    // 将图片设置到剪贴板
    clipboard->setPixmap(m_currPixmap);
    this->accept();
}

void ScreenCaptureWgt::onClose()
{
    this->reject();
}

void ScreenCaptureWgt::mouseMoveEvent(QMouseEvent *e)
{
    if(!m_isPress)
    {
        calculateStatus(e->pos());
        return;
    }
    QPoint pos = e->pos();
    QRect rect = scRect();
    QPoint offset(pos.x() - m_currPos.x(), pos.y() - m_currPos.y());
    switch (m_status) {
    case LEFT:
    {
        m_startPos = rect.topLeft();
        m_endPos = rect.bottomRight();
        m_startPos.setX(m_startPos.x() + offset.x());
        if(m_startPos.x() > m_endPos.x())
        {
            m_status = RIGHT;
        }
    }
        break;
    case RIGHT:
    {
        m_startPos = rect.topLeft();
        m_endPos = rect.bottomRight();
        m_endPos.setX(m_endPos.x() + offset.x());
        if(m_endPos.x() < m_startPos.x())
        {
            m_status = LEFT;
        }
    }
        break;
    case TOP:
    {
        m_startPos = rect.topLeft();
        m_endPos = rect.bottomRight();
        m_startPos.setY(m_startPos.y() + offset.y());
        if(m_startPos.y() > m_endPos.y())
        {
            m_status = BOTTON;
        }
    }
        break;
    case BOTTON:
    {
        m_startPos = rect.topLeft();
        m_endPos = rect.bottomRight();
        m_endPos.setY(m_endPos.y() + offset.y());
        if(m_endPos.y() < m_startPos.y())
        {
            m_status = TOP;
        }
    }
        break;
    case TR:
    {
        this->setCursor(Qt::SizeBDiagCursor);
        m_startPos = rect.topLeft();
        m_endPos = rect.bottomRight();
        m_startPos.setY(m_startPos.y() + offset.y());
        m_endPos.setX(m_endPos.x() + offset.x());
        if(m_startPos.x() > m_endPos.x() && m_startPos.y() > m_endPos.y())
        {
            m_status = BL;
        }
        else if(m_startPos.x() > m_endPos.x())
        {
            m_status = TL;
        }
        else if(m_startPos.y() > m_endPos.y())
        {
            m_status = BR;
        }
    }
        break;
    case TL:
    {
        this->setCursor(Qt::SizeFDiagCursor);
        m_startPos = rect.topLeft();
        m_endPos = rect.bottomRight();
        m_startPos.setY(m_startPos.y() + offset.y());
        m_startPos.setX(m_startPos.x() + offset.x());
        if(m_startPos.x() > m_endPos.x() && m_startPos.y() > m_endPos.y())
        {
            m_status = BR;
        }
        else if(m_startPos.x() > m_endPos.x())
        {
            m_status = TR;
        }
        else if(m_startPos.y() > m_endPos.y())
        {
            m_status = BL;
        }
    }
        break;
    case BR:
    {
        this->setCursor(Qt::SizeFDiagCursor);
        m_startPos = rect.topLeft();
        m_endPos = rect.bottomRight();
        m_endPos.setY(m_endPos.y() + offset.y());
        m_endPos.setX(m_endPos.x() + offset.x());
        if(m_startPos.x() > m_endPos.x() && m_startPos.y() > m_endPos.y())
        {
            m_status = TL;
        }
        else if(m_startPos.x() > m_endPos.x())
        {
            m_status = BL;
        }
        else if(m_startPos.y() > m_endPos.y())
        {
            m_status = TR;
        }
    }
        break;
    case BL:
    {
        this->setCursor(Qt::SizeBDiagCursor);
        m_startPos = rect.topLeft();
        m_endPos = rect.bottomRight();
        m_startPos.setX(m_startPos.x() + offset.x());
        m_endPos.setY(m_endPos.y() + offset.y());
        if(m_startPos.x() > m_endPos.x() && m_startPos.y() > m_endPos.y())
        {
            m_status = TR;
        }
        else if(m_startPos.x() > m_endPos.x())
        {
            m_status = BR;
        }
        else if(m_startPos.y() > m_endPos.y())
        {
            m_status = TL;
        }
    }
        break;
    case SELECT:
        m_endPos = pos;
        break;
    case CENTER:
    {
        int sx = m_startPos.x() + offset.x();
        int ex = m_endPos.x() + offset.x();
        int sy = m_startPos.y() + offset.y();
        int ey = m_endPos.y() + offset.y();
        if(sx >= 0 && sx <= m_maxRect.width()
            && ex >= 0 && ex <= m_maxRect.width())
        {
            m_startPos.setX(sx);
            m_endPos.setX(ex);
        }
        if(sy >= 0 && sy <= m_maxRect.height()
            && ey >= 0 && ey <= m_maxRect.height())
        {
            m_startPos.setY(sy);
            m_endPos.setY(ey);
        }
    }
        break;
    default:
        break;
    }
    if(m_startPos.x() < 0)
    {
        m_startPos.setX(0);
    }
    if(m_endPos.x() < 0)
    {
        m_endPos.setX(0);
    }
    if(m_startPos.y() < 0)
    {
        m_startPos.setY(0);
    }
    if(m_endPos.y() < 0)
    {
        m_endPos.setY(0);
    }
    if(m_startPos.x() > m_maxRect.width())
    {
        m_startPos.setX(m_maxRect.width());
    }
    if(m_endPos.x() > m_maxRect.width())
    {
        m_endPos.setX(m_maxRect.width());
    }
    if(m_startPos.y() > m_maxRect.height())
    {
        m_startPos.setY(m_maxRect.height());
    }
    if(m_endPos.y() > m_maxRect.height())
    {
        m_endPos.setY(m_maxRect.height());
    }
    m_currPos = pos;
    this->update();
}

void ScreenCaptureWgt::mousePressEvent(QMouseEvent *e)
{
    if(e->button() != Qt::LeftButton)
    {
        this->reject();
        return;
    }

    if(m_pMenu->geometry().contains(e->pos()))
    {
        return;
    }
    m_isPress = true;
    m_pressPos = e->pos();
    m_currPos = m_pressPos;
    m_pMenu->move(10000, 10000);
    m_pMenu->hide();
    if (m_status == SELECT)
    {
        m_startPos = m_pressPos;
        m_endPos = m_pressPos;
    }

    this->update();
}

void ScreenCaptureWgt::mouseReleaseEvent(QMouseEvent *)
{
    m_isPress = false;
    if (m_status == SELECT) {
        m_status = NONE;
    }
    m_pMenu->show();
    QPoint pos = scRect().bottomRight() + QPoint(-10, 10);
    pos.setX(pos.x() - m_pMenu->width());
    if(pos.x() < 5)
    {
        pos.setX(scRect().topLeft().x() + 10);
    }
    if(pos.y() > m_maxRect.height() - m_pMenu->height() - 5)
    {
        pos.setY(scRect().bottomRight().y() - m_pMenu->height() - 10);
    }
    m_pMenu->move(pos);
}
