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
	Node **Children;
	// Количество потомков
	int n;
	// Имя узла
	string Name;
	Node() {
		n = 0;
	}
};

// Дерево
class Tree {
private:
	// Корневой узел	
	Node *Root;
	// Глубина дерева, заданного пользователем
	int Depth;
	// Максимальная глубина получившегося дерева
	int MaxDepth;
	// Вероятность появления нового узла
	double Prob;
	// Добавляет потомков к Parent и останавливается, если глубина >= Depth
	void Add(Node *Parent);
	// Достраивает дерево до глубины Depth
	void ReBuild();
	// Проверяет существование узла (на случай, если пользователь ввел неправильное имя)
	bool Exist(string&);
public:
	Tree(int &depth, double &prob) {
		Root = new Node;
		this->Depth = depth;
		this->Prob = prob;
		Root->Name = "";
		MaxDepth = 0;
	}
	~Tree() {
		cout << "Удаляем дерево\n";
		Delete(Root);
	}
	// Начинаем построение дерева
	void Start() {
		Add(Root);
		// Если дерево получилось недостаточно глубоким, достраиваем его
		if (MaxDepth < Depth) {
			ReBuild();
		}
	}
	// Найти общего предка узлов Node1 и Node2
	string FindParent(string &Node1, string &Node2);
	// Возвращает указатель на узел с указанным именем name
	Node* FindNode(string &Name);
	// Возвращает случайное имя
	string RandName();
	// Удаляет узел и всех его потомков
	void Delete(Node *);
};

// Добавляет потомков к Parent и останавливается, если глубина > depth
void Tree::Add(Node *Parent)
{
	// Если достигнута максимальная глубина, останавливаемся
	if (Parent->Name.length() >= Depth) {
		cout << "Уровень " << Parent->Name.length() << ",узел (терминальный) № " << Parent->Name << "\n";
		return;
	}
	cout << "Уровень " << Parent->Name.length() << ",узел № " << Parent->Name << "\n";
	// Распределение Бернулли с вероятностью Prob 
	bernoulli_distribution distribution(Prob);
	default_random_engine generator;

	// Число потомков - случайное число от 2 до 9
	int num = rand() % 8 + 2;
	Parent->n = num;
	Parent->Children = new Node*[num];

	// Создаем потомков
	for (int i = 0; i < num; i++) {
		Parent->Children[i] = new Node;
		Parent->Children[i]->Name = Parent->Name + to_string(i);
		MaxDepth = max((int)Parent->Children[i]->Name.length(), MaxDepth);
		// Если случайная величина == 1, этот потомок продолжит ветку
		if (distribution(generator)) {
			Add(Parent->Children[i]);
		}
		// Если случайная величина == 0, это конечный узел
		else {
			cout << "Уровень " << Parent->Children[i]->Name.length() << ",узел (терминальный) № " << Parent->Children[i]->Name << "\n";
		}
	}
}
// Достраивает дерево до глубины Depth
void Tree::ReBuild() {
	Node *node = Root;
	for (int i = 0; i < Depth; i++) {
		// Если это конечный узел, но недостаточно глубокий, добавляем ему два потомка
		if (node->n == 0) {
			node->n = 2;
			node->Children = new Node*[2];
			node->Children[0] = new Node;
			node->Children[1] = new Node;
			node->Children[0]->Name = node->Name + "0";
			node->Children[1]->Name = node->Name + "1";
			cout << "Уровень " << node->Name.length() + 1 << ",узел (достроенный) № " << node->Children[0]->Name << "\n";
			cout << "Уровень " << node->Name.length() + 1 << ",узел (достроенный) № " << node->Children[1]->Name << "\n";
		}
		int child_index = rand() % node->n;
		node = node->Children[child_index];
	}
}
// Проверяет существование узла (на случай, если пользователь ввел неправильное имя)
bool Tree::Exist(string &Name) {
	Node *node = Root;
	// Проходимся по всему имени
	for (int i = 0; i < Name.length(); i++) {
		// Очередная буква - индеск потомка
		int child_index = (int)Name[i] - 48;
		if (child_index >= node->n)
			return false;
		node = node->Children[child_index];
	}
	return true;
}
// Возвращает указатель на узел с указанным именем name
Node* Tree::FindNode(string &Name) {
	Node *node = Root;
	// Проходимся по всему имени
	for (int i = 0; i < Name.length(); i++) {
		// Индекс потомка узла из имени
		int child_index = (int)Name[i] - 48;
		node = node->Children[child_index];
	}
	return node;
}
// Найти общего предка узлов Node1 и Node2
string Tree::FindParent(string &Node1, string &Node2) {
	// Если узла(ов) не существует, возвращаем ошибку
	if (!Exist(Node1) || !Exist(Node2)) {
		return "Error";
	}
	int min_len = min(Node1.length(), Node2.length());
	string Parent = "";
	// Находим максимально длинное общее подслово
	for (int i = 0; i < min_len; i++) {
		if (Node1[i] == Node2[i]) {
			Parent += Node2[i];
		}
		else {
			break;
		}
	}
	// Если нет предка, кроме корня
	if (Parent == "") {
		Parent = "root";
	}
	return Parent;
}
// Возвращает случайное имя
string Tree::RandName() {
	Node *node = Root;
	string name;
	while (node != NULL) {
		name = node->Name;
		// Если у узла нет потомков, возвращаем его имя
		if (node->n == 0) {
			return name;
		}
		// Случайный индекс потомка, к которому мы пойдем следующим
		int child_index = rand() % (node->n);
		node = node->Children[child_index];
	}
	return name;
}
// Удаляет узел и всех его потомков
void Tree::Delete(Node * node) {
	for (int i = 0; i < node->n; i++) {
		Delete(node->Children[i]);
	}
	delete[] node;
}
// Достает данные из тега <value>...</value>
string GetNumber(string name, TiXmlDocument reader) {
	// Перебираем имена <name>...</name> и находим совпадение
	TiXmlElement *root = reader.FirstChildElement("tree")->FirstChildElement("property");
	do {
		if (root->FirstChildElement("name")->GetText() == name) {
			return root->FirstChildElement("value")->GetText();
		}
		root = root->NextSiblingElement("property");
	} while (root != nullptr);
	return "0";
}

int main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(0));
	// Глубина дерева
	int depth;
	// Вероятность появления нового узла
	double prob;
	cout << "Укажите путь до файла: ";
	string path;
	cin >> path;
	cout <<"\n";
	// Парсим данные из .xml файла
	TiXmlDocument reader(path.c_str());
	if (!reader.LoadFile()) {
		cout << "Файл не удалось загрузить!";
		return 0;
	}
	string sDepth = GetNumber("depth",reader);
	string sProb = GetNumber("probability", reader);
	// Перевод строчек в цифры
	depth = atoi(sDepth.c_str());
	prob = atof(sProb.c_str());
	cout << "Глубина дерева: " << depth << ", вероятность: " << prob << "\n";
	// Иницилизируем дерево
	Tree t(depth, prob);
	// Создаем узлы дерева
	t.Start();
	// Ввод имен
	string Name1, Name2;
	cout << "Введите имена узлов: ";
	cin >> Name1 >> Name2;
	cout << "\n";
	// Поиск родителя 
	string answer = t.FindParent(Name1, Name2);
	if (answer == "Error") {
		cout << "Вы ввели имя несуществующего узла\n";
		return 0;
	}
	cout << "Общий предок имеет имя: " << answer << "\n";
	system("pause");
	return 0;
}