# Project Guidelines

## Build And Test

- Configure and build with CMake:
	- cmake -S . -B build
	- cmake --build build --config Release
- Run tests after build:
	- ctest --output-on-failure
- If tests are needed locally and not built by default, configure with WITH_TESTS enabled.

## Architecture

- Keep client and server concerns separated:
	- Client code lives under client/
	- Server code lives under server/
- Server command handling follows a command pattern:
	- Interface: server/commands/ICommand.hpp
	- Dispatch/factory: server/commands/commandHandling/
- Preserve model boundaries in server/models/:
	- User, Team, Channel, Thread, Reply, Message
- Keep low-level system and socket interactions in dedicated wrappers under server/sys/, server/net/, client/sys/, and client/network/.

## Conventions

- Follow repository C++ style from .clang-format:
	- Based on Google style
	- 2-space indentation
	- 80-column limit
- Keep includes sorted and avoid unrelated formatting changes.
- Prefer small, targeted changes that preserve existing public behavior and command protocol semantics.

## Subject-Critical Constraints

- Treat .github/subject.md as authoritative for protocol behavior and required features.
- Server must handle multiple clients with poll; do not introduce fork or threads.
- Keep security constraints from the subject intact:
	- Do not expose unauthorized data
	- Enforce team subscription/context permissions for operations and events
- Keep persistence behavior intact:
	- Server loads previous state at startup (if present)
	- Server saves state on shutdown (including Ctrl-C path)
- Respect logging-library requirements from subject and libs headers for output behavior.

## Code Review Guidance

- When performing code review, always check changes against .github/subject.md requirements first.
- Prioritize findings in this order:
	- Functional regressions against required commands/features
	- Security/authorization violations
	- poll/read-write event-loop correctness
	- Persistence and logging compliance

## Rules
- Every mistake becomes a rule
