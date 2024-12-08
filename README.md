Smart Traffic Management System Simulator

Overview

The Smart Traffic Management System Simulator is a project designed to mimic real-world traffic management within an urban environment. It optimizes traffic flow by leveraging data structures such as graphs, heaps, and priority queues. This simulator includes features for vehicle routing, congestion monitoring, emergency vehicle handling, and disruption simulation.

Features
City Traffic Network (Graph Representation)
Nodes: Intersections.
Edges: Roads connecting intersections with weights representing travel time or congestion levels.
Dynamic Updates:
Add/remove intersections and roads.
Modify road weights dynamically based on vehicle counts.
Vehicle Routing System
Dynamically calculates the shortest or fastest routes using Dijkstra's Algorithm.
Re-routes vehicles when traffic conditions change.
Tracks vehicles to monitor their progress through the network.
Traffic Signal Management
Adjusts signal timings dynamically based on traffic density.
Overrides signals for emergency vehicles.
Simulates real-world traffic flow with green light durations.
Congestion Monitoring
Monitors vehicle density on roads in real-time.
Adjusts road weights dynamically to simulate congestion effects.
Provides rerouting options for vehicles to alleviate congestion.
Emergency Vehicle Handling
Prioritizes emergency vehicles by overriding normal traffic signals.
Dynamically calculates the fastest route using a graph-based approach.
Accident and Road Closure Simulation
Closes specific roads or intersections dynamically.
Recalculates routes for affected vehicles.
Simulates system performance under disruptive conditions.
Real-Time Monitoring and Simulation Dashboard
Interactive console-based dashboard for:
Monitoring traffic.
Adjusting road weights and closures.
Managing traffic signals.
Simulating emergency vehicle routes.
System Requirements
Programming Language: C++ (Standard 11 or later).
Compiler: GCC or Visual Studio.
Dependencies: None (Standard C++ libraries used).
3. Input Files
Ensure the following CSV files are available in the same directory as the program:

road_network.csv: Defines intersections and roads.
vehicles.csv: Contains vehicle data (ID, start, and end points).
traffic_signal_timings.csv: Specifies green light durations.
emergency_vehicles.csv: Emergency vehicle data (ID, start, end, and priority).
accidents_or_closures.csv: Simulated disruptions for road closures or accidents.
Usage Instructions
Upon launching the program, an interactive menu will guide you through the following options:

Load and Display All Data Files: Loads and prints the contents of all required files.
Add Intersection: Adds a new intersection to the network.
Add Road: Connects two intersections with a road.
Update Road Weight: Changes the travel time for a specific road.
Find Shortest Path: Calculates the shortest route between two intersections.
Set Vehicle Count: Updates the vehicle count for a road.
Get Vehicle Count: Displays the current vehicle count for a specific road.
Close Road: Simulates a road closure.
Reopen Road: Reopens a previously closed road.
Simulate Disruption: Recalculates routes affected by closures or accidents.
Emergency Vehicle Routing: Prioritizes an emergency vehicle's route.
Real-Time Traffic Monitoring: Displays real-time traffic updates.
Display Traffic Signal Timers: Shows current green light durations.
Exit: Ends the simulation.
Key Algorithms
Dijkstra's Algorithm: Finds the shortest path in the traffic network.
Dynamic Weight Adjustment: Modifies road weights based on real-time vehicle density.
Priority-Based Traffic Signal Management: Adjusts green light durations dynamically.
Emergency Vehicle Routing: Clears paths for emergency vehicles while maintaining network flow.
Simulation Logs
The program generates real-time logs for:

Vehicle movements and routes.
Changes in traffic signal durations.
Road closures and reopenings.
Emergency vehicle route calculations.
