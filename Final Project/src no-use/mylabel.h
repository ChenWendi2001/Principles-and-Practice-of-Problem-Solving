#pragma once

#include <QLabel>


enum EmDirection
{
    DIR_TOP = 0,
    DIR_BOTTOM,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_LEFTTOP,
    DIR_LEFTBOTTOM,
    DIR_RIGHTTOP,
    DIR_RIGHTBOTTOM,
    DIR_MIDDLE,
    DIR_NONE
};

#define EDGPADDING       5
#define CORPADDING       6

#define MIN_WIDTH        5
#define MIN_HEIGHT       5

#define POINT_WIDTH      6
#define POINT_HEIGHT     6

#define EDGE_WIDTH       2
#define MIDDLELINE_WIDTH 2

//#define DRAW_TEN_POINT
//#define DRAW_SUB_LINE

class QMenu;
class QAction;

class MyLabel : public QLabel
{
    Q_OBJECT

public:
    MyLabel(QWidget *parent = nullptr);
    ~MyLabel();
    QRect getRoiRect() const;
    void setBackImage(const QImage &img);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void keyPressEvent(QKeyEvent *ev);
    void contextMenuEvent(QContextMenuEvent *ev);

private:
    void initViewer();
    void saveROIImage();
    EmDirection region(const QPoint &point);
    void scaleRect(const QPoint &mousePoint);
    void paintRect(const QPoint &mousePoint);
    void moveRect(const QPoint &mousePoint);

private:
    bool m_bPainterPressed;
    bool m_bMovedPressed;
    bool m_bScalePressed;
    QPoint m_paintStartPoint;
    QPoint m_moveStartPoint;
    QRect m_roiRect;
    EmDirection m_emCurDir;

    QImage m_backImage;

    QMenu *m_pOptMenu;
    QAction *m_pDelAction;
    QAction *m_pSaveAction;
};
