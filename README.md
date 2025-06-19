# **ft_irc – Internet Relay Chat Server**

## 📘 **Summary**

A simple **Internet Relay Chat (IRC)** client implemented in **C++98**. The goal of this project is to provide a basic yet functional IRC client that allows users to connect to IRC servers, join channels, send and receive messages, and interact with other users in real-time.

---

> [!NOTE]
> to test ft_irc run the tester:
> ```bash
> # run the executable before running the tester
> ./ircserv <PORT> > <yourpassword>
> ```
> ```bash
> ./irc_test.sh <PORT> > <yourpassword>
> ```


## 🧱 **Requirements**

### General

* No crashes or unexpected quits.
* Must include a Makefile with rules: `$(NAME)`, `all`, `clean`, `fclean`, `re`.
* Must compile with: `c++ -Wall -Wextra -Werror -std=c++98`.
* Use C++ equivalents of C functions where possible.
* No external libraries (including Boost).
* No forking. All I/O must be **non-blocking**.
* Must use exactly **one** `poll()` (or equivalent) to handle all I/O.
* Communication must happen over **TCP/IP (IPv4 or IPv6)**.

---

## 🛠️ **Build and Run**

* Executable name: `ircserv`
* Run as:

  ```bash
  ./ircserv <port> <password>
  ```
* Acceptable source files:

  * `.cpp`, `.h`, `.hpp`, `.tpp`, `.ipp`, optional config file

---

## 🔌 **Networking Functions Allowed**

You can use:
Everything in C++ 98.
`socket`, `close`, `setsockopt`, `getaddrinfo`, `freeaddrinfo`, `bind`, `connect`, `listen`, `accept`, `htons`, `ntohs`, `send`, `recv`, `signal`, `fcntl`, `getsockname`, `getprotobyname`, `gethostbyname`, `htonl`, `ntohl`, `sigaction`, `lseek`, `fstat`, `poll` (or equivalent).

---

## 🧑‍🤝‍🧑 **Required Features**

Your server must:

* Support **multiple clients** simultaneously.
* Allow clients to:

  * Authenticate (with password)
  * Set nickname and username
  * Join channels
  * Send/receive private and channel messages
* Forward all messages sent in a channel to all other participants.
* Distinguish between **operators** and **regular users**.

### IRC Commands to Implement

* **User commands**:
  * `NICK`, `USER`, `JOIN`, `PRIVMSG`, etc.
* **Channel operator commands**:
  * `KICK`: Eject user from channel
  * `INVITE`: Invite a user to a channel
  * `TOPIC`: View or change channel topic
  * `MODE` (with sub-modes):
    * `i`: Invite-only channel
    * `t`: Topic can only be changed by ops
    * `k`: Channel key (password)
    * `o`: Give/take operator status
    * `l`: User limit for the channel

---

## 📌 **Implementation Tips**

* Handle partial reads/writes (e.g. messages arriving in segments).
* Parse and rebuild full commands before processing.
* The reference client should connect and work without errors, just like with a real IRC server.

---

## 🎁 **Bonus (only if mandatory part is perfect)**

Optional features:

* File transfers
* IRC bot

---

## ✅ **Test Example (Manual Test)**

Using `netcat`:

```bash
nc -C 127.0.0.1 6667
```

Type input like:

```
com
man
d
```
(using `Ctrl+D` to simulate broken-up message delivery).

---

## Useful Links

- [Beej's Guide to Network Programming (Simple Stream Server)](https://beej.us/guide/bgnet/html/split/client-server-background.html#a-simple-stream-server)
- [IRCDocs – Servers](https://modern.ircdocs.horse/#servers)
- [IRCDocs – ERR_NOMOTD (422)](https://modern.ircdocs.horse/#errnomotd-422)
- [RFC 1459 – Internet Relay Chat Protocol (RFC Editor)](https://www.rfc-editor.org/rfc/rfc1459.html)
- [IRCDocs – USER Message](https://modern.ircdocs.horse/#user-message)
- [University of Chicago CHIRC – IRC Examples](http://chi.cs.uchicago.edu/chirc/irc_examples.html)
- [IRCDocs – CAP Message](https://modern.ircdocs.horse/#cap-message)
- [Irssi GitHub Repository](https://github.com/irssi/irssi)


check the user name it should not have characyer ,   the printed messages, joining chaneel with a key check, join zero should remove the client from all channels, if we did TOPIC #channelname: it should show the message without a topic, which docs did we follow, who will be the operator if the first one left, for the key it should be cannot join +K, check if there is a key after the channel it should apply it as key not just after changing the key to new one, number provided with the error messages, KICK multiple users from one channel
