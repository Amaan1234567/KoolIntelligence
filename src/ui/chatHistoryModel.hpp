#pragma once

#include <QAbstractListModel>
#include <QVector>

struct ChatMessage {
    QString chatText;
    QString author;
    QString time;
    bool alignLeft;
};

class ChatHistoryModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum ChatRoles {
        ChatTextRole = Qt::UserRole + 1,
        AuthorRole,
        TimeRole,
        AlignLeftRole
    };

    ChatHistoryModel(QObject *parent = nullptr) : QAbstractListModel(parent){}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addChatMessage(const QString &chatText, const QString &author, const QString &time, bool alignLeft);

private:
    QVector<ChatMessage> m_chatHistory;
};

