Smart Traffic Management System Simulator

Introduction

The Smart Traffic Management System Simulator is a practical project designed to simulate real-world traffic management in urban environments. By leveraging advanced data structures such as graphs, heaps, and priority queues, this system optimizes traffic flow, dynamically adjusts traffic signals, handles congestion, and prioritizes emergency vehicles. It offers a comprehensive solution for managing urban traffic efficiently while simulating the challenges faced in real-time traffic systems.

Key Features

City Traffic Network Representation:

Represents the city’s traffic infrastructure as a weighted, directed graph.
Supports dynamic addition and removal of roads and intersections.
Visualizes the traffic network for better understanding and analysis.

Dynamic Vehicle Routing:

Uses Dijkstra’s Algorithm to calculate optimal vehicle paths.
Dynamically recalculates routes based on changing traffic conditions.
Tracks vehicle movement across the network.

Traffic Signal Management:

Controls traffic signals at intersections to optimize vehicle flow.
Adjusts signal timings dynamically based on traffic density.
Implements emergency vehicle overrides for critical situations.

Congestion Monitoring:

Monitors vehicle counts on roads to detect congestion.
Identifies congested roads and reroutes traffic accordingly.
Provides real-time updates and adjustments for smoother traffic flow.

Emergency Vehicle Handling:

Prioritizes emergency vehicles by dynamically adjusting traffic signals.
Calculates the fastest route for emergency vehicles.
Restores normal traffic flow after emergency vehicles pass.

Accident and Road Closure Simulation:

Simulates road closures or accidents affecting the network.
Recalculates affected vehicle routes based on closures.
Monitors the impact of road closures on traffic conditions.

Simulation Dashboard:

Interactive console interface displaying real-time traffic data.
Allows manual addition or removal of roads, intersections, and vehicles.
Generates logs of all system actions including rerouting and signal changes.

How to Run the Code

Prerequisites:
C++ Compiler (GCC or Visual Studio)
Standard C++ Libraries
Input Files: Ensure the following CSV files are present in the same directory as the program:

road_network.csv (Defines intersections and roads).

vehicles.csv (Vehicle data: ID, start, and end points).

traffic_signal_timings.csv (Intersection green light timings).

emergency_vehicles.csv (Emergency vehicle data: ID, start, end, and priority).

accidents_or_closures.csv (Simulated disruptions like road closures).
![AF](https://github.com/user-attachments/assets/66b35578-e603-469f-9f46-b9df64f41f45)

![A2](https://github.com/user-attachments/assets/b3dc7502-6f9d-44cd-9303-a9e51210c878)

![A3](https://github.com/user-attachments/assets/dd9aa9dd-b840-4a53-90e6-e36c17daeed5)

![A4](https://github.com/user-attachments/assets/c066a5dd-4029-4468-82a8-5e3c393bc7c8)

![A5](https://github.com/user-attachments/assets/6ac36799-a6a8-4902-9968-6e9bbff22309)

Conclusion

The Smart Traffic Management System Simulator is a comprehensive and practical solution to urban traffic management challenges. By leveraging advanced data structures and algorithms, it efficiently models real-world traffic scenarios, providing dynamic routing, congestion monitoring, and traffic signal optimization. The system's ability to adapt to changing traffic conditions and prioritize emergency vehicles highlights its robustness and scalability.


This project not only serves as an academic exercise in implementing graph-based algorithms but also offers practical insights into the complexities of modern traffic systems. It demonstrates how computational techniques can address real-world problems, paving the way for smarter, more efficient urban mobility solutions.


By integrating features like road closure simulations, dynamic travel times, and real-time monitoring, this simulator provides a holistic view of traffic management. Its modular and interactive design ensures flexibility, making it an excellent tool for future enhancements and applications in smart city development.
