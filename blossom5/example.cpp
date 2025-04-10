#include "example.h"

void LoadString(int& node_num, int& edge_num, int*& edges, int*& weights, const string& data)
{
    int e = 0;
    
    int format = -1; // 0: DIMACS format. node id's start from 1
                     // 1: simpler format (without "p" and "e"). node id's start from 0
    
    edge_num = -1;
    
    // Utilisation d'un stringstream pour lire la chaîne ligne par ligne
    std::istringstream stream(data);
    std::string line;
    
    while (std::getline(stream, line))
    {
        if (line.empty()) continue;
        if (line[0] == 'c') continue;
        
        if (format < 0)
        {
            if (line[0] == 'p')
            {
                format = 0;
                if (sscanf(line.c_str(), "p edge %d %d", &node_num, &edge_num) != 2) 
                { 
                    printf("Input string: wrong format #1\n"); 
                    exit(1); 
                }
            }
            else
            {
                format = 1;
                if (sscanf(line.c_str(), "%d %d", &node_num, &edge_num) != 2) 
                { 
                    printf("Input string: wrong format #1\n"); 
                    exit(1); 
                }
            }
            
            //////////////////////////////////////////////////////////////////////////////////
            if (node_num <= 0 || edge_num < 0) 
            { 
                printf("# of nodes and edges should be positive\n"); 
                exit(1); 
            }
            if (node_num & 1) 
            { 
                printf("# of nodes is odd: perfect matching cannot exist\n"); 
                exit(1); 
            }
            edges = new int[2*edge_num];
            weights = new int[edge_num];
            //////////////////////////////////////////////////////////////////////////////////
        }
        else
        {
            int i, j, len;
            if (format == 0)
            {
                if (line[0] != 'e') continue;
                if (sscanf(line.c_str() + 1, "%d %d %d", &i, &j, &len) != 3) continue;
                i--; j--; // Ajustement car format DIMACS commence à 1
            }
            else
            {
                if (sscanf(line.c_str(), "%d %d %d", &i, &j, &len) != 3) continue;
            }
            
            edges[2*e] = i;
            edges[2*e+1] = j;
            weights[e] = len;
            e++;
        }
    }
    
    if (e != edge_num) 
    { 
        printf("Input string: wrong format #3\n"); 
        exit(1); 
    }
}


string SaveMatching(int node_num, PerfectMatching* pm)
{
	stringstream ss;
	ss << node_num << " " << node_num/2 << endl;
	int i, j;
	for (i=0; i<node_num; i++)
	{
		j = pm->GetMatch(i);
		if (i < j) ss << i << " " << j << endl;
	}
	return ss.str();
}





string coupling(string data)
{
	struct PerfectMatching::Options options;
	bool check_perfect_matching = false;
	int e, node_num, edge_num;
	int* edges;
	int* weights;


	options.verbose = false;

	LoadString(node_num, edge_num, edges, weights, data);


	PerfectMatching *pm = new PerfectMatching(node_num, edge_num);
	for (e=0; e<edge_num; e++) pm->AddEdge(edges[2*e], edges[2*e+1], weights[e]);
	pm->options = options;
	pm->Solve();
	if (check_perfect_matching)
	{
		int res = CheckPerfectMatchingOptimality(node_num, edge_num, edges, weights, pm);
		printf("check optimality: res=%d (%s)\n", res, (res==0) ? "ok" : ((res==1) ? "error" : "fatal error"));
	}
	double cost = ComputePerfectMatchingCost(node_num, edge_num, edges, weights, pm);
	string outpout = SaveMatching(node_num, pm);
	delete pm;


	delete [] edges;
	delete [] weights;

	return outpout;
}


