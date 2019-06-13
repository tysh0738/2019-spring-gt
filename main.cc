/* 
    Your main program goes here
*/
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <queue>
#include <map>
#include "network_manager.h"
#include "gplot.h"
#include "path.h"

using namespace std;

// create NetworkManager first
NetworkManager *nm = new NetworkManager();
map<string, vector<Edge *>> adv;
//vector<Edge *> bfs_paths;
//vector<Vertex *> existed_vertices;
vector<string> vertex_name;
vector<string> odd_vertex;
vector<int> degree;
//queue<Edge *> bfs_search;

void bfs(string src);

int main(int argc, char** argv){

    // 1.create and print graph
    nm->interpret("./topo3.txt");
    nm->print_all_e();
    nm->print_all_v();

    // 2.odd or even degree
    // record vetices' name
    Vertex *header;
    for(int i=0;i<nm->tablesize;i++){
        if(nm->vlist[i] != NULL) {
            header=nm->vlist[i];
            vertex_name.push_back(header->name);
            //cout<<vertex_name[i]<<endl;
         }
         else break;
    }

    // calculate degree
    cout<<"==========================================="<<endl<<"degree:"<<endl;

    int *degree;
    degree = new int [nm->tablesize];

    for(int i=0;i<nm->tablesize;i++){
        if(nm->vlist[i] != NULL){
            for(int j=0;j<nm->tablesize;j++){
                if(nm->vlist[j] != NULL){
                    if(!nm->connected(vertex_name[i], vertex_name[j]))  degree[i]++;                 
                }
                else break;
            }
             cout<<vertex_name[i]<<": "<<degree[i]<<endl;
             if(degree[i]%2==1){ // store odd degree's vertices
               odd_vertex.push_back(vertex_name[i]);              
             }
        }
        else break;

    }    

    cout<<"odd degree's vertices:"<<endl;
    for(int i=0;i<odd_vertex.size();i++)
        cout<<odd_vertex[i]<<" ";

    cout<<endl<<endl<<"degree finish"<<endl<<"============================================="<<endl;

    // 3.find maximum matching of odd vertices and add edges (if there are odd degree's vertice)
if(odd_vertex.size() != 0){

    Path *path;
    path = new Path();
    path->append(nm->elist);
    std::vector<std::vector<Edge *>> avail_path; 

    vector<int> path_length[odd_vertex.size()-1];
    vector<string> path_vertex[odd_vertex.size()-1];

    // find all matching, and save result in path_length and path_vertex
    for(int i=1;i<odd_vertex.size();i++){
        //matching: part1
        avail_path = path->find_paths(std::string(odd_vertex[0]), std::string(odd_vertex[i]));
        //path->debug();

        int shorest_path = 0;
        for(int j=0;j<avail_path.size();j++){ // choose shorest path
           if(avail_path[j].size()<avail_path[0].size()) shorest_path = j;
        }

        path_length[i-1].push_back(avail_path[shorest_path].size());
        //cout<<"path_length: "<<path_length[i-1][0]<<endl;

        for(int j=0; j<avail_path[shorest_path].size();j++)
            path_vertex[i-1].push_back(avail_path[shorest_path][j]->head->name);
        path_vertex[i-1].push_back(odd_vertex[i]);

        //matching: part2
        if(odd_vertex.size()>2){
            int s=( (i+1) > (odd_vertex.size()-1) )?(i+2-odd_vertex.size()):(i+1);
            int t=( (i+2) > (odd_vertex.size()-1) )?(i+3-odd_vertex.size()):(i+2);
            avail_path = path->find_paths(std::string(odd_vertex[s]), std::string(odd_vertex[t]));
            //path->debug();

            shorest_path = 0;
            for(int j=0;j<avail_path.size();j++){ // choose shorest path
               if(avail_path[j].size()<avail_path[0].size()) shorest_path = j;
            }

            //cout<<"shorest path:" <<shorest_path<<endl;

            path_length[i-1].push_back(avail_path[shorest_path].size());
            //cout<<"path_length: "<<path_length[i-1][1]<<endl;

            for(int j=0; j<avail_path[shorest_path].size();j++)
                path_vertex[i-1].push_back(avail_path[shorest_path][j]->head->name);
            path_vertex[i-1].push_back(odd_vertex[t]);
      
        // calculate total path length
        path_length[i-1].push_back(path_length[i-1][0]+path_length[i-1][1]);
        }
    }

    cout<<"path_length:"<<endl;
    for(int i=0;i<odd_vertex.size()-1;i++){
        for(int j=0;j<3;j++){
            cout<<path_length[i][j]<<" ";
        }cout<<endl;
    }cout<<endl;

    cout<<"path_vertex:"<<endl;
    for(int i=0;i<odd_vertex.size()-1;i++){
        for(int j=0;j<path_vertex[i].size();j++){
            cout<<path_vertex[i][j]<<" ";
        }cout<<endl;
    }cout<<endl;

    //maximum matching
    int max_matching=0;
    if(odd_vertex.size()>2){
        for(int i=1;i<odd_vertex.size()-1;i++){
           if(path_length[i][2]<path_length[max_matching][2])
               max_matching = i;
        }
    }
    //cout<<"max_matching:" <<max_matching<<endl;

    // add edge for reapted path and set capacity to 3
    for(int i=0;i<path_length[max_matching][0];i++){
       nm->connect_r(std::string(path_vertex[max_matching][i]),std::string(path_vertex[max_matching][i+1]));
       nm->get_edge(std::string(path_vertex[max_matching][i]),std::string(path_vertex[max_matching][i+1]))->set_cap(3);
       nm->get_edge(std::string(path_vertex[max_matching][i+1]),std::string(path_vertex[max_matching][i]))->set_cap(3);
    }

    if(odd_vertex.size()>2){
        for(int i=0;i<path_length[max_matching][1];i++){
           nm->connect_r(std::string(path_vertex[max_matching][i+path_length[max_matching][0]+1]),std::string(path_vertex[max_matching][i+path_length[max_matching][0]+2]));
           nm->get_edge(std::string(path_vertex[max_matching][i+path_length[max_matching][0]+1]),std::string(path_vertex[max_matching][i+path_length[max_matching][0]+2]))->set_cap(3);
           nm->get_edge(std::string(path_vertex[max_matching][i+path_length[max_matching][0]+2]),std::string(path_vertex[max_matching][i+path_length[max_matching][0]+1]))->set_cap(3);
    }
    }
    //nm->print_all_e();
    //nm->print_all_v();
}
    // 4.find chinese postman shortest circuit
    // calcuate the number of edges in new graph(that is minmum path length*2)     
    Edge *traversal = nm->elist;
    int min_length = 0;
    while(traversal!=NULL) {
        min_length++;
        traversal=traversal->next;
    }
    min_length=min_length/2; // because use two di-edge to implement non-direction edge
    //cout<<"minmum path length: "<<min_length<<endl;

    // find shorest circuit
    vector<string> circuit; // record vertices on the shorest circuit
    int current = 0; // circuit begin at vertex_name[0]
    int next_temp = 0; // be next vertex if no connected edge's capacity=3
    int check = 0;
    circuit.push_back(vertex_name[current]);

    for(int i=0;i<min_length;i++){
        check=0;
        for(int j=0;j<vertex_name.size();j++){
            if( !nm->connected(std::string(vertex_name[current]),std::string(vertex_name[j])) ){
                if(nm->get_edge(std::string(vertex_name[current]),std::string(vertex_name[j]))->cap == 3){
                    circuit.push_back(vertex_name[j]);
                    nm->disconnect(std::string(vertex_name[current]),std::string(vertex_name[j]));
                    nm->disconnect(std::string(vertex_name[j]),std::string(vertex_name[current]));
                    current=j;
                    check=1;
                    break;
                }
                else 
                    next_temp = j;
            }
        }
        if(check==0){
            circuit.push_back(vertex_name[next_temp]);
            nm->disconnect(std::string(vertex_name[current]),std::string(vertex_name[next_temp]));
            nm->disconnect(std::string(vertex_name[next_temp]),std::string(vertex_name[current]));
            current = next_temp;  
        }
        //nm->print_all_e();        
    }

    // print result
    cout<<"========================================="<<endl;
    cout<<"        POSTMAN SHORSTEST CIRCUIT        "<<endl;
    cout<<"========================================="<<endl;
    cout<<"minmum path length: "<<min_length<<endl;

    cout<<"path:"<<endl<<circuit[0];
    for(int i=1;i<circuit.size();i++){
       cout<<" -> "<<circuit[i];
    }cout<<endl;

    nm->clear();

    return 0;
}
