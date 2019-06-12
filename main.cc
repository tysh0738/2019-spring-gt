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
vector<string> edge_name;
vector<int> degree;
queue<Edge *> bfs_search;

void bfs(string src);

int main(int argc, char** argv){

    // create and print graph
    nm->interpret("./topo.txt");
    nm->print_all_e();
    nm->print_all_v();

    // odd or even degree
    Vertex *header;
    for(int i=0;i<nm->tablesize;i++){
        if(nm->vlist[i] != NULL) {
            header=nm->vlist[i];
            edge_name.push_back(header->name);
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
                    if(!nm->connected(edge_name[i],edge_name[j]))  degree[i]++;                 
                }
                else break;
            }cout<<edge_name[i]<<": "<<degree[i]<<endl;
        }
        else break;

    }    

    cout<<"degree finish"<<endl;

    // all even degree: Eulerian path

    // have even degree: maximum matching, BFS    


    /*Path *path;
    path = new Path();
    path->append(nm->elist);
    std::vector<std::vector<Edge *>> avail_path = path->find_paths(std::string("a"), std::string("c"));
    path->debug();*/

     //Edge *elist = nm->elist;

    // build the edges belongs to vertex
    /*while(elist!=NULL){
        adv[elist->head->name].push_back(elist);
        elist=elist->next;
    }

    //BFS
    existed_vertices.push_back(nm->get_node("a"));
    bfs("a");
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
    }


    Gplot *gp = new Gplot();
    gp->gp_add(nm->elist);
    gp->gp_dump(true);
    gp->gp_export("bfs");*/

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
