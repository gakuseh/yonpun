# Goals and Constraints

## Time representation

Adhering to the name of this program, Yonpun (i.e. quarters of an hour), all dates usable by this program have intervals
of 15 minutes between them. Unix time underlies date representation in this program, and so date handling quirks (e.g.
leap seconds, length of the day) is shared with Unix time. The only difference is epoch and precision of this time.

All dates are in **Yotsuba time**. All dates are represented by a **signed**
**long that is at least 32-bits in width** that represents the amount of 15 minute intervals since/before the **Yotsuba
epoch**, defined as 1043107200 unix time seconds since the Unix epoch. A "15 minute interval" is defined as 900 unix
time seconds.

This lets Yotsuba time represent about 60 thousand years before and after the Yotsuba epoch. The program does not expect
values larger than the maximum

## Tasks

User inputs tasks

All tasks have a name, duration, and minimum split size

Tasks can either be repeating or once

Once tasks

- Happen only once

- Have a due date

- Have a schedule after date

Repeating tasks

- Happen multiple times; each time is an "iteration"

- Repeats every X number of days

- Have a starting date

- Cannot be scheduled before the due date of the previous iteration of the same repeating task

- Missed iterations of repeating tasks "accumulate", i.e. they do not get deleted after the due date is passed, do not
  get merged with the next iteration of the task, and remains on the to do list.

Tasks can be scheduled so that they are "split", e.g. not done consecutively. the minimum split size states how short
each split can be. by default, the split size is -1

## Off Time

Periods of time where tasks cannot be scheduled

All off-times have a name, start date, duration, and is_buffer quality

OffTimes can either be repeating or once

Once off times

- Happen only once

Repeating off times

- Repeat every X number of days

The `is_buffer` quality lets the computer schedule tasks in this off time _if_
_necessary_, i.e. if tasks cannot be completed before their due date while also respecting the off times

## Output

result is a calendar that contains both off times and when to do tasks

when presented to the user, date times must be in local time and presentation should adhere to the locale of the user
(e.g. time zone, date presentation)

otherwise, if final internal represenation right before user presentation, dates may be in Yotsuba time.

# Design Above Scheduling Algorithm

## Representing tasks, off time, and persistence

Persistence is similar to Active Record pattern

A single Task object per row that lives the entire lifetime of the program

Task objects live on the heap, and are kept track by a `repo_vector` of shared_ptrs to the Tasks on the heap

Users then get weak_ptrs that they can turn into shared_ptrs

Task objects then have a instance method save, which saves the data to db. Save updates the db for all fields of that
row, but not all objects.

Same thing for offtimes

Have a different class for Repeating/Once; don't try to combine them or do inheritence or anything

### Justification, discussion

No stale objects, bc only one object = one db row

Minimal queries to db, by using save to save all fields at once

Storing objects all in memory might take up a lot of space, but likely won't have a lot of tasks/off times to merit
concern

Can always switch to cache or smth later

Handing out weak_ptrs so that every time you try to read and write you have to check if the object still exists, which
is good

> std::weak_ptr models temporary ownership: when an object needs to be accessed
> only if it exists, and it may be deleted at any time by someone else
>
> https://en.cppreference.com/cpp/memory/weak_ptr

e.g. suppose a function takes in a task, then gives it to a nested function call that may or may not delete the object.
if it was just a shared pointer, then you may not know the nested call deleted the object and then you end up
manipulating something that should be deleted but runs silently. if you have raw pointers, then you try to manipulate a
dangling pointer, which hopefully ends in a crash

## User interface

For the MVP, only text based

Prompt based, user chooses from menu to do

1. See all tasks
2. Create a new task
3. Edit a task
4. Delete a task
5. See all off times
6. Create a new off time
7. Edit an off time
8. Delete off times
9. Schedule everything out and create a calendar

# Scheduling Algorithm Structures and Design

## Data structures and types

### `Schedule` object

Schedule is some data structure that

- Given a time, tells you what to do at that time
- Given a task/offtime, tells you when you need to do it
- Be able to print out some representation of it (e.g. text for MVP, UI, ics/calendar app integration eventually)

Schedule is essentially a wrapped vector of `ScheduleVariant`s (see below.)

Schedule objects cannot be created directly; instead, use the factory method `Schedule::create()`, which will also run
the scheduling algorithm.

`Schedule` objects are immutable once constructed. Any modifications to tasks or off-times require regenerating a new
`Schedule` rather than mutating an existing instance.

Public accessor methods of Calendar include:

- `CalendarEvent get_event_at_time(YotsubaTime time)`
- `std::vector<YotsubaTime> get_times_of_event(ScheduleVariant event)`

As well as a factory method that takes in vector of shared_ptrs to OnceTasks, ..., and RepeatingOffTimes and returns a
Schedule.

For the time --> `ScheduleVariant` direction, use a vector. the vector only lasts from now to the last task/event or 30
days, which ever is greater. memory use of vector shouldn't be too bad, but if it is a problem then we'll need to fix it
later. the vector should be coupled with data about what time the 0th index block is. will be abstracted away by methods
that go with a calendar.

For the `ScheduleVariant` to time direction, use a map. The keys should be a `ScheduleVariant`, and the values will be a
vector of YotsubaTimes.

Finally, text representation would just loop through the time --> task/offtime vector and print out everything

The `Schedule` class represents the calculated schedule containing both tasks and off-times mapped across 15-minute
Yotsuba time intervals.

### `ScheduleVariant`

```cpp
using ScheduleVariant = std::variant<std::monostate,
                                     std::weak_ptr<OnceTask>,
                                     std::weak_ptr<RepeatingTask>,
                                     std::weak_ptr<OnceOffTime>,
                                     std::weak_ptr<RepeatingOffTime>>;
```

### TaskSplit

A `TaskSplit` is a struct representing what times we still need to schedule for a task.

A `TaskSplit` holds a `YotsubaTime duration` and a
`std::variant<std::weak_ptr<OnceTask>, std::weak_ptr<RepeatingTask>> event`. The duration of each TaskSplit should be
the same size of the minimum split size dictated by the Task. The only exception to this is if there is a
"remainder" duration that is smaller than the minimum split size. In this exception, the TaskSplit will be the same size
as the remaining duration.

The scheduling algorithm uses a collection of `TaskSplit`s to keep track of what tasks to schedule and what times have
already been scheduled. `TaskSplits` are not actually placed into the schedule; rather, `ScheduleVariant`s are placed
directly into the schedule.

### `TaskSplitCollection`, `TaskSplitIterator`, and iterating through `TaskSplits`

Correctly iterating through TaskSplits in a controlled way helps to reach successful schedules faster.

To do this, we wrap the `TaskSplit` collection with a `TaskSplitCollection`
class. The Constructor takes in a collection of Task objects. The class then provides an `TaskSplitIterator` iterator
object. Iterating works such that
`TaskSplits` with earliest due dates and longest durations that haven't been hidden/placed in the schedule appear FIRST.
Dereferencing the iterator produces a `TaskSplit`. Incrementing the iterator makes the iterator point to the next
`TaskSplit`. `TaskSplitCollection` has both an `hide` and `show` method that takes in a `TaskSplitIterator`. Hiding
means any future created iterators will not give the erased `TaskSplit`, and showing "brings back" the `TaskSplit` that
was "hidden", so that iterators created in the future will give the `TaskSplit`
again. This is intended to be used in the following manner: when a `TaskSplit`
is scheduled, it should be hidden. Further recursive steps will thus not see this `TaskSplit` through the interator, and
so `TaskSplits` will not be placed twice. Then, if the recursion backtracks and the `TaskSplit` is removed from the
Schedule, it should be shown again.

Internally, `TaskSplitCollection` uses a `const std::vector<TaskSplit>` to store the `TaskSplit`s, as well as a
`std::vector<bool>` that is the same size as the aforementioned vector, which keeps track if a `TaskSplit` is hidden.
Hiding/showing `TaskSplits` updates the
`std::vector<bool>`.

As for `TaskSplitIterator`, it keeps track of a `size_t` index into the aforementioned vector. Every time
`TaskSplitIterator` is incremented, this index is incremented until it points to a `TaskSplit` that is not hidden.

## Algorithm

This algorithm is a "prototype" algorithm, used as a minimum viable product, and is essentially a naive backtracking
algorithm with minimal heuristics. Future versions should improve the heuristics of this backtracking or use a more
sophisticated algorithm, i.e. with scheduling theory algorithms.

Create a representation for "TaskSplit". A TaskSplit will be the elements we place into our Schedule, and it is the
object that is placed by each recursive step of the Backtracking algorithm. A TaskSplit is some structure that keeps
track of the task it is for, as well as the duration to work on this Task. The duration of each TaskSplit should be the
same size of the minimum split size dictated by the Task. The only exception to this is if there is a "remainder"
duration that is smaller than the minimum split size. In this exception, the TaskSplit will be the same size as the
remaining duration.

For example, a Task with duration 75 minutes and minimum split size 30 minutes will have three TaskSplits, two with
duration 30 minutes (following the minimum split size) and one with duration 15 minutes (which holds the remainder).

Each TaskSplit also keeps track of its "schedule after time" and its due date.

Before we begin backtracking, we create a  `TaskSplitCollection`. Essentially, it is a vector of `TaskSplit`
objects, where earliest due date and longest duration are FIRST, but also provides extra methods for iterating through
the collection and also preventing duplicate TaskSplits in the schedule. The reasoning for this data structure should
become clear as we describe the rest of the algorithm.

We now finally begin backtracking. Backtracking consists of a recursive function. The function returns a true if all
task splits have been scheduled and a solution has been found, and false otherwise.

There are two base cases for this recursive function. One, there are no more TaskSplits we need to schedule, and so we
return true and end all recursive calls. Two, we still have TaskSplits we need to schedule, but there are no more gaps
in the schedule where we can place things. In this case, we return false, and the above recursive call handles the rest.

Now we describe the recursive step of this backtracking algorithm. First, we need a loop. Every loop iteration, we need
to find the next TaskSplit to schedule. Thus, before we enter the loop, we create a `TaskSplitIterator`
object. Next, we enter the loop. In every iteration, we dereference the iterator and hide the `TaskSplit` it points to.

We then enter another loop. Every iteration of this nested loop tries a different starting time in the schedule, and
places the `TaskSplit` at that time. If the placement is valid, then we recursively call the backtracking function
again. If the recursive call returns true, then we return true. If the recursive call returns false, then we continue to
the next iteration of the nested loop. If we have tried all possible starting times and none of them work, then we exit
the nested loop.

Once we are out of the nested loop, we show the `TaskSplit`, and increment the iterator. We then continue to the next
iteration.

If the nested loop never ends up returning true, then we know that there is no valid schedule for the remaining
`TaskSplits`. In this case, we return false.

### Discussion

I believe the biggest source of optimization will come from smartly selecting the next time to place the TaskSplit. We
already have a heuristic controls which TaskSplit comes next, so the same thinking can be applied to controlling which
gap comes next. We can find a place to schedule where the gap is as small and close to the TaskSplit's duration as
possible. Any implementations need to be careful to not have finding a gap take too much time, and also the complexity
of handling how gap representations and their ordering (if used)
might change once a `TaskSplit` is placed

For example, if a `TaskSplit` is placed in the middle of a gap, we have to create two new gaps structures, fit that in
our gap collection in a smart way, etc. This problem doesn't happen with
`TaskSplits` because placing a `TaskSplit`
doesn't split up the `TaskSplit` and produce new `TaskSplits`.

Beyond backtracking, we can also study scheduling theory algorithms. I believe this problem can be described roughly as
$1 | r_j | unit penalty$ in scheduling notation.

Other, more minor ways to optimize include improving finding the next task split so that it isn't O (n) worst case.  