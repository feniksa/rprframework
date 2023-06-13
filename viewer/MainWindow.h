#pragma once

#include <QMainWindow>
#include <QTimer>

class GLWidget;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool timerEnabled() const { return m_timer->isActive(); }
    void resizeEvent(QResizeEvent *);

protected:
    void closeEvent(QCloseEvent*);

private slots:
    void about();
    void updateIntervalChanged(int value);
    void timerUsageChanged(bool enabled);
private:
    void readSettings();
    void writeSettings();

    QTimer *m_timer;
    Ui::MainWindow *ui;
    GLWidget* m_centralWidget;
};
