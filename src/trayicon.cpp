#include <QMenu>
#include <QSystemTrayIcon>
#include <QCoreApplication>
#include "trayicon.h"

namespace m2i {
    extern bool quit;
}

m2iTrayIcon::m2iTrayIcon()
{
    createActions();
    createTrayIcon();

    trayIcon->show();

    setWindowTitle(tr("About m2i"));
    resize(400, 300);
}

void 
m2iTrayIcon::setVisible( bool visible )
{
    QDialog::setVisible( visible );
}

void
m2iTrayIcon::createActions()
{
    quitAction = new QAction(tr("&Quit"), this);
    // FIXME instead of qt quit, i just need it to end the event loop.
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void
m2iTrayIcon::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction( quitAction );

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    QIcon::setThemeName( "gnome" );
    trayIcon->setIcon( QIcon::fromTheme("emblem-new") );//FIXME needs better icon
}
