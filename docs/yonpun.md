# Goals and Constraints

## Time representation

Adhering to the name of this program, Yonpun (i.e. quarters of an hour), all
dates usable by this program have intervals of 15 minutes between them. Unix
time underlies date representation in this program, and so date handling quirks
(e.g. leap seconds, length of the day) is shared with Unix time. The only
difference is epoch and precision of this time.

All dates are in **Yotsuba time**. All dates are represented by a **signed**
**long that is at least 32-bits in width** that represents the amount of 15
minute intervals since/before the **Yotsuba epoch**, defined as 1043107200 unix
time seconds since the Unix epoch. A "15 minute interval" is defined as 900 unix
time seconds.

This lets Yotsuba time represent about 60 thousand years before and after the
Yotsuba epoch. The program does not expect values larger than the maximum

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

- Cannot be scheduled before the due date of the previous iteration of the same
  repeating task

- Missed iterations of repeating tasks "accumulate", i.e. they do not get
  deleted after the due date is passed, do not get merged with the next
  iteration of the task, and remains on the to do list.

Tasks can be scheduled so that they are "split", e.g. not done consecutively.
the minimum split size states how short each split can be. by default, the split
size is -1

## Off Time

Periods of time where tasks cannot be scheduled

All off-times have a name, start date, duration, and is_buffer quality

OffTimes can either be repeating or once

Once off times

- Happen only once

Repeating off times

- Repeat every X number of days

The `is_buffer` quality lets the computer schedule tasks in this off time _if_
_necessary_, i.e. if tasks cannot be completed before their due date while also
respecting the off times

## Output

result is a calendar that contains both off times and when to do tasks

when presented to the user, date times must be in local time and presentation
should adhere to the locale of the user (e.g. time zone, date presentation)

otherwise, if final internal represenation right before user presentation, dates
may be in Yotsuba time.

# Design

## Representing tasks, off time, and persistence

Persistence is similar to Active Record pattern

A single Task object per row that lives the entire lifetime of the program

Task objects live on the heap, and are kept track by a `repo_vector` of
shared_ptrs to the Tasks on the heap

Users then get weak_ptrs that they can turn into shared_ptrs

Task objects then have a instance method save, which saves the data to db. Save
updates the db for all fields of that row, but not all objects.

Same thing for offtimes

Have a different class for Repeating/Once; don't try to combine them or do
inheritence or anything

### Justification, discussion

No stale objects, bc only one object = one db row

Minimal queries to db, by using save to save all fields at once

Storing objects all in memory might take up a lot of space, but likely won't
have a lot of tasks/off times to merit concern

Can always switch to cache or smth later

Handing out weak_ptrs so that every time you try to read and write you have to
check if the object still exists, which is good

> std::weak_ptr models temporary ownership: when an object needs to be accessed
> only if it exists, and it may be deleted at any time by someone else
>
> https://en.cppreference.com/cpp/memory/weak_ptr

e.g. suppose a function takes in a task, then gives it to a nested function call
that may or may not delete the object. if it was just a shared pointer, then you
may not know the nested call deleted the object and then you end up manipulating
something that should be deleted but runs silently. if you have raw pointers,
then you try to manipulate a dangling pointer, which hopefully ends in a crash

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

## Calendar representation

Calendar is some data structure that

- Given a time, tells you what to do at that time
- Given a task/offtime, tells you when you need to do it
- Be able to print out some representation of it (e.g. text for MVP, UI,
  ics/calendar app integration eventually)

As an alias, `CalendarEvent` is typedef for
`std::variant<weak_ptr<OnceTask>, ... , weak_ptr<RepeatingOffTime>>`

Public accessor methods of Calendar include:

- `CalendarEvent get_event_at_time(YotsubaTime time)`
- `std::vector<YotsubaTime> get_times_of_event(CalendarEvent event)`

As well as a factory method that takes in vector of shared_ptrs to OnceTasks,
..., and RepeatingOffTimes and returns a Calendar.

For the time --> task/offtime direction, use a vector. the vector only lasts
from now to the last task/offtime or 30 days, which ever is greater. memory use
of vector shouldn't be too bad, but if it is a problem then we'll need to fix it
later. the vector should be coupled with data about what time the 0th index
block is. will be abstracted away by methods that go with a calendar.

For the task/offtime direction, use a map. The keys should be a weak_ptr to a
Task object, and output should be yotsuba times of washen its done

Finally, text representation would just loop through the time --> task/offtime
vector and print out everything

## Scheduling algorithm

> This is incomplete. The to-do right now is this get this thing figured out.
>
> Algorithm 1: Take all the tasks, and split them up into splits. Sort splits by
> due date first, then size first. Then, recurse through the splits. Put the
> split into the next space. If it doesn't work, try the next split
>
> Algorithm 1 also but i overcomplicated it lmao: We first find all the blank
> spaces. We loop through blank spaces, earliest blank space first. We then find
> the largest split of a task that also has the earliest due date that fits
> within the blank space. Put it into the blank space, go onto the next blank
> space. Repeat until we don't have any splits (in which case we're done), or if
> we still have > tasks but don't have any small enough to fit in the space
> (failure). If failure, recurse back and try SAME duedate but smaller split
> size.
>
> But the second part might be good bc we might not need a giant vector

Scheduling algorithm gets a vector of tasks & offtimes and returns a calendar

Scheduling algorithm may create its own representations of the tasks and
offtimes; these must be internal to the scheduling "unit" and should not leak
out

Algorithm for now should be constraints programming (?)

Start by identifying the last occurance of OnceTask or OnceOffTime

Create a vector that lasts from now to the last occurance. If now is not an
exact interval of 15 minutes, then use the next 15 minute interval as the start.
The vector holds
`std::variant<std::weak_ptr<OnceTask>, std::weak_ptr<RepeatingTask>, std::weak_ptr<OnceOffTime>, std::weak_ptr<RepeatingOffTime>>`.

Fill in the schedule with Blocks that point to OffTimes

....

Then, create a vector of `TaskSplit` structs. They have a weak_ptr to a
Once/Repeating Task, as well as the size in 15 min intervals as an
`unsigned int`. The point of the `TaskSplit` is to ensure that the minimum split
size is respected for each task. `TaskSplit`s are created from `OnceOffTime` and
`RepeatingTask` shared pointers and their information. `TaskSplit` only store
their size as

Sort of the vector of TaskSplits by due date first, then size of task split
within the due date, in increasing order, i.e. earliest due date then
