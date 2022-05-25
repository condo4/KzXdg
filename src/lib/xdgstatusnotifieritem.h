#ifndef XDGSTATUSNOTIFIERITEM_H
#define XDGSTATUSNOTIFIERITEM_H

#include <QObject>

#define XDG_CATEGORY_APPLICATIONSTATUS "ApplicationStatus"
#define XDG_CATEGORY_COMMUNICATIONS "Communications"
#define XDG_CATEGORY_SYSTEMSERVICE "SystemServices"
#define XDG_CATEGORY_HARDWARE "Hardware"

#define XDG_STATUS_PASSIVE "Passive"
#define XDG_STATUS_ACTIVE "Active"
#define XDG_STATUS_NEEDSATTENTION "NeedsAttention"

namespace KaZoe {

    class StatusNotifierItem : public QObject
    {
        Q_OBJECT

        /* This properties are used for DBus object connection */
        Q_PROPERTY(QString Category READ Category WRITE setCategory NOTIFY CategoryChanged)
        Q_PROPERTY(QString Id READ Id WRITE setId NOTIFY IdChanged)
        Q_PROPERTY(QString Title READ Title WRITE setTitle NOTIFY TitleChanged)
        Q_PROPERTY(QString Status READ Status WRITE setStatus NOTIFY StatusChanged)
        Q_PROPERTY(QString WindowId READ WindowId WRITE setWindowId NOTIFY WindowIdChanged)
        Q_PROPERTY(QString IconName READ IconName WRITE setIconName NOTIFY IconNameChanged)
        Q_PROPERTY(QString OverlayIconName READ OverlayIconName WRITE setOverlayIconName NOTIFY overlayIconNameChanged)
        Q_PROPERTY(QString AttentionIconName READ AttentionIconName WRITE setAttentionIconName NOTIFY AttentionIconNameChanged)
        Q_PROPERTY(QString AttentionMovieName READ AttentionMovieName WRITE setAttentionMovieName NOTIFY AttentionMovieNameChanged)

        /* This properties are used for QML */
        Q_PROPERTY(QString category READ Category WRITE setCategory NOTIFY CategoryChanged)
        Q_PROPERTY(QString idItem READ Id WRITE setId NOTIFY IdChanged)
        Q_PROPERTY(QString title READ Title WRITE setTitle NOTIFY TitleChanged)
        Q_PROPERTY(QString status READ Status WRITE setStatus NOTIFY StatusChanged)
        Q_PROPERTY(QString windowId READ WindowId WRITE setWindowId NOTIFY WindowIdChanged)
        Q_PROPERTY(QString iconName READ IconName WRITE setIconName NOTIFY IconNameChanged)
        Q_PROPERTY(QString overlayIconName READ OverlayIconName WRITE setOverlayIconName NOTIFY overlayIconNameChanged)
        Q_PROPERTY(QString attentionIconName READ AttentionIconName WRITE setAttentionIconName NOTIFY AttentionIconNameChanged)
        Q_PROPERTY(QString attentionMovieName READ AttentionMovieName WRITE setAttentionMovieName NOTIFY AttentionMovieNameChanged)

    public:
        const QStringList xdgCategories {XDG_CATEGORY_APPLICATIONSTATUS, XDG_CATEGORY_COMMUNICATIONS, XDG_CATEGORY_SYSTEMSERVICE, XDG_CATEGORY_HARDWARE};
        const QStringList xdgStatus {XDG_STATUS_PASSIVE, XDG_STATUS_ACTIVE, XDG_STATUS_NEEDSATTENTION};

        explicit StatusNotifierItem(QObject *parent = nullptr);

        QString Title() const;
        void setTitle(const QString &newTitle);

        QString IconName() const;
        void setIconName(const QString &newIconName);

        QString Status() const;
        void setStatus(const QString &newStatus);


        QString OverlayIconName() const;
        void setOverlayIconName(const QString &newOverlayIconName);

        QString Category() const;
        void setCategory(const QString &newCategory);

        QString Id() const;
        void setId(const QString &newId);

        QString WindowId() const;
        void setWindowId(const QString &newWindowId);

        QString AttentionIconName() const;
        void setAttentionIconName(const QString &newAttentionIconName);

        QString AttentionMovieName() const;
        void setAttentionMovieName(const QString &newAttentionMovieName);

    public slots:
        /* DBus API */
        virtual void Activate(uint x, uint y);
        virtual void SecondaryActivate(uint x, uint y);

    signals:
        void NewTitle();
        void NewIcon();
        void NewAttentionIcon();
        void NewOverlayIcon();
        void NewToolTip();
        void NewStatus(QString status);

        void TitleChanged();
        void IconNameChanged();
        void StatusChanged();
        void overlayIconNameChanged();
        void CategoryChanged();
        void IdChanged();
        void WindowIdChanged();

        void activated(uint x, uint y);
        void secondaryActivated(uint x, uint y);


        void AttentionIconNameChanged();

        void AttentionMovieNameChanged();

    private:
        QString m_Title;
        QString m_IconName;
        QString m_Status {XDG_STATUS_ACTIVE};
        QString m_OverlayIconName;
        QString m_Category {XDG_CATEGORY_APPLICATIONSTATUS};
        QString m_Id;
        QString m_WindowId;
        QString m_AttentionIconName;
        QString m_AttentionMovieName;
    };
};

#endif // XDGSTATUSNOTIFIERITEM_H
