#include "stdafx.h"
#include "string"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <time.h>
#include "tinyxml.h"
using namespace std;

// Структура узла:
struct Node {
	// Указатель на массив потомков
	Node **lpChildren;
	// Количество потомков
	int n;
	// Имя узла
	string sName;
	Node() {
		n = 0;
	}
};

// Дерево
class Tree {
private:
	// Корневой узел	
	Node *pRoot;
	// Глубина дерева, заданного пользователем
	int iDepth;
	// Максимальная глубина получившегося дерева
	int iMaxDepth;
	// Вероятность появления нового узла
	double dblProb;
	// Добавляет потомков к Parent и останавливается, если глубина >= Depth
	void Add(Node *Parent);
	// Достраивает дерево до глубины Depth
	void ReBuild();
	// Проверяет существование узла (на случай, если пользователь ввел неправильное имя)
	bool Exist(string&);
public:
	Tree(int &idepth, double &dblprob) {
		pRoot = new Node;
		this->iDepth = idepth;
		this->dblProb = dblprob;
		pRoot->sName = "";
		iMaxDepth = 0;
	}
	// Начинаем построение дерева
	void Start() {
		Add(pRoot);
		// Если дерево получилось недостаточно глубоким, достраиваем его
		if (iMaxDepth < iDepth) {
			ReBuild();
		}
	}
	// Найти общего предка узлов Node1 и Node2
	string FindParent(string &sNode1, string &sNode2);
	// Возвращает указатель на узел с указанным именем name
	Node* FindNode(string &Name);
	// Удаляет узел и всех его потомков
	void Delete(Node *);
};

// Добавляет потомков к Parent и останавливается, если глубина > depth
void Tree::Add(Node *pParent){
	// Если достигнута максимальная глубина, останавливаемся
	if (pParent->sName.length() >= iDepth) {
		cout << "Level " << pParent->sName.length() << ",node (terminal)  " << pParent->sName << "\n";
		return;
	}
	cout << "Level " << pParent->sName.length() << ",node  " << pParent->sName << "\n";
	// Распределение Бернулли с вероятностью Prob 
	bernoulli_distribution distribution(dblProb);
	default_random_engine generator;

	// Число потомков - случайное число от 2 до 9
	int nnum = rand() % 8 + 2;
	pParent->n = nnum;
	pParent->lpChildren = new Node*[nnum];

	// Создаем потомков
	for (int i = 0; i < nnum; i++) {
		pParent->lpChildren[i] = new Node;
		pParent->lpChildren[i]->sName = pParent->sName + to_string(i);
		iMaxDepth = max((int)pParent->lpChildren[i]->sName.length(), iMaxDepth);
		// Если случайная величина == 1, этот потомок продолжит ветку
		if (distribution(generator)) {
			Add(pParent->lpChildren[i]);
		}
		// Если случайная величина == 0, это конечный узел
		else {
			cout << "Level " << pParent->lpChildren[i]->sName.length() << ",node (terminal)  " <<pParent->lpChildren[i]->sName << "\n";
		}
	}
}
// Достраивает дерево до глубины Depth
void Tree::ReBuild() {
	Node *node = pRoot;
	for (int i = 0; i < iDepth; i++) {
		// Если это конечный узел, но недостаточно глубокий, добавляем ему два потомка
		if (node->n == 0) {
			node->n = 2;
			node->lpChildren = new Node*[2];
			node->lpChildren[0] = new Node;
			node->lpChildren[1] = new Node;
			node->lpChildren[0]->sName = node->sName + "0";
			node->lpChildren[1]->sName = node->sName + "1";
			cout << "Level " << node->sName.length() + 1 << ",node (complete)  " << node->lpChildren[0]->sName << "\n";
			cout << "Level " << node->sName.length() + 1 << ",node (complete)  " << node->lpChildren[1]->sName << "\n";
		}
		int nchild = rand() % node->n;
		node = node->lpChildren[nchild];
	}
}
// Проверяет существование узла (на случай, если пользователь ввел неправильное имя)
bool Tree::Exist(string &sName) {
	Node *pnode = pRoot;
	// Проходимся по всему имени
	for (int i = 0; i < sName.length(); i++) {
		// Очередная буква - индеск потомка
		int nchild = (int)sName[i] - 48;
		if (nchild >= pnode->n)
			return false;
		pnode = pnode->lpChildren[nchild];
	}
	return true;
}
// Возвращает указатель на узел с указанным именем name
Node* Tree::FindNode(string &sName) {
	Node *pnode = pRoot;
	// Проходимся по всему имени
	for (int i = 0; i < sName.length(); i++) {
		// Индекс потомка узла из имени
		int nchild = (int)sName[i] - 48;
		pnode = pnode->lpChildren[nchild];
	}
	return pnode;
}
// Найти общего предка узлов Node1 и Node2
string Tree::FindParent(string &sNode1, string &sNode2) {
	// Если узла(ов) не существует, возвращаем ошибку
	if (!Exist(sNode1) || !Exist(sNode2)) {
		return "Error";
	}
	int min_len = min(sNode1.length(), sNode2.length());
	string sParent = "";
	// Находим максимально длинное общее подслово
	for (int i = 0; i < min_len; i++) {
		if (sNode1[i] == sNode2[i]) {
			sParent += sNode2[i];
		}
		else {
			break;
		}
	}
	// Если нет предка, кроме корня
	if (sParent == "") {
		sParent = "root";
	}
	return sParent;
}
// Удаляет узел и всех его потомков
void Tree::Delete(Node * pnode) {
	for (int i = 0; i < pnode->n; i++) {
		Delete(pnode->lpChildren[i]);
	}
	delete[] pnode;
}
// Достает данные из тега <value>...</value>
string GetNumber(string sname, TiXmlDocument reader) {
	// Перебираем имена <name>...</name> и находим совпадение
	TiXmlElement *root = reader.FirstChildElement("tree")->FirstChildElement("property");
	do {
		if (root->FirstChildElement("name")->GetText() == sname) {
			return root->FirstChildElement("value")->GetText();
		}
		root = root->NextSiblingElement("property");
	} while (root != nullptr);
	return "0";
}

int main(){
	setlocale(LC_ALL,"");
	srand(time(0));
	// Глубина дерева
	int idepth;
	// Вероятность появления нового узла
	double dblprob;
	cout << "Enter path: ";
	string spath;
	cin >> spath;
	cout << "\n";
	// Парсим данные из .xml файла
	TiXmlDocument reader(spath.c_str());
	if (!reader.LoadFile()) {
		cout << "File loading failed!";
		return 0;
	}
	string sDepth = GetNumber("depth", reader);
	string sProb = GetNumber("probability", reader);
	// Перевод строчек в цифры
	idepth = atoi(sDepth.c_str());
	dblprob = atof(sProb.c_str());
	cout << "Tree's depth: " << idepth << ",probability : " << dblprob << "\n";
	// Иницилизируем дерево
	Tree t(idepth, dblprob);
	// Создаем узлы дерева
	t.Start();
	// Ввод имен
	string sName1, sName2;
	cout << "Enter node names: ";
	cin >> sName1 >> sName2;
	// Поиск родителя 
	string sAnswer = t.FindParent(sName1, sName2);
	if (sAnswer == "Error") {
		cout << "This node doesn't exist\n";
		return 0;
	}
	cout << "Ancient name: " << sAnswer << "\n";
	return 0;
}
