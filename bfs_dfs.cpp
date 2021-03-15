#include <iostream>
#include <list>

using namespace std;

// Classe que representa um no
class Node {
  public:
    list<Node*> children;
    Node *parent;
    int puzzle[3][3];
    int posX;
    int posY;
    int height;

    Node(int p[][3]) {
      for(int i = 0 ; i < 3 ; i++ ){
        for(int j = 0; j < 3 ; j++ ){
          puzzle[i][j] = p[i][j];
        }
      }
    };

    void CopyPuzzle(int a[][3], int b[][3] ){
      for(int i = 0 ; i < 3 ; i++ ){
        for(int j = 0; j < 3 ; j++ ){
          a[i][j] = b[i][j];
        }
      }
    }

    void ExpandNode(char a){
      for(int i = 0 ; i < 3 ; i++ ){
        for(int j = 0; j < 3 ; j++ ){
          if(this->puzzle[i][j]==0){
            posX=i;
            posY=j;
          }
        }
      }
      MoveN(a);
      MoveS(a);
      MoveE(a);
      MoveW(a);
    }

    void MoveN(char search){
      if(posY != 0){
        int pc[3][3];
        CopyPuzzle(pc,this->puzzle);

        int temp = pc[posX][posY-1];
        pc[posX][posY-1] = this->puzzle[posX][posY];
        pc[posX][posY] = temp;

        Node *child = new Node(pc);

        switch(search) {
          case 'b'://bfs
            children.push_back(child);//children.add(child);
            break;
          case 'd'://dfs
            children.push_front(child);//children.add(child);
            break;
          default:
            break;
        }
        
        child->parent = this;
      }
    }

    void MoveS(char search){
      if(posY != 2){
        int pc[3][3];
        CopyPuzzle(pc,this->puzzle);

        int temp = pc[posX][posY+1];
        pc[posX][posY+1] = this->puzzle[posX][posY];
        pc[posX][posY] = temp;

        Node *child = new Node(pc);  
        
        switch(search) {
          case 'b'://bfs
            children.push_back(child);//children.add(child);
            break;
          case 'd'://dfs
            children.push_front(child);//children.add(child);
            break;
          default:break;
        }
        
        child->parent = this;
      }      
    }

    void MoveE(char search){ // ->
      if(posX != 2){
        int pc[3][3];
        CopyPuzzle(pc,this->puzzle);

        int temp = pc[posX+1][posY];
        pc[posX+1][posY] = this->puzzle[posX][posY];
        pc[posX][posY] = temp;

        Node *child = new Node(pc);  
        
        switch(search) {
          case 'b'://bfs
            children.push_back(child);//children.add(child);
            break;
          case 'd'://dfs
            children.push_front(child);//children.add(child);
            break;
          default:break;
        }
        
        child->parent = this;
      }
    }

    void MoveW(char search){ // <-
      if(posX != 0){
        int pc[3][3];
        CopyPuzzle(pc,this->puzzle);

        int temp = pc[posX-1][posY];
        pc[posX-1][posY] = this->puzzle[posX][posY];
        pc[posX][posY] = temp;

        Node *child = new Node(pc);  
        
        switch(search) {
          case 'b'://bfs
            children.push_back(child);//children.add(child);
            break;
          case 'd'://dfs
            children.push_front(child);//children.add(child);
            break;
          default:break;
        }
        
        child->parent = this;
      }
    }

    void PrintPuzzle(){
      cout << endl;
      for(int i = 0 ; i < 3 ; i++ ){
        for(int j = 0; j < 3 ; j++ ){
          cout << this->puzzle[i][j] << " " ;
        }
        cout << endl;
      }
    }

    bool Compare(int p[][3]){
      for(int i = 0 ; i < 3 ; i++ ){
        for(int j = 0; j < 3 ; j++ ){
          if(this->puzzle[i][j] != p[i][j])
            return false;
        }
      }
      return true;
    }
};

class NodeSearch{
  public:
    list<Node*> Bfs(Node* root, Node* final ){
      list<Node*> Path;
      list<Node*> OpenList;//nodes on queue
      list<Node*> ClosedList;//nodes visited

      OpenList.push_back(root);

      bool goalFound = false;

      while(!OpenList.empty()  && !goalFound){
        Node* currentNode = *OpenList.begin();
        ClosedList.push_back(currentNode);
        OpenList.pop_front();

        currentNode->ExpandNode('b');
        
        for(auto const& i : currentNode->children){
          Node *currentChild = i;

          if(currentChild->Compare(final->puzzle)){
            cout << "Goal Found."<<endl;
            final->PrintPuzzle();
            goalFound = true;
            PathTrace(Path,currentChild);
          }
          if(!Contains(OpenList, currentChild) && !Contains(ClosedList, currentChild) && !goalFound){
            OpenList.push_back(currentChild);
            cout<<"expand"<<endl;
          }
        }
      }
      return Path;
    }

    
    list<Node*> Dfs(Node* root, Node* final){
      list<Node*> Path;
      list<Node*> OpenList;//nodes on queue
      list<Node*> ClosedList;//nodes visited

      OpenList.push_front(root);

      root->height=0;

      bool goalFound = false;

      while(!OpenList.empty()  && !goalFound){
        Node* currentNode = *OpenList.begin();
        ClosedList.push_back(currentNode);
        OpenList.pop_front();

        currentNode->ExpandNode('d');
        
        for(auto const& i : currentNode->children){
          Node *currentChild = i;
          currentChild->height = currentNode->height + 1;

          if(currentChild->Compare(final->puzzle)){
            cout << "Goal Found."<<endl;
            final->PrintPuzzle();
            goalFound = true;
            PathTrace(Path,currentChild);
          }
          if(!Contains(OpenList, currentChild) && !Contains(ClosedList, currentChild) && !goalFound && currentChild->height < 80){
            OpenList.push_front(currentChild);
            cout<<"expand - " << currentChild->height <<endl;
          }
        }
      }
      return Path;
    }


    void PathTrace(list<Node*> path, Node* n){
      //cout<<"Path found."<<endl;
      Node *current = n;

      path.push_back(current);

      while(current->parent != NULL ){
        current=current->parent;
        path.push_back(current);

      }
    }

    bool Contains(list<Node*> list , Node* n){
      for(auto const& i : list){
        if(i->Compare(n->puzzle))
          return true;
      }
      return false;
    }
};

int main() {
  int p[3][3] = {{1,2,3},
                 {4,0,5},
                 {6,7,8}};
  
  int b[3][3] = {{1,2,3},
                 {4,5,6},
                 {7,0,8}};

  int c[3][3] = {{1,2,0},
                 {4,5,3},
                 {7,8,6}};

  int d[3][3] = {{1,2,3},
                 {4,5,6},
                 {0,7,8}};

  int s[3][3] = {{1,2,3},
                 {4,5,6},
                 {7,8,0}};


  Node *problem = new Node(s);
  Node *solution = new Node(d);
  NodeSearch *a = new NodeSearch();
  list<Node*> path = a->Dfs(problem,solution);
}