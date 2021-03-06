#pragma once
#include <QQuickFramebufferObject>
#include <QMutex>

class GraphicalDisplay : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString fragmentShader READ fragmentShader WRITE setFragmentShader NOTIFY fragmentShaderChanged)

public:
    QString fragmentShader();
    void setFragmentShader(QString fragmentShader);

signals:
    void fragmentShaderChanged(QString value);

protected:
    QQuickFramebufferObject::Renderer *createRenderer() const;
    QString m_fragmentShader;
    QMutex m_fragmentShaderLock;
};
