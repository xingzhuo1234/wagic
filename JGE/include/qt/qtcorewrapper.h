#ifndef QTCOREWRAPPER_H
#define QTCOREWRAPPER_H

#include "../include/corewrapper.h"

#include <QObject>
#include <QElapsedTimer>
#ifndef QT_WIDGET
#include <QtDeclarative>
#include <QGraphicsItem>
#endif //QT_WIDGET

#if (defined Q_WS_MAEMO_5)
// For screen on/off events support
#include <mce/dbus-names.h>
#include <mce/mode-names.h>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusInterface>
#endif //Q_WS_MAEMO_5


#ifdef QT_WIDGET
class QtWagicCore : public QGLWidget
#else
class QtWagicCore : public QDeclarativeItem
#endif
{
private:
#ifdef QT_WIDGET
    typedef QGLWidget super;
#else
  typedef QDeclarativeItem super;
#endif //QT_WIDGET

public:
    Q_OBJECT
    Q_PROPERTY(int nominalWidth READ getNominalWidth CONSTANT)
    Q_PROPERTY(int nominalHeight READ getNominalHeight CONSTANT)
    Q_PROPERTY(float nominalRatio READ getNominalRatio CONSTANT)
    Q_PROPERTY(bool active READ getActive WRITE setActive NOTIFY activeChanged)


public:
    explicit QtWagicCore(super *parent = 0);
    virtual ~QtWagicCore();

    Q_INVOKABLE void doOK() {
        m_Wagic.doOK();
    };
    Q_INVOKABLE void doNext() {
        m_Wagic.doNext();
    };
    Q_INVOKABLE void doCancel() {
        m_Wagic.doCancel();
    };
    Q_INVOKABLE void doMenu() {
        m_Wagic.doMenu();
    };
    Q_INVOKABLE void done() {
        m_Wagic.done();
    };
    Q_INVOKABLE void pixelInput(int x, int y);
    Q_INVOKABLE qint64 getTick() {
        return g_startTimer.elapsed();
    };
    Q_INVOKABLE void doScroll(int x, int y, int) {
        m_Wagic.doScroll(x, y);
    };
    int getNominalHeight(){ return SCREEN_HEIGHT;};
    int getNominalWidth(){ return SCREEN_WIDTH;};
    float getNominalRatio() { return ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);};
    bool getActive() { return m_active; };
    void setActive(bool active);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    static char* getApplicationName() {
        return JGameLauncher::GetName();
    };

#ifdef QT_WIDGET
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void tapAndHoldTriggered(QTapAndHoldGesture* gesture);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    bool gestureEvent(QGestureEvent* event);
    bool event(QEvent *event);
    void wheelEvent(QWheelEvent *event);
#else
    void wheelEvent ( QGraphicsSceneWheelEvent * event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
#endif

#ifdef QT_WIDGET
public slots:
    void start(int);
#endif

#ifdef Q_WS_MAEMO_5
public slots:
    void displayStateChanged(const QDBusMessage &message);
#endif //Q_WS_MAEMO_5


signals:
    void activeChanged();

private slots:

private:
    int lastPosx(){
#if QT_VERSION >= 0x050100
        return m_lastPos.x()*devicePixelRatio();
#else
        return m_lastPos.x();
#endif
    }
    int lastPosy(){
#if QT_VERSION >= 0x050100
        return m_lastPos.y()*devicePixelRatio();
#else
        return m_lastPos.y();
#endif
    }
    void timerEvent( QTimerEvent* );

public:
    // used mainly to mesure the delta between 2 updates
    static QElapsedTimer g_startTimer;
private:
    WagicCore m_Wagic;

    qint64 m_lastTickCount;
    int m_timerId;
    bool m_active;
    QRect m_viewPort;
    QPoint m_lastPos;
#ifdef QT_WIDGET
#if (defined Q_WS_MAEMO_5) || (defined MEEGO_EDITION_HARMATTAN) || (defined Q_WS_ANDROID)
  int mMouseDownX;
  int mMouseDownY;
  qint64 mLastFingerDownTime;
#endif //Q_WS_MAEMO_5
#endif //QT_WIDGET

#ifdef Q_WS_MAEMO_5
    QDBusConnection dBusConnection;
    QDBusInterface* dBusInterface;
 #endif //Q_WS_MAEMO_5
};
#ifndef QT_WIDGET
QML_DECLARE_TYPE(WagicCore)
#endif //QT_WIDGET

#endif // QTCOREWRAPPER_H
