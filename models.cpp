#include "models.hpp"

#include <SQLiteCpp/SQLiteCpp.h>

namespace {

std::unique_ptr<SQLite::Database> db;

SQLite::Database& database()
{
    return *db;
}

void open_database(const char* database_path)
{
    if (!db || db->getFilename() != database_path) {
        db.reset(new SQLite::Database(
            database_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE));
    }
}

}

std::vector<std::shared_ptr<OnceTask>> OnceTask::repo_vector;
std::vector<std::shared_ptr<RepeatingTask>> RepeatingTask::repo_vector;
std::vector<std::shared_ptr<OnceOffTime>> OnceOffTime::repo_vector;
std::vector<std::shared_ptr<RepeatingOffTime>> RepeatingOffTime::repo_vector;

void setup_all(const char* database_path)
{
    open_database(database_path);
    OnceTask::setup(database_path);
    RepeatingTask::setup(database_path);
    OnceOffTime::setup(database_path);
    RepeatingOffTime::setup(database_path);
}

void OnceTask::setup(const char* database_path)
{
    open_database(database_path);
    database().exec(
        "CREATE TABLE IF NOT EXISTS once_tasks ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "duration INTEGER NOT NULL, "
        "minimum_split_size INTEGER NOT NULL, "
        "due_date INTEGER NOT NULL, "
        "schedule_after INTEGER NOT NULL"
        ")");

    repo_vector.clear();
    SQLite::Statement query(database(),
        "SELECT id, name, duration, minimum_split_size, due_date, schedule_after "
        "FROM once_tasks");
    while (query.executeStep()) {
        repo_vector.push_back(std::shared_ptr<OnceTask>(new OnceTask(
            query.getColumn(0).getInt64(),
            query.getColumn(1).getString(),
            query.getColumn(2).getInt64(),
            query.getColumn(4).getInt64(),
            query.getColumn(5).getInt64(),
            query.getColumn(3).getInt64())));
    }
}

std::shared_ptr<OnceTask> OnceTask::create(
    std::string name,
    YotsubaTime duration,
    YotsubaTime due_date,
    YotsubaTime schedule_after,
    YotsubaTime minimum_split_size)
{
    SQLite::Statement insert(database(),
        "INSERT INTO once_tasks "
        "(name, duration, minimum_split_size, due_date, schedule_after) "
        "VALUES (?, ?, ?, ?, ?)");
    insert.bind(1, name);
    insert.bind(2, duration);
    insert.bind(3, minimum_split_size);
    insert.bind(4, due_date);
    insert.bind(5, schedule_after);
    insert.exec();

    auto task = std::shared_ptr<OnceTask>(new OnceTask(
        database().getLastInsertRowid(), std::move(name), duration, due_date,
        schedule_after, minimum_split_size));
    repo_vector.push_back(task);
    return task;
}

void OnceTask::save()
{
    SQLite::Statement update(database(),
        "UPDATE once_tasks SET name = ?, duration = ?, minimum_split_size = ?, "
        "due_date = ?, schedule_after = ? WHERE id = ?");
    update.bind(1, name);
    update.bind(2, duration);
    update.bind(3, minimum_split_size);
    update.bind(4, due_date);
    update.bind(5, schedule_after);
    update.bind(6, id);
    update.exec();

    if (database().getChanges() == 0) {
        SQLite::Statement insert(database(),
            "INSERT INTO once_tasks "
            "(id, name, duration, minimum_split_size, due_date, schedule_after) "
            "VALUES (?, ?, ?, ?, ?, ?)");
        insert.bind(1, id);
        insert.bind(2, name);
        insert.bind(3, duration);
        insert.bind(4, minimum_split_size);
        insert.bind(5, due_date);
        insert.bind(6, schedule_after);
        insert.exec();
    }
}

void RepeatingTask::setup(const char* database_path)
{
    open_database(database_path);
    database().exec(
        "CREATE TABLE IF NOT EXISTS repeating_tasks ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "duration INTEGER NOT NULL, "
        "minimum_split_size INTEGER NOT NULL, "
        "starting_date INTEGER NOT NULL, "
        "repeat_every_days INTEGER NOT NULL"
        ")");

    repo_vector.clear();
    SQLite::Statement query(database(),
        "SELECT id, name, duration, minimum_split_size, starting_date, repeat_every_days "
        "FROM repeating_tasks");
    while (query.executeStep()) {
        repo_vector.push_back(std::shared_ptr<RepeatingTask>(new RepeatingTask(
            query.getColumn(0).getInt64(),
            query.getColumn(1).getString(),
            query.getColumn(2).getInt64(),
            query.getColumn(4).getInt64(),
            query.getColumn(5).getInt64(),
            query.getColumn(3).getInt64())));
    }
}

std::shared_ptr<RepeatingTask> RepeatingTask::create(
    std::string name,
    YotsubaTime duration,
    YotsubaTime starting_date,
    YotsubaTime repeat_every_days,
    YotsubaTime minimum_split_size)
{
    SQLite::Statement insert(database(),
        "INSERT INTO repeating_tasks "
        "(name, duration, minimum_split_size, starting_date, repeat_every_days) "
        "VALUES (?, ?, ?, ?, ?)");
    insert.bind(1, name);
    insert.bind(2, duration);
    insert.bind(3, minimum_split_size);
    insert.bind(4, starting_date);
    insert.bind(5, repeat_every_days);
    insert.exec();

    auto task = std::shared_ptr<RepeatingTask>(new RepeatingTask(
        database().getLastInsertRowid(), std::move(name), duration, starting_date,
        repeat_every_days, minimum_split_size));
    repo_vector.push_back(task);
    return task;
}

void RepeatingTask::save()
{
    SQLite::Statement update(database(),
        "UPDATE repeating_tasks SET name = ?, duration = ?, minimum_split_size = ?, "
        "starting_date = ?, repeat_every_days = ? WHERE id = ?");
    update.bind(1, name);
    update.bind(2, duration);
    update.bind(3, minimum_split_size);
    update.bind(4, starting_date);
    update.bind(5, repeat_every_days);
    update.bind(6, id);
    update.exec();

    if (database().getChanges() == 0) {
        SQLite::Statement insert(database(),
            "INSERT INTO repeating_tasks "
            "(id, name, duration, minimum_split_size, starting_date, repeat_every_days) "
            "VALUES (?, ?, ?, ?, ?, ?)");
        insert.bind(1, id);
        insert.bind(2, name);
        insert.bind(3, duration);
        insert.bind(4, minimum_split_size);
        insert.bind(5, starting_date);
        insert.bind(6, repeat_every_days);
        insert.exec();
    }
}

void OnceOffTime::setup(const char* database_path)
{
    open_database(database_path);
    database().exec(
        "CREATE TABLE IF NOT EXISTS once_off_times ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "start_date INTEGER NOT NULL, "
        "duration INTEGER NOT NULL, "
        "is_buffer INTEGER NOT NULL"
        ")");

    repo_vector.clear();
    SQLite::Statement query(database(),
        "SELECT id, name, start_date, duration, is_buffer "
        "FROM once_off_times");
    while (query.executeStep()) {
        repo_vector.push_back(std::shared_ptr<OnceOffTime>(new OnceOffTime(
            query.getColumn(0).getInt64(),
            query.getColumn(1).getString(),
            query.getColumn(2).getInt64(),
            query.getColumn(3).getInt64(),
            query.getColumn(4).getInt() != 0)));
    }
}

std::shared_ptr<OnceOffTime> OnceOffTime::create(
    std::string name,
    YotsubaTime start_date,
    YotsubaTime duration,
    bool is_buffer)
{
    SQLite::Statement insert(database(),
        "INSERT INTO once_off_times (name, start_date, duration, is_buffer) "
        "VALUES (?, ?, ?, ?)");
    insert.bind(1, name);
    insert.bind(2, start_date);
    insert.bind(3, duration);
    insert.bind(4, is_buffer ? 1 : 0);
    insert.exec();

    auto off_time = std::shared_ptr<OnceOffTime>(new OnceOffTime(
        database().getLastInsertRowid(), std::move(name), start_date, duration, is_buffer));
    repo_vector.push_back(off_time);
    return off_time;
}

void OnceOffTime::save()
{
    SQLite::Statement update(database(),
        "UPDATE once_off_times SET name = ?, start_date = ?, duration = ?, "
        "is_buffer = ? WHERE id = ?");
    update.bind(1, name);
    update.bind(2, start_date);
    update.bind(3, duration);
    update.bind(4, is_buffer ? 1 : 0);
    update.bind(5, id);
    update.exec();

    if (database().getChanges() == 0) {
        SQLite::Statement insert(database(),
            "INSERT INTO once_off_times (id, name, start_date, duration, is_buffer) "
            "VALUES (?, ?, ?, ?, ?)");
        insert.bind(1, id);
        insert.bind(2, name);
        insert.bind(3, start_date);
        insert.bind(4, duration);
        insert.bind(5, is_buffer ? 1 : 0);
        insert.exec();
    }
}

void RepeatingOffTime::setup(const char* database_path)
{
    open_database(database_path);
    database().exec(
        "CREATE TABLE IF NOT EXISTS repeating_off_times ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "start_date INTEGER NOT NULL, "
        "duration INTEGER NOT NULL, "
        "repeat_every_days INTEGER NOT NULL, "
        "is_buffer INTEGER NOT NULL"
        ")");

    repo_vector.clear();
    SQLite::Statement query(database(),
        "SELECT id, name, start_date, duration, repeat_every_days, is_buffer "
        "FROM repeating_off_times");
    while (query.executeStep()) {
        repo_vector.push_back(std::shared_ptr<RepeatingOffTime>(new RepeatingOffTime(
            query.getColumn(0).getInt64(),
            query.getColumn(1).getString(),
            query.getColumn(2).getInt64(),
            query.getColumn(3).getInt64(),
            query.getColumn(4).getInt64(),
            query.getColumn(5).getInt() != 0)));
    }
}

std::shared_ptr<RepeatingOffTime> RepeatingOffTime::create(
    std::string name,
    YotsubaTime start_date,
    YotsubaTime duration,
    YotsubaTime repeat_every_days,
    bool is_buffer)
{
    SQLite::Statement insert(database(),
        "INSERT INTO repeating_off_times "
        "(name, start_date, duration, repeat_every_days, is_buffer) "
        "VALUES (?, ?, ?, ?, ?)");
    insert.bind(1, name);
    insert.bind(2, start_date);
    insert.bind(3, duration);
    insert.bind(4, repeat_every_days);
    insert.bind(5, is_buffer ? 1 : 0);
    insert.exec();

    auto off_time = std::shared_ptr<RepeatingOffTime>(new RepeatingOffTime(
        database().getLastInsertRowid(), std::move(name), start_date, duration,
        repeat_every_days, is_buffer));
    repo_vector.push_back(off_time);
    return off_time;
}

void RepeatingOffTime::save()
{
    SQLite::Statement update(database(),
        "UPDATE repeating_off_times SET name = ?, start_date = ?, duration = ?, "
        "repeat_every_days = ?, is_buffer = ? WHERE id = ?");
    update.bind(1, name);
    update.bind(2, start_date);
    update.bind(3, duration);
    update.bind(4, repeat_every_days);
    update.bind(5, is_buffer ? 1 : 0);
    update.bind(6, id);
    update.exec();

    if (database().getChanges() == 0) {
        SQLite::Statement insert(database(),
            "INSERT INTO repeating_off_times "
            "(id, name, start_date, duration, repeat_every_days, is_buffer) "
            "VALUES (?, ?, ?, ?, ?, ?)");
        insert.bind(1, id);
        insert.bind(2, name);
        insert.bind(3, start_date);
        insert.bind(4, duration);
        insert.bind(5, repeat_every_days);
        insert.bind(6, is_buffer ? 1 : 0);
        insert.exec();
    }
}
