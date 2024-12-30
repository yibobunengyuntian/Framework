#include "screencapturewgt.h"


ScreenCaptureWgt::ScreenCaptureWgt(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(Qt::Tool /*| Qt::WindowStaysOnTopHint*/ | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    m_maxRect = QGuiApplication::primaryScreen()->geometry();

    m_pMenu = new QWidget(this);
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

    QPainter painter(this);

    QPen pen;
    pen.setColor(QColor(0x0099ff));
    pen.setWidth(2);
    // pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawPixmap(0, 0, m_pBgScreen);

    if (w > 1 && h > 1)
    {
        painter.drawPixmap(x, y, m_pFullScreen.copy(x, y, w, h));
    }

    painter.drawRect(x, y, w, h);

    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawText(x + 2, y - 8, tr("位置( %1 x %2 )  大小：( %3 x %4 )").arg(x).arg(y).arg(w).arg(h));
}

void ScreenCaptureWgt::showEvent(QShowEvent *)
{
    QPoint point(-1, -1);
    m_stratPos = point;
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

QRect ScreenCaptureWgt::scRect()
{
    return QRect(QPoint(qMin(m_stratPos.x(), m_endPos.x()), qMin(m_stratPos.y(), m_endPos.y())),
               QPoint(qMax(m_stratPos.x(), m_endPos.x()), qMax(m_stratPos.y(), m_endPos.y())));
}

void ScreenCaptureWgt::onOk()
{
    QRect rect = scRect();
    int x = rect.x();
    int y = rect.y();
    int w = rect.width();
    int h = rect.height();

    m_currPixmap = m_pFullScreen.copy(x, y, w, h);

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
    if (m_status == SELECT)
    {
        m_endPos = e->pos();
    }
    else if (m_status == MOV)
    {
        QPoint p(e->x() - m_movPos.x(), e->y() - m_movPos.y());
        int sx = m_stratPos.x() + p.x();
        int ex = m_endPos.x() + p.x();
        int sy = m_stratPos.y() + p.y();
        int ey = m_endPos.y() + p.y();
        if(sx >= 0 && sx <= m_maxRect.width()
            && ex >= 0 && ex <= m_maxRect.width())
        {
            m_stratPos.setX(sx);
            m_endPos.setX(ex);
        }
        if(sy >= 0 && sy <= m_maxRect.height()
            && ey >= 0 && ey <= m_maxRect.height())
        {
            m_stratPos.setY(sy);
            m_endPos.setY(ey);
        }
        m_movPos = e->pos();
    }

    this->update();
}

void ScreenCaptureWgt::mousePressEvent(QMouseEvent *e)
{
    if(e->button() != Qt::LeftButton)
    {
        this->reject();
        return;
    }
    m_pMenu->hide();
    if (m_status == SELECT)
    {
        m_stratPos = e->pos();
        m_endPos = e->pos();
    }
    else if (m_status == MOV)
    {
        QRect rect = scRect();
        if (!rect.contains(e->pos()))
        {
            m_stratPos = e->pos();
            m_status = SELECT;
        }
        else
        {
            m_movPos = e->pos();
            this->setCursor(Qt::SizeAllCursor);
        }
    }

    this->update();
}

void ScreenCaptureWgt::mouseReleaseEvent(QMouseEvent *)
{
    if (m_status == SELECT) {
        m_status = MOV;
    } else if (m_status == MOV) {
        this->setCursor(Qt::ArrowCursor);
    }
    m_pMenu->show();
    QPoint pos = scRect().bottomRight() + QPoint(0, 10);
    pos.setX(pos.x() - m_pMenu->width());
    if(pos.x() < 0)
    {
        pos.setX(0);
    }
    if(pos.x() > m_maxRect.width() - m_pMenu->width())
    {
        pos.setX(m_maxRect.width() - m_pMenu->width());
    }
    if(pos.y() < 0)
    {
        pos.setY(0);
    }
    if(pos.y() > m_maxRect.height() - m_pMenu->height())
    {
        pos.setY(m_maxRect.height() - m_pMenu->height());
    }
    m_pMenu->move(pos);
}
