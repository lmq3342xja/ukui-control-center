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
#include "desktop.h"
#include "ui_desktop.h"

#include <QGSettings/QGSettings>

#include "SwitchButton/switchbutton.h"

#include <QDebug>

//#define DESKTOP_SCHEMA "org.ukui.peony.desktop"
#define DESKTOP_SCHEMA "org.ukui.control-center.desktop"

#define COMPUTER_VISIBLE_KEY "computer-icon-visible"
#define HOME_VISIBLE_KEY "home-icon-visible"
#define NETWORK_VISIBLE_KEY "network-icon-visible"
#define TRASH_VISIBLE_KEY "trash-icon-visible"
#define VOLUMES_VISIBLE_KEY "volumes-visible"

#define COMPUTER_LOCK_KEY "computer-icon-locking"
#define FILESYSTEM_LOCK_KEY "filesystem-icon-locking"
#define SETTINGS_LOCK_KEY "settings-icon-locking"
#define TRASH_LOCK_KEY "trash-icon-locking"

Desktop::Desktop()
{
    ui = new Ui::Desktop;
    pluginWidget = new QWidget;
    pluginWidget->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(pluginWidget);

    pluginName = tr("desktop");
    pluginType = PERSONALIZED;

    pluginWidget->setStyleSheet("background: #ffffff;");

    ui->deskComputerWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px;}");
    ui->deskTrashWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px;}");
    ui->deskHomeWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px;}");
    ui->deskVolumeWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px;}");
    ui->deskNetworkWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px;}");

    ui->menuComputerWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px;}");
    ui->menuTrashWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px;}");
    ui->menuFilesystemWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px;}");
    ui->menuSettingWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px;}");


    const QByteArray id(DESKTOP_SCHEMA);
    dSettings = new QGSettings(id);

    setupComponent();
    setupConnect();
    initVisibleStatus();
    initLockingStatus();
}

Desktop::~Desktop()
{
    delete ui;

    delete dSettings;
}

QString Desktop::get_plugin_name(){
    return pluginName;
}

int Desktop::get_plugin_type(){
    return pluginType;
}

QWidget *Desktop::get_plugin_ui(){
    return pluginWidget;
}

void Desktop::plugin_delay_control(){

}

void Desktop::setupComponent(){

    ui->deskComputerLabel->setPixmap(QPixmap("://img/plugins/desktop/computer.png"));
    ui->deskHomeLabel->setPixmap(QPixmap("://img/plugins/desktop/homefolder.png"));
    ui->deskTrashLabel->setPixmap(QPixmap("://img/plugins/desktop/trash.png"));
    ui->deskNetworkLabel->setPixmap(QPixmap("://img/plugins/desktop/default.png"));
    ui->deskVolumeLabel->setPixmap(QPixmap("://img/plugins/desktop/default.png"));


    deskComputerSwitchBtn = new SwitchButton(pluginWidget);
    ui->deskComputerHorLayout->addWidget(deskComputerSwitchBtn);

    deskTrashSwitchBtn = new SwitchButton(pluginWidget);
    ui->deskTrashHorLayout->addWidget(deskTrashSwitchBtn);


    deskHomeSwitchBtn = new SwitchButton(pluginWidget);
    ui->deskHomeHorLayout->addWidget(deskHomeSwitchBtn);

    deskVolumeSwitchBtn = new SwitchButton(pluginWidget);
    ui->deskVolumeHorLayout->addWidget(deskVolumeSwitchBtn);

    deskNetworkSwitchBtn = new SwitchButton(pluginWidget);
    ui->deskNetworkHorLayout->addWidget(deskNetworkSwitchBtn);

    ui->menuComputerLabel->setPixmap(QPixmap("://img/plugins/desktop/computer.png"));
    ui->menuFilesystemLabel->setPixmap(QPixmap("://img/plugins/desktop/default.png"));
    ui->menuSettingsLabel->setPixmap(QPixmap("://img/plugins/desktop/default.png"));
    ui->menuTrashLabel->setPixmap(QPixmap("://img/plugins/desktop/trash.png"));

    menuComputerSwitchBtn = new SwitchButton(pluginWidget);
    ui->menuComputerHorLayout->addWidget(menuComputerSwitchBtn);

    menuTrashSwitchBtn = new SwitchButton(pluginWidget);
    ui->menuTrashHorLayout->addWidget(menuTrashSwitchBtn);

    menuFilesystemSwitchBtn = new SwitchButton(pluginWidget);
    ui->menuFilesystemHorLayout->addWidget(menuFilesystemSwitchBtn);

    menuSettingSwitchBtn = new SwitchButton(pluginWidget);
    ui->menuSettingHorLayout->addWidget(menuSettingSwitchBtn);

}

void Desktop::setupConnect(){
    connect(deskComputerSwitchBtn, &SwitchButton::checkedChanged, this, [=](bool checked){dSettings->set(COMPUTER_VISIBLE_KEY, checked);});
    connect(deskTrashSwitchBtn, &SwitchButton::checkedChanged, this, [=](bool checked){dSettings->set(TRASH_VISIBLE_KEY, checked);});
    connect(deskHomeSwitchBtn, &SwitchButton::checkedChanged, this, [=](bool checked){dSettings->set(HOME_VISIBLE_KEY, checked);});
    connect(deskVolumeSwitchBtn, &SwitchButton::checkedChanged, this, [=](bool checked){dSettings->set(VOLUMES_VISIBLE_KEY, checked);});
    connect(deskNetworkSwitchBtn, &SwitchButton::checkedChanged, this, [=](bool checked){dSettings->set(NETWORK_VISIBLE_KEY, checked);});

    connect(menuComputerSwitchBtn, &SwitchButton::checkedChanged, [=](bool checked){
        dSettings->set(COMPUTER_LOCK_KEY, checked);
    });
    connect(menuFilesystemSwitchBtn, &SwitchButton::checkedChanged, [=](bool checked){
        dSettings->set(FILESYSTEM_LOCK_KEY, checked);
    });
    connect(menuSettingSwitchBtn, &SwitchButton::checkedChanged, [=](bool checked){
        dSettings->set(SETTINGS_LOCK_KEY, checked);
    });
    connect(menuTrashSwitchBtn, &SwitchButton::checkedChanged, [=](bool checked){
        dSettings->set(TRASH_LOCK_KEY, checked);
    });
}

void Desktop::initVisibleStatus(){
    deskComputerSwitchBtn->blockSignals(true);
    deskHomeSwitchBtn->blockSignals(true);
    deskTrashSwitchBtn->blockSignals(true);
    deskVolumeSwitchBtn->blockSignals(true);
    deskNetworkSwitchBtn->blockSignals(true);

    deskComputerSwitchBtn->setChecked(dSettings->get(COMPUTER_VISIBLE_KEY).toBool());
    deskHomeSwitchBtn->setChecked(dSettings->get(HOME_VISIBLE_KEY).toBool());
    deskTrashSwitchBtn->setChecked(dSettings->get(TRASH_VISIBLE_KEY).toBool());
    deskVolumeSwitchBtn->setChecked(dSettings->get(VOLUMES_VISIBLE_KEY).toBool());
    deskNetworkSwitchBtn->setChecked(dSettings->get(NETWORK_VISIBLE_KEY).toBool());

    deskComputerSwitchBtn->blockSignals(false);
    deskHomeSwitchBtn->blockSignals(false);
    deskTrashSwitchBtn->blockSignals(false);
    deskVolumeSwitchBtn->blockSignals(false);
    deskNetworkSwitchBtn->blockSignals(false);
}

void Desktop::initLockingStatus(){
    menuComputerSwitchBtn->blockSignals(true);
    menuFilesystemSwitchBtn->blockSignals(true);
    menuSettingSwitchBtn->blockSignals(true);
    menuTrashSwitchBtn->blockSignals(true);

    menuComputerSwitchBtn->setChecked(dSettings->get(COMPUTER_LOCK_KEY).toBool());
    menuFilesystemSwitchBtn->setChecked(dSettings->get(FILESYSTEM_LOCK_KEY).toBool());
    menuSettingSwitchBtn->setChecked(dSettings->get(SETTINGS_LOCK_KEY).toBool());
    menuTrashSwitchBtn->setChecked(dSettings->get(TRASH_LOCK_KEY).toBool());

    menuComputerSwitchBtn->blockSignals(false);
    menuFilesystemSwitchBtn->blockSignals(false);
    menuSettingSwitchBtn->blockSignals(false);
    menuTrashSwitchBtn->blockSignals(false);
}
