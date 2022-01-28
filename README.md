# 🔥 HotPochato 🥔

---

## Who's Involved?
#### Anthony Nosaryev
_Pd 5_

---

## What is HotPochato?
Have you ever wanted to use a chat room, but *worse*?

Have you ever been *bothered* by *freedom*?

Have you ever started typing a message only for *somebody else* to send *theirs* before *you*, *advancing* the conversation *beyond your point*?

HotPochato is the revolutionary new-very-bad-not-great-exceptionally-boring chat server that provides chatters to speak in an orderly, snail-paced fashion!

---

## What is HotPochato Really?
So, this is very explicitly a different project from my initial proposal. For my initial proposal, I severely overestimated how easy it would be to make a *good* chat server that allows for multiple users. Such a chat room was always the goal (gimmick aside), and this strange approach is the closest I could get without resorting to overly-ludicrous methods.

I go over this in my DEVLOG, but my greatest difficulty with this project was constantly changing very crucial parts of my approach because things did not make sense. Eventually, I figured I had to use multithreading, something I was 99% unaware of until now (I can't say I've never heard the word).

I finished this on the 27th of January, 3 days after the deadline. All in all, I think I bit off more than I could chew with this one, but it's done now.

---

## What Systems Concepts Are (or could be) Involved?
1. **Allocating Memory:** I allocate memory for different threads of the same process to modify the same memory and to allow for memory sharing across processes. 
2. **Processes:** Much forking occurs.
3. **Pipes:** Pipes facilitate server to subserver communication.
4. **Sockets:** Sockets facilitate subserver to client communication.
5. **PThread:** (honorary) PThreading enables the server to concurrently connect clients and control chatting.

---
