#include "todomodel.h"

TodoModel::TodoModel(QObject *parent) : QAbstractListModel(parent) {}

int TodoModel::rowCount(const QModelIndex&) const { return m_items.size(); }

QVariant TodoModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) return {};
    const auto &it = m_items.at(index.row());
    if (role == TitleRole) return it.title;
    if (role == DoneRole) return it.done;
    return {};
}

QHash<int, QByteArray> TodoModel::roleNames() const {
    return { {TitleRole, "title"}, {DoneRole, "done"} };
}

void TodoModel::addTodo(const QString &title) {
    beginInsertRows({}, m_items.size(), m_items.size());
    m_items.append({title, false});
    endInsertRows();
}

void TodoModel::remove(int row) {
    if (row < 0 || row >= m_items.size()) return;
    beginRemoveRows({}, row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

void TodoModel::setDone(int row, bool done) {
    if (row < 0 || row >= m_items.size()) return;
    m_items[row].done = done;
    const QModelIndex idx = createIndex(row, 0);
    emit dataChanged(idx, idx, {DoneRole});
}