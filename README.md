*This project has been created as part of the 42 curriculum by cyakisan.*

# Codexion

## Description

Codexion is a concurrency simulation written in C with POSIX threads.
Several coder threads work around a circular table and share one dongle with
each neighbouring coder. A coder must acquire both adjacent dongles before
compiling.

After compiling, a coder releases the dongles, debugs, and refactors. It then
requests both dongles again. Every coder must start a new compilation before
its burnout deadline. The simulation stops when either:

- one coder burns out; or
- every coder has completed the requested number of compilations.

The project focuses on thread synchronization, resource arbitration,
condition variables, cooldown periods, deadline monitoring, and serialized
logging.

## Features

- One POSIX thread per coder.
- One dongle between every pair of neighbouring coders.
- Circular neighbour relationships: coder `1` is next to coder `N`.
- Two dongles required simultaneously for compilation.
- Configurable compilation, debugging, refactoring, burnout, and cooldown
  durations, expressed in milliseconds.
- FIFO scheduling based on request order.
- EDF scheduling based on the earliest burnout deadline.
- A manually implemented bounded request queue for each dongle.
- A dedicated monitor thread for burnout detection.
- Serialized state-change logging.
- Clean shutdown and mutex/thread cleanup.

## Instructions

### Requirements

- A C compiler compatible with `cc`.
- POSIX threads.
- `make`.
- A Unix-like environment.

The project does not use libft or a third-party library.

### Compilation

Build the executable with:

```sh
make
```

The Makefile uses:

```text
-Wall -Wextra -Werror -pthread
```

Useful targets are:

```sh
make all
make clean
make fclean
make re
```

The executable is named `codexion`.

### Execution

Run the program with exactly eight arguments:

```sh
./codexion number_of_coders time_to_burnout time_to_compile \
    time_to_debug time_to_refactor number_of_compiles_required \
    dongle_cooldown scheduler
```

All time values are expressed in milliseconds.

The scheduler must be exactly one of:

- `fifo`: serve requests in arrival order;
- `edf`: serve the request with the earliest burnout deadline.

Example:

```sh
./codexion 5 3000 200 200 200 10 800 fifo
```

Another example using EDF scheduling:

```sh
./codexion 6 2000 500 100 200 3 300 edf
```

The parser rejects missing arguments, non-numeric values, values outside the
supported integer range, and scheduler names other than `fifo` and `edf`.

### Output format

Every state change is printed as:

```text
timestamp_in_ms coder_id message
```

Possible messages are:

```text
X has taken a dongle
X is compiling
X is debugging
X is refactoring
X burned out
```

For every compilation, two `has taken a dongle` messages are printed before
the `is compiling` message.

Example:

```text
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
200 1 is debugging
400 1 is refactoring
```

## Technical choices

### Coder lifecycle

Each coder repeatedly follows this state sequence:

```text
COMPILING -> DEBUGGING -> REFACTORING -> COMPILING
```

The coder thread stops when the monitor sets the shared simulation stop flag
or when the coder has completed all required compilations.

### Request queues and scheduling

Each dongle owns a small request queue protected by its own mutex. Requests
contain the coder identifier and the timestamp of its previous compilation.

For FIFO scheduling, requests remain in arrival order. For EDF scheduling,
the request with the earliest deadline is placed first, where the deadline is
derived from the coder's last compilation start and burnout duration. The
queue is implemented directly in the project; no standard-library priority
queue is used.

### Time management

Simulation timestamps are based on `gettimeofday()` and represented in
milliseconds. A dongle records its last usage time. Before a new acquisition,
the cooldown interval is checked so that a released dongle cannot be reused
too early.

## Blocking cases handled

### Deadlock prevention

Each coder acquires its two dongle mutexes in a consistent identifier order.
This prevents circular lock acquisition between neighbouring coders and
removes the circular-wait condition from the classic Coffman deadlock
conditions.

Heap mutexes are also acquired in the same dongle order when a request queue
is inspected or updated.

### Cooldown handling

After compilation, both dongles receive the same last-usage timestamp.
Subsequent requests wait until the configured cooldown has elapsed before
compilation can begin.

### Starvation and scheduling fairness

Every request is inserted into both dongle queues. A coder can compile only
when its request is eligible in both queues, so one dongle cannot grant access
without the other. FIFO preserves arrival order, while EDF gives precedence to
the earliest deadline.

### Burnout detection

A separate monitor thread checks each unfinished coder's deadline. When a
deadline is reached, it sets the shared stop state, wakes waiting coder
threads through the condition variable, and emits the burnout message.

### Log serialization

All output is protected by the shared logging mutex. A complete log line is
written while holding that mutex, preventing messages from different threads
from interleaving.

### Shutdown

The main thread joins every coder thread and then joins the monitor thread.
Allocated coder and dongle arrays, condition variables, and initialized
mutexes are released during cleanup.

## Thread synchronization mechanisms

### `pthread_mutex_t`

The project uses mutexes for distinct shared states:

- a shared logging mutex protects complete output lines;
- a shared state mutex protects simulation start and stop state;
- a condition mutex protects condition-variable coordination;
- one mutex per dongle protects dongle ownership and compilation access;
- one mutex per dongle queue protects request insertion and removal;
- per-coder mutexes protect compilation counters and compilation timestamps;
- a boolean-state mutex protects the availability flag used by dongle
  availability checks.

The two dongle mutexes are acquired in ascending dongle identifier order.
This ordering is used consistently to avoid lock cycles.

### `pthread_cond_t`

All coder threads share a condition variable. A coder waits while its request
is not eligible, while one of its dongles is unavailable, or while the
simulation is still waiting for another request to be served.

When a compilation releases its dongles, the owning thread broadcasts on the
condition variable so waiting coders can re-evaluate their conditions. The
monitor also broadcasts when a burnout stops the simulation, ensuring that
waiting threads can leave instead of sleeping forever.

The condition variable is always used with a loop: waking up only means that
the state may have changed, so the coder checks its scheduling conditions
again before proceeding.

### Custom event implementation

This project does not use a separate custom event library. The event/wakeup
mechanism is implemented with the shared `pthread_cond_t`, the condition
mutex, the shared stop flag, and broadcasts performed after resource-state
changes.

### Race-condition examples

- A request queue cannot be modified while another thread is inspecting or
  removing a request because the queue mutex is held.
- A coder cannot compile while another coder owns one of its dongles because
  the dongle mutex is held for the compilation period.
- Log messages cannot overlap because every complete message uses the shared
  logging mutex.
- The monitor and coder threads communicate through the protected stop state
  and condition-variable broadcasts.

## Resources

### Documentation and references

- `pthread_create`, `pthread_join`, and POSIX threads:
  <https://man7.org/linux/man-pages/man7/pthreads.7.html>
- POSIX mutexes:
  <https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html>
- POSIX condition variables:
  <https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html>
- POSIX `gettimeofday`:
  <https://man7.org/linux/man-pages/man2/gettimeofday.2.html>
- cppreference overview of condition variables:
  <https://en.cppreference.com/w/c/thread/condition>
- Operating-system scheduling concepts:
  <https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched.pdf>

### AI usage

AI tools were used as an assistance and review resource, not as a substitute
for understanding or testing the project. They were used for:

- clarifying POSIX mutex and condition-variable behavior;
- reviewing possible deadlock and race-condition paths;
- comparing FIFO and EDF scheduling logic;
- suggesting README structure and checking that the required sections were
  present;
- helping formulate targeted runtime tests.

All generated suggestions were reviewed against the source code, compiled
with the project Makefile, and adapted to the project's actual data
structures and synchronization model.

## Project layout

| File or group | Responsibility |
| --- | --- |
| `codexion.c` | Program entry point and lifecycle orchestration |
| `parsing*.c`, `parsing.h` | Argument validation and object initialization |
| `structures.h` | Configuration and runtime data structures |
| `heap_utils.c` | Request creation and queue operations |
| `simulation*.c` | Thread creation, initialization, and simulation setup |
| `simulation_actions.c` | Compile, debug, and refactor actions |
| `simulation_utils*.c` | Timing, synchronization, logging, and state helpers |
| `monitoring.c` | Deadline monitoring and burnout detection |
| `memory_management*.c` | Allocation and cleanup |
| `Makefile` | Build and cleanup rules |

## License

This project is provided for educational purposes as part of the 42
curriculum.
