#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Spacecraft {

public:
    Spacecraft(std::string _name, int _crew_capacity, std::string _mission) :
        name(_name),
        crew_capacity(_crew_capacity),
        mission(_mission)
    {
        std::cout << "Spacecraft " + name + "constructed for mission " + mission << std::endl;
    }

    ~Spacecraft()
    {
        std::cout << "Spacecraft " + name + " decommisioned." << std::endl;
    }

    void launch()
    {
        std::cout << "Spacecraft launched" << std::endl;
    }

    void land()
    {
        std::cout << "Spacecraft " + name + " landed." << std::endl;
    }

    std::string getName()
    {
        return name;
    }

    int getCrewCapacity()
    {
        return crew_capacity;
    }

    
private:
    std::string name;
    int crew_capacity;
    std::string mission;
};

class SpacecraftMonitor {
public:

    SpacecraftMonitor() {}
    ~SpacecraftMonitor() {}

    void monitorSpacecraft(std::shared_ptr<Spacecraft> spacecraft)
    {
        monitored_spacecraft_ = spacecraft;
    }


    void displayStatus() {
        if (auto sc = monitored_spacecraft_.lock()) {
            std::cout << "Monitoring Spacecraft " << sc->getName() << " is active.\n";
        }
        else {
            std::cout << "Monitored spacecraft is no longer active.\n";
        }
    }
private:
    std::weak_ptr<Spacecraft> monitored_spacecraft_;

};

class Astronaut {
public:
    Astronaut(std::string _name, int _age) :
        name(_name),
        age(_age)
    {
        std::cout << "Astronaut " + name + " registered." << std::endl;
    }
    ~Astronaut()
    {
        std::cout << "Astronaut " + name + " has left the system." << std::endl;
    }

    void assignToSpacecraft(std::shared_ptr<Spacecraft> spacecraft)
    {
        assigned_spacecraft = spacecraft;
        std::cout << "Astronaut " + name + " assigned to spacecraft " + assigned_spacecraft->getName() << std::endl;
    }

    void releaseFromSpacecraft()
    {
        if (assigned_spacecraft != nullptr)
        {
            std::cout << "Astronaut " + name + " released from spacecraft " + assigned_spacecraft->getName() << std::endl;
            assigned_spacecraft.reset();
        }
    }

    std::string getName() { return name; }

    std::shared_ptr<Spacecraft> getAssignedSpacecraft() { return assigned_spacecraft; }
private:

    std::string name;
    int age;
    std::shared_ptr<Spacecraft> assigned_spacecraft;
};

class MissionControl {
public:
    MissionControl() { std::cout << "Mission Control Initialized." << std::endl; }
    ~MissionControl() { std::cout << "Mission Control shutting down." << std::endl; }

    void addSpacecraft(std::unique_ptr<Spacecraft> Spacecraft)
    {
        spacecrafts.push_back(std::move(Spacecraft));
    }
    void addAstronaut(std::shared_ptr<Astronaut> astronaut)
    {
        astronauts.push_back(astronaut);
    }

    void assignAstronautToSpacecraft(const std::string& astronautName, const std::string& spacecraftName)
    {
        if (getAstronautsCountOnSpacecraft(spacecraftName) >= findSpacecraft(spacecraftName)->getCrewCapacity())
        {
            std::cout << "Cannot add " + astronautName + ", full capacity" << std::endl;
        }

        else
        {
            findAstronaut(astronautName)->assignToSpacecraft(findSpacecraft(spacecraftName));
        }
    }

    void launchSpacecraft(const std::string& spacecraftName)
    {
        for (int i = 0; i < spacecrafts.size(); i++)
        {
            if (spacecrafts[i]->getName() == spacecraftName)
            {
                std::cout << "Spacecraft " + spacecraftName + " has launched!";
                return;
            }
        }
        std::cout << "No Spacecraft with that name";
    }

    void decommissionSpacecraft(const std::string& spacecraftName)
    {
        for (int i = 0; i < spacecrafts.size(); i++)
        {
            if (spacecrafts[i]->getName() == spacecraftName)
            {
                for (int y = 0; y < astronauts.size(); y++)
                {
                    if (astronauts[y]->getAssignedSpacecraft()->getName() == spacecraftName)
                    {
                        astronauts[y]->releaseFromSpacecraft();
                    }
                }
                spacecrafts[i]->land();
                spacecrafts.erase(spacecrafts.begin() + i);
                return;
            }
        }
        std::cout << "No Spacecraft with that name";
    }

    void displayStatus() {
        std::cout << "\nMission Control Status:\n";
        std::cout << "Spacecrafts:\n";
        for (const auto& sc : spacecrafts) { // for each spacecraft
            std::cout << " - " << sc->getName() << " (Capacity: " << sc->getCrewCapacity() << ")\n";
        }
        std::cout << "Astronauts:\n";
        for (const auto& astro : astronauts) { // for each astronaut
            std::cout << " - " << astro->getName();
            if (auto sc = astro->getAssignedSpacecraft()) {
                std::cout << " assigned to " << sc->getName() << "\n";
            }
            else {
                std::cout << " not assigned to any spacecraft\n";
            }
        }
        std::cout << std::endl;
    } // displayStatus

    std::shared_ptr<Spacecraft> findSpacecraft(std::string name)
    {
        for (int i = 0; i < spacecrafts.size(); i++)
        {
            if (spacecrafts[i]->getName() == name)
            {
                return std::shared_ptr<Spacecraft>(spacecrafts[i].get(), [](Spacecraft*) {});
            }
        }
        std::cout << "No Spacecraft with that name";
    }

    std::shared_ptr<Astronaut> findAstronaut(std::string name)
    {
        for (int i = 0; i < astronauts.size(); i++)
        {
            if (astronauts[i]->getName() == name)
            {
                return astronauts[i];
            }
        }

        std::cout << "No Spacecraft with that name";
    }

    int getAstronautsCountOnSpacecraft(const std::string& spacecraftName)
    {
        int count = 0;
        for (int i = 0; i < astronauts.size(); i++)
        {
            if (astronauts[i]->getAssignedSpacecraft() != nullptr)
            {
                return astronauts[i]->getAssignedSpacecraft().use_count();
            }
        }
        return 0;
    }

private:
    std::vector<std::unique_ptr<Spacecraft>> spacecrafts;
    std::vector<std::shared_ptr<Astronaut>> astronauts;
};

struct Assignment2b {
   static void Execute() {
      {
         MissionControl missionControl;

         // Add spacecrafts
         missionControl.addSpacecraft(std::make_unique<Spacecraft>("Odyssey", 3, "Moon Mission"));
         missionControl.addSpacecraft(std::make_unique<Spacecraft>("Enterprise", 5, "Mars Mission"));

         // Add astronauts
         std::shared_ptr<Astronaut> astro1 = std::make_shared<Astronaut>("Alice", 35);
         std::shared_ptr<Astronaut> astro2 = std::make_shared<Astronaut>("Bob", 40);
         std::shared_ptr<Astronaut> astro3 = std::make_shared<Astronaut>("Charlie", 29);
         std::shared_ptr<Astronaut> astro4 = std::make_shared<Astronaut>("Diana", 32);

         missionControl.addAstronaut(astro1);
         missionControl.addAstronaut(astro2);
         missionControl.addAstronaut(astro3);
         missionControl.addAstronaut(astro4);

         // Assign astronauts to spacecrafts
         missionControl.assignAstronautToSpacecraft("Alice", "Odyssey");
         missionControl.assignAstronautToSpacecraft("Bob", "Odyssey");
         missionControl.assignAstronautToSpacecraft("Charlie", "Enterprise");
         missionControl.assignAstronautToSpacecraft("Diana", "Enterprise");

         // Launch spacecraft
         missionControl.launchSpacecraft("Odyssey");

         // Monitor spacecraft
         SpacecraftMonitor monitor;
         monitor.monitorSpacecraft(astro1->getAssignedSpacecraft());

         // Display status
         missionControl.displayStatus();
         monitor.displayStatus();

         // Decommission spacecraft
         missionControl.decommissionSpacecraft("Odyssey");

         // Display status after decommission
         missionControl.displayStatus();
         monitor.displayStatus();

         // Astronaut leaves spacecraft
         astro1->releaseFromSpacecraft();

         // Display final status
         missionControl.displayStatus();
      }
      // All resources are automatically cleaned up here
   }
};