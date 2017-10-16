
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

private:
    void createActions();
    void createTrayIcon();

    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif//M2I_TRAYICON_H
