/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#ifndef NETCONNECT_H
#define NETCONNECT_H


#include <QObject>
#include <QtPlugin>

#include <QNetworkInterface>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QtNetwork>

#include <QTimer>
#include <QStringList>
#include <QGSettings/qgsettings.h>

#include "shell/interface.h"


#include "SwitchButton/switchbutton.h"


enum {
    DISCONNECTED,
    NOINTERNET,
    CONNECTED
};

typedef enum {
    LOOPBACK,
    ETHERNET,
    WIFI
}NetType;

typedef struct _CardInfo{
    QString name;
    NetType type;
    bool status;
}CardInfo;

namespace Ui {
class NetConnect;
}


class NetConnect : public QObject, CommonInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kycc.CommonInterface")
    Q_INTERFACES(CommonInterface)

public:
    NetConnect();
    ~NetConnect();

    QString get_plugin_name() Q_DECL_OVERRIDE;
    int get_plugin_type() Q_DECL_OVERRIDE;
    QWidget * get_plugin_ui() Q_DECL_OVERRIDE;
    void plugin_delay_control() Q_DECL_OVERRIDE;

public:
    void initComponent();
    void getNetList();
    void rebuildNetStatusComponent();
    void rebuildAvailComponent(QString iconpath, QString netName);

    void _acquireCardInfo();
    void runExternalApp();

private:
    Ui::NetConnect *ui;

    QString pluginName;
    int pluginType;
    QWidget * pluginWidget;

    SwitchButton *wifiBtn;

    QNetworkConfigurationManager * nmg;

    QList<CardInfo> cardinfoQList;

    QMap<QString, int> connectedWifi;
    QMap<QString,int> wifiList;     //wifi list<name,Signal strength>
    QStringList lanList;            // list of wired network
    QString connectedLan;
    QGSettings *m_gsettings = nullptr;


private:
    int setSignal(QString lv); //get wifi's strength
    QStringList execGetLanList();
    QStringList execGetWifiList();
    void getWifiListDone(QStringList wifislist, QStringList lanList);
    bool getSwitchStatus(QString key);

private slots:
    void wifiSwitchSlot(bool signal);



};

#endif // NETCONNECT_H
