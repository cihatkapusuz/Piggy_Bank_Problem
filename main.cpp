#include<iostream>
#include <list>
#include <stack>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;
class Graph
{
public:
    
    int V;
    list<int> *adj;
    vector <int> groups;
    vector <int> one_element_of_scc;
    int n_of_scc=0;
    int dfs_index = 0;

    void SCCUtil(int u, int bankNumber[], int low[],
                 stack<int> *st, bool stackMember[]);
    Graph(int V);
    void addEdge(int v, int w);
    void SCC();
};

Graph::Graph(int V) {

    this->V = V;
    adj = new list<int>[V+1];
    for (int i = 0; i <=V; ++i) {
        groups.push_back(0);
    }
    for (int i = 0; i <=V; ++i) {
        one_element_of_scc.push_back(0);
    }
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
}

void Graph::SCCUtil(int u, int bankNumber[], int low[], stack<int> *st,
                    bool stackMember[])
{

    bankNumber[u] = low[u] = ++dfs_index;
    st->push(u);
    stackMember[u] = true;

   list<int>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i)
    {
        int v = *i;
        if (bankNumber[v] == -1)
        {
            SCCUtil(v, bankNumber, low, st, stackMember);
            low[u] = min(low[u], low[v]);
        }

        else if (stackMember[v] == true)
            low[u] = min(low[u], bankNumber[v]);
    }

    int w = 0;
    if (low[u] == bankNumber[u])
    {
        while (st->top() != u)
        {
            w = (int) st->top();
            groups[w]=n_of_scc;
            stackMember[w] = false;
            st->pop();
        }
        w = (int) st->top();
        stackMember[w] = false;
        st->pop();
        groups[w]=n_of_scc;
        one_element_of_scc[n_of_scc]=w;
        n_of_scc++;
     }
}

void Graph::SCC()
{
    int *bankNumber = new int[V+1];
    int *low = new int[V+1];
    bool *stackMember = new bool[V+1];
    stack<int> *st = new stack<int>();
    for (int i = 1; i <= V; i++)
    {
        bankNumber[i] = -1;
        low[i] = -1;
        stackMember[i] = false;
    }
    for (int i = 1; i <= V; i++)
        if (bankNumber[i] == -1)
        {
            SCCUtil(i, bankNumber, low, st, stackMember);
        }
}
int unbreakable[100005];
int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    using namespace std;
    if (argc != 3) {
        return 1;
    }

    ifstream infile(argv[1]);
    string line;
    getline(infile, line);
    stringstream  stream(line);
    int n_of_banks;
    stream >> n_of_banks;
    Graph piggy(n_of_banks);
    int n_of_keys;
    for(int i=0; i<n_of_banks; i++){
        getline(infile, line);
        stringstream  stream(line);
        stream >> n_of_keys;
        for(int j=0 ; j<n_of_keys; j++){
            int k;
            stream >> k;
            piggy.addEdge(i+1,k);
        }
    }
    piggy.SCC();
    for(int i=0; i<n_of_banks; i++){
        list<int>::iterator j;
        for (j = piggy.adj[i+1].begin(); j != piggy.adj[i+1].end(); ++j)
        {
            if(piggy.groups[*j]!=piggy.groups[i+1]){
                unbreakable[piggy.groups[*j]]=1;
            }
        }
    }
    ofstream myfile;
    myfile.open(argv[2]);
    int broken_ones=0;
    for(int i=0; i< piggy.n_of_scc; i++){
        if(unbreakable[i]==0){
            broken_ones++;
        }
    }
    myfile << broken_ones << " " ;

    for(int i=0; i< piggy.n_of_scc; i++){
        if(unbreakable[i]==0){
                myfile << piggy.one_element_of_scc[i] << " " ;
        }
    }
    myfile.close();


    return 0;
}
