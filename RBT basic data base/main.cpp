// Arbore Rosu-Negru implementare , baza de date biblioteca.
// Student: Neacsu Bogdan

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <ctype.h>

using namespace std;

TCHAR getch() {
	DWORD mode, cc;
	HANDLE h = GetStdHandle(STD_INPUT_HANDLE);// Setam h ca bufferul consolei
	if (h == NULL) {
		return 0; // Consola nu a fost gasita
	}
	//Preia un caracter in bufferul consolei
	GetConsoleMode(h, &mode);
	SetConsoleMode(h, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT)); //Seteaza bufferul consolei
	TCHAR c = 0;
	ReadConsole(h, &c, 1, &cc, NULL); //citeste in c caracterul apoi il sterge din buffer
	SetConsoleMode(h, mode);
	return c;
}

// STRUCTURA UNUI NOD
struct Node {
	int id;// cheia
	string year;// anul cartii
    string name;// numele cartii
    string madeby;// autorul cartii
    Node *parent; // pointer la parinte
	Node *left; // pointer fiul stang
	Node *right; // pointer fiul drept
	int color; // 1 -> Red, 0 -> Black // culoarea
};

typedef Node *NodePtr;//definim un nume pentru structura de date Node ca sa nu trebuiasca sa punem * peste tot

// Clasa RBTree , implementam functile in arborele rosu-negru
class RBTree {
private:
	NodePtr root;
	NodePtr TNULL;
    void inOrderWriter(NodePtr node,ofstream &file)
    {
        if(node!=TNULL)
        {
            file<<node->id<<","<<node->name<<","<<node->madeby<<","<<node->year<<endl;
            inOrderWriter(node->left,file);
			inOrderWriter(node->right,file);
        }
    }
	void preOrderHelper(NodePtr node) {
		if (node != TNULL) {
			cout<<node->id<<" ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(NodePtr node) {
		if (node != TNULL) {
			inOrderHelper(node->left);
			cout<<"ID: "<<node->id<<endl<<"NUME: "<<node->name<<endl<<"AUTOR: "<<node->madeby<<endl<<"AN: "<<node->year<<endl<<endl;
			inOrderHelper(node->right);
		}
	}

	void postOrderHelper(NodePtr node) {
		if (node != TNULL) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout<<node->id<<" ";
		}
	}

	NodePtr searchTreeHelper(NodePtr node, int key) {
	int ok=0;
		if (node != NULL ) {

		NodePtr temp=node;
		 while(temp!=TNULL||temp->id!=key)
         {
             if (key==temp->id)
             {
                 ok=1;
                 cout<<"GASIT!"<<endl;
                 cout<<"ID: "<<temp->id<<endl<<"NUME: "<<temp->name<<endl<<"AUTOR: "<<temp->madeby<<endl<<"AN: "<<temp->year<<endl<<endl;
                 break;
             }
             if(temp->left==0&&temp->right==0)
             {
                 break;
             }
             if(key<temp->id)
             {
                 temp=temp->left;
             }
             else if (key>=temp->id)
             {
                 temp=temp->right;
             }
         }
            if (ok==0)
    {
        cout<<"ELEMENTUL NU A FOST GASIT!"<<endl;
    }
	}
    return 0;
	}

	// Repara arborele dupa stergere
	void fixDelete(NodePtr x) {
		NodePtr s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					s->color = 1;
					x = x->parent;
				} else {
					if (s->right->color == 0) {
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					}
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			} else {
				s = x->parent->left;
				if (s->color == 1) {
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == 0 && s->right->color == 0) {
					s->color = 1;
					x = x->parent;
				} else {
					if (s->left->color == 0) {
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					}
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = 0;
	}


	void rbTransplant(NodePtr u, NodePtr v){
		if (u->parent == nullptr) {
			root = v;
		} else if (u == u->parent->left){
			u->parent->left = v;
		} else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNodeHelper(NodePtr node, int key) {
		// Gaseste nodul utilizand keya
		NodePtr z = TNULL;
		NodePtr x, y;
		while (node != TNULL){
			if (node->id == key) {
				z = node;
			}

			if (node->id <= key) {
				node = node->right;
			} else {
				node = node->left;
			}
		}

		if (z == TNULL) {
			cout<<"ELEMENTUL NU SE AFLA IN ARBORE"<<endl;
			return;
		}

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		} else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		} else {
			y = minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			} else {
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
        cout<<"STERS CU SUCCES!!"<<endl;
		if (y_original_color == 0){
			fixDelete(x);
		}
	}

	// Repara arborele
	void fixInsert(NodePtr k){
		NodePtr u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // u devine unchi stang
				if (u->color == 1) {
					// Daca unchiul este tot rosu caz 1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->left) {
						// Daca nodul se afla in stanga rotatie dreapta Caz 2
						k = k->parent;
						rightRotate(k);
					}
					// Daca nodul se afla in dreapta rotatie stanga Caz 3
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			} else {
				u = k->parent->parent->right; // u devine unchi drept apoi se verifica cazurile anterioare in oglinda

				if (u->color == 1) {
					// Caz 1 oglinda
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->right) {
						// Caz 2 oglinda
						k = k->parent;
						leftRotate(k);
					}
					// Caz 3 oglinda
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

public:
	RBTree() {
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
		}

	// Traversare in preordine
	void preorder() {
		preOrderHelper(this->root);
	}

    //Traversare in ordine
	void inorder() {
	    if (getRoot()!=TNULL)
        {
		inOrderHelper(this->root);
        }
        else
            cout<<"arborele este gol"<<endl;
	}

    //Traversare postordine
	void postorder() {
		postOrderHelper(this->root);
	}

	// Cauta in arbore o keye data (id)
	NodePtr searchTree(int k) {
		return searchTreeHelper(this->root, k);
	}

	// Gaseste nodul cu keya cea mai mica
	NodePtr minimum(NodePtr node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	// Gaseste nodul cu keya cea mai mare
	NodePtr maximum(NodePtr node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}

	// Gaseste succesorul unui nod dat
	NodePtr successor(NodePtr x) {
		// dacă subarborele drept nu este nul,
		// succesorul este nodul cel mai din stânga din subarborele drept
		if (x->right != TNULL) {
			return minimum(x->right);
		}

		// altfel este cel mai mic strămoș al lui x al cărui copilul stâng este, de asemenea, un strămoș al lui x.
		NodePtr y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// Gaseste predecesorul unui nod
	NodePtr predecessor(NodePtr x) {
		// dacă subarborele din stânga nu este nul,
		// predecesorul este nodul cel mai din dreapta din subarborele stang
		if (x->left != TNULL) {
			return maximum(x->left);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	// rotatie stanga la nodul x
	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// Rotatie dreapta la nodul x
	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// Insereaza un nou nod in arbore
	// Repara arborele
	void insert(int key,string name,string made,string an) {
		// Insertie obisnuita BST
		NodePtr node = new Node;
		node->parent = nullptr;
		node->id = key;
		node->name=name;
		node->madeby=made;
		node->year= an;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // noul nod este rosu

		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL) {
			y = x;
			if (node->id < x->id) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		// y este parintele lui x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		} else if (node->id < y->id) {
			y->left = node;
		} else {
			y->right = node;
		}

		// Daca noul nod este radacina
		if (node->parent == nullptr){
			node->color = 0;
			return;
		}

		// Daca nepotule null
		if (node->parent->parent == nullptr) {
			return;
		}

		// Repara arborele
		fixInsert(node);
	}

	NodePtr getRoot(){
		return this->root;
	}

	// Sterge elementul din arbore
	void deleteNode(int id) {
		deleteNodeHelper(this->root, id);
	}

void readFile(string x)
{
    string y=".txt";
    x.append(y);
    ifstream myFileStream(x);
    if(!myFileStream.is_open()){
        cout<<"FISIERUL NU A PUTUT FI DESCHIS"<<endl;
            cout<<"Press any key to continue"<<endl;
            getch();
    }
    else{
    string name, made, tempString, year;
    int id;
    string myString;
    string line;
    while(getline(myFileStream, line)){
 stringstream ss(line);
 //getline(ss, tempString,' ');
 getline(ss, myString,',');
 id=stoi(myString);
 getline(ss, name,',');
 getline(ss, made,',');
//getline(ss, tempString,' ');
 getline(ss, year,',');
 //cout<<"ID: "<<id<<endl<<"NUME: "<<name<<endl<<"AUTOR: "<<made<<endl<<"AN: "<<year<<endl<<endl;
 insert(id,name,made,year);
}
        cout<<"ELEMENTELE DIN FISIER AU FOST ADAUGATE CU SUCCES IN ARBORE"<<endl<<endl;
        cout<<"Press any key to continue"<<endl;
            getch();
    }
myFileStream.close();
}
void writeFile()
{
    if(getRoot()!=TNULL)
    {
    string x;
    string y=".txt";
    cout<<"RBT V-A FI TRANSCRIS IN ORDINE INTR-UN FISIER CU EXTENSIA .txt SUB FORMA"<<endl;
    cout<<"ID , NUMELE CARTII , AUTORUL CARTII , ANUL"<<endl;
    cout<<"INTRODU NUMELE PENTRU FISIERUL CE VA FI CREAT"<<endl;
    getline(cin,x);
    x.append(y);
     ofstream MyFile(x);
     inOrderWriter(this->root,MyFile);
     cout<<"A FOST CREAT CU SUCCES FISIERUL CU NUMELE: "<<x<<endl;
     MyFile.close();
    }
    else cout<<"NU EXISTA ELEMENTE DE SCRIS IN DOCUMENT"<<endl;
}
void writeFileback(string x)
{
    if(getRoot()!=TNULL)
    {
    string y=".txt";
    x.append(y);
     ofstream MyFile(x);
     inOrderWriter(this->root,MyFile);
     cout<<"FISIERUL A FOST MODIFICAT CU SUCCES"<<endl;
     MyFile.close();
    }
    else{
        cout<<"NU EXISTA ELEMENTE DE SCRIS IN DOCUMENT"<<endl;
    }
}
int verify(string id)
    {
        int countr=0;
        for(int i=0;i<id.length();i++)
        {
            if(isdigit(id[i])== true)
            {
                countr++;
            }
        }
        if(countr==id.length())
        {
            return 1;
        }
        else
        {
            cout<<"VA ROG INTRODUCETI O VALOARE VALIDA (DOAR NUMERE)"<<endl;
        }
        return 0;
        }
};

int main() {
	RBTree bst;
	string ids,name,made,year,filename;
	int id,c=-1,p=0;
	//MENU
	while (c!=48)
    {
        if(p==0)
        {
            system("CLS");
        cout<<"MENIU"<<endl;
        cout<<"1.ADAUGA IN RBT"<<endl;
        cout<<"2.CAUTA IN RBT"<<endl;
        cout<<"3.STERGE DIN RBT"<<endl;
        cout<<"4.PRINT RBT/TRANSCRIE RBT"<<endl;
        cout<<"0.EXIT"<<endl;
        }
        c=getch();
        p=2;
        if (c==49)
        {
            system("CLS");
            cout<<"1.INTRODUCERE MANUALA"<<endl;
            cout<<"2.INTRODUCERE DINTR-UN FISIER"<<endl;
            cout<<"0.INAPOI LA MENIU"<<endl;
            c=1;
            while(c!=49&&c!=50&&c!=48)
            {
            c=getch();
            }
            if(c==49)
            {
                while(p!=1)
                {
                    if (p==2){
                    system("CLS");
    cout<<"Introduceti id-ul de identificare al carti(doar numere):"<<endl;
	while(p!=1)
    {
    cin>>ids;
	cin.ignore(256, '\n');
    if(bst.verify(ids)==1)
    {
        p=1;
        id=stoi(ids);
    }
    }
	cout<<"Introduceti numele carti:"<<endl;
	getline(cin,name);
	cout<<"Introduceti autorul carti:"<<endl;
	getline(cin,made);
	cout<<"Introduceti anul carti:"<<endl;
	getline(cin,year);
	c=2;
                }
          system("CLS");
	cout<<"Ati introdus datele corect ?:"<<endl<<endl;
	cout<<"ID: "<<id<<endl<<"NUME: "<<name<<endl<<"AUTOR: "<<made<<endl<<"AN: "<<year<<endl<<endl;
	cout<<"1.DA"<<endl;
	cout<<"2.NU"<<endl;
	cout<<endl;
	while(c!=49&&c!=50)
    {
	c=getch();
    }
	if(c==50)
    {
        p=2;
    }
    else if (c==49)
    {
        p=1;
    }
                }
    if (c!=48)
    {
	bst.insert(id,name,made,year);
    }
    }
    if(c==50)
    {
        c=2;
        system("CLS");
        cout<<"DOCUMENTUL DIN CARE VOR FI EXTRASE DATELE TREBUIE SA AIBA EXTENSIA .txt SI FORMATUL URMATOR:"<<endl;
        cout<<"ID,NUMELE CARTI,NUMELE AUTORULUI,ANUL"<<endl;
        cout<<"EXEMPLU: 3,Murder on the Orient Express,Agatha Christie,1934"<<endl;
        cout<<"DACA VREI SA CONTINUI APASA 1 SAU 0 PENTRU A REVENI LA MENIU"<<endl;
        while(c!=49&&c!=48)
        {
        c=getch();
        }
        if(c==49)
        {
        system("CLS");
    cout<<"INTRODU NUMELE FISIERULUI"<<endl;
    cout<<"FISIERUL TREBUIE SA FIE UN DOCUMENT CU EXTENSIA .txt"<<endl;
    getline(cin,filename);
        bst.readFile(filename);
        }
    }
	c=2;
	p=0;
        }
        if(c==50)
        {
            system("CLS");
            cout<<"SCRIETI ID-UL PE CARE IL CAUTATI"<<endl;
            cin >> c;
            //cin.ignore(256, '\n');
            bst.searchTree(c);
            cout<<endl<<endl;
            cout<<"Press any key to continue"<<endl;
            getch();
            c=2;
            p=0;
        }
        if(c==51)
        {
            system("CLS");
            cout<<"SCRIETI ID-UL PE CARE VRETI SA-L STERGETI"<<endl;
            cin>>c;
            cin.ignore(256, '\n');
            bst.deleteNode(c);
           cout<<"Press any key to continue"<<endl;
           getch();
           c=2;
           p=0;
        }
        if (c==52)
        {
            system("CLS");
            cout<<"1.PRINT RBT"<<endl;
            cout<<"2.TRANSCRIE RBT INTR-UN DOCUMENT"<<endl;
            while(c!=49&&c!=50)
            {
                c=getch();
            }
            if(c==49)
            {
             system("CLS");
             bst.inorder();
             cout<<endl<<endl;
             cout<<"Press any key to continue"<<endl;
             getch();
             c=2;
             p=0;
            }
            if(c==50)
            {
                c=2;
                system("CLS");
                cout<<"1.SALVATI MODIFICARILE IN FISIERUL DEJA DESCHIS"<<endl;
                cout<<"2.CREATI UN FISIER NOU"<<endl;
                while(c!=49&&c!=50)
            {
                c=getch();
            }
                if (c==49)
                {
                    system("CLS");
                    bst.writeFileback(filename);
                }
                if(c==50)
                {
                system("CLS");
                bst.writeFile();
                }
                             cout<<endl<<endl;
             cout<<"Press any key to continue"<<endl;
             getch();
             c=2;
             p=0;
            }
        }
    }
	return 0;
}
