# Preemptive Multitasking OS Simulator

## Scheduling Algorithms:

- **RR**: Round Robin
- **SRTF**: Shortest Remaining Time First
- **PRIO**: Preemptive Priority
- **PRIOe**: Priority with Aging (dynamic priority adjustment)

## Configuration File Specification

### 1. System Header (Line 1)

Defines the global kernel parameters.
`<ALGORITHM>; <QUANTUM>; [<AGING_FACTOR>]`

### 2. Task Descriptor (Line 2...N)

Each subsequent line represents a Process/Task to be loaded into the Ready Queue.
`<ID>; <COLOR>; <ARRIVAL>; <BURST>; <PRIORITY>; [<EVENT_STREAM>]`

### 3. Event Stream Syntax

Events are triggers that interrupt the CPU flow. All timestamps are relative to the process start time (logical clock of the process), not the global system clock.

#### 3.1. A. Mutex Operations

Implements binary semaphore behavior.

- Lock Request: `ML<mutex_id>:<time>`
- Unlock Release: `MU<mutex_id>:<time>`

#### 3.2. I/O Operations

Simulates hardware interrupts and blocking calls.

Syntax: `IO:<start_time>-<duration>`

> Note: Durations are relative to the moment the I/O request is issued.

## Example Configuration Payload

```
PRIO;2
t01;#FF0000;0;2;6;IO:2-1;IO:3-2
t02;#00FF00;0;3;1;IO:3-1
t03;#0000FF;5;5;2
t04;#FFFFFF;6;9;8;IO:2-1
t05;#000000;4;6;3;IO:2-1
```
