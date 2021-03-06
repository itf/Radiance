#pragma once

#include "Output.h"
#include "Model.h"
#include <QList>
#include <QQuickWindow>

class Context : public QObject {
    Q_OBJECT
    Q_PROPERTY(Model *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QVariantList outputs READ outputsQml WRITE setOutputsQml NOTIFY outputsChanged)
    Q_PROPERTY(QSize previewSize READ previewSize WRITE setPreviewSize NOTIFY previewSizeChanged)
    Q_PROPERTY(QQuickWindow *previewWindow READ previewWindow WRITE setPreviewWindow NOTIFY previewWindowChanged)

public:
    Context(bool hasPreview=true);
   ~Context() override;

public slots:
    Model *model();
    void setModel(Model *model);
    QSize previewSize(); // thread-safe
    void setPreviewSize(QSize size);
    QQuickWindow *previewWindow(); // thread-safe
    void setPreviewWindow(QQuickWindow *window);

    QList<Output *> outputs();
    void setOutputs(QList<Output *> outputs);
    QVariantList outputsQml();
    void setOutputsQml(QVariantList outputs);

    // Use this method to retrieve
    // rendered preview textures
    GLuint previewTexture(int videoNodeId);

protected slots:
    void onRenderRequested(Output *output);
    void onBeforeSynchronizing();

signals:
    void previewSizeChanged(QSize size);
    void outputsChanged(QList<Output *> outputs);
    void modelChanged(Model *model);
    void previewWindowChanged(QQuickWindow *window);

protected:
    void chainsChanged();
    QList<QSharedPointer<Chain>> chains();

    Model *m_model;
    QList<Output *> m_outputs;
    bool m_hasPreview;
    QSize m_previewSize;
    QSharedPointer<Chain> m_previewChain;
    QQuickWindow *m_previewWindow;
    QMap<int, GLuint> m_lastPreviewRender;
    QMutex m_previewLock;
};
