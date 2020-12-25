#include "MyLabel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QStandardPaths>
#include <QFileDialog>
#include <QDebug>

MyLabel::MyLabel(QWidget *parent)
    : QLabel(parent)
{
    this->initViewer();
}

MyLabel::~MyLabel()
{
}

void MyLabel::initViewer()
{
    m_bPainterPressed = false;
    m_bMovedPressed = false;
    m_bScalePressed = false;
    m_roiRect = QRect(0, 0, 0, 0);
    m_emCurDir = EmDirection::DIR_NONE;

    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);

    m_pOptMenu = new QMenu(this);
    m_pDelAction = new QAction(QStringLiteral("ɾ��"), this);
    connect(m_pDelAction, &QAction::triggered, this, [&]() { m_roiRect = QRect(0, 0, 0, 0); });
    m_pSaveAction = new QAction(QStringLiteral("����"), this);
    connect(m_pSaveAction, &QAction::triggered, this, &MyLabel::saveROIImage);

    m_pOptMenu->addAction(m_pDelAction);
    m_pOptMenu->addAction(m_pSaveAction);
}


void MyLabel::saveROIImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), "*.png");
    if (fileName.isEmpty())
    {
        return;
    }

    QImage saveImg = m_backImage.copy(m_roiRect);
    saveImg.save(fileName);
}


QRect MyLabel::getRoiRect() const
{
    return m_roiRect;
}


void MyLabel::setBackImage(const QImage & img)
{
    m_backImage = img;
    this->setMinimumSize(img.width(), img.height());
    update();
}


void MyLabel::paintEvent(QPaintEvent * event)
{
#if 1
    QLabel::paintEvent(event);

    if (m_backImage.isNull())
        return;

    QPixmap rawImg = QPixmap::fromImage(m_backImage);
    QString strPoint = QString("X:%0, Y:%1").arg(m_roiRect.x()).arg(m_roiRect.y());
    QString strSize = QString("W:%0, H:%1").arg(m_roiRect.width()).arg(m_roiRect.height());

    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(EDGE_WIDTH);

    QFont font;
    font.setPixelSize(16);

    QPainter painter;
    painter.begin(this);
    painter.setBrush(QBrush(QColor(0, 0, 200, 0)));
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawPixmap(0, 0, rawImg);
    //painter.drawText(m_roiRect.topLeft().x(), m_roiRect.topLeft().y() - 5, strSize);
    //painter.drawText(m_roiRect.topLeft().x(), m_roiRect.topLeft().y() - 18, strPoint);
    painter.drawRect(m_roiRect);

    if (m_roiRect.width() != 0 && m_roiRect.height() != 0)
    {
#ifdef DRAW_SUB_LINE

        QPen dashPen(Qt::white);
        dashPen.setWidth(MIDDLELINE_WIDTH);
        dashPen.setStyle(Qt::DashDotLine);
        painter.setPen(dashPen);
        painter.drawLine(m_roiRect.topLeft().x() + m_roiRect.width() / 2, m_roiRect.topLeft().y() + EDGE_WIDTH, m_roiRect.bottomRight().x() - m_roiRect.width() / 2, m_roiRect.bottomRight().y());
        painter.drawLine(m_roiRect.topLeft().x() + EDGE_WIDTH, m_roiRect.topLeft().y() + m_roiRect.height() / 2, m_roiRect.bottomRight().x(), m_roiRect.bottomRight().y() - m_roiRect.height() / 2);
#endif

#ifdef DRAW_TEN_POINT

        painter.setBrush(Qt::green);
        pen.setWidth(0);
        painter.setPen(pen);

        painter.drawRect(m_roiRect.topLeft().x(), m_roiRect.topLeft().y(), POINT_WIDTH, POINT_HEIGHT); //���Ͻ�
        painter.drawRect(m_roiRect.topLeft().x(), m_roiRect.topLeft().y() + m_roiRect.height() / 2 - POINT_WIDTH / 2, POINT_WIDTH, POINT_HEIGHT); //������ĵ�
        painter.drawRect(m_roiRect.bottomLeft().x(), m_roiRect.bottomLeft().y()- POINT_WIDTH, POINT_WIDTH, POINT_HEIGHT); //���½�
        painter.drawRect(m_roiRect.topLeft().x() + m_roiRect.width() / 2 - POINT_WIDTH / 2, m_roiRect.topLeft().y(), POINT_WIDTH, POINT_HEIGHT);  //��������
        painter.drawRect(m_roiRect.topLeft().x() + m_roiRect.width() / 2 - POINT_WIDTH /2, m_roiRect.topLeft().y() + m_roiRect.height() / 2 - POINT_WIDTH / 2, POINT_WIDTH, POINT_HEIGHT);  //���ĵ�
        painter.drawRect(m_roiRect.bottomLeft().x() + m_roiRect.width() / 2 - POINT_WIDTH / 2, m_roiRect.bottomLeft().y() - POINT_WIDTH, POINT_WIDTH, POINT_HEIGHT); //�ײ����ĵ�
        painter.drawRect(m_roiRect.topRight().x() - POINT_WIDTH, m_roiRect.topRight().y(), POINT_WIDTH, POINT_HEIGHT); //���Ͻ�
        painter.drawRect(m_roiRect.topRight().x() - POINT_WIDTH / 2, m_roiRect.topRight().y() + m_roiRect.height() / 2 - POINT_WIDTH /2, POINT_WIDTH, POINT_HEIGHT); //�ұ����ĵ�
        painter.drawRect(m_roiRect.bottomRight().x() - POINT_WIDTH, m_roiRect.bottomRight().y() - POINT_WIDTH, POINT_WIDTH, POINT_HEIGHT); //���½ǵ�
#endif
    }


    painter.end();

    //qDebug() << m_roiRect;
#else
    QLabel::paintEvent(event);

    if (m_backImage.isNull())
        return;
    QPixmap rawImg = QPixmap::fromImage(m_backImage);
    QPainter painter(this);
    painter.begin(&rawImg);
    painter.setBrush(Qt::gray);
    painter.drawRect(30,30,100,100);
    painter.end();
    this->setPixmap(rawImg);
#endif
}


void MyLabel::mousePressEvent(QMouseEvent * ev)
{
    if (ev->buttons() & Qt::LeftButton)
    {
        EmDirection dir = region(ev->pos());

        if (dir == DIR_MIDDLE)
        {

            this->setCursor(Qt::ClosedHandCursor);
            m_moveStartPoint.setX(ev->pos().x());
            m_moveStartPoint.setY(ev->pos().y());
            m_bMovedPressed = true;
        }
        else if (dir == DIR_NONE)
        {

            this->setCursor(Qt::ArrowCursor);
            m_bPainterPressed = true;
            m_paintStartPoint.setX(ev->pos().x());
            m_paintStartPoint.setY(ev->pos().y());
        }
        else
        {

            m_moveStartPoint.setX(ev->pos().x());
            m_moveStartPoint.setY(ev->pos().y());
            m_bScalePressed = true;
            m_emCurDir = dir;
        }
    }
}


void MyLabel::mouseMoveEvent(QMouseEvent * ev)
{
    if (ev->buttons() & Qt::LeftButton)
    {
        if (m_bPainterPressed)
        {

            paintRect(ev->pos());

        }
        else if (m_bMovedPressed)
        {

            moveRect(ev->pos());
        }
        else if (m_bScalePressed)
        {

            scaleRect(ev->pos());
        }


        update();
        return;
    }


    EmDirection dir = region(ev->pos());

    if (dir == DIR_NONE)
    {
        setCursor(Qt::ArrowCursor);
    }
    else if (dir == DIR_MIDDLE)
    {
        setCursor(Qt::OpenHandCursor);
    }
}


void MyLabel::mouseReleaseEvent(QMouseEvent * ev)
{

    if (m_roiRect.contains(ev->pos()))
    {

        if (m_bMovedPressed)
        {
            this->setCursor(Qt::OpenHandCursor);
        }
        else
        {
            this->setCursor(Qt::ArrowCursor);
        }
    }

    m_paintStartPoint = QPoint();
    m_bMovedPressed = false;
    m_bPainterPressed = false;
    m_bScalePressed = false;
}


void MyLabel::keyPressEvent(QKeyEvent * ev)
{
    if (ev->key() == Qt::Key_Delete)
    {
        m_roiRect = QRect(0,0,0,0);
        update();
    }
}


void MyLabel::contextMenuEvent(QContextMenuEvent * ev)
{
    QPoint mousePos = ev->pos();

    if (m_roiRect.contains(mousePos))
    {
        m_pOptMenu->exec(QCursor::pos());
    }

    ev->accept();
}


EmDirection MyLabel::region(const QPoint & point)
{
    int mouseX = point.x();
    int mouseY = point.y();

    QPoint roiTopLeft = m_roiRect.topLeft();
    QPoint roiBottomRight = m_roiRect.bottomRight();

    EmDirection dir = DIR_NONE;

    if (mouseX <= roiTopLeft.x() + CORPADDING && mouseX >= roiTopLeft.x() && mouseY <= roiTopLeft.y() + CORPADDING && mouseY >= roiTopLeft.y())
    {
        this->setCursor(Qt::SizeFDiagCursor);
        dir = DIR_LEFTTOP;
    }
    else if (mouseX >= roiBottomRight.x() - CORPADDING && mouseX < roiBottomRight.x() && mouseY <= roiTopLeft.y() + CORPADDING && mouseY >= roiTopLeft.y())
    {

        this->setCursor(Qt::SizeBDiagCursor);
        dir = DIR_RIGHTTOP;
    }
    else if (mouseX <= roiTopLeft.x() + CORPADDING && mouseX >= roiTopLeft.x() && mouseY >= roiBottomRight.y() - CORPADDING && mouseY <= roiBottomRight.y())
    {

        this->setCursor(Qt::SizeBDiagCursor);
        dir = DIR_LEFTBOTTOM;
    }
    else if (mouseX >= roiBottomRight.x() - CORPADDING && mouseX < roiBottomRight.x() && mouseY >= roiBottomRight.y() - CORPADDING && mouseY <= roiBottomRight.y())
    {

        this->setCursor(Qt::SizeFDiagCursor);
        dir = DIR_RIGHTBOTTOM;
    }
    else if (mouseX >= roiBottomRight.x() - EDGPADDING && mouseX <= roiBottomRight.x() && mouseY >= roiTopLeft.y() && mouseY <= roiBottomRight.y())
    {

        this->setCursor(Qt::SizeHorCursor);
        dir = DIR_RIGHT;
    }
    else if (mouseY <= roiTopLeft.y() + EDGPADDING && mouseY >= roiTopLeft.y() && mouseX >= roiTopLeft.x() && mouseX <= roiBottomRight.x())
    {

        this->setCursor(Qt::SizeVerCursor);
        dir = DIR_TOP;
    }
    else if (mouseY >= roiBottomRight.y() - EDGPADDING && mouseY <= roiBottomRight.y() && mouseX >= roiTopLeft.x() && mouseX <= roiBottomRight.x())
    {

        this->setCursor(Qt::SizeVerCursor);
        dir = DIR_BOTTOM;
    }
    else if (mouseX <= roiTopLeft.x() + EDGPADDING && mouseX >= roiTopLeft.x() && mouseY >= roiTopLeft.y() && mouseY <= roiBottomRight.y())
    {

        this->setCursor(Qt::SizeHorCursor);
        dir = DIR_LEFT;
    }
    else if(m_roiRect.contains(point))
    {

        dir = DIR_MIDDLE;
    }
    else
    {
        dir = DIR_NONE;
    }

    return dir;
}


void MyLabel::scaleRect(const QPoint & mousePoint)
{
    QRect newRect(m_roiRect.topLeft(), m_roiRect.bottomRight());
    int width = mousePoint.x() - m_moveStartPoint.x();
    int height = mousePoint.y() - m_moveStartPoint.y();


    switch (m_emCurDir)
    {
    case DIR_LEFT:
        newRect.setLeft(mousePoint.x());
        break;
    case DIR_RIGHT:
        newRect.setRight(mousePoint.x());
        break;
    case DIR_TOP:
        newRect.setTop(mousePoint.y());
        break;
    case DIR_BOTTOM:
        newRect.setBottom(mousePoint.y());
        break;
    case DIR_LEFTTOP:
        newRect.setTopLeft(mousePoint);
        break;
    case DIR_LEFTBOTTOM:
        newRect.setBottomLeft(mousePoint);
        break;
    case DIR_RIGHTTOP:
        newRect.setTopRight(mousePoint);
        break;
    case DIR_RIGHTBOTTOM:
        newRect.setBottomRight(mousePoint);
        break;
    }

    if (newRect.width() < MIN_WIDTH || newRect.height() < MIN_HEIGHT)
    {

        return;
    }

    m_roiRect = newRect;
    m_moveStartPoint = mousePoint;
}


void MyLabel::paintRect(const QPoint & mousePoint)
{
    this->setCursor(Qt::ArrowCursor);

    int width = mousePoint.x() - m_paintStartPoint.x();
    int height = mousePoint.y() - m_paintStartPoint.y();

    if (width < 0 && height < 0)
    {

        m_roiRect.setX(mousePoint.x());
        m_roiRect.setY(mousePoint.y());
    }
    else if (width < 0)
    {

        m_roiRect.setX(mousePoint.x());
        m_roiRect.setY(m_paintStartPoint.y());
    }
    else if (height < 0)
    {

        m_roiRect.setX(m_paintStartPoint.x());
        m_roiRect.setY(mousePoint.y());
    }
    else
    {

        m_roiRect.setX(m_paintStartPoint.x());
        m_roiRect.setY(m_paintStartPoint.y());
    }


    m_roiRect.setSize(QSize(abs(width), abs(height)));
}


void MyLabel::moveRect(const QPoint & mousePoint)
{
    this->setCursor(Qt::ClosedHandCursor);

    int width = mousePoint.x() - m_moveStartPoint.x();
    int height = mousePoint.y() - m_moveStartPoint.y();

    QRect ret;
    ret.setX(m_roiRect.x() + width);
    ret.setY(m_roiRect.y() + height);
    ret.setSize(m_roiRect.size());
    m_roiRect = ret;
    m_moveStartPoint = mousePoint;
}
