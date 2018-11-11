#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <utility>
#include <limits>

using namespace std;

#define MAX_NODES 100
const int infinity = numeric_limits<int>::max();

class graph{
private:
    const int static size_of_weight_matrix = MAX_NODES;
    int weight_matrix[size_of_weight_matrix][size_of_weight_matrix];
    int number_of_nodes;
    const static int default_value_for_no_connection = -1.0;

public:
    graph(){
        for(int i = 0; i < size_of_weight_matrix; i++)
            for(int j = 0; j < size_of_weight_matrix; j++)
                weight_matrix[i][j] = default_value_for_no_connection;
        number_of_nodes = 0;
    }

    int add_edge(int node1, int node2, int weight){
        if(node1 >= 100 || node2 >= 100){
            cout << "Too many nodes!";
            return 0;
        }
        weight_matrix[node1][node2] = weight;
        weight_matrix[node2][node1] = weight;
        if(number_of_nodes < node1 + 1 || number_of_nodes < node2 + 1)
            number_of_nodes = (node1 > node2)? node1 + 1: node2 + 1;
        return 1;
    }

    void print_graph(){
        for(int i = 0; i < number_of_nodes; i++){
            cout << "Node " << i << " is connected to" << endl;
            for(int j = 0; j < number_of_nodes; j++)
                if(weight_matrix[i][j] != default_value_for_no_connection)
                    cout << "\t" << j << "(" << weight_matrix[i][j] << ")" << endl;
        }
    }

    int get_weight(int node1, int node2){
        return weight_matrix[node1][node2];
    }

    int get_number_of_nodes(){
        return number_of_nodes;
    }

    map<int, vector< pair<int, int> > > get_weight_to_edges_map(){
        map<int, vector< pair<int, int> > > weight_to_edges;
        for(int i = 0; i < number_of_nodes; i++){
            for(int j = i; j < number_of_nodes; j++){
                if(weight_matrix[i][j] != default_value_for_no_connection){
                    if(weight_to_edges.find(weight_matrix[i][j]) == weight_to_edges.end())
                        weight_to_edges[weight_matrix[i][j]] = vector <pair <int, int> >();
                    weight_to_edges[weight_matrix[i][j]].push_back(make_pair(i, j));
                }
            }
        }

        return weight_to_edges;
    }

};

// #define print_in_pairs(a, b, c) cout << a << "\t<" << b << "," << c << ">" << endl;
//
// void print_vector_of_ints(vector <int> object){
//     for(auto a: object)
//         cout << a << " ";
//     cout << endl;
// }
//
// void print_vector_of_vector_of_ints(vector <vector <int> > object){
//     for(vector <vector <int> > :: iterator it1 = object.begin(); it1 != object.end(); ++it1){
//         for(vector <int> :: iterator it2 = it1->begin(); it2 != it1->end(); ++it2)
//             cout << *it2 << " ";
//         cout << "\t";
//     }
//     cout << endl;
// }

vector <vector <int> > :: iterator find_the_set_in_set_where_it_belongs(vector <vector <int> > &set_of_sets, int value){
    for(vector <vector <int> > :: iterator set_it = set_of_sets.begin(); set_it != set_of_sets.end(); ++set_it)
        for(vector <int> :: iterator vec_it = (*set_it).begin(); vec_it != (*set_it).end(); ++ vec_it)
            if(*vec_it == value){
                return set_it;
            }
}

void kruskal(graph G){
    graph temp;     //for storing the kruskal spanning tree
    map<int, vector <pair<int, int> > > weight_to_edges_map = G.get_weight_to_edges_map();  //mapping of weight to edges(sorted automatically)
    vector <vector<int> > kruskal_set; //implementing set as vectors (no repeating elements so set union is easy)

    //creating my initial set with all nodes disjoint initially
    for(int i = 0; i < G.get_number_of_nodes(); i++){
        vector <int> new_set;
        new_set.push_back(i);
        kruskal_set.push_back(new_set);
    }

    //main loop for set find and union
    for(map<int, vector <pair<int, int> > > :: iterator map_it = weight_to_edges_map.begin(); map_it != weight_to_edges_map.end(); ++map_it)
        for(vector <pair<int, int> >:: iterator vec_it = (map_it->second).begin(); vec_it != (map_it->second).end(); ++vec_it){

            //iterators to required sets
            vector <vector<int> > :: iterator it1 = find_the_set_in_set_where_it_belongs(kruskal_set, vec_it->first);
            vector <vector<int> > :: iterator it2 = find_the_set_in_set_where_it_belongs(kruskal_set, vec_it->second);
            vector <int> dummy_union_set;

            //set union and add if above are disjoint sets
            if(it1 != it2){
                for(int a: *it1)
                    dummy_union_set.push_back(a);
                for(int a: *it2)
                    dummy_union_set.push_back(a);
                kruskal_set.push_back(dummy_union_set);
                kruskal_set.erase(find_the_set_in_set_where_it_belongs(kruskal_set, vec_it->first));
                kruskal_set.erase(find_the_set_in_set_where_it_belongs(kruskal_set, vec_it->second));
                temp.add_edge(vec_it->first, vec_it->second, map_it->first);
            }
        }
    cout << "\n\nResults of kruskal:\n";
    temp.print_graph();
    cout << endl << endl;
}

void print_map_of_pair_of_int(map <int, pair<int, int> > object){
    for(auto a: object){
        cout << a.first << " : " << a.second.first << ", " << a.second.second << "\t";
    }
    cout << endl;
}

void prim(graph G){
    graph prim_tree;     //for storing the prim spanning tree
    map <int, pair<int, int> > leftover_vertices;    //FORM:pair<min_weight, min_node> -> the vertices yet to be added to the spanning tree

    //all vertices initialized to infinity in the set of not-added
    for(int i = 1; i < G.get_number_of_nodes(); i++)    //we dont add the 0th vertex. We assume its added and start from there
        leftover_vertices[i] = pair<int, int> (infinity, -1);

    int last_add_vertex = 0;

    while(!leftover_vertices.empty()){
        //first we will update values of the remaining values in the map
        for(auto vertex: leftover_vertices){
            int my_weight = G.get_weight(vertex.first, last_add_vertex);
            if(my_weight != -1 && my_weight < leftover_vertices[vertex.first].first){
                //                             pair <min_weight, min_node>
                leftover_vertices[vertex.first] = pair<int, int> (my_weight, last_add_vertex);
            }
        }

        print_map_of_pair_of_int(leftover_vertices);

        //find the minimum
        int min_node = leftover_vertices.begin() -> first;
        for(auto vertex: leftover_vertices)
            if(leftover_vertices[vertex.first].first < leftover_vertices[min_node].first)
                min_node = vertex.first;

        //add it to the prim MST
        prim_tree.add_edge(min_node, leftover_vertices[min_node].second, leftover_vertices[min_node].first);
        leftover_vertices.erase(min_node);
        last_add_vertex = min_node;
    }
    cout << "Results of Prim:\n";
    prim_tree.print_graph();
    cout << "\n\n";
}

int main(){
    graph G;
    G.add_edge(0, 1, 5);
    G.add_edge(1, 2, 1);
    G.add_edge(2, 3, 6);
    G.add_edge(0, 3, 5);
    G.add_edge(0, 2, 4);
    G.add_edge(1, 3, 9);

    cout << "Original graph:\n";
    G.print_graph();

    kruskal(G);
    prim(G);

    return 0;
}
