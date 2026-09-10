#include "models.hpp"

#include <SQLiteCpp/SQLiteCpp.h>
#include <type_traits>

namespace
{

    std::unique_ptr<SQLite::Database> db;

    SQLite::Database &database()
    {
        return *db;
    }

    void open_database(const char *database_path)
    {
        if (!db || db->getFilename() != database_path)
        {
            db.reset(new SQLite::Database(
                database_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE));
        }
    }

}

std::vector<std::shared_ptr<OnceTask>> OnceTask::repo_vector;
std::vector<std::shared_ptr<RepeatingTask>> RepeatingTask::repo_vector;
std::vector<std::shared_ptr<OnceOffTime>> OnceOffTime::repo_vector;
std::vector<std::shared_ptr<RepeatingOffTime>> RepeatingOffTime::repo_vector;

void setup_all(const char *database_path)
{
    open_database(database_path);
    OnceTask::setup(database_path);
    RepeatingTask::setup(database_path);
    OnceOffTime::setup(database_path);
    RepeatingOffTime::setup(database_path);
}

void OnceTask::setup(const char *database_path)
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
    while (query.executeStep())
    {
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

    if (database().getChanges() == 0)
    {
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

void RepeatingTask::setup(const char *database_path)
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
    while (query.executeStep())
    {
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

    if (database().getChanges() == 0)
    {
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

void OnceOffTime::setup(const char *database_path)
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
    while (query.executeStep())
    {
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

    if (database().getChanges() == 0)
    {
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

void RepeatingOffTime::setup(const char *database_path)
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
    while (query.executeStep())
    {
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

    if (database().getChanges() == 0)
    {
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

std::size_t ScheduleVariantHash::operator()(const ScheduleVariant &event) const
{
    const std::size_t type = event.index();
    const std::size_t id = std::visit(
        [](const auto &event_pointer)
        {
            using Event = std::decay_t<decltype(event_pointer)>;

            if constexpr (std::is_same_v<Event, std::monostate>)
            {
                return std::size_t{0};
            }
            else
            {
                const auto event_object = event_pointer.lock();
                return event_object ? static_cast<std::size_t>(event_object->id) : 0;
            }
        },
        event);

    // Hash mixing source - https://stackoverflow.com/a/50978188 by Wolfgang Brehm
    // casting uint64 to size_t might've messed it up... im not sure how to make
    // hash functions
    std::uint64_t x = static_cast<std::uint64_t>(type) ^
                      (static_cast<std::uint64_t>(id) << 32);
    constexpr std::uint64_t m = 0xe9846afb1a615dULL;
    x ^= x >> 32;
    x *= m;
    x ^= x >> 32;
    x *= m;
    x ^= x >> 28;
    return static_cast<std::size_t>(x);
}

bool ScheduleVariantEqual::operator()(
    const ScheduleVariant &left, const ScheduleVariant &right) const
{
    if (left.index() != right.index())
    {
        return false;
    }

    if (left.index() == 0 && right.index() == 0)
    {
        return true;
    }

    return std::visit(
        [](const auto &left_event, const auto &right_event)
        {
            using Left = std::decay_t<decltype(left_event)>;
            using Right = std::decay_t<decltype(right_event)>;

            if constexpr (std::is_same_v<Left, std::monostate> ||
                          std::is_same_v<Right, std::monostate>)
            {
                return std::is_same_v<Left, Right>;
            }
            else
            {
                const auto left_object = left_event.lock();
                const auto right_object = right_event.lock();
                const auto left_id = left_object ? left_object->id : 0;
                const auto right_id = right_object ? right_object->id : 0;
                return left_id == right_id;
            }
        },
        left, right);
}

Schedule::Schedule(
    YotsubaTime first_time,
    std::vector<ScheduleVariant> &&events_by_time,
    std::unordered_map<ScheduleVariant, std::vector<YotsubaTime>,
                       ScheduleVariantHash, ScheduleVariantEqual> &&times_by_event)
    : first_time(first_time),
      events_by_time(std::move(events_by_time)),
      times_by_event(std::move(times_by_event))
{
}

const Schedule Schedule::create(
    std::vector<std::shared_ptr<OnceTask>> once_tasks,
    std::vector<std::shared_ptr<RepeatingTask>> repeating_tasks,
    std::vector<std::shared_ptr<OnceOffTime>> once_off_times,
    std::vector<std::shared_ptr<RepeatingOffTime>> repeating_off_times)
{
    /* TODO: Write scheduling algorithm */

    (void)once_tasks;
    (void)repeating_tasks;
    (void)once_off_times;
    (void)repeating_off_times;
    return Schedule(
        0,
        std::vector<ScheduleVariant>{},
        std::unordered_map<ScheduleVariant, std::vector<YotsubaTime>,
                           ScheduleVariantHash, ScheduleVariantEqual>{});
}

ScheduleVariant Schedule::get_event_at_time(YotsubaTime time) const
{
    if (time < this->first_time || time >= this->first_time + static_cast<YotsubaTime>(events_by_time.size()))
    {
        return std::monostate{};
    }
    return events_by_time[static_cast<std::size_t>(time - this->first_time)];
}

const std::vector<YotsubaTime> Schedule::get_times_of_event(ScheduleVariant event) const
{
    (void)event;
    return {};
}
