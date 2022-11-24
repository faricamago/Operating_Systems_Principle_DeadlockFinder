// this is the ONLY file you should edit and submit to D2L

#include "find_deadlock.h"
#include "common.h"
Word2Int convert;
std::vector<std::string> nodes;
std::vector<std::string> procs;


// code of FastGraph taken from Appendix1 - hints for Q1
class FastGraph {
public:
    std::vector<std::vector<int>> adj_list;
    std::vector<int> out_counts;

} graph;

/// this is the function you need to (re)implement
///
/// parameter edges[] contains a list of request- and assignment- edges
///   example of a request edge, process "p1" resource "r1"
///     "p1 -> r1"
///   example of an assignment edge, process "XYz" resource "XYz"
///     "XYz <- XYz"
///
/// You need to process edges[] one edge at a time, and run a deadlock
/// detection after each edge. As soon as you detect a deadlock, your function
/// needs to stop processing edges and return an instance of Result structure
/// with 'index' set to the index that caused the deadlock, and 'procs' set
/// to contain names of processes that are in the deadlock.
///
/// To indicate no deadlock was detected after processing all edges, you must
/// return Result with index=-1 and empty procs.
///
Result find_deadlock(const std::vector<std::string> & edges)
{
    // implementing the pseudocode given in Appendix1 - hints for Q1
    // from our Assignment manual

    //Result result
    Result result;

    //for i = 0 to edges.size():
    for (long j = 0 ; j < long(edges.size()) ; j++) 
    {
        std::string edge = edges[j];
        //parse edge represented by edges[i]
        std::vector<std::string> parsing = split(edge);

        // adding a prefix P_ and R_ to separate process from resouce
        std::string process = "P_" + parsing[0];
        std::string resource = "R_" + parsing[2];

        // converting process and resource to corresponding integers
        int process_integer = convert.get(process);
        int resource_integer = convert.get(resource);
        int begin;
        int end;

        // insering appropriate edge into graph        
        if ((long)nodes.size() == process_integer)
        {
            nodes.push_back(process);
        }        
        if ((long)nodes.size() == resource_integer)
        {
            nodes.push_back(resource);
        }
        if (parsing[1] == "<-")
        {
            // dealing with P <- R
            begin = resource_integer;
            end = process_integer;

        }
        else if (parsing[1] == "->")
        {
            // dealing with P-> R
            begin = process_integer;
            end = resource_integer;
        }else
        {
            // dealing with invalid input
            continue;
        }

        //resizeing to the correct number of node to remove segmentation fault
        long size_of_nodes = (long)nodes.size();
        graph.adj_list.resize(size_of_nodes);
        graph.out_counts.resize(size_of_nodes);

        // inseting correct edge to graph
        graph.adj_list[end].push_back(begin);

        // updating out_counts
        graph.out_counts[begin]++;

        //starting topological sort
        // converting pseudo-code given in Appendix1- hints for Q1
        //for topological sort into c++ code

        //out = out_counts # copy out_counts so that we can modify it
        std::vector<int> out = graph.out_counts;

        //zeros[] = find all nodes in graph with outdegree == 0
        std::vector<int> zeros;

        for(long i = 0 ; i < long(out.size()) ; i++)
        {
            if(out[i]==0)
            {
                zeros.push_back(i);
            }
        }
       
        //while zeros is not empty:
        while (!zeros.empty()) 
        {
            //n = remove one entry from zeros[]
            long n = zeros[zeros.size() - 1];
            zeros.pop_back();

            //for every n2 of adj_list[n]:
            for (auto n2 : graph.adj_list[n])
            {
                //out[n2] --
                out[n2]--;

                //if out[n2] == 0:
                if (out[n2] == 0)
                {
                    //append n2 to zeros[]
                    zeros.push_back(n2);
                }
            }
        }

        //procs[] = all nodes n that represent a process and out[n]>0

        for(long i = 0 ; i < long(out.size()) ; i++)
        {
            if(out[i]>0)
            {
                std::string my_string = nodes[i];
                if (my_string[0] == 'P')
                {
                    procs.push_back(my_string.erase(0, 2));
                }
            }

        }

        //if toposort detected cycle:
        if (!procs.empty())
        {
            //result.procs = nodes that toposort did not remove that represent processes
            result.procs = procs;

            //result.index = i
            result.index = j;

            //return result
            return result;
        }
    }

    //result.index = -1
    result.index = -1;
    return result;
}