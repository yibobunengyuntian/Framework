#ifndef SCREENCAPTUREWGT_H
#define SCREENCAPTUREWGT_H

#include <QDialog>
#include <QMenu>
#include <QPoint>
#include <QSize>
#include <QApplication>
#include <QPainter>
#include <QScreen>
#include <QMouseEvent>
#include <QMutexLocker>
#include <QFileDialog>
#include <QClipboard>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPainterPath>

class ScreenCaptureWgt : public QDialog
{
    Q_OBJECT
public:
    explicit ScreenCaptureWgt(QWidget *parent = nullptr);

    QPixmap pixmap();

protected:
    enum STATUS {NONE,SELECT, DRAW, CENTER, TOP, LEFT, RIGHT, BOTTON, TL, TR, BL, BR};
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);
    double distanceBetweenPoints(const QPoint &p1, const QPoint &p2);
    void calculateStatus(QPoint pos);
    QRect scRect();

protected slots:
    void onOk();
    void onClose();

private:

    QPixmap m_pFullScreen;    //保存全屏图像
    QPixmap m_pBgScreen;      //模糊背景图
    QPixmap m_currPixmap;
    QPoint m_startPos;
    QPoint m_endPos;
    QRect m_maxRect;
    STATUS m_status = SELECT;

    QWidget *m_pMenu = nullptr;
    QPushButton *m_pBtnOk = nullptr;
    QPushButton *m_pBtnClose = nullptr;

    bool m_isPress = false;
    QPoint m_pressPos;
    QPoint m_currPos;
    float m_roundedRadius = 0; // 圆角半径
    int m_opRadius = 5;
};

#endif // SCREENCAPTUREWGT_H
