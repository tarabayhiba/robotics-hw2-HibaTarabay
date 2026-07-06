## What this is

A C++ Robot Fleet Manager built around an abstract `Robot` base class
(`MobileRobot`, `CleaningRobot`, `CookingRobot`) managed by a `Fleet`. 
The fleet tracks robots by id, runs `work()`/`charge()` on them, and queues `Task`s by priority
`main.cpp` manages it all through a text menu (add/remove
robots, work single/all, charge all, assign tasks, show the task queue, and run a robot's timed background work on its own thread).

## Compile & run

```
g++ -std=c++17 -Wall -Wextra src/*.cpp main.cpp -I include -o fleet_app
./fleet_app
```

## Problems I ran into

**`operator<<` only ever showed base `Robot` fields.** Since it's a
non virtual free function, `Fleet`'s printer (`os << *robot`) always resolved
to `Robot`'s version no matter the robot's real type — `speed_`,
`dirt_capacity_`, and `temperature_` never showed up when listing the fleet,
and the fields it did print had no labels. 
I fixed this by adding a `virtual std::string details() const` hook on `Robot` that each subclass overrides,
& having `operator<<` append it plus label every field

**Negative `speed`/`dirt_capacity`/`temperature` were silently accepted.**
`battery_` was already clamped to `[0, 100]` in the `Robot` constructor, but
the derived classes never did the same for their own fields, so a negative
value entered at the menu just got stored as is with no way to see it (see problem above) 
I fixed it by clamping each to `>= 0` in its constructor, the same
pattern `Robot` already used for `battery_`

**Timed work (`start_work`) looked like it never returned to the menu.**
`start_work` launches a background thread and returns immediately, so
`main()`'s loop reprints the menu and blocks on `Choice: ` right away but
the background thread keeps printing one status line per second underneath
that already printed prompt, with nothing marking when it's actually done.
It looked hung even though the program was still waiting for input the whole time the real prompt was just buried above the countdown output, not missing. I fixed it by printing a `"[id] timed work finished, back to main
menu."` line once the background loop ends, followed by a fresh `Choice: `
prompt, so there's something visible to type into near the bottom instead of it looking dead

