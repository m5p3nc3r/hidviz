#include "Item.hh"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace hidviz {
namespace hid {

    Item::Item() : QWidget{} {
        m_mainLayout = new QVBoxLayout{};

        m_headerLayout = new QHBoxLayout{};

        m_name = new QLabel{"name"};
        m_name->setStyleSheet("font-weight: bold;");
        m_headerLayout->addWidget(m_name);

        m_usage = new QLabel{"usage"};
        m_headerLayout->addWidget(m_usage);

        m_mainLayout->addLayout(m_headerLayout);

        setLayout(m_mainLayout);
        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    }
}
}
