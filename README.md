# 🔥 HotPochato 🥔
Not quite ready yet, but there's a groundwork, and I'm willing to work more

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

I am not done. I feel close to being done, but I don't know how many more errors may pop up, and this can certainly be optimised much more. As it is now, most of it seems to be operating as intended but there may be some read/write misplacement. The system works by having a thread of the server choose the next subserver, which will send its PID to its respetcive client to enable their messaging ability. Sometimes, this gets heaped in with the wrong read/write, causing it to not send at all or send at the end of a regular chat message.

If you will allow me, I will keep working on this and inform you when this is in better standing. I understand that the deadline is 24 Jan and no assignments are allowed after that date, but I would still appreciate the opportunity to improve this submission.

All in all, I think I bit off more than I could chew with this one, but I'd still like to see it through.

---

## What Systems Concepts Are (or could be) Involved?
1. **Allocating Memory:** I allocate memory for different threads of the same process to modify the same memory and to allow for memory sharing across processes. 
2. **Processes:** Much forking occurs.
3. **Pipes:** Pipes facilitate server to subserver communication.
4. **Sockets:** Sockets facilitate subserver to client communication.
5. **PThread:** (honourary) PThreading enables the server to concurrently connect clients and control chatting.

---
