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

    }

    ~Spacecraft()
    {
        std::cout << "Space Craft Decommisioned" << std::endl;
    }

    void launch()
    {
        std::cout << "Spacecraft launched" << std::endl;
    }

    void land()
    {
        std::cout << "Spacecraft laned" << std::endl;
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

};

class Astronaut {
public:
    Astronaut(std::string _name, int _age) :
        name(_name),
        age(_age)
    {

    }
    ~Astronaut()
    {
        std::cout << "Astronaut no longer in system" << std::endl;
    }

    void assignToSpacecraft(std::shared_ptr<Spacecraft> spacecraft)
    {
        assigned_spacecraft = spacecraft;
    }

    void releaseFromSpacecraft()
    {
        if (assigned_spacecraft != nullptr)
        {
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
    MissionControl() {}
    ~MissionControl()
    {
        // ?
    }
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
        // var to astronaut (define me)
        for (int i = 0; i < astronauts.size(); i++)
        {
            if (astronauts[i].get()->getName() == astronautName)
            {
                //(assign me)
                break;
            }
        }

        for (int y = 0; y < spacecrafts.size(); y++)
        {
            if (spacecrafts[y].get()->getName() == spacecraftName)
            {
                if (getAstronautCountOnSpacecraft(const )
                {
                    astronauts[i].get()->assignToSpacecraft(std::make_shared<Spacecraft>(spacecrafts[y]));
                }
            }
        }
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