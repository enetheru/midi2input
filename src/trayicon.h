
#ifndef M2I_TRAYICON_H
#define M2I_TRAYICON_H
#include <QSystemTrayIcon>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class m2iTrayIcon : public QDialog
{
    Q_OBJECT

public:
    m2iTrayIcon();

    void setVisible(bool visible) override;

private slots:
    void quit();

private:
    void createActions();
    void createTrayIcon();

    QAction *quitAction{nullptr};
    QSystemTrayIcon *trayIcon{nullptr};
    QMenu *trayIconMenu{nullptr};
};

#endif//M2I_TRAYICON_H
