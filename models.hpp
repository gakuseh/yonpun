#ifndef YONPUN_MODELS_HPP
#define YONPUN_MODELS_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using YotsubaTime = std::int64_t;

void setup_all(const char* database_path);

class OnceTask {
public:
  static std::shared_ptr<OnceTask> create(
    std::string name,
    YotsubaTime duration,
    YotsubaTime due_date,
    YotsubaTime schedule_after = -1,
    YotsubaTime minimum_split_size = -1);

    void save();

    std::string name;
    YotsubaTime duration;
    YotsubaTime minimum_split_size;
    YotsubaTime due_date;
    YotsubaTime schedule_after;

    static void setup(const char* database_path);

  private:
    static std::vector<std::shared_ptr<OnceTask>> repo_vector;
    std::int64_t id;

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

class RepeatingTask {
public:
    static std::shared_ptr<RepeatingTask> create(
      std::string name,
      YotsubaTime duration,
      YotsubaTime starting_date,
      YotsubaTime repeat_every_days,
      YotsubaTime minimum_split_size = -1);

    void save();

    std::string name;
    YotsubaTime duration;
    YotsubaTime minimum_split_size;
    YotsubaTime starting_date;
    YotsubaTime repeat_every_days;

    static void setup(const char* database_path);

  private:
    static std::vector<std::shared_ptr<RepeatingTask>> repo_vector;
    std::int64_t id;

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

class OnceOffTime {
public:
    static std::shared_ptr<OnceOffTime> create(
      std::string name,
      YotsubaTime start_date,
      YotsubaTime duration,
      bool is_buffer);

    void save();

    std::string name;
    YotsubaTime start_date;
    YotsubaTime duration;
    bool is_buffer;

    static void setup(const char* database_path);

  private:
    static std::vector<std::shared_ptr<OnceOffTime>> repo_vector;
    std::int64_t id;

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

class RepeatingOffTime {
public:
    static std::shared_ptr<RepeatingOffTime> create(
      std::string name,
      YotsubaTime start_date,
      YotsubaTime duration,
      YotsubaTime repeat_every_days,
      bool is_buffer);

    void save();

    std::string name;
    YotsubaTime start_date;
    YotsubaTime duration;
    YotsubaTime repeat_every_days;
    bool is_buffer;

    static void setup(const char* database_path);

private:
    static std::vector<std::shared_ptr<RepeatingOffTime>> repo_vector;
  std::int64_t id;

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

#endif