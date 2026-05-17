# Multithread Mayhem

> Learn concurrency by breaking programs. Learn operating systems by building threads inside one.

## Overview

**Multithread Mayhem** is a hands-on systems programming project that introduces the core ideas behind **threads, concurrency, synchronization, scheduling, and kernel-level thread implementation**.

The project is divided into two connected halves:

1. **User-space concurrency with POSIX threads (pthreads)**
   You will learn how multiple threads run together, why race conditions happen, and how synchronization tools like mutexes and condition variables solve real problems.

2. **Kernel-level thread implementation in xv6**
   You will explore the internals of a small operating system and implement your own thread support inside the xv6 kernel.

No prior Operating Systems course is required. The project starts from the basics and builds up step by step.

---

## Why this project?

Most people use threads without ever seeing what happens underneath. This project is designed to make concurrency feel real.

By the end, mentees will be able to:

* explain the difference between a process and a thread
* identify and reproduce race conditions
* write thread-safe programs using synchronization primitives
* build and test multithreaded applications
* understand how an operating system schedules work
* modify xv6 and implement kernel-level threads

---

## Prerequisites

You only need:

* basic knowledge of **C programming**
* comfort with compiling and running programs from the terminal
* curiosity and willingness to experiment

---

## Project Timeline

| Week | Topic                        | Focus                                                       |
| ---- | ---------------------------- | ----------------------------------------------------------- |
| 1    | Threads & Concurrency Basics | Processes vs threads, shared memory, race conditions        |
| 2    | Synchronization              | Mutexes, locks, condition variables, thread-safe structures |
| 3    | Pthreads API                 | Thread lifecycle, creation, joining, classic problems       |
| 4    | xv6 Setup                    | OS basics, user vs kernel space, codebase exploration       |
| 5    | System Calls                 | Traps, syscall flow, adding a custom syscall                |
| 6    | Processes & Scheduling       | PCB, scheduler basics, locks in xv6                         |
| 7    | Thread Design                | Thread structure, stacks, context switching                 |
| 8    | Thread Implementation        | thread_create, thread_join, running programs on xv6         |

---

## Milestones

The project is organized around five major checkpoints:

1. **Basic multithreaded programs working**

2. **Thread-safe programs using synchronization**

3. **A complete multithreaded application with measurable speedup**

4. **xv6 set up with a custom system call added and tested**

5. **Threads successfully implemented and running inside xv6**

---

## How the project works

Each week is built in the same style so learning stays consistent:

* **Theory notes** explain the core ideas from scratch
* **Demo programs** show the concept in action
* **Guided exercises** aid in practice
* **A weekly submission** checks understanding

This project is meant to be learned by doing, not by reading alone.
Hope you have fun working on this project :)
---
