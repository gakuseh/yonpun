#include <QAbstractListModel>

struct TodoItem { QString title; bool done = false; };

class TodoModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { TitleRole = Qt::UserRole + 1, DoneRole };
    explicit TodoModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addTodo(const QString &title);
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE void setDone(int row, bool done);

private:
    QVector<TodoItem> m_items;
};