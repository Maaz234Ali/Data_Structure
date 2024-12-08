#include <iostream>
#include <queue> // For priority queue
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;


// Constants
#define MAX_INTERSECTIONS 100 // Maximum number of intersections
const int INF = 99999; // A large value to represent "infinite" distance

// Structure for roads (edges)
struct Edge {
    int to;       // Destination index
    int weight;   // Weight (travel time or congestion level)
    Edge* next;   // Pointer to the next road

    Edge() : to(-1), weight(0), next(NULL) {} // Initialize members
};

// Structure for intersections (nodes)
// Structure for intersections (nodes)
struct Intersection {
    string name;            // Name of the intersection
    Edge* head;             // Head of the linked list of roads
    int trafficSignal;      // Green light duration for the intersection (in seconds)
    bool emergencyOverride; // Flag to indicate if traffic signal is overridden for emergency vehicles

    Intersection() : name(""), head(NULL), trafficSignal(30), emergencyOverride(false) {} // Initialize with default traffic signal duration
};



// Graph Class
class TrafficGraph {
private:
    Intersection intersections[MAX_INTERSECTIONS];
    int count;
    int roadVehicleCounts[MAX_INTERSECTIONS][MAX_INTERSECTIONS]; // Tracking vehicle counts on roads
    bool closedRoads[MAX_INTERSECTIONS][MAX_INTERSECTIONS]; // Road closure status


public:
    TrafficGraph() : count(0) {
        // Initialize vehicle count to 0 and closed roads to false
        for (int i = 0; i < MAX_INTERSECTIONS; ++i) {
            for (int j = 0; j < MAX_INTERSECTIONS; ++j) {
                roadVehicleCounts[i][j] = 0; // Initialize all roads with 0 vehicles
                closedRoads[i][j] = false; // No roads are closed initially
            }
        }
    }
    void monitorTraffic() {
        cout << "\nReal-Time Traffic Monitoring:\n";
        for (int i = 0; i < count; ++i) {
            cout << "Intersection: " << intersections[i].name << "\n";
            Edge* current = intersections[i].head;
            while (current != NULL) {
                cout << "  Road to " << intersections[current->to].name
                    << " - Vehicle Count: " << roadVehicleCounts[i][current->to]
                    << " - Weight: " << current->weight << "\n";
                current = current->next;
            }
            cout << "-----------------------------------\n";
        }
    }


    // Function to load and display the content of a file
    void loadAndDisplayFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) { // Check if the file opens successfully
            cout << "Error: Unable to open file " << filename << "\n";
            return;
        }

        cout << "\nContents of " << filename << ":\n";
        string line;
        while (getline(file, line)) {
            cout << line << "\n"; // Display each line of the file
        }

        file.close();
        cout << "-------------------------------------\n";
    }

    void adjustGreenLightDuration(int intersectionIndex) {
        // If emergency vehicle is overriding the signal, set a fixed duration (e.g., 10 seconds for emergency)
        if (intersections[intersectionIndex].emergencyOverride) {
            intersections[intersectionIndex].trafficSignal = 10; // Emergency override to 10 seconds
            cout << "Emergency override: Green light at " << intersections[intersectionIndex].name
                << " is fixed to 10 seconds." << endl;
            return;
        }

        // Otherwise, adjust the green light duration based on traffic density
        int totalVehicleCount = 0;
        Edge* current = intersections[intersectionIndex].head;

        while (current != NULL) {
            int vehicleCount = roadVehicleCounts[intersectionIndex][current->to];
            totalVehicleCount += vehicleCount;
            current = current->next;
        }

        // Adjust green light duration based on traffic density (simple model)
        int newDuration = 30 + totalVehicleCount / 10; // Example: 10 vehicles per second increase the green light duration by 1 second
        intersections[intersectionIndex].trafficSignal = newDuration;

        cout << "Adjusted green light duration at " << intersections[intersectionIndex].name
            << " to " << newDuration << " seconds due to traffic density." << endl;
    }



    // Public method to access the vehicle count for a specific road
    int getVehicleCount(int fromIndex, int toIndex) {
        return roadVehicleCounts[fromIndex][toIndex];
    }
    void setVehicleCount(int fromIndex, int toIndex, int count)
    {
        // Ensure the indices are valid
        if (fromIndex < 0 || fromIndex >= MAX_INTERSECTIONS || toIndex < 0 || toIndex >= MAX_INTERSECTIONS) {
            cout << "Invalid intersection indices." << endl;
            return;
        }

        // Update the vehicle count for the road from 'fromIndex' to 'toIndex'
        roadVehicleCounts[fromIndex][toIndex] = count;

        // If the roads are bidirectional, set the reverse road count as well
        roadVehicleCounts[toIndex][fromIndex] = count;

        // Output for debugging purposes
        cout << "Vehicle count from " << intersections[fromIndex].name
            << " to " << intersections[toIndex].name
            << " set to " << count << endl;

        // Adjust the green light duration based on the vehicle counts
        adjustGreenLightDuration(fromIndex);
        adjustGreenLightDuration(toIndex);
    }




    int findIntersection(const string& name) {
        for (int i = 0; i < count; i++) {
            if (intersections[i].name == name) {
                return i; // Intersection found
            }
        }
        return -1; // Intersection not found
    }

    void adjustRoadWeightForCongestion(int fromIndex, int toIndex) {
        int vehicleCount = roadVehicleCounts[fromIndex][toIndex];
        int congestionLevel = vehicleCount / 10; // You can change the divisor to better fit traffic scale

        if (congestionLevel > 0) {
            Edge* current = intersections[fromIndex].head;
            while (current != NULL) {
                if (current->to == toIndex) {
                    current->weight += congestionLevel;  // Increase weight based on congestion
                    cout << "Increased weight on road from " << intersections[fromIndex].name
                        << " to " << intersections[toIndex].name << " due to congestion."
                        << " New weight: " << current->weight << endl;
                    return;
                }
                current = current->next;
            }
        }
    }


    // Method to recalculate the route when rerouting is required
    void rerouteTraffic(int fromIndex, int toIndex) {
        // Adjust the road weight based on congestion
        adjustRoadWeightForCongestion(fromIndex, toIndex);

        // Recalculate the shortest path after adjusting the road weight
        cout << "Recalculating the shortest path due to traffic congestion...\n";
        string fromName = intersections[fromIndex].name;
        string toName = intersections[toIndex].name;
        dijkstra(fromName, toName);
    }


    void addIntersection(const string& name) {
        // Check if the intersection already exists
        if (findIntersection(name) != -1) {
            cout << "Intersection already exists: " << name << endl;
            return;
        }
        // Check if we've reached the maximum number of intersections
        if (count >= MAX_INTERSECTIONS) {
            cout << "Maximum intersections reached." << endl;
            return;
        }
        // Add the new intersection
        intersections[count].name = name;
        intersections[count].head = nullptr;
        count++;
        cout << "Intersection added: " << name << endl;
    }



    // Method to add a road between two intersections
   // Method to add a road between two intersections
    void addRoad(const string& from, const string& to, int weight) {
        // Find the indices of the intersections
        int fromIndex = findIntersection(from);
        int toIndex = findIntersection(to);

        // Check if one or both intersections do not exist
        if (fromIndex == -1) {
            cout << "Intersection not found: " << from << endl;
            return;
        }
        if (toIndex == -1) {
            cout << "Intersection not found: " << to << endl;
            return;
        }

        // Check for duplicate roads (optional)
        Edge* current = intersections[fromIndex].head;
        while (current) {
            if (current->to == toIndex) {
                cout << "Road already exists between " << from << " and " << to << endl;
                return;
            }
            current = current->next;
        }
        // Adding road in the opposite direction for bi-directional graph
        Edge* reverseEdge = new Edge();
        reverseEdge->to = fromIndex;
        reverseEdge->weight = weight;
        reverseEdge->next = intersections[toIndex].head;
        intersections[toIndex].head = reverseEdge;


        // Create the new road (edge)
        Edge* newEdge = new Edge();
        newEdge->to = toIndex;
        newEdge->weight = weight;

        // Insert the new edge at the head of the adjacency list
        newEdge->next = intersections[fromIndex].head;
        intersections[fromIndex].head = newEdge;

        // Feedback to the user
        cout << "Road added between " << from << " and " << to << " with weight " << weight << endl;
    }


    // Method to update the weight (time) of an existing road
    void updateRoadWeight(const string& from, const string& to, int newWeight) {
        int fromIndex = findIntersection(from);
        int toIndex = findIntersection(to);

        if (fromIndex == -1 || toIndex == -1) {
            cout << "One or both intersections not found: " << from << ", " << to << endl;
            return;
        }

        Edge* current = intersections[fromIndex].head;
        while (current != NULL) {
            if (current->to == toIndex) {
                current->weight = newWeight;
                cout << "Updated road weight from " << from << " to " << to << " to " << newWeight << endl;
                return;
            }
            current = current->next;
        }
        cout << "Road not found: " << from << " to " << to << endl;
    }
    // Method to display current traffic signal timers
    void displaySignalTimers() {
        cout << "\nCurrent Traffic Signal Timers:\n";
        for (int i = 0; i < count; ++i) {
            cout << intersections[i].name << " - Green Light Duration: "
                << intersections[i].trafficSignal << " seconds.\n";
        }
    }


    // Method to calculate the shortest path using Dijkstra's Algorithm
    void dijkstra(const string& startName, const string& endName) {
        int startIndex = findIntersection(startName);
        int endIndex = findIntersection(endName);

        // If one or both intersections are not found
        if (startIndex == -1 || endIndex == -1) {
            cout << "Start or end intersection not found: " << startName << ", " << endName << endl;
            return;
        }

        // Show green light durations for the start and end intersections
        cout << "Green light duration at " << startName << ": "
            << intersections[startIndex].trafficSignal << " seconds." << endl;
        cout << "Green light duration at " << endName << ": "
            << intersections[endIndex].trafficSignal << " seconds." << endl;

        // Initialize distances, visited status, and previous node tracker
        int dist[MAX_INTERSECTIONS];
        bool visited[MAX_INTERSECTIONS];
        int prev[MAX_INTERSECTIONS];

        for (int i = 0; i < count; ++i) {
            dist[i] = INF;    // Set all distances to infinity
            visited[i] = false;  // Mark all as unvisited
            prev[i] = -1;     // No previous nodes initially
        }
        dist[startIndex] = 0; // Distance to the start node is zero

        // Main Dijkstra loop
        for (int i = 0; i < count; ++i) {
            int minDist = INF, u = -1;

            // Find the unvisited node with the smallest distance
            for (int j = 0; j < count; ++j) {
                if (!visited[j] && dist[j] < minDist) {
                    minDist = dist[j];
                    u = j;
                }
            }

            if (u == -1 || u == endIndex) break;  // No reachable nodes left, or found the destination

            visited[u] = true;

            // Traverse all neighbors (edges) of the current node
            Edge* current = intersections[u].head;
            while (current != NULL) {
                int v = current->to;

                // Skip if the road is closed or if the node is visited
                if (!visited[v] && dist[u] + current->weight < dist[v] && !closedRoads[u][v]) {
                    dist[v] = dist[u] + current->weight;
                    prev[v] = u;
                }
                current = current->next;
            }
        }

        // If no path found to the destination, print the error message
        if (dist[endIndex] == INF) {
            cout << "No path found from " << startName << " to " << endName << endl;
            return;
        }

        // Print the shortest path information
        cout << "Shortest path from " << startName << " to " << endName << " is " << dist[endIndex] << " time units." << endl;
        cout << "Path: ";
        printPath(prev, endIndex);
    }


    // Method to simulate road closure
    void closeRoad(const string& from, const string& to) {
        int fromIndex = findIntersection(from);
        int toIndex = findIntersection(to);

        if (fromIndex == -1 || toIndex == -1) {
            cout << "One or both intersections not found: " << from << ", " << to << endl;
            return;
        }

        closedRoads[fromIndex][toIndex] = true;
        closedRoads[toIndex][fromIndex] = true; // Assuming roads are bidirectional

        cout << "Road from " << from << " to " << to << " is now closed." << endl;
    }
    void printPath(int prev[], int current) {
        if (current == -1) {
            return; // Base case: we've reached the start node
        }
        printPath(prev, prev[current]); // Recursively print the path from the start to the current node
        cout << intersections[current].name << " "; // Print the current intersection's name
    }


    // Method to simulate road reopening
    void reopenRoad(const string& from, const string& to) {
        int fromIndex = findIntersection(from);
        int toIndex = findIntersection(to);

        if (fromIndex == -1 || toIndex == -1) {
            cout << "One or both intersections not found: " << from << ", " << to << endl;
            return;
        }

        closedRoads[fromIndex][toIndex] = false;
        closedRoads[toIndex][fromIndex] = false; // Assuming roads are bidirectional

        cout << "Road from " << from << " to " << to << " is now reopened." << endl;
    }

    // Method to simulate system performance under road closures
    void simulateDisruption() {
        // Example: Simulate the shortest path calculation during disruptions
        string start, end;
        cout << "Enter start intersection: ";
        cin >> start;
        cout << "Enter end intersection: ";
        cin >> end;
        dijkstra(start, end); // Recalculate shortest path affected by closures
    }

    void emergencyVehicleRouting(const string& startName, const string& endName) {
        cout << "Routing emergency vehicle from " << startName << " to " << endName << endl;

        int startIndex = findIntersection(startName);
        int endIndex = findIntersection(endName);

        if (startIndex == -1 || endIndex == -1) {
            cout << "Start or end intersection not found: " << startName << ", " << endName << endl;
            return;
        }

        intersections[startIndex].emergencyOverride = true;
        intersections[endIndex].emergencyOverride = true;

        // Call Dijkstra or another pathfinding algorithm to find the optimal route
        dijkstra(startName, endName);

        intersections[startIndex].emergencyOverride = false;
        intersections[endIndex].emergencyOverride = false;
    }


};

#include <iostream>
#include <string>
using namespace std;

#include <iostream>
#include <string>
using namespace std;

// ANSI escape codes for colors
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BG_BLUE "\033[44m"
#define BG_YELLOW "\033[43m"

// Function to print a decorative border
void printBorder() {
    cout << BOLD << CYAN << "===========================================================" << RESET << endl;
}

// Main function
int main() {
    TrafficGraph cityGraph;
    int choice;

    printBorder();
    cout << endl;
    cout << BOLD << CYAN << "===   _____   Smart Traffic Management System   _____    ===" << endl;
    cout << "    __|_|_|__                                 __|_|_|__       " << endl;
    cout << "   |_0_____0_|                               |_0_____0_|      " << RESET << endl;
    cout << endl;
    printBorder();

    do {
        cout << endl;

        // Display the menu options with colored and styled text
        cout << BOLD << BLUE << "Main Menu" << RESET << endl;
        printBorder();

        cout << GREEN << "1. Load And Display All 5 Data Files" << RESET << endl;
        cout << GREEN << "2. Add Intersection" << RESET << endl;
        cout << GREEN << "3. Add Road" << RESET << endl;
        cout << GREEN << "4. Update Road Weight" << RESET << endl;
        cout << GREEN << "5. Find Shortest Path" << RESET << endl;
        cout << GREEN << "6. Set Vehicle Count" << RESET << endl;
        cout << GREEN << "7. Get Vehicle Count" << RESET << endl;
        cout << GREEN << "8. Close Road" << RESET << endl;
        cout << GREEN << "9. Reopen Road" << RESET << endl;
        cout << GREEN << "10. Simulate Disruption" << RESET << endl;
        cout << GREEN << "11. Emergency Vehicle Routing" << RESET << endl;
        cout << GREEN << "12. Real-Time Traffic Monitoring" << RESET << endl;
        cout << GREEN << "13. Display Traffic Signal Timers" << RESET << endl;
        cout << RED << "14. Exit" << RESET << endl;

        printBorder();

        // Asking for the user's choice
        cout << CYAN << "Enter your choice: " << RESET;
        cin >> choice;

        if (choice == 1) {
            // List of files to load
            string filenames[] = {
                "accidents_or_closures.csv",
                "emergency_vehicles.csv",
                "road_network.csv",
                "traffic_signal_timings.csv",
                "vehicles.csv"
            };

            // Iterate through each file and load it
            for (const string& filename : filenames) {
                cityGraph.loadAndDisplayFile(filename); // Corrected function call
            }
        }
        else if (choice == 2) {
            string name;
            cout << GREEN << "Enter intersection name: " << RESET;
            cin >> name;
            cityGraph.addIntersection(name);
        }
        else if (choice == 3) {
            string from, to;
            int weight;
            cout << GREEN << "Enter start intersection: " << RESET;
            cin >> from;
            cout << GREEN << "Enter end intersection: " << RESET;
            cin >> to;
            cout << GREEN << "Enter road weight: " << RESET;
            cin >> weight;
            cityGraph.addRoad(from, to, weight);
        }
        else if (choice == 4) {
            string from, to;
            int newWeight;
            cout << GREEN << "Enter start intersection: " << RESET;
            cin >> from;
            cout << GREEN << "Enter end intersection: " << RESET;
            cin >> to;
            cout << GREEN << "Enter new road weight: " << RESET;
            cin >> newWeight;
            cityGraph.updateRoadWeight(from, to, newWeight);
        }
        else if (choice == 5) {
            string start, end;
            cout << GREEN << "Enter start intersection: " << RESET;
            cin >> start;
            cout << GREEN << "Enter end intersection: " << RESET;
            cin >> end;
            cityGraph.dijkstra(start, end);
        }
        else if (choice == 6) {
            string from, to;
            int count;
            cout << GREEN << "Enter start intersection: " << RESET;
            cin >> from;
            cout << GREEN << "Enter end intersection: " << RESET;
            cin >> to;
            cout << GREEN << "Enter vehicle count: " << RESET;
            cin >> count;
            int fromIndex = cityGraph.findIntersection(from);
            int toIndex = cityGraph.findIntersection(to);
            cityGraph.setVehicleCount(fromIndex, toIndex, count);
        }
        else if (choice == 7) {
            string from, to;
            cout << GREEN << "Enter start intersection: " << RESET;
            cin >> from;
            cout << GREEN << "Enter end intersection: " << RESET;
            cin >> to;
            int fromIndex = cityGraph.findIntersection(from);
            int toIndex = cityGraph.findIntersection(to);
            cout << CYAN << "Vehicle count: " << cityGraph.getVehicleCount(fromIndex, toIndex) << RESET << endl;
        }
        else if (choice == 8) {
            string from, to;
            cout << GREEN << "Enter start intersection: " << RESET;
            cin >> from;
            cout << GREEN << "Enter end intersection: " << RESET;
            cin >> to;
            cityGraph.closeRoad(from, to);
        }
        else if (choice == 9) {
            string from, to;
            cout << GREEN << "Enter start intersection: " << RESET;
            cin >> from;
            cout << GREEN << "Enter end intersection: " << RESET;
            cin >> to;
            cityGraph.reopenRoad(from, to);
        }
        else if (choice == 10) {
            cityGraph.simulateDisruption();
        }
        else if (choice == 11) {
            string start, end;
            cout << GREEN << "Enter start intersection: " << RESET;
            cin >> start;
            cout << GREEN << "Enter end intersection: " << RESET;
            cin >> end;
            cityGraph.emergencyVehicleRouting(start, end);
        }
        else if (choice == 12) {
            cityGraph.monitorTraffic();
        }
        else if (choice == 13) {
            cityGraph.displaySignalTimers();
        }

        else if (choice == 14) {
            cout << RED << "Exiting program..." << RESET << endl;
        }
        else {
            cout << YELLOW << "Invalid choice. Please try again." << RESET << endl;
        }

    } while (choice != 14);

    return 0;
}
