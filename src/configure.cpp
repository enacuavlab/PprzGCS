#include "configure.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <iostream>
#include <QDir>
#include <QVariant>
#include <QStandardPaths>
#include <QSettings>
#include <QtWidgets>
#include <QFileInfo>
#include <pprzmain.h>

void default_setting(QString key, QVariant value) {
    QSettings settings(qApp->property("SETTINGS_PATH").toString(), QSettings::IniFormat);

    if(!settings.contains(key)) {
        settings.setValue(key, value);
    }
}

void configure() {
    QSettings settings(qApp->property("SETTINGS_PATH").toString(), QSettings::IniFormat);

    default_setting("ivy/name", "PprzGCS");
    default_setting("pprzlink/id", "pprzcontrol");
    default_setting("ivy/bus", "127.255.255.255:2010");

    default_setting("PAPARAZZI_HOME", QString(qgetenv("PAPARAZZI_HOME")));
    default_setting("PAPARAZZI_SRC", QString(qgetenv("PAPARAZZI_SRC")));

    default_setting("pprzlink/messages", settings.value("PAPARAZZI_HOME").toString() + "/var/messages.xml");
    default_setting("path/gcs_icons", settings.value("PAPARAZZI_HOME").toString() + "/data/pictures/gcs_icons");
    default_setting("path/aircraft_icon", settings.value("APP_DATA_PATH").toString() + "/pictures/aircraft_icons");
    default_setting("map/default_tiles", "Google");

    default_setting("map/move_hyteresis", 20);
    default_setting("map/waypoint/size", 8);
    default_setting("map/circle/stroke", 4);
    default_setting("map/size_highlight_factor", 1.5);

    default_setting("map/z_values/unhighlighted", 100);
    default_setting("map/z_values/highlighted", 200);
    default_setting("map/z_values/aircraft", 400);
    default_setting("map/z_values/carrot", 500);
    default_setting("map/z_values/nav_shape", 600);

    default_setting("map/items_font", 18);
    default_setting("map/aircraft/size", 40);

    default_setting("map/tiles_path", settings.value("USER_DATA_PATH").toString() + "/map");


    default_setting("map/aircraft/track_max_chunk", 10);
    default_setting("map/aircraft/track_chunk_size", 20);

    default_setting("aircraft_default_color", "red");

    default_setting("APP_STYLE_FILE", settings.value("APP_DATA_PATH").toString() + "/default_style.qss");
    default_setting("APP_LAYOUT_FILE", settings.value("APP_DATA_PATH").toString() + "/default_layout.xml");

}



SettingsEditor::SettingsEditor(bool standalone, QWidget* parent): QDialog(parent)
{
    setWindowTitle("PprzGCS application settings");
    auto lay = new QVBoxLayout(this);

    auto tabWidget = new QTabWidget(this);
    lay->addWidget(tabWidget);

    QSettings settings(qApp->property("SETTINGS_PATH").toString(), QSettings::IniFormat);

    // Tab Ivy
    auto w_ivy = new QWidget(tabWidget);
    auto l_ivy = new QGridLayout(w_ivy);
    int row = 0;
    auto cb = addSetting("Ivy name", "ivy/name", w_ivy, l_ivy, row, Type::STRING);
    callbacks.append(cb);
    cb = addSetting("Bus", "ivy/bus", w_ivy, l_ivy, row, Type::STRING);
    callbacks.append(cb);
    cb = addSetting("Pprzlink ID", "pprzlink/id", w_ivy, l_ivy, row, Type::STRING);
    callbacks.append(cb);
    tabWidget->addTab(w_ivy, "Ivy");

    // Tab Map
    auto w_map = new QWidget(tabWidget);
    auto l_map = new QGridLayout(w_map);
    row = 0;
    cb = addSetting("Waypoint Size", "map/waypoint/size", w_map, l_map, row, Type::INT);
    callbacks.append(cb);
    cb = addSetting("Items Font", "map/items_font", w_map, l_map, row, Type::INT);
    callbacks.append(cb);
    cb = addSetting("Tiles path", "map/tiles_path", w_map, l_map, row, Type::PATH_DIR);
    callbacks.append(cb);
    cb = addSetting("Move Hysteresis", "map/move_hyteresis", w_map, l_map, row, Type::INT);
    callbacks.append(cb);
    cb = addSetting("Size highlight factor", "map/size_highlight_factor", w_map, l_map, row, Type::DOUBLE);
    callbacks.append(cb);
    cb = addSetting("Aircraft Size", "map/aircraft/size", w_map, l_map, row, Type::INT);
    callbacks.append(cb);
    cb = addSetting("Track size", "map/aircraft/track_max_chunk", w_map, l_map, row, Type::INT);
    callbacks.append(cb);
    tabWidget->addTab(w_map, "Map");

    // Tab General
    auto w_general = new QWidget(tabWidget);
    auto l_general = new QGridLayout(w_general);
    row = 0;
    cb = addSetting("Layout", "APP_LAYOUT_FILE", w_general, l_general, row, Type::PATH_FILE);
    callbacks.append(cb);
    cb = addSetting("Style", "APP_STYLE_FILE", w_general, l_general, row, Type::PATH_FILE);
    callbacks.append(cb);
    tabWidget->addTab(w_general, "General");


    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    lay->addWidget(buttonBox);

    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [=](){
        for(auto &cb:callbacks) {
            cb();
        }
        accept();
        qDebug() << "Restarting application...";
        qApp->exit( PprzMain::EXIT_CODE_REBOOT );
    });

    connect(buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, [=](){
        reject();
        if(standalone) {
            qDebug() << "Restarting application...";
            qApp->exit( PprzMain::EXIT_CODE_REBOOT );
        }
    });
}


std::function<void()> SettingsEditor::addSetting(QString name, QString key, QWidget* w, QGridLayout* gl, int &row, Type type) {
    QSettings settings(qApp->property("SETTINGS_PATH").toString(), QSettings::IniFormat);
    auto label = new QLabel(name, w);

    int r = row;
    row += 1;


    gl->addWidget(label, r, 0);

    if(type == Type::STRING) {
        auto edit = new QLineEdit(settings.value(key).toString(), w);
        gl->addWidget(edit, r, 1);

        auto cb = [=]() {
          QSettings settings(qApp->property("SETTINGS_PATH").toString(), QSettings::IniFormat);
          settings.setValue(key, edit->text());
        };

        return cb;
    } else if(type == Type::INT) {
        auto edit = new QLineEdit(settings.value(key).toString(), w);
        gl->addWidget(edit, r, 1);

        auto cb = [=]() {
          QSettings settings(qApp->property("SETTINGS_PATH").toString(), QSettings::IniFormat);
          settings.setValue(key, edit->text().toInt());
        };

        return cb;
    } else if(type == Type::DOUBLE) {
        auto edit = new QLineEdit(settings.value(key).toString(), w);
        gl->addWidget(edit, r, 1);

        auto cb = [=]() {
          QSettings settings(qApp->property("SETTINGS_PATH").toString(), QSettings::IniFormat);
          settings.setValue(key, edit->text().toDouble());
        };

        return cb;
    } else if(type == Type::PATH_DIR) {
        auto edit = new QLineEdit(settings.value(key).toString(), w);
        gl->addWidget(edit, r, 1);

        QToolButton* but = new QToolButton(w);
        but->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
        gl->addWidget(but, r, 2);
        connect(but, &QToolButton::clicked, this, [=]() {
            auto dir = QFileDialog::getExistingDirectory(w, name, edit->text());
            qDebug() << dir;
            if(dir != "") {
                edit->setText(dir);
            }
        });

        auto cb = [=]() {
          QSettings settings(qApp->property("SETTINGS_PATH").toString(), QSettings::IniFormat);
          settings.setValue(key, edit->text());
        };

        return cb;
    }  else if(type == Type::PATH_FILE) {
        auto edit = new QLineEdit(settings.value(key).toString(), w);
        gl->addWidget(edit, r, 1);

        QToolButton* but = new QToolButton(w);
        but->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
        gl->addWidget(but, r, 2);
        connect(but, &QToolButton::clicked, this, [=]() {
            QFileInfo fi = (edit->text());
            auto dir = QFileDialog::getOpenFileName(w, name, fi.absoluteDir().path());
            qDebug() << dir;
            if(dir != "") {
                edit->setText(dir);
            }
        });

        auto cb = [=]() {
          QSettings settings(qApp->property("SETTINGS_PATH").toString(), QSettings::IniFormat);
          settings.setValue(key, edit->text());
        };

        return cb;
    } else {
        throw std::runtime_error("Error: Setting type not handled !");
    }
}
