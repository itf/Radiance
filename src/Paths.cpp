#include "Paths.h"
#include <QDebug>
#include <QStandardPaths>

bool Paths::m_initialized = false;
QString Paths::m_library;
QString Paths::m_qml;
QString Paths::m_glsl;

QString Paths::library() {
    Q_ASSERT(m_initialized);
    return Paths::m_library;
}

QString Paths::qml() {
    Q_ASSERT(m_initialized);
    return Paths::m_qml;
}

QString Paths::glsl() {
    Q_ASSERT(m_initialized);
    return Paths::m_glsl;
}

void Paths::initialize(bool debug) {
    if (debug) {
        Paths::m_library = "../resources/library/";
        Paths::m_qml = "../resources/qml/";
        Paths::m_glsl = "../resources/glsl/";
    } else {
        Paths::m_library = QStandardPaths::locate(QStandardPaths::AppDataLocation, "library/", QStandardPaths::LocateDirectory);
        Paths::m_qml = QStandardPaths::locate(QStandardPaths::AppDataLocation, "qml/", QStandardPaths::LocateDirectory);
        Paths::m_glsl = QStandardPaths::locate(QStandardPaths::AppDataLocation, "glsl/", QStandardPaths::LocateDirectory);
    }
    Paths::m_initialized = true;
    qDebug() << "Library path is:" << Paths::m_library;
    qDebug() << "QML path is:" << Paths::m_qml;
    qDebug() << "GLSL path is:" << Paths::m_glsl;
}
