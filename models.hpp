#ifndef YONPUN_MODELS_HPP
#define YONPUN_MODELS_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

using YotsubaTime = std::int64_t;

void setup_all(const char *database_path);

class OnceTask
{
public:
    static std::shared_ptr<OnceTask> create(
        std::string name,
        YotsubaTime duration,
        YotsubaTime due_date,
        YotsubaTime schedule_after = -1,
        YotsubaTime minimum_split_size = -1);

    void save();

    std::string name;
    const std::int64_t id;
    YotsubaTime duration;
    YotsubaTime minimum_split_size;
    YotsubaTime due_date;
    YotsubaTime schedule_after;

    static void setup(const char *database_path);

private:
    static std::vector<std::shared_ptr<OnceTask>> repo_vector;

    OnceTask(std::int64_t id,
             std::string name,
             YotsubaTime duration,
             YotsubaTime due_date,
             YotsubaTime schedule_after,
             YotsubaTime minimum_split_size)
        : id(id),
          name(std::move(name)),
          duration(duration),
          minimum_split_size(minimum_split_size),
          due_date(due_date),
          schedule_after(schedule_after) {}
};

class RepeatingTask
{
public:
    static std::shared_ptr<RepeatingTask> create(
        std::string name,
        YotsubaTime duration,
        YotsubaTime starting_date,
        YotsubaTime repeat_every_days,
        YotsubaTime minimum_split_size = -1);

    void save();

    std::string name;
    const std::int64_t id;
    YotsubaTime duration;
    YotsubaTime minimum_split_size;
    YotsubaTime starting_date;
    YotsubaTime repeat_every_days;

    static void setup(const char *database_path);

private:
    static std::vector<std::shared_ptr<RepeatingTask>> repo_vector;

    RepeatingTask(std::int64_t id,
                  std::string name,
                  YotsubaTime duration,
                  YotsubaTime starting_date,
                  YotsubaTime repeat_every_days,
                  YotsubaTime minimum_split_size)
        : id(id),
          name(std::move(name)),
          duration(duration),
          minimum_split_size(minimum_split_size),
          starting_date(starting_date),
          repeat_every_days(repeat_every_days) {}
};

class OnceOffTime
{
public:
    static std::shared_ptr<OnceOffTime> create(
        std::string name,
        YotsubaTime start_date,
        YotsubaTime duration,
        bool is_buffer);

    void save();

    std::string name;
    const std::int64_t id;
    YotsubaTime start_date;
    YotsubaTime duration;
    bool is_buffer;

    static void setup(const char *database_path);

private:
    static std::vector<std::shared_ptr<OnceOffTime>> repo_vector;

    OnceOffTime(std::int64_t id,
                std::string name,
                YotsubaTime start_date,
                YotsubaTime duration,
                bool is_buffer)
        : id(id),
          name(std::move(name)),
          start_date(start_date),
          duration(duration),
          is_buffer(is_buffer) {}
};

class RepeatingOffTime
{
public:
    static std::shared_ptr<RepeatingOffTime> create(
        std::string name,
        YotsubaTime start_date,
        YotsubaTime duration,
        YotsubaTime repeat_every_days,
        bool is_buffer);

    void save();

    std::string name;
    const std::int64_t id;
    YotsubaTime start_date;
    YotsubaTime duration;
    YotsubaTime repeat_every_days;
    bool is_buffer;

    static void setup(const char *database_path);

private:
    static std::vector<std::shared_ptr<RepeatingOffTime>> repo_vector;

    RepeatingOffTime(std::int64_t id,
                     std::string name,
                     YotsubaTime start_date,
                     YotsubaTime duration,
                     YotsubaTime repeat_every_days,
                     bool is_buffer)
        : id(id),
          name(std::move(name)),
          start_date(start_date),
          duration(duration),
          repeat_every_days(repeat_every_days),
          is_buffer(is_buffer) {}
};

using CalendarVariant = std::variant<std::monostate,
                                     std::weak_ptr<OnceTask>,
                                     std::weak_ptr<RepeatingTask>,
                                     std::weak_ptr<OnceOffTime>,
                                     std::weak_ptr<RepeatingOffTime>>;

struct CalendarVariantHash
{
    std::size_t operator()(const CalendarVariant &event) const;
};

struct CalendarVariantEqual
{
    bool operator()(const CalendarVariant &left, const CalendarVariant &right) const;
};

class Calendar
{
public:
    /* Schedules Tasks and returns a calendar */
    static Calendar create(
        std::vector<std::shared_ptr<OnceTask>> once_tasks,
        std::vector<std::shared_ptr<RepeatingTask>> repeating_tasks,
        std::vector<std::shared_ptr<OnceOffTime>> once_off_times,
        std::vector<std::shared_ptr<RepeatingOffTime>> repeating_off_times);

    /* Returns the event at the given time, or an empty event
    (i.e. std::monostate) if either no event is scheduled at that time or is out of bounds. */
    CalendarVariant get_event_at_time(YotsubaTime time) const;
    std::vector<YotsubaTime> get_times_of_event(CalendarVariant event) const;

private:
    Calendar(
        YotsubaTime first_time,
        std::vector<CalendarVariant> &&events_by_time,
        std::unordered_map<CalendarVariant, std::vector<YotsubaTime>,
                           CalendarVariantHash, CalendarVariantEqual> &&times_by_event);

    YotsubaTime first_time;
    std::vector<CalendarVariant> events_by_time;
    std::unordered_map<CalendarVariant, std::vector<YotsubaTime>, CalendarVariantHash,
                       CalendarVariantEqual>
        times_by_event;
};

#endif