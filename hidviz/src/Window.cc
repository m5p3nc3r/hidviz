#include "Window.hh"

#include "ui_Window.h"

#include "DeviceView.hh"
#include "DeviceSelector.hh"
#include "src/hid/CollectionWidget.hh"

#include <libhidx/hid/Collection.hh>
#include <libhidx/hid/Control.hh>
#include <libhidx/Interface.hh>
#include <libhidx/LibHidx.hh>

#include <QSettings>

#include <cassert>

namespace hidviz {

    Window::Window() : QWidget{}, ui{new Ui::Window} {
        ui->setupUi(this);

        QSettings settings{"hidviz"};
        restoreGeometry(settings.value( "geometry", saveGeometry() ).toByteArray());
        move(settings.value( "pos", pos() ).toPoint());
        resize(settings.value( "size", size() ).toSize());

        connect(ui->selectDeviceButton, &QToolButton::clicked, this, &Window::openDeviceSelector);
    }

    Window::~Window() {
        delete ui;
    }

    void Window::openDeviceSelector() {
        auto dialog = new DeviceSelector(getLibhidx());
        dialog->show();
        connect(dialog, &DeviceSelector::deviceSelected, this,
                &Window::selectDevice);
        connect(dialog, &DeviceSelector::listCleared, this, &Window::clearModel);
    }

    void Window::selectDevice(libhidx::Interface& interface) {
        assert(interface.isHid());

        if(&interface == m_selectedInterface){
            // do not open already opened interface
            return;
        }

        m_selectedInterface = &interface;

        ui->titleLabel->setText(QString::fromStdString(interface.getName()));

        m_deviceView = new DeviceView{interface};
        connect(ui->showAllUsages, &QPushButton::clicked, m_deviceView, &DeviceView::hideInactiveUsagesChanged);
        ui->contentWidget->setWidget(m_deviceView);
    }

    void Window::closeEvent(QCloseEvent* event) {
        QSettings settings{"hidviz"};
        settings.setValue( "geometry", saveGeometry() );
        settings.setValue( "maximized", isMaximized() );
        if ( !isMaximized() ) {
            settings.setValue( "pos", pos() );
            settings.setValue( "size", size() );
        }
        QWidget::closeEvent(event);
    }

    void Window::updateData() {

        m_deviceView->updateData();
    }

    void Window::clearModel() {
        delete m_deviceView;
        m_deviceView = nullptr;
    }

    libhidx::LibHidx& Window::getLibhidx() {
        if(!m_lib){
            m_lib = std::make_unique<libhidx::LibHidx>();
        }
        return *m_lib;
    }
}
