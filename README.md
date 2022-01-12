# 🔒 SecureChat 🔑
_(tentatively)_

---

## Who's Involved?
#### Anthony Nosaryev
_Pd 5_

---

## What is SecureChat?
SecureChat is the **most secure** chatroom service available.
Gone are the days of primitive servers cowering behind measly passwords.
Gone is the dissatisfaction of a boring user experience.

SecureChat enables chatroom hosts to create time-limited*<sup>1</sup>, customisable*<sup>2,3</sup> and flexible*<sup>4,5</sup> security questions to prevent unwanted _types_ of users from accessing their services.

\*<sup>1</sup>: if I have the time and ability

\*<sup>2</sup>: customisability includes answer length*<sup>3</sup>, case sensitivity*<sup>3</sup> and more*<sup>3</sup>

\*<sup>3</sup>: if I have the time and ability

\*<sup>4</sup>: allowing for multiple answers, either by specific declaration*<sup>5</sup> or criteria checks*<sup>5</sup>

\*<sup>5</sup>: if I have the time and ability

## What is SecureChat Really?
For this project, I decided to make a chat room with a highly customisable password system.

I intend to implement a server with a TCP socketted connection as we have already done.

Paired with the server would be a file containing the security prompts, answers and surrounding customisation options.
Documentation for how this file is structured will be provided (possibly in the README), and I believe that this file will require manual customisation.

Clients would connect to the server's listening socket, and upon establishing a connection, be prompted with security questions.
After answering correctly, they will gain access to the chat room (or, upon answer incorrectly, lose their connection to the server).

I anticipate that creating a functioning chat room will be more difficult for me than the security system because I am not very confident in my ability to connect multiple clients to the same server, but at the very least I hope to create a functioning chat room.

## What Systems Concepts Are (or could be) Involved?

1. **Allocating Memory:** The socket process involves dynamic memory, and a stretch goal of mine is to enable users to alter security questions from the chat using commands, which would certainly involve more memory allocation.
2. **Working With Files:** Security questions, answers and settings will be stored in one or multiple files and read after connection establishment.
3. **Processes:** I believe that processes will still be involved despite seeking to connect my clients to the same server, though I am not entirely sure.
4. **Semaphores:** As another stretch goal, I could implement semaphores to regulate the chatting process.
5. **Sockets:** Duh

## Expected Progress Timeline

This is difficult to estimate because I do not know when this project is due (apologies if this was mentioned in class at any point).

Nonetheless, I hope for the following:

- **17 Jan:** Functioning base chat service

- **18 Jan:** Security file outline planned

- **21 Jan:** Security implemented after client-server connection

**Any remaining time:** Bonus security options, semaphores and/or chat-based security alteration
