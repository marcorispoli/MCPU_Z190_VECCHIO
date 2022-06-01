#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class startupWindow; }
QT_END_NAMESPACE


class startupWindow : public QWidget
{
    Q_OBJECT

public:
    startupWindow( QWidget *parent = nullptr);

    ~startupWindow();

    void initWindow(void); //!< Override function for the opening activities
    void exitWindow(void); //!< Override function for the exiting activities


public slots:

    void timerEvent(QTimerEvent* ev);

private:
   Ui::startupWindow *ui;
   int timeEv;


};


#endif // STARTUPWINDOW_H
