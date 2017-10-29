#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <queue>
#include <functional>
#include <algorithm>
#include <limits>

using namespace std;

class Nodes {
public:
  string name;
  priority_queue<tuple<float,string>, vector<tuple<float,string>>, greater<tuple<float,string>>> edges;
  void set_values(string,  priority_queue<tuple<float,string>, vector<tuple<float,string>>, greater<tuple<float,string>>> );
  void add_edge(tuple<float,string> );
};

void Nodes::set_values (string x, priority_queue<tuple<float,string>, vector<tuple<float,string>>, greater<tuple<float,string>>>e ) {
  name = x;
  edges=e;
}

void Nodes::add_edge (tuple<float,string> e ) {
  edges.push(e);
}


int dijkstra(vector<Nodes> nodes, Nodes start, Nodes end){
  int pos=stoi(start.name);
  priority_queue<tuple<float,string>, vector<tuple<float,string>>, greater<tuple<float,string>>> edges={};
  for(size_t i=0; i<nodes[pos].edges.size();i++){
    edges.push(nodes[pos].edges.top());
    nodes[pos].edges.pop();
  }
  //Path is a vector of the distance to the source, the path to the nodes and the name of target t
  vector<tuple<float,vector<string>,string>> paths;
  float infi =numeric_limits<float>::infinity();

  for(size_t i=0; i<nodes.size();i++)
  {
    tuple<float,vector<string>,string> elem (infi,{},nodes[i].name);
    paths.push_back(elem);
  }
  get<1>(paths[pos]).push_back(nodes[pos].name);
  string a;
  int nextNode =-1;
  float length=0;
  tuple <float,string> test;
  if (!(edges.empty())){
    test=edges.top();
    a=get<1>(test);
    nextNode=stoi(a);
    //int nextNode=stoi(get<1>(edges.top()));

    length= get<0>(edges.top());
    edges.pop();
  }


  while((nextNode!=stoi(end.name)) & (nextNode!=-1)){
  // Check if new edges give a shorter path to node
  for(size_t i=0; i<nodes[nextNode].edges.size();i++){
    int newPos= stoi(get<1>(nodes[nextNode].edges.top()));
    float newLen= length+(get<0>(nodes[nextNode].edges.top()));
    vector<string> path2newNode=get<1>(paths[nextNode]);
    path2newNode.push_back(get<2>(paths[nextNode]));
    if (newLen< get<0>(paths[newPos])){
      get<0>(paths[newPos])=newLen;
      get<1>(paths[newPos])=path2newNode;
      tuple <float,string> edgi (newLen,to_string(newPos));
      edges.push(edgi);

    }
    nodes[nextNode].edges.pop();
  }
  if (!(edges.empty())){
  nextNode=stoi(get<1>(edges.top()));
  length= get<0>(edges.top());
  edges.pop();}
  else{nextNode=-1;}

  }
  pos=stoi(end.name);
  if (get<0>(paths[pos])!=infi){
    cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    cout.precision(2);
    cout<< get<0>(paths[pos]);
    cout<<"\n";
    get<1>(paths[pos]).push_back(nodes[pos].name);
    string out="";
    cout<<start.name;
    cout<<" ";
    for(size_t i=0; i<get<1>(paths[pos]).size();i++)
    {out+=get<1>(paths[pos])[i]+" ";}
    out+="\n";
    cout<<out;
    return 0;
  }
  else{return 1;}
  return 0;
}


int main(int argc, char const *argv[]) {
  // If there are not enough arguments given
  if (argc!=4){return 2;}
  string line;
  vector<Nodes> nodes;
  ifstream myfile ( argv[1] );
  if ( !myfile.is_open() ){
    return 2;}
    else {
      int start=1;
      int j=0;
      while ( getline (myfile,line) )
    {
      if (line[0]=='#')
      {start=0; }
      else if(start==1) {
        /*int i=0;
        string word="";
        while(line[i]!=' '){
          i++;
          word +=line[i];
        }
        cout<<word;*/

        Nodes node;
        node.set_values(to_string(j),{});
        nodes.push_back(node);
        j++;
      }
      else{
        string node1="";
        string node2="";
        string dist="";
        int i=0;
        while(line[i]!=' '){
          node1 +=line[i];
          i++;
        }
        i++;
        while(line[i]!=' '){
          node2 +=line[i];
          i++;
        }
        i++;
        while(line[i]!='\n'){
          dist +=line[i];
          i++;
        }
        //Assumes all nodes are given, so name of node is it position in list
        int pos=stoi(node1);
        tuple<float,string> edge (stof(dist),node2);
        nodes[pos].add_edge(edge);

      }

    }

  }
  //cout<< nodes[0];
  int pos_start=stoi(argv[2]);
  int pos_end=stoi(argv[3]);
  Nodes start=nodes[pos_start];
  Nodes end=nodes[pos_end];
  return dijkstra(nodes,start,end);
}
