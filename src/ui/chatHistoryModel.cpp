#include "chatHistoryModel.hpp"

int ChatHistoryModel::addChatMessage(const QString &chatText, const QString &author, const QString &time, bool alignLeft)
{
    beginInsertRows(QModelIndex(), m_chatHistory.count(), m_chatHistory.count());
    m_chatHistory.append({chatText, author, time, alignLeft});
    endInsertRows();
    return m_chatHistory.count() - 1;
}

QHash<int, QByteArray> ChatHistoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ChatTextRole] = "chatText";
    roles[AuthorRole] = "author";
    roles[TimeRole] = "time";
    roles[AlignLeftRole] = "alignLeft";
    return roles;
}

QVariant ChatHistoryModel::data(const QModelIndex &index, int role) const
{
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

int ChatHistoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_chatHistory.count();
}

void ChatHistoryModel::removeChatMessage(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_chatHistory.remove(index);
    endRemoveRows();
}

void ChatHistoryModel::clearChatHistory()
{
    beginResetModel();
    m_chatHistory.clear();
    endResetModel();
}