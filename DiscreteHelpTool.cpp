#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <climits>
#include <stack>

using namespace std;

void GraphMainMenu();

class Graph{
    public:
        Graph(int nodes);
        void PrintAdjacencyMatrix();
        void IsPathBetween();
        void EditAdjacencyMatrix();
        void ShortestDistanceBetween();
        void IsThisGraphAcyclic();
		void MultiplyMatrix();
    private:
        //fields
        vector<vector<int> > _adjacencyMatrix;
        vector<vector<int> > _shortestPaths;
        vector<bool> _isCircuit;
        int _nodes;

        //methods
        void _printMatrixForInput(int spot);
        bool _depthFirstSearchFor(int start, int end, int path[]);
        void _findAllShortestPaths();
        void _AcyclicTest();
		vector<vector<int> > _multiplyMatrix(int pow);
};

Graph::Graph(int nodes){
    _nodes = nodes;
    vector<int> tmp;
    for(int i = 0; i < nodes; i++){
        tmp.push_back(0);
    }
    for(int i = 0; i < nodes; i++){
        _adjacencyMatrix.push_back(tmp);
    }

    for(int i = 0; i < nodes; i++){
        for(int x = 0; x < nodes; x++){
            _printMatrixForInput((i*nodes) + x);
            cout << endl << "Enter Numbers into matrix, or -1 to delete last node's value: ";
            cin >> _adjacencyMatrix[i][x];
            if(_adjacencyMatrix[i][x] == -1){
                if(x != 0)
                    x -= 2;
                else if(i != 0){
                    i--;
                    x = nodes - 2;
                }
                else{
                    x = -1;
                }
            }
        }
    }

    for(int i = 0; i < _nodes; i++){
        _isCircuit.push_back(true);
    }

    _AcyclicTest();

    _findAllShortestPaths();
}

void Graph::PrintAdjacencyMatrix(){
    _printMatrixForInput(INT_MAX);
}

void Graph::IsPathBetween(){
    int start, end;
    cout << "First node in graph is 0 and last node is number of nodes - 1" << endl;
    cout <<"Start node: ";
    cin >> start;
    cout <<"End node  : ";
    cin >> end;
    int path[_nodes] = {0};

    if(_depthFirstSearchFor(start,end, path)){
        cout << "Yes, there is a walk between " << start << " and " << end << endl;
        cout << "Path: ";
        for(int i = 0; path[i+1] != -1; i++){
            cout << " " << path[i] << "-" << path[i+1] << " ";
        }
        cout << endl;
    }
    else
        cout << "No, there is not a walk between " << start << " and " << end << endl;
}


void Graph::EditAdjacencyMatrix(){
    _printMatrixForInput(INT_MAX);
    int start, end;
    cout << "Edit edge from: ";
    cin >> start;
    cout << "Edit edge from " << start << " to: ";
    cin >> end;
    cout << "The current value from " << start << " to " << end << " is " << _adjacencyMatrix[start][end] << endl;
    cout << "Change to: ";
    cin >> _adjacencyMatrix[start][end];
    
    _findAllShortestPaths();
    _AcyclicTest();

    _printMatrixForInput(INT_MAX);
        for(int i = 0; i < _nodes; i++){
        _isCircuit.push_back(true);
    }

}


void Graph::ShortestDistanceBetween(){
    int start, end, jnk[_nodes];
    cout << "Starting node: ";
    cin >> start;
    cout << "Ending node: ";
    cin >> end;
    if(_depthFirstSearchFor(start,end,jnk)){
        cout << "The shortest distance between node " << start << " and node " << end << " is " << _shortestPaths[start][end] << endl;
    }
    else
        cout << "There is no path between node " << start << "and node " << end << endl;
}



void Graph::IsThisGraphAcyclic(){

    bool isAcyclic = true;
    vector<int> notCircuits;

    for(int i = 0; i < _nodes; i++){
        if(_isCircuit[i] == false){
            isAcyclic = false;
            notCircuits.push_back(i);
        }
    }

    if(isAcyclic == false){
        cout << "No, this graph is not Acyclic, these are the nodes that have circuits: ";
        for(int i = 0; i < notCircuits.size(); i++){
            cout << notCircuits[i] << " ";
        }
        cout << endl;
    }

    else{
        cout << "Yes, this graph is Acyclic since it has no circuits" << endl;
    }

}

void Graph::MultiplyMatrix(){
    cout << "What power do you want to multiply the matrix to: ";
    int power;
    cin >> power;
    vector<vector<int> > NewMatrix;

    for(int i = 1; i <= power; i++){
        cout << "Power: " << i << endl;

     NewMatrix = _multiplyMatrix(power);
        for(int x = 0; x < _nodes; x++){
            cout << "|";
            for(int y = 0; y < _nodes; y++){
                printf("%4d", NewMatrix[x][y]);
                cout << "|";
            }
            cout << endl;
        }

    }
}

    //private functions
void Graph::_printMatrixForInput(int spot){
    system("clear");
    int first = spot / _nodes;
    int second = spot % _nodes;
    bool hasSpotHappened = false;
    for(int i = 0; i < _nodes; i++){
        cout << "| ";
        for(int x = 0; x < _nodes; x++){
            if(i == first && x == second){
                cout << "#";
                hasSpotHappened = true;
            }
            else if(!hasSpotHappened)
                    cout << _adjacencyMatrix[i][x];
            else
                    cout << " ";
            cout << " | ";
        }
        cout << endl;
        for(int x = 0; x <= _nodes * 4; x++)
            cout << "-";
        cout << endl;
    }
}

bool Graph::_depthFirstSearchFor(int start, int end, int path[]){
    stack<int> depthStack;
    stack<int> pathS;
    bool findAny = false;
    bool isUsed[_nodes] = {false};
    depthStack.push(start);
    int tmp;
    isUsed[start] = true;

    while(!depthStack.empty()){
        tmp = depthStack.top();
        pathS.push(tmp);
        depthStack.pop();
        for(int i = 0; i < _nodes; i++){
            if(_adjacencyMatrix[tmp][i] != 0 && isUsed[i] == false){
                if(i == end){
                    pathS.push(i);
                    int q = pathS.size() - 1;
                    path[q+1] = -1;
                    while(!pathS.empty()){
                        path[q] = pathS.top();
                        pathS.pop();
                        q--;
                    }
                    return true;
                }
                isUsed[i] = true;
                depthStack.push(i);
                findAny = true;
            }
        }
        if(findAny == false)
            pathS.pop();
        findAny = false;
    }
    return false;
}


//Find all shortest paths using Floyd Warshall's algorithm
void Graph::_findAllShortestPaths(){
    vector<int> tmp;
    for(int i = 0; i < _adjacencyMatrix.size(); i++){
        tmp.push_back(INT_MAX);        
    }
    for(int i = 0; i < _adjacencyMatrix.size(); i++){
        _shortestPaths.push_back(tmp);
    }
    int i, j, k;
 
    for (i = 0; i < _adjacencyMatrix.size(); i++)
        for (j = 0; j < _adjacencyMatrix[1].size(); j++){
            if(_adjacencyMatrix[i][j] != 0)
            _shortestPaths[i][j] = _adjacencyMatrix[i][j];
        }
    for (k = 0; k < _adjacencyMatrix.size(); k++)
    {
        for (i = 0; i < _adjacencyMatrix.size(); i++)
        {
            for (j = 0; j < _adjacencyMatrix.size(); j++)
            {
                if (_shortestPaths[i][k] + _shortestPaths[k][j] < _shortestPaths[i][j] && _shortestPaths[i][k] + _shortestPaths[k][j] > 0)
                    _shortestPaths[i][j] = _shortestPaths[i][k] + _shortestPaths[k][j];
            }
        }
    }
}

void Graph::_AcyclicTest(){
    bool isUsed[_nodes];
    int tmp;
    stack<int> depthStack;


	for(int x = 0; x < _nodes; x++){
		for(int i = 0; i < _nodes; i++)
			isUsed[i] = false;
		depthStack.push(x);
	   // isUsed[x] = true;
		while(!depthStack.empty()){
			tmp = depthStack.top();
			depthStack.pop();
			for(int i = 0; i < _nodes; i++){
				if(_adjacencyMatrix[tmp][i] != 0 && (isUsed[i] == false || i == x)){
					if(i == x){
						_isCircuit[x] = false;
					}
					else{
					isUsed[i] = true;
					depthStack.push(i);
					}
				}
			}
		}
	}
}

vector<vector<int> > Graph::_multiplyMatrix(int pow) {
	int tmpMat[_nodes][_nodes];
	int tmpMat2[_nodes][_nodes] = { 0 };

	for (int r = 0; r < _nodes; r++) {
		for (int c = 0; c < _nodes; c++) {
			tmpMat[r][c] = _adjacencyMatrix[r][c];
		}
	}

	if (pow == 0) {
		//TODO: SPECIAL CASE
	} else {
		for (int i = 1; i < pow; i++) {
			for (int r = 0; r < _nodes; r++) {
				for (int c = 0; c < _nodes; c++) {
					int val = 0;
					for (int n = 0; n < _nodes; n++) {
						val += tmpMat[r][n] * tmpMat[n][c];
					}

					tmpMat2[r][c] = val;
				}
			}

			for (int r = 0; r < _nodes; r++) {
				for (int c = 0; c < _nodes; c++) {
					tmpMat[r][c] = tmpMat2[r][c];
				}
			}
		}
	}

	for (int r = 0; r < _nodes; r++) {
		for (int c = 0; c < _nodes; c++) {
			printf(" %4d",	tmpMat[r][c]);
		}
		printf("\n");
	}

	return vector<vector<int> >();
}



int main(){
    GraphMainMenu();
    return 0;
}


void GraphMainMenu(){
    int x;
    cout << "How many nodes are in the graph: ";
    cin >> x;
    Graph graph(x);
    while(1){
    cout << "1. Print Adjacency Matrix"<<endl
         << "2. Edit Adjacency Matrix" << endl
         << "3. Is there a Path Between"<<endl
         << "4. Shortest Distance Between"<<endl
         << "5. Is this Graph Acyclic"<<endl
         << "6. Compute Walk Lengths"<<endl
         << "99. Exit"<<endl
         <<endl;



    cout << endl << "Input number corresponding with what function you want to do: ";
    cin >> x;
    system("clear");
        switch(x){
                case 1:
                        graph.PrintAdjacencyMatrix();
                        break;
                case 2:
                        graph.EditAdjacencyMatrix();
                        break;
                case 3:
                        graph.IsPathBetween();
                        break;
                case 4:
                        graph.ShortestDistanceBetween();
                        break;
                case 5:
                        graph.IsThisGraphAcyclic();
                        break;
                case 6:
                        graph.MultiplyMatrix();
                        break;

                case 99:
                        return;
        }
    }
}
