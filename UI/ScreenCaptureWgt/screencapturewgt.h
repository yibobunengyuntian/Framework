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

class ScreenCaptureWgt : public QDialog
{
    Q_OBJECT
public:
    enum STATUS {SELECT, MOV, SET_W_H};
    explicit ScreenCaptureWgt(QWidget *parent = nullptr);

    QPixmap pixmap();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);

    QRect scRect();

protected slots:
    void onOk();
    void onClose();

private:
    QPixmap m_pFullScreen;    //保存全屏图像
    QPixmap m_pBgScreen;      //模糊背景图
    QPixmap m_currPixmap;
    QPoint m_movPos;          //坐标
    QPoint m_stratPos;
    QPoint m_endPos;
    QRect m_maxRect;
    STATUS m_status = SELECT;

    QWidget *m_pMenu = nullptr;
    QPushButton *m_pBtnOk = nullptr;
    QPushButton *m_pBtnClose = nullptr;
};

#endif // SCREENCAPTUREWGT_H
