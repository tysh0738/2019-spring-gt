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
vector<Edge *> bfs_paths;
vector<Vertex *> existed_vertices;
vector<string> vertex_name;
vector<string> odd_vertex;
vector<int> degree;
queue<Edge *> bfs_search;

void bfs(string src);

int main(int argc, char** argv){

    // 1.create and print graph
    nm->interpret("./topo.txt");
    nm->print_all_e();
    nm->print_all_v();

    // 2.odd or even degree
    Vertex *header;
    for(int i=0;i<nm->tablesize;i++){
        if(nm->vlist[i] != NULL) {
            header=nm->vlist[i];
            vertex_name.push_back(header->name);
            //cout<<edge_name[i]<<endl;
         }
         else break;
    }

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

    //cout<<endl<<"index_odd:"<<index_odd;
    cout<<endl<<endl<<"degree finish"<<endl<<"============================================="<<endl;

    // 3.find maximum matching of odd vertices and add edges
    Edge *elist = nm->elist; 

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

        /*cout<<"path_vertex: ";
        for(int j=0;j<=avail_path[shorest_path].size();j++)
            cout<<path_vertex[i-1][j]<<" ";
        cout<<endl;
        */

        //matching: part2
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

        /*cout<<"path_vertex: ";
        for(int j=0;j<=path_vertex[i-1].size();j++)
            cout<<path_vertex[i-1][j]<<" ";
        cout<<endl;
        */
        
        // calculate total path length
        path_length[i-1].push_back(path_length[i-1][0]+path_length[i-1][1]);

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
    for(int i=1;i<odd_vertex.size()-1;i++){
       if(path_length[i][2]<path_length[max_matching][2])
           max_matching = i;
    }
    //cout<<"max_matching:" <<max_matching<<endl;

    // add edge for reapted path
    for(int i=0;i<path_length[max_matching][0];i++){
       nm->connect_r(std::string(path_vertex[max_matching][i]),std::string(path_vertex[max_matching][i+1]));
    }

    for(int i=0;i<path_length[max_matching][1];i++){
       nm->connect_r(std::string(path_vertex[max_matching][i+path_length[max_matching][0]+1]),std::string(path_vertex[max_matching][i+path_length[max_matching][0]+2]));
    }

    nm->print_all_e();
    nm->print_all_v();

    // 4.find postman shortest cycle
 
   // build the edges belongs to vertex
   /* while(elist!=NULL){
        adv[elist->head->name].push_back(elist);
        elist=elist->next;
    }

    //BFS
    existed_vertices.push_back(nm->get_node("e"));
    bfs("e");
    while(!bfs_search.empty()){
        bfs(bfs_search.front()->tail->name);
        bfs_search.pop();
    }

    Edge *bfs_elist=NULL, *tmp=bfs_elist;

    cout << "BFS: " << endl;
    for(int i=0;i<bfs_paths.size();i++){
        cout << bfs_paths.at(i)->head->name << "->" << bfs_paths.at(i)->tail->name << endl;
        if(tmp==NULL){
            bfs_elist = new Edge(bfs_paths.at(i));
            tmp = bfs_elist;
        } else {
            tmp->next = new Edge(bfs_paths.at(i));
            tmp = tmp->next;
        }
    }*/


   /* Gplot *gp = new Gplot();
    gp->gp_add(nm->elist);
    gp->gp_dump(true);
    gp->gp_export("bfs");
   */
    nm->clear();

    return 0;
}

void bfs(string src){
    for(int i=0;i<adv[src].size();i++){
        // cout << adv[src].at(i)->head->name << "=>" << adv[src].at(i)->tail->name << endl;
        // check the tail is in the existed_vertices or not
        if(find(bfs_paths.begin(), bfs_paths.end(), adv[src].at(i))==bfs_paths.end()){
            // if tail is not existed, then push into existed vertices
            if(find(existed_vertices.begin(), existed_vertices.end(), adv[src].at(i)->tail)==existed_vertices.end()){
                // and push this edge into bfs result
                bfs_paths.push_back(adv[src].at(i));
                // not existed, then push into existed
                existed_vertices.push_back(adv[src].at(i)->tail);
                // push
                bfs_search.push(adv[src].at(i));
            }
        }
    }
}
