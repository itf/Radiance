#pragma once

#include "VideoNode.h"
#include "NodeType.h"
#include "OpenGLWorker.h"
#include "OpenGLUtils.h"
#include <QOpenGLFramebufferObject>
#include <QMutex>
#include <QReadWriteLock>
#include <QOpenGLShaderProgram>
#include <mpv/client.h>
#include <mpv/opengl_cb.h>
#include <mpv/qthelper.hpp>
#include <vector>
#include <array>

class MovieNode;

class MovieNodeRenderState {
public:
    std::atomic<bool> m_ready{false};
    Pass              m_pass {};
};
Q_DECLARE_METATYPE(QSharedPointer<MovieNodeRenderState>);

class MovieType : public NodeType {
    Q_OBJECT
    Q_PROPERTY(QString pathFormat READ pathFormat WRITE setPathFormat NOTIFY pathFormatChanged)
public:
    MovieType(NodeRegistry *r = nullptr, QObject *p = nullptr);
   ~MovieType() override;
public slots:
    QString pathFormat() const;
    void setPathFormat(QString fmt);
    VideoNode *create(QString) override;
signals:
    void pathFormatChanged(QString);
protected:
    QString m_pathFormat{};
};
///////////////////////////////////////////////////////////////////////////////

class MovieNodeOpenGLWorker : public OpenGLWorker {
    Q_OBJECT

public:
    MovieNodeOpenGLWorker(MovieNode *p);
    ~MovieNodeOpenGLWorker() override;
    std::array<QSharedPointer<QOpenGLFramebufferObject>,3> m_frames;
//    QSharedPointer<QOpenGLFramebufferObject> m_lastFrame;
//    QSharedPointer<QOpenGLFramebufferObject> m_nextFrame;
    QMutex m_rwLock;

signals:
    void message(QString str);
    void warning(QString str);
    void fatal(QString str);
    void positionChanged(qreal position);
    void durationChanged(qreal duration);
    void videoSizeChanged(QSize size);
    void muteChanged(bool mute);
    void pauseChanged(bool pause);
    void initialized();
    void prepareState(QSharedPointer<MovieNodeRenderState> state);

public slots:
    void onChainSizeChanged(QSize);
    void onVideoChanged();
    void command(const QVariant &params);
    void drawFrame();
    void setPosition(qreal position);
    void setMute(bool mute);
    void setPause(bool pause);

    void onPrepareState(QSharedPointer<MovieNodeRenderState> state);
protected:
    void handleEvent(mpv_event *event);

    static constexpr int BUFFER_COUNT = 3; // TODO double buffering doesn't quite work for some reason
    MovieNode *m_p;
    mpv::qt::Handle m_mpv;
    mpv_opengl_cb_context *m_mpv_gl;
    QSize m_size;
    QSize m_videoSize;
    QSize m_chainSize;
    QSharedPointer<MovieNodeRenderState> m_state;
protected slots:
    void updateSizes();
    void initialize();
    void onEvent();

private:
    bool loadBlitShader();
};

///////////////////////////////////////////////////////////////////////////////

// This class extends VideoNode to provide a video using libMPV
class MovieNode
    : public VideoNode {
    Q_OBJECT
    Q_PROPERTY(QString videoPath READ videoPath WRITE setVideoPath NOTIFY videoPathChanged)
    Q_PROPERTY(QSize videoSize READ videoSize NOTIFY videoSizeChanged)
    Q_PROPERTY(qreal duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(qreal position READ position NOTIFY positionChanged WRITE setPosition)
    Q_PROPERTY(bool mute READ mute NOTIFY muteChanged WRITE setMute)
    Q_PROPERTY(bool pause READ pause NOTIFY pauseChanged WRITE setPause)

    friend class MovieNodeOpenGLWorker;

public:
    MovieNode(NodeType *nr);
    MovieNode(const MovieNode &other);
    ~MovieNode();

    QString serialize() override;

    QSharedPointer<VideoNode> createCopyForRendering(QSharedPointer<Chain>) override;
    GLuint paint(QSharedPointer<Chain> chain, QVector<GLuint> inputTextures) override;

public slots:
    QSize videoSize();
    QString videoPath();
    qreal position();
    qreal duration();
    bool mute();
    bool pause();
    void setVideoPath(QString videoPath);
    void setPosition(qreal position);
    void setMute(bool mute);
    void setPause(bool pause);

protected slots:
    void onInitialized();
    void onVideoSizeChanged(QSize size);
    void onPositionChanged(qreal position);
    void onDurationChanged(qreal duration);
    void onMuteChanged(bool mute);
    void onPauseChanged(bool pause);

signals:
    void videoPathChanged(QString videoPath);
    void videoSizeChanged(QSize size);
    void chainSizeChanged(QSize size);
    void positionChanged(qreal position);
    void durationChanged(qreal duration);
    void muteChanged(bool mute);
    void pauseChanged(bool pause);

protected:
    void chainsEdited(QList<QSharedPointer<Chain>> added, QList<QSharedPointer<Chain>> removed) override;

    QString m_videoPath;
    QSharedPointer<MovieNodeOpenGLWorker> m_openGLWorker;
    QMap<QSharedPointer<Chain>, QSharedPointer<MovieNodeRenderState>> m_renderFbos;
    QSharedPointer<QOpenGLShaderProgram> m_blitShader;
    QSharedPointer<OpenGLWorkerContext> m_openGLWorkerContext;
    QSize m_videoSize;
    QSize m_chainSize;

    bool m_ready;
    qreal m_position;
    qreal m_duration;
    bool m_mute;
    bool m_pause;
};
