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

    ChatHistoryModel(QObject *parent = nullptr) : QAbstractListModel(parent) {
        // Initialize with some data
        m_chatHistory.append({ QStringLiteral("This is what someone said jnnneosijnf fjesonfneso nisfnienkjnsk  nuiksnfikenksn niksnfikenks knikfneikjnsik kfnesiknkfes"), QStringLiteral("LLM"), QStringLiteral("12:00"), false });
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return m_chatHistory.count();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= m_chatHistory.count())
            return QVariant();

        const ChatMessage &message = m_chatHistory[index.row()];
        if (role == ChatTextRole)
            return message.chatText;
        else if (role == AuthorRole)
            return message.author;
        else if (role == TimeRole)
            return message.time;
        else if (role == AlignLeftRole)
            return message.alignLeft;
        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[ChatTextRole] = "chatText";
        roles[AuthorRole] = "author";
        roles[TimeRole] = "time";
        roles[AlignLeftRole] = "alignLeft";
        return roles;
    }

    void addChatMessage(const QString &chatText, const QString &author, const QString &time, bool alignLeft) {
        beginInsertRows(QModelIndex(), m_chatHistory.count(), m_chatHistory.count());
        m_chatHistory.append({ chatText, author, time, alignLeft });
        endInsertRows();
    }

private:
    QVector<ChatMessage> m_chatHistory;
};

