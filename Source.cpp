#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Edge {
    int to;
    int weight;
};

struct Vehicle {
    int id;
    vector<int> route;
};

struct TrafficSignal {
    int intersection;
    int greenDuration;
    int vehicleDensity;

    bool operator<(const TrafficSignal& other) const {
        return vehicleDensity < other.vehicleDensity;
    }
};

class Graph {
private:
    unordered_map<int, list<Edge>> adjList;
    unordered_map<int, int> vehicleCounts;
    unordered_map<int, int> signalTimings;
    unordered_map<int, unordered_map<int, int>> congestionMap; // Road congestion tracking
    int congestionThreshold = 10; // Example threshold for congestion

    vector<Vehicle> vehicles;

public:
    void loadRoadNetwork(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            int from, to, weight;
            ss >> from >> to >> weight;
            adjList[from].push_back({ to, weight });
        }
        file.close();
    }

    void loadAccidents(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            int from, to;
            ss >> from >> to;
            removeRoad(from, to);
        }
        file.close();
    }

    void loadEmergencyVehicles(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            int id, routeSize, node;
            ss >> id >> routeSize;
            Vehicle v{ id };
            for (int i = 0; i < routeSize; ++i) {
                ss >> node;
                v.route.push_back(node);
            }
            vehicles.push_back(v);
        }
        file.close();
    }

    void loadTrafficSignalTimings(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            int intersection, timing;
            ss >> intersection >> timing;
            signalTimings[intersection] = timing;
        }
        file.close();
    }

    void loadVehicles(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            int id, routeSize, node;
            ss >> id >> routeSize;
            Vehicle v{ id };
            for (int i = 0; i < routeSize; ++i) {
                ss >> node;
                v.route.push_back(node);
            }
            vehicles.push_back(v);
        }
        file.close();
    }

    void displayGraph() {
        for (const auto& pair : adjList) {
            cout << "Node " << pair.first << ": ";
            for (const auto& edge : pair.second) {
                cout << " -> (" << edge.to << ", " << edge.weight << ")";
            }
            cout << endl;
        }
    }

    void addIntersection(int node) {
        if (adjList.count(node)) {
            cout << "Intersection " << node << " already exists.\n";
        }
        else {
            adjList[node] = list<Edge>();
            cout << "Intersection " << node << " added.\n";
        }
    }

    void removeIntersection(int node) {
        adjList.erase(node);
        for (auto& pair : adjList) {
            pair.second.remove_if([node](const Edge& edge) { return edge.to == node; });
        }
        vehicleCounts.erase(node);
        signalTimings.erase(node);
        cout << "Intersection " << node << " removed.\n";
    }

    vector<int> calculateShortestRoute(int src, int dest) {
        unordered_map<int, int> dist; // Distance from src to each node
        unordered_map<int, int> prev; // To reconstruct path
        for (const auto& pair : adjList) {
            dist[pair.first] = INT_MAX;
            prev[pair.first] = -1;
        }
        dist[src] = 0;

        using PII = pair<int, int>; // {distance, node}
        priority_queue<PII, vector<PII>, greater<PII>> pq;
        pq.push({ 0, src });

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (u == dest) break;

            for (const Edge& edge : adjList[u]) {
                int v = edge.to, weight = edge.weight;
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.push({ dist[v], v });
                }
            }
        }

        // Reconstruct path
        vector<int> path;
        for (int at = dest; at != -1; at = prev[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
        return (path.size() == 1 && path[0] != src) ? vector<int>() : path;
    }

    // Update vehicle routes dynamically after traffic changes
    void recalculateRoutes() {
        for (auto& vehicle : vehicles) {
            if (vehicle.route.size() < 2) continue;
            int src = vehicle.route.front();
            int dest = vehicle.route.back();
            vehicle.route = calculateShortestRoute(src, dest);
            if (!vehicle.route.empty()) {
                cout << "Updated route for Vehicle " << vehicle.id << ": ";
                for (int node : vehicle.route) {
                    cout << node << " -> ";
                }
                cout << "End\n";
            }
            else {
                cout << "No valid route found for Vehicle " << vehicle.id << ".\n";
            }
        }
    }

    // Modified traffic update
    void updateTrafficConditions(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            int from, to, newWeight;
            ss >> from >> to >> newWeight;
            for (auto& edge : adjList[from]) {
                if (edge.to == to) {
                    edge.weight = newWeight;
                    cout << "Updated weight of road from " << from << " to " << to << " to " << newWeight << ".\n";
                    break;
                }
            }
        }
        file.close();
        recalculateRoutes(); // Recalculate routes dynamically
    }

    void addRoad(int from, int to, int weight) {
        adjList[from].push_back({ to, weight });
        cout << "Road from " << from << " to " << to << " added with weight " << weight << ".\n";
    }

    void removeRoad(int from, int to) {
        if (adjList.count(from)) {
            adjList[from].remove_if([to](const Edge& edge) { return edge.to == to; });
            cout << "Road from " << from << " to " << to << " removed.\n";
        }
        else {
            cout << "No such road exists.\n";
        }
    }

    void trackVehicleMovement(int id) {
        auto it = find_if(vehicles.begin(), vehicles.end(),
            [id](const Vehicle& v) { return v.id == id; });
        if (it == vehicles.end()) {
            cout << "Vehicle " << id << " not found.\n";
            return;
        }
        cout << "Tracking movement of Vehicle " << id << ": ";
        for (size_t i = 0; i < it->route.size() - 1; ++i) {
            int from = it->route[i];
            int to = it->route[i + 1];
            congestionMap[from][to]++; // Increment vehicle count for the road
            cout << from << " -> ";

            // Check congestion on this road
            if (congestionMap[from][to] > congestionThreshold) {
                cout << "(Congested)";
            }
        }
        cout << "End\n";
    }
    void monitorCongestion() {
        cout << "\nRoad Congestion Levels:\n";
        for (const auto& fromEntry : congestionMap) {
            for (const auto& toEntry : fromEntry.second) {
                int from = fromEntry.first;
                int to = toEntry.first;
                int count = toEntry.second;
                cout << "Road from " << from << " to " << to << ": " << count << " vehicles";
                if (count > congestionThreshold) {
                    cout << " - Congested\n";
                }
                else {
                    cout << " - Normal\n";
                }
            }
        }
    }

    vector<int> rerouteTraffic(int congestedRoadFrom, int congestedRoadTo) {
        cout << "Rerouting traffic due to congestion on road from " << congestedRoadFrom << " to " << congestedRoadTo << ".\n";
        // Perform a BFS to find an alternative route
        vector<int> newRoute = calculateShortestRoute(congestedRoadFrom, congestedRoadTo);
        return newRoute;
    }

    // Traffic Signal Management
    void manageTrafficSignals() {
        priority_queue<TrafficSignal> pq;

        for (const auto& entry : vehicleCounts) {
            pq.push({ entry.first, signalTimings[entry.first], entry.second });
        }

        while (!pq.empty()) {
            TrafficSignal ts = pq.top();
            pq.pop();

            cout << "Intersection " << ts.intersection
                << " has green light for " << ts.greenDuration
                << " seconds (Vehicle density: " << ts.vehicleDensity << ")\n";
        }
    }

    void emergencyOverride(int intersection) {
        cout << "Emergency override activated at Intersection " << intersection << "!\n";
        signalTimings[intersection] = 60; // Default emergency timing
    }
};

int main() {
    Graph graph;

    // Load initial data
    graph.loadRoadNetwork("road_network.csv");
    graph.loadAccidents("accidents_or_closures.csv");
    graph.loadEmergencyVehicles("emergency_vehicles.csv");
    graph.loadTrafficSignalTimings("traffic_signal_timings.csv");
    graph.loadVehicles("vehicles.csv");

    // Menu-driven operations
    cout << "\nChoose an operation:\n";
    cout << "1. Display Road Network\n";
    cout << "2. Add Intersection\n";
    cout << "3. Remove Intersection\n";
    cout << "4. Track Vehicle Movement\n";
    cout << "5. Update Traffic Conditions\n";
    cout << "6. Monitor Road Congestion\n";  // New menu option
    cout << "7. Manage Traffic Signals\n";
    cout << "8. Emergency Override\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    int intersection, from, to, weight, id;
    string file;
    switch (choice) {
    case 1:
        graph.displayGraph();
        break;
    case 2:
        cout << "Enter intersection ID: ";
        cin >> intersection;
        graph.addIntersection(intersection);
        break;
    case 3:
        cout << "Enter intersection ID to remove: ";
        cin >> intersection;
        graph.removeIntersection(intersection);
        break;
    case 4:
        cout << "Enter vehicle ID: ";
        cin >> id;
        graph.trackVehicleMovement(id);
        break;
    case 5:
        cout << "Enter traffic conditions file: ";
        cin.ignore();
        getline(cin, file);
        graph.updateTrafficConditions(file);
        break;
    case 6:
        graph.monitorCongestion();  // Check for congestion levels
        break;
    case 7:
        graph.manageTrafficSignals();
        break;
    case 8:
        cout << "Enter intersection ID for emergency override: ";
        cin >> intersection;
        graph.emergencyOverride(intersection);
        break;
    case 9:
        break;
    default:
        cout << "Invalid choice.\n";
        break;
    }

    return 0;
}