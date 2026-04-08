# MyTeams Protocol — RFC
**Version:** 1.0
**Date:** March 2026
**Authors:** Loann, Yannis, Lukas

---

## 1. Abstract

The MyTeams Protocol (MTP) is a text-based, line-oriented client-server protocol built on top of TCP. It enables a collaborative communication application where users can exchange private messages and interact within teams, channels, threads, and replies. This document defines the exact message format, command set, response codes, event notifications, state machine, and security rules that both the server (`myteams_server`) and CLI client (`myteams_cli`) must implement.

---

## 2. Terminology

| Term | Definition |
|---|---|
| **Client** | A connected instance of `myteams_cli` |
| **Server** | The `myteams_server` binary managing all state |
| **UUID** | A universally unique identifier (RFC 4122 format) |
| **User** | An account identified by a UUID and a name |
| **Team** | A collaborative workspace containing channels |
| **Channel** | A topic-based space inside a team, containing threads |
| **Thread** | An initial post inside a channel, containing replies |
| **Reply** | A comment posted inside a thread |
| **Subscriber** | A user who has subscribed to a team's events |
| **Context** | The current team/channel/thread selected via `/use` |

---

## 3. Transport Layer

- **Protocol:** TCP
- **Encoding:** UTF-8
- **Message delimiter:** `\r\n` (carriage return + line feed)
- **Connection:** The client opens a TCP connection to the server. No handshake beyond TCP is required.
- **Disconnection:** If a client disconnects without `/logout`, the server treats it as an implicit logout and fires the appropriate events.

---

## 4. Field Limits

| Field | Max Length |
|---|---|
| `name` (user, team, channel) | 32 characters |
| `description` (team, channel) | 255 characters |
| `body` (thread, reply, message) | 512 characters |
| `title` (thread) | 32 characters |

Arguments exceeding these limits MUST be rejected with `400 BAD_REQUEST`.

---

## 5. Message Format

All messages are plain text, terminated by `\r\n`. Arguments containing spaces MUST be enclosed in double quotes.

### 5.1 Requests — Client → Server

```
COMMAND ["arg1"] ["arg2"] ...\r\n
```

Examples:
```
LOGIN "alice"\r\n
CREATE "my team" "a great team"\r\n
SEND "550e8400-e29b-41d4-a716-446655440000" "hello there"\r\n
```

### 5.2 Responses — Server → Client

Single-line response:
```
CODE [args...]\r\n
```

Multi-line response (for lists):
```
CODE-BEGIN TYPE\r\n
CODE DATA ...\r\n
CODE DATA ...\r\n
CODE-END TYPE\r\n
```

Examples:
```
200 OK\r\n
201 CREATED "550e8400-e29b-41d4-a716-446655440000" "my team" "a great team"\r\n
210-BEGIN TEAMS\r\n
210 "uuid1" "team1" "desc1"\r\n
210 "uuid2" "team2" "desc2"\r\n
210-END TEAMS\r\n
```

### 5.3 Events — Server → Client(s)

Events are pushed by the server to notify clients of actions they did not initiate.

```
EVENT TYPE ["arg1"] ["arg2"] ...\r\n
```

Examples:
```
EVENT USER_LOGGED_IN "uuid" "alice"\r\n
EVENT TEAM_CREATED "uuid" "my team" "desc"\r\n
EVENT PRIVATE_MESSAGE "sender_uuid" "hello there"\r\n
```

---

## 6. Response Codes

| Code | Name | Description |
|---|---|---|
| `200` | `OK` | Generic success |
| `201` | `CREATED` | Resource successfully created |
| `210` | `LIST` | One item in a list response |
| `400` | `BAD_REQUEST` | Malformed command or missing/invalid arguments |
| `401` | `UNAUTHORIZED` | Not logged in, or action not allowed |
| `403` | `FORBIDDEN` | Logged in but missing required subscription |
| `404` | `NOT_FOUND` | Requested UUID does not exist |
| `409` | `ALREADY_EXISTS` | Resource with this name already exists |

---

## 7. State Machine

A client progresses through states. Certain commands are only valid in certain states.

```
UNAUTHENTICATED
      │
      │  LOGIN "name"
      ▼
AUTHENTICATED
      │
      │  USE "team_uuid"
      ▼
IN_TEAM
      │
      │  USE "team_uuid" "channel_uuid"
      ▼
IN_CHANNEL
      │
      │  USE "team_uuid" "channel_uuid" "thread_uuid"
      ▼
IN_THREAD
```

- `USE` with no arguments resets to `AUTHENTICATED`
- `USE "team_uuid"` resets to `IN_TEAM`
- `USE "team_uuid" "channel_uuid"` resets to `IN_CHANNEL`
- All states except `UNAUTHENTICATED` are accessible after login

---

## 8. Commands Reference

### 8.1 LOGIN

**Syntax:** `LOGIN "user_name"\r\n`
**Valid in state:** `UNAUTHENTICATED`
**Description:** Authenticate as `user_name`. If the user does not exist, it is created.

| Outcome | Response |
|---|---|
| Success (user created) | `200 OK "uuid" "user_name"\r\n` |
| Success (user loaded) | `200 OK "uuid" "user_name"\r\n` |
| Already logged in | `401 UNAUTHORIZED\r\n` |

**Events fired:**
```
EVENT USER_LOGGED_IN "uuid" "user_name"\r\n  → all connected clients
```

---

### 8.2 LOGOUT

**Syntax:** `LOGOUT\r\n`
**Valid in state:** `AUTHENTICATED`, `IN_TEAM`, `IN_CHANNEL`, `IN_THREAD`

| Outcome | Response |
|---|---|
| Success | `200 OK\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

**Events fired:**
```
EVENT USER_LOGGED_OUT "uuid" "user_name"\r\n  → all connected clients
```

---

### 8.3 USERS

**Syntax:** `USERS\r\n`
**Valid in state:** `AUTHENTICATED` and above
**Description:** List all users registered on the server.

| Outcome | Response |
|---|---|
| Success | Multi-line `210` list |
| Not logged in | `401 UNAUTHORIZED\r\n` |

**Response format:**
```
210-BEGIN USERS\r\n
210 "uuid" "name" <0|1>\r\n
210-END USERS\r\n
```
Status: `0` = disconnected, `1` = connected.

---

### 8.4 USER

**Syntax:** `USER "user_uuid"\r\n`
**Valid in state:** `AUTHENTICATED` and above

| Outcome | Response |
|---|---|
| Success | `200 USER "uuid" "name" <0\|1>\r\n` |
| Not found | `404 NOT_FOUND "user_uuid"\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

---

### 8.5 SEND

**Syntax:** `SEND "user_uuid" "message_body"\r\n`
**Valid in state:** `AUTHENTICATED` and above
**Description:** Send a private message to another user.

| Outcome | Response |
|---|---|
| Success | `200 OK\r\n` |
| User not found | `404 NOT_FOUND "user_uuid"\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

**Events fired (if recipient is connected):**
```
EVENT PRIVATE_MESSAGE "sender_uuid" "message_body"\r\n  → recipient only
```

---

### 8.6 MESSAGES

**Syntax:** `MESSAGES "user_uuid"\r\n`
**Valid in state:** `AUTHENTICATED` and above
**Description:** List all private messages exchanged with a user.

| Outcome | Response |
|---|---|
| Success | Multi-line `210` list |
| User not found | `404 NOT_FOUND "user_uuid"\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

**Response format:**
```
210-BEGIN MESSAGES\r\n
210 "sender_uuid" <timestamp> "message_body"\r\n
210-END MESSAGES\r\n
```

---

### 8.7 SUBSCRIBE

**Syntax:** `SUBSCRIBE "team_uuid"\r\n`
**Valid in state:** `AUTHENTICATED` and above

| Outcome | Response |
|---|---|
| Success | `200 OK "user_uuid" "team_uuid"\r\n` |
| Team not found | `404 NOT_FOUND "team_uuid"\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

---

### 8.8 UNSUBSCRIBE

**Syntax:** `UNSUBSCRIBE "team_uuid"\r\n`
**Valid in state:** `AUTHENTICATED` and above

| Outcome | Response |
|---|---|
| Success | `200 OK "user_uuid" "team_uuid"\r\n` |
| Team not found | `404 NOT_FOUND "team_uuid"\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

---

### 8.9 SUBSCRIBED

**Syntax (list my teams):** `SUBSCRIBED\r\n`
**Syntax (list team subscribers):** `SUBSCRIBED "team_uuid"\r\n`

| Outcome | Response |
|---|---|
| Success (my teams) | Multi-line `210` list of teams |
| Success (team subscribers) | Multi-line `210` list of users |
| Team not found | `404 NOT_FOUND "team_uuid"\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

**Response format (my teams):**
```
210-BEGIN TEAMS\r\n
210 "uuid" "name" "description"\r\n
210-END TEAMS\r\n
```

**Response format (team subscribers):**
```
210-BEGIN USERS\r\n
210 "uuid" "name" <0|1>\r\n
210-END USERS\r\n
```

---

### 8.10 USE

**Syntax:** `USE ["team_uuid"] ["channel_uuid"] ["thread_uuid"]\r\n`
**Valid in state:** `AUTHENTICATED` and above
**Description:** Set the current context. Arguments are optional and positional.

| Outcome | Response |
|---|---|
| Success | `200 OK\r\n` |
| Team not found | `404 NOT_FOUND "team_uuid"\r\n` |
| Channel not found | `404 NOT_FOUND "channel_uuid"\r\n` |
| Thread not found | `404 NOT_FOUND "thread_uuid"\r\n` |
| Not subscribed to team | `403 FORBIDDEN\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

---

### 8.11 CREATE

**Syntax and behavior depend on the current context:**

| Context | Syntax | Creates |
|---|---|---|
| None | `CREATE "name" "description"\r\n` | Team |
| IN_TEAM | `CREATE "name" "description"\r\n` | Channel |
| IN_CHANNEL | `CREATE "title" "body"\r\n` | Thread |
| IN_THREAD | `CREATE "body"\r\n` | Reply |

**Create Team:**

| Outcome | Response |
|---|---|
| Success | `201 CREATED TEAM "uuid" "name" "desc"\r\n` |
| Already exists | `409 ALREADY_EXISTS\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

Events fired:
```
EVENT TEAM_CREATED "uuid" "name" "desc"\r\n  → all connected clients
```

**Create Channel:**

| Outcome | Response |
|---|---|
| Success | `201 CREATED CHANNEL "uuid" "name" "desc"\r\n` |
| Already exists | `409 ALREADY_EXISTS\r\n` |
| Team not found | `404 NOT_FOUND "team_uuid"\r\n` |
| Not subscribed | `403 FORBIDDEN\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

Events fired:
```
EVENT CHANNEL_CREATED "uuid" "name" "desc"\r\n  → subscribers of team
```

**Create Thread:**

| Outcome | Response |
|---|---|
| Success | `201 CREATED THREAD "uuid" "user_uuid" <timestamp> "title" "body"\r\n` |
| Already exists | `409 ALREADY_EXISTS\r\n` |
| Channel not found | `404 NOT_FOUND "channel_uuid"\r\n` |
| Not subscribed | `403 FORBIDDEN\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

Events fired:
```
EVENT THREAD_CREATED "uuid" "user_uuid" <timestamp> "title" "body"\r\n  → subscribers of team
```

**Create Reply:**

| Outcome | Response |
|---|---|
| Success | `201 CREATED REPLY "thread_uuid" "user_uuid" <timestamp> "body"\r\n` |
| Thread not found | `404 NOT_FOUND "thread_uuid"\r\n` |
| Not subscribed | `403 FORBIDDEN\r\n` |
| Not logged in | `401 UNAUTHORIZED\r\n` |

Events fired:
```
EVENT REPLY_CREATED "team_uuid" "thread_uuid" "user_uuid" "body"\r\n  → subscribers of team
```

---

### 8.12 LIST

**Behavior depends on the current context:**

| Context | Lists |
|---|---|
| None | All teams |
| IN_TEAM | Channels of current team |
| IN_CHANNEL | Threads of current channel |
| IN_THREAD | Replies of current thread |

**List Teams:**
```
210-BEGIN TEAMS\r\n
210 "uuid" "name" "description"\r\n
210-END TEAMS\r\n
```

**List Channels:**
```
210-BEGIN CHANNELS\r\n
210 "uuid" "name" "description"\r\n
210-END CHANNELS\r\n
```

**List Threads:**
```
210-BEGIN THREADS\r\n
210 "uuid" "user_uuid" <timestamp> "title" "body"\r\n
210-END THREADS\r\n
```

**List Replies:**
```
210-BEGIN REPLIES\r\n
210 "thread_uuid" "user_uuid" <timestamp> "body"\r\n
210-END REPLIES\r\n
```

Errors: same as `USE` (NOT_FOUND, FORBIDDEN, UNAUTHORIZED).

---

### 8.13 INFO

**Behavior depends on the current context:**

| Context | Displays |
|---|---|
| None | Currently logged-in user |
| IN_TEAM | Current team details |
| IN_CHANNEL | Current channel details |
| IN_THREAD | Current thread details |

**Responses:**
```
200 INFO USER "uuid" "name" <0|1>\r\n
200 INFO TEAM "uuid" "name" "description"\r\n
200 INFO CHANNEL "uuid" "name" "description"\r\n
200 INFO THREAD "uuid" "user_uuid" <timestamp> "title" "body"\r\n
```

---

## 9. Events Reference

| Event | Fired when | Recipients |
|---|---|---|
| `EVENT USER_LOGGED_IN "uuid" "name"` | User logs in | All connected clients |
| `EVENT USER_LOGGED_OUT "uuid" "name"` | User logs out or disconnects | All connected clients |
| `EVENT TEAM_CREATED "uuid" "name" "desc"` | Team is created | All connected clients |
| `EVENT CHANNEL_CREATED "uuid" "name" "desc"` | Channel is created | Subscribers of team |
| `EVENT THREAD_CREATED "uuid" "user_uuid" <ts> "title" "body"` | Thread is created | Subscribers of team |
| `EVENT REPLY_CREATED "team_uuid" "thread_uuid" "user_uuid" "body"` | Reply is created | Subscribers of team |
| `EVENT PRIVATE_MESSAGE "sender_uuid" "body"` | Private message sent | Recipient only (if connected) |

---

## 10. Security Rules

- A client in state `UNAUTHENTICATED` may ONLY send `LOGIN`. All other commands return `401 UNAUTHORIZED`.
- A client must be subscribed to a team to: create a channel, thread, or reply inside it; receive events from it.
- A client that is not subscribed receives `403 FORBIDDEN` when trying to `USE` a team or create resources within it.
- There is no password authentication.

---

## 11. Full Exchange Examples

### Login and create a team

```
C: LOGIN "alice"\r\n
S: 200 OK "550e8400-e29b-41d4-a716-446655440000" "alice"\r\n
S→ALL: EVENT USER_LOGGED_IN "550e8400-e29b-41d4-a716-446655440000" "alice"\r\n

C: CREATE "dev team" "our dev workspace"\r\n
S: 201 CREATED TEAM "661f9511-f3ac-52e5-b827-557766551111" "dev team" "our dev workspace"\r\n
S→ALL: EVENT TEAM_CREATED "661f9511-f3ac-52e5-b827-557766551111" "dev team" "our dev workspace"\r\n
```

### Subscribe and create a channel + thread + reply

```
C: SUBSCRIBE "661f9511-f3ac-52e5-b827-557766551111"\r\n
S: 200 OK "550e8400-..." "661f9511-..."\r\n

C: USE "661f9511-f3ac-52e5-b827-557766551111"\r\n
S: 200 OK\r\n

C: CREATE "general" "general discussion"\r\n
S: 201 CREATED CHANNEL "772g0622-..." "general" "general discussion"\r\n
S→SUBSCRIBERS: EVENT CHANNEL_CREATED "772g0622-..." "general" "general discussion"\r\n

C: USE "661f9511-..." "772g0622-..."\r\n
S: 200 OK\r\n

C: CREATE "First thread" "hello everyone"\r\n
S: 201 CREATED THREAD "883h1733-..." "550e8400-..." 1711708800 "First thread" "hello everyone"\r\n
S→SUBSCRIBERS: EVENT THREAD_CREATED "883h1733-..." "550e8400-..." 1711708800 "First thread" "hello everyone"\r\n

C: USE "661f9511-..." "772g0622-..." "883h1733-..."\r\n
S: 200 OK\r\n

C: CREATE "this is my reply"\r\n
S: 201 CREATED REPLY "883h1733-..." "550e8400-..." 1711708900 "this is my reply"\r\n
S→SUBSCRIBERS: EVENT REPLY_CREATED "661f9511-..." "883h1733-..." "550e8400-..." "this is my reply"\r\n
```

### Private message

```
C: SEND "994i2844-..." "hey, how are you?"\r\n
S: 200 OK\r\n
S→RECIPIENT: EVENT PRIVATE_MESSAGE "550e8400-..." "hey, how are you?"\r\n
```

### Error example

```
C: USE "nonexistent-uuid"\r\n
S: 404 NOT_FOUND "nonexistent-uuid"\r\n

C: CREATE "body of reply"\r\n   (without being subscribed)
S: 403 FORBIDDEN\r\n
```
