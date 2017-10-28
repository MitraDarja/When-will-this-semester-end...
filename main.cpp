#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <limits>
#include <deque>
#include <algorithm>

int main(int argc, char** argv)
{
    if (argc < 4) return 1;  // check if there are 3 arguments
    
    /* initialize nodes data structure and fill it
     * {node_name : {neighbour_node : distance}}
     * i.e. {1 : {2 : 13.5, 5 : 35.2, ...}, ...}
     */
    std::unordered_map<std::string, std::unordered_map<std::string, double>> nodes;
    
    std::ifstream tgf(argv[1]);
    std::string line;
    if (tgf.is_open())
    {
        // initialize all nodes first
        while(getline(tgf, line))
        {
            if (line == "#") break;  // go to next step

            std::string node_name(line.substr(0, line.find(" ")));
            nodes[node_name] = {};
        }
        
        // store neighbour nodes distance information for every node
        std::array<std::string, 3> splitted_line;
        std::string node_1_name;
        std::string node_2_name;
        double dist;

        while(getline(tgf, line))
        {
            // split line and fill "splitted_line" vector
            for(int array_ind(0); array_ind < 3; ++array_ind)
            {
                splitted_line[array_ind] = line.substr(0, line.find(" "));
                line = line.substr(line.find(" ") + 1);
            }
            
            node_1_name = splitted_line[0];
            node_2_name = splitted_line[1];
            dist = std::stod(splitted_line[2]);
            
            nodes[node_1_name][node_2_name] = dist;
        }
        
        tgf.close();
    }

    else return 2;  // if can't read the file
    
    /* initialize data structures to store shortest path for every node and
     * previous node from which the shortest path was found:
     * {node : shortest_path};
     * {node : previous node}
     * also initialize queue structure, an array with possibility to erase
     * its elements by position
     */
    std::unordered_map<std::string, double> distances;
    std::unordered_map<std::string, std::string> previous;
    std::deque<std::string> queue;
 
    // set every shortest path to infinity and add all nodes to queue
    for (auto&& node : nodes)
    {
        distances[node.first] = std::numeric_limits<double>::infinity();
        queue.push_back(node.first);
    }
   
    // store source and target and check if they are valid
    std::string source(argv[2]);
    std::string target(argv[3]);
    if (!nodes.count(source) || !nodes.count(target)) return 2;

    // lambda function returns index of the node with shortest path in queue
    auto get_min_dist_node_idx = [&distances] (std::string a, std::string b)
    { return distances[a] < distances[b]; };
    
    // begin the algorithm: set source shortest path to 0
    distances[source] = 0;
    std::string current_node;
    std::string neighbour_node;
    bool path_exists(false);
    int min_dist_node_idx;
    double current_dist;
    
    while(true)
    {
        // set the node with shartest path as current_node and remove in from queue
        min_dist_node_idx = std::distance(queue.begin(),
                std::min_element(queue.begin(), queue.end(), get_min_dist_node_idx));
        current_node = queue[min_dist_node_idx];
        queue.erase(queue.begin() + min_dist_node_idx);
        
        // break if current node is target or if the queue is empty (==all nodes are visited)
        if (current_node == target)
        {
            path_exists = true;
            break;
        }
        if (queue.empty()) break;
        
        // main loop: check if there are shorter path
        for (auto&& dist : nodes[current_node])
        {
            neighbour_node = dist.first;
            current_dist = distances[current_node] + dist.second;
            
            // update shortest path information if found and add previous node
            if (current_dist < distances[neighbour_node])
            {
                distances[neighbour_node] = current_dist;
                previous[neighbour_node] = current_node;
            }
        }
    }
    
    if (!path_exists) return 1;  // if no path exists

    // read shortest path by reverse iteration, start from target
    std::deque<std::string> path;
    current_node = target;
    while (current_node != source)
    {
        path.push_front(current_node);
        current_node = previous[current_node];
    }
    path.push_front(source);
    
    // print out the shortest path from source to target and the path itself
    std::cout << distances[target] << std::endl;
    for (auto&& node : path)
    {
        if (node == target) std::cout << node << std::endl;
        else std::cout << node << " ";
    }

    return 0;
}