# Assignment 2: Implementing Resource Management with RAII and Smart Pointers Using a Space Mission Analogy

## Objective

Create a C++ application that simulates a space mission control system using RAII principles and smart pointers (`std::unique_ptr`, `std::shared_ptr`, and `std::weak_ptr`). This assignment focuses on understanding resource management, ownership semantics, and the use of smart pointers in practical scenarios.

## Background

In a space mission, various resources like spacecraft, astronauts, and mission data are critical and need careful management. Mismanagement can lead to resource leaks or mission failure. By modeling this system, you will explore how RAII and smart pointers help manage resources efficiently and safely.

## Scenario Analogy

Imagine a space agency managing multiple spacecraft missions. Each spacecraft is a resource that must be carefully constructed, managed, and decommissioned. Astronauts are assigned to spacecraft, and mission control monitors the spacecraft without owning them. This setup provides a rich context for applying RAII and smart pointers.

## Requirements

### 1. Spacecraft Class

Implement a `Spacecraft` class with the following attributes:

- `name` (`std::string`)
- `crew_capacity` (`int`)
- `mission` (`std::string`)

Methods to implement:

- **Constructor**: Initializes the spacecraft with its name, crew capacity, and mission.
- **Destructor**: Outputs a message when the spacecraft is decommissioned.
- `launch()`: Outputs a message indicating the spacecraft has launched.
- `land()`: Outputs a message indicating the spacecraft has landed.
- `getName()`: Returns the spacecraft's name.
- `getCrewCapacity()`: Returns the spacecraft's crew capacity.

### 2. Astronaut Class

Implement an `Astronaut` class with the following attributes:

- `name` (`std::string`)
- `age` (`int`)
- `assigned_spacecraft` (`std::shared_ptr<Spacecraft>`)

Methods to implement:

- **Constructor**: Initializes the astronaut with a name and age.
- **Destructor**: Outputs a message when the astronaut is no longer in the system.
- `assignToSpacecraft(std::shared_ptr<Spacecraft>)`: Assigns the astronaut to a spacecraft.
- `releaseFromSpacecraft()`: Removes the astronaut's assignment.
- `getName()`: Returns the astronaut's name.
- `getAssignedSpacecraft()`: Returns the spacecraft the astronaut is assigned to.

### 3. MissionControl Class

Implement a `MissionControl` class that manages spacecraft and astronauts.

Attributes:

- `spacecrafts` (`std::vector<std::unique_ptr<Spacecraft>>`)
- `astronauts` (`std::vector<std::shared_ptr<Astronaut>>`)

Methods to implement:

- **Constructor**: Initializes mission control.
- **Destructor**: Ensures all resources are properly deallocated.
- `addSpacecraft(std::unique_ptr<Spacecraft>)`: Adds a new spacecraft.
- `addAstronaut(std::shared_ptr<Astronaut>)`: Adds a new astronaut.
- `assignAstronautToSpacecraft(const std::string& astronautName, const std::string& spacecraftName)`: Assigns an astronaut to a spacecraft.
- `launchSpacecraft(const std::string& spacecraftName)`: Launches a spacecraft.
- `decommissionSpacecraft(const std::string& spacecraftName)`: Decommissions a spacecraft.
- `displayStatus()`: Displays current status of all spacecraft and astronauts.
- `findSpacecraft(const std::string& spacecraftName)`: Finds a spacecraft by name.
- `findAstronaut(const std::string& astronautName)`: Finds an astronaut by name.
- `getAstronautCountOnSpacecraft(const std::string& spacecraftName)`: Returns the number of astronauts assigned to a spacecraft.

### 4. Monitoring System

Implement a monitoring system using `std::weak_ptr` that observes spacecraft without affecting their ownership.

Attributes:

- `monitored_spacecraft` (`std::weak_ptr<Spacecraft>`)

Methods:

- `monitorSpacecraft(std::shared_ptr<Spacecraft>)`: Sets the spacecraft to monitor.
- `displayStatus()`: Displays the status of the monitored spacecraft if it's still active.

### 5. Application

In your `Execute()` function:

- Create a `MissionControl` instance.
- Add several spacecraft and astronauts.
- Assign astronauts to spacecraft, demonstrating shared ownership.
- Launch and decommission spacecraft, demonstrating resource release.
- Use the monitoring system to observe spacecraft status without affecting ownership.

## Instructions

- Use **RAII principles** to manage resource allocation and deallocation.
- Use `std::unique_ptr` for spacecraft to ensure exclusive ownership by mission control.
- Use `std::shared_ptr` for astronauts who may share assignments to spacecraft.
- Use `std::weak_ptr` in the monitoring system to observe spacecraft without owning them.
- Ensure that all resources are properly released when they are no longer needed.
- Include comments in your code explaining how RAII and smart pointers are used.

## UML

```plaintext
+------------------------------------------------------------------------------+
|     MissionControl                                                           |
+------------------------------------------------------------------------------+
| - spacecrafts_: vector<unique_ptr<Spacecraft>>                               |
| - astronauts_: vector<shared_ptr<Astronaut>>                                 |
+------------------------------------------------------------------------------+
| + MissionControl()                                                           |
| + ~MissionControl()                                                          |
| + addSpacecraft(spacecraft: unique_ptr<Spacecraft>)                          |
| + addAstronaut(astronaut: shared_ptr<Astronaut>)                             |
| + assignAstronautToSpacecraft(astronautName: string, spacecraftName: string) |
| + launchSpacecraft(spacecraftName: string)                                   |
| + decommissionSpacecraft(spacecraftName: string)                             |
| + displayStatus()                                                            |
| + findSpacecraft(name: string): shared_ptr<Spacecraft>                       |
| + findAstronaut(name: string): shared_ptr<Astronaut>                         |
| + getAstronautCountOnSpacecraft(spacecraft: shared_ptr<Spacecraft>): int     |
+------------------------------------------------------------------------------+

              |
              | owns (unique_ptr)
              v

+----------------------------------------------------------------+
|      Spacecraft                                                |
+----------------------------------------------------------------+
| - name_: string                                                |
| - crew_capacity_: int                                          |
| - mission_: string                                             |
+----------------------------------------------------------------+
| + Spacecraft(name: string, crewCapacity: int, mission: string) |
| + ~Spacecraft()                                                |
| + launch()                                                     |
| + land()                                                       |
| + getName(): string                                            |
| + getCrewCapacity(): int                                       |
+----------------------------------------------------------------+

              ^
              | shared access (shared_ptr with custom deleter)
              |

+----------------------------------------------------------+
|                        Astronaut                         |
+----------------------------------------------------------+
| - name_: string                                          |
| - age_: int                                              |
| - assigned_spacecraft_: shared_ptr<Spacecraft>           |
+----------------------------------------------------------+
| + Astronaut(name: string, age: int)                      |
| + ~Astronaut()                                           |
| + assignToSpacecraft(spacecraft: shared_ptr<Spacecraft>) |
| + releaseFromSpacecraft()                                |
| + getName(): string                                      |
| + getAssignedSpacecraft(): shared_ptr<Spacecraft>        |
+----------------------------------------------------------+

              |
              | observes (weak_ptr)
              v

+---------------------------------------------------------+
|                  SpacecraftMonitor                      |
+---------------------------------------------------------+
| - monitored_spacecraft_: weak_ptr<Spacecraft>           |
+---------------------------------------------------------+
| + SpacecraftMonitor()                                   |
| + ~SpacecraftMonitor()                                  |
| + monitorSpacecraft(spacecraft: shared_ptr<Spacecraft>) |
| + displayStatus()                                       |
+---------------------------------------------------------+

```

## Explanation of RAII and Smart Pointers Usage

- **RAII Principle**: Each class manages its resources in its constructor and destructor, ensuring that resources are acquired and released properly.
- **`std::unique_ptr`**: Used for `Spacecraft` objects in `MissionControl` to ensure exclusive ownership and automatic deallocation when the spacecraft is decommissioned.
- **`std::shared_ptr`**: Used for `Astronaut` objects since they may be shared across different parts of the system, and multiple references may exist.
- **`std::weak_ptr`**: Used in `SpacecraftMonitor` to observe spacecraft without affecting their lifetime, preventing potential circular references and memory leaks.

## Notes

- Ensure all `#include` directives are added as necessary (e.g., `<iostream>`, `<memory>`, `<vector>`, `<algorithm>`, `<string>`).
- Pay attention to object lifetimes and ownership to prevent undefined behavior.
- The use of `std::shared_ptr<Spacecraft>(sc.get(), [](Spacecraft*) {});` provides a way to create a `std::shared_ptr` that does not manage the object's lifetime since `MissionControl` owns the spacecraft exclusively.
