#include <vector>
#include <string>
#include <map>

using namespace std;

class GraphSearch
{
public:
	GraphSearch();
	~GraphSearch();


	struct point {
		double x;
		double y;
		string name;
	};


	void defineNode(double x, double y);
	void defineNode(double x, double y, string name);
	void defineEdge(double x1, double y1, double x2, double y2);
	void defineEdge(string name1, string name2);
	void defineEdge(string name1, double x, double y);
	vector<point> getPath(string name1, string name2);
	vector<point> getPath(string name1, double x, double y);
	vector<point> getPath(double x1, double y1, double x2, double y2);
	vector<point> getPath(double x, double y, string name1);


	vector< vector<point> > *theGraph;

};