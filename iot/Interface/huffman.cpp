#include <bits/stdc++.h>
#include "camera.h"
using namespace std;
Camera cam;

typedef basic_string<unsigned char> ustring;

struct node{
	bool fin;
	int freq, id;
	int c;
	int le, ri;
	node(bool _fin, int _freq, int _id){
		fin  =_fin, freq = _freq; id = _id;
	}
	node(){}
};

bool operator<(const node a, const node b){
	return a.freq > b.freq;
}

int freq[260];
string mapa[260];
priority_queue <node> pq;
vector<node> graph;

void create_graph(ustring s){
	for(uchar c: s)
		freq[c]++;
	for(int i = 0 ; i < 260 ; i++)
		if(freq[i]){ 
			node u = node(1, freq[i], graph.size());
			u.c = i;
			pq.push(u);
			graph.push_back(u);
		}
	
	while(pq.size() > 1){
		
		node u = pq.top(); pq.pop();
		node v = pq.top(); pq.pop();

		node w = node(0, u.freq + v.freq, graph.size());
		w.le = u.id; w.ri = v.id;
		graph.push_back(w), pq.push(w);
	}
}
int c = 0;
void explore_graph(int pos, int level = 0, string carr = ""){
	if(graph[pos].fin) mapa[graph[pos].c] =  carr;
	else{
		explore_graph(graph[pos].le, level + 1, carr + "1");
		explore_graph(graph[pos].ri, level + 1, carr + "0");
	}
}


void compress(ustring s){
	create_graph(s);
	cout << "hola\n";
	explore_graph(graph.size()-1);
	cout << "hola\n";
	for(char ch : s){
		c += mapa[ch].size();
	}
	cout << endl;
}

int main(){
	cam = Camera(0);
    cam.open();
	unsigned char buff[5000000];
	cam.getFrame();
	mat2Buff(&cam.bwframe, buff);
	ustring s(buff);
	cout << s.size() *8 << endl;
	compress(s);
	cout << 1.0*c / (s.size()*8) << endl;
	return 0;
}
