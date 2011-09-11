/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2010-2011  nowrep
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "desktopnotification.h"
#include "ui_desktopnotification.h"

DesktopNotification::DesktopNotification(bool setPosition)
   : QWidget(0)
   , ui(new Ui::DesktopNotification)
   , m_settingPosition(setPosition)
   , m_timeout(6000)
   , m_timer(new QTimer(this))
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    Qt::WindowFlags flags = Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint;
  #ifdef Q_WS_WIN
    flags |= Qt::ToolTip;
  #endif
    setWindowFlags(flags);
    setWindowOpacity(0.9);

    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(close()));

    if (m_settingPosition)
        setCursor(Qt::OpenHandCursor);
}

void DesktopNotification::show()
{
    ui->icon->setPixmap(m_icon);
    ui->heading->setText(m_heading);
    ui->text->setText(m_text);

    if (!m_settingPosition) {
        m_timer->stop();
        m_timer->setInterval(m_timeout);
        m_timer->start();
    }

    QWidget::show();
}

void DesktopNotification::enterEvent(QEvent*)
{
    if (!m_settingPosition)
        setWindowOpacity(0.5);
}

void DesktopNotification::leaveEvent(QEvent*)
{
    if (!m_settingPosition)
        setWindowOpacity(0.9);
}

void DesktopNotification::mousePressEvent(QMouseEvent *e)
{
    if (!m_settingPosition) {
        close();
        return;
    }

    if (e->button() == Qt::LeftButton) {
        m_dragPosition = e->globalPos() - frameGeometry().topLeft();
        e->accept();
    }
}

void DesktopNotification::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton) {
        move(e->globalPos() - m_dragPosition);
        e->accept();
    }
}

DesktopNotification::~DesktopNotification()
{
    delete ui;
}
