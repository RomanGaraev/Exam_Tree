#include "stdafx.h"
#include "string"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <time.h>
#include "tinyxml.h"
using namespace std;

// ��������� ����:
struct Node {
	// ��������� �� ������ ��������
	Node **Children;
	// ���������� ��������
	int n;
	// ��� ����
	string Name;
	Node() {
		n = 0;
	}
};

// ������
class Tree {
private:
	// �������� ����	
	Node *Root;
	// ������� ������, ��������� �������������
	int Depth;
	// ������������ ������� ������������� ������
	int MaxDepth;
	// ����������� ��������� ������ ����
	double Prob;
	// ��������� �������� � Parent � ���������������, ���� ������� >= Depth
	void Add(Node *Parent);
	// ����������� ������ �� ������� Depth
	void ReBuild();
	// ��������� ������������� ���� (�� ������, ���� ������������ ���� ������������ ���)
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
		cout << "������� ������\n";
		Delete(Root);
	}
	// �������� ���������� ������
	void Start() {
		Add(Root);
		// ���� ������ ���������� ������������ ��������, ����������� ���
		if (MaxDepth < Depth) {
			ReBuild();
		}
	}
	// ����� ������ ������ ����� Node1 � Node2
	string FindParent(string &Node1, string &Node2);
	// ���������� ��������� �� ���� � ��������� ������ name
	Node* FindNode(string &Name);
	// ���������� ��������� ���
	string RandName();
	// ������� ���� � ���� ��� ��������
	void Delete(Node *);
};

// ��������� �������� � Parent � ���������������, ���� ������� > depth
void Tree::Add(Node *Parent)
{
	// ���� ���������� ������������ �������, ���������������
	if (Parent->Name.length() >= Depth) {
		cout << "������� " << Parent->Name.length() << ",���� (������������) � " << Parent->Name << "\n";
		return;
	}
	cout << "������� " << Parent->Name.length() << ",���� � " << Parent->Name << "\n";
	// ������������� �������� � ������������ Prob 
	bernoulli_distribution distribution(Prob);
	default_random_engine generator;

	// ����� �������� - ��������� ����� �� 2 �� 9
	int num = rand() % 8 + 2;
	Parent->n = num;
	Parent->Children = new Node*[num];

	// ������� ��������
	for (int i = 0; i < num; i++) {
		Parent->Children[i] = new Node;
		Parent->Children[i]->Name = Parent->Name + to_string(i);
		MaxDepth = max((int)Parent->Children[i]->Name.length(), MaxDepth);
		// ���� ��������� �������� == 1, ���� ������� ��������� �����
		if (distribution(generator)) {
			Add(Parent->Children[i]);
		}
		// ���� ��������� �������� == 0, ��� �������� ����
		else {
			cout << "������� " << Parent->Children[i]->Name.length() << ",���� (������������) � " << Parent->Children[i]->Name << "\n";
		}
	}
}
// ����������� ������ �� ������� Depth
void Tree::ReBuild() {
	Node *node = Root;
	for (int i = 0; i < Depth; i++) {
		// ���� ��� �������� ����, �� ������������ ��������, ��������� ��� ��� �������
		if (node->n == 0) {
			node->n = 2;
			node->Children = new Node*[2];
			node->Children[0] = new Node;
			node->Children[1] = new Node;
			node->Children[0]->Name = node->Name + "0";
			node->Children[1]->Name = node->Name + "1";
			cout << "������� " << node->Name.length() + 1 << ",���� (�����������) � " << node->Children[0]->Name << "\n";
			cout << "������� " << node->Name.length() + 1 << ",���� (�����������) � " << node->Children[1]->Name << "\n";
		}
		int child_index = rand() % node->n;
		node = node->Children[child_index];
	}
}
// ��������� ������������� ���� (�� ������, ���� ������������ ���� ������������ ���)
bool Tree::Exist(string &Name) {
	Node *node = Root;
	// ���������� �� ����� �����
	for (int i = 0; i < Name.length(); i++) {
		// ��������� ����� - ������ �������
		int child_index = (int)Name[i] - 48;
		if (child_index >= node->n)
			return false;
		node = node->Children[child_index];
	}
	return true;
}
// ���������� ��������� �� ���� � ��������� ������ name
Node* Tree::FindNode(string &Name) {
	Node *node = Root;
	// ���������� �� ����� �����
	for (int i = 0; i < Name.length(); i++) {
		// ������ ������� ���� �� �����
		int child_index = (int)Name[i] - 48;
		node = node->Children[child_index];
	}
	return node;
}
// ����� ������ ������ ����� Node1 � Node2
string Tree::FindParent(string &Node1, string &Node2) {
	// ���� ����(��) �� ����������, ���������� ������
	if (!Exist(Node1) || !Exist(Node2)) {
		return "Error";
	}
	int min_len = min(Node1.length(), Node2.length());
	string Parent = "";
	// ������� ����������� ������� ����� ��������
	for (int i = 0; i < min_len; i++) {
		if (Node1[i] == Node2[i]) {
			Parent += Node2[i];
		}
		else {
			break;
		}
	}
	// ���� ��� ������, ����� �����
	if (Parent == "") {
		Parent = "root";
	}
	return Parent;
}
// ���������� ��������� ���
string Tree::RandName() {
	Node *node = Root;
	string name;
	while (node != NULL) {
		name = node->Name;
		// ���� � ���� ��� ��������, ���������� ��� ���
		if (node->n == 0) {
			return name;
		}
		// ��������� ������ �������, � �������� �� ������ ���������
		int child_index = rand() % (node->n);
		node = node->Children[child_index];
	}
	return name;
}
// ������� ���� � ���� ��� ��������
void Tree::Delete(Node * node) {
	for (int i = 0; i < node->n; i++) {
		Delete(node->Children[i]);
	}
	delete[] node;
}
// ������� ������ �� ���� <value>...</value>
string GetNumber(string name, TiXmlDocument reader) {
	// ���������� ����� <name>...</name> � ������� ����������
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
	// ������� ������
	int depth;
	// ����������� ��������� ������ ����
	double prob;
	cout << "������� ���� �� �����: ";
	string path;
	cin >> path;
	cout <<"\n";
	// ������ ������ �� .xml �����
	TiXmlDocument reader(path.c_str());
	if (!reader.LoadFile()) {
		cout << "���� �� ������� ���������!";
		return 0;
	}
	string sDepth = GetNumber("depth",reader);
	string sProb = GetNumber("probability", reader);
	// ������� ������� � �����
	depth = atoi(sDepth.c_str());
	prob = atof(sProb.c_str());
	cout << "������� ������: " << depth << ", �����������: " << prob << "\n";
	// ������������� ������
	Tree t(depth, prob);
	// ������� ���� ������
	t.Start();
	// ���� ����
	string Name1, Name2;
	cout << "������� ����� �����: ";
	cin >> Name1 >> Name2;
	cout << "\n";
	// ����� �������� 
	string answer = t.FindParent(Name1, Name2);
	if (answer == "Error") {
		cout << "�� ����� ��� ��������������� ����\n";
		return 0;
	}
	cout << "����� ������ ����� ���: " << answer << "\n";
	system("pause");
	return 0;
}