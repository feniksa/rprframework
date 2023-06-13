#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "GLWidget.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_centralWidget = new GLWidget(this);
    m_timer = new QTimer(this);
    m_timer->setInterval(10);

    connect(ui->actionQuit, SIGNAL(triggered(bool)), SLOT(close()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), SLOT(about()));

    setWindowIcon(QIcon(":/images/logo.ico"));
    setCentralWidget(m_centralWidget);

    readSettings();

    connect(m_timer, &QTimer::timeout, m_centralWidget, QOverload<>::of(&QWidget::update));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateIntervalChanged(int value)
{
    m_timer->setInterval(value);
    if (m_timer->isActive())
        m_timer->start();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    //m_centralWidget->setMinimumSize(size() + QSize(128, 128));
}

void MainWindow::timerUsageChanged(bool enabled)
{
    if (enabled) {
        m_timer->start();
    } else {
        m_timer->stop();
        /*for (QOpenGLWidget *w : std::as_const(m_glWidgets))
            w->update();*/
        m_centralWidget->update();
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"), tr("<h2>RPR Framework Viewer</h2>"
                             "<p>Copyright &copy; 2023 Maksym Sditanov</p>"
                             "Render scenes with ProRender"));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    event->accept();
}

void MainWindow::readSettings()
{
    QSettings settings("rprframework", "Viewer");

    restoreGeometry(settings.value("geometry").toByteArray());
}

void MainWindow::writeSettings()
{
    QSettings settings("rprframework", "Viewer");

    settings.setValue("geometry", saveGeometry());
}
