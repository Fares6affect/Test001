#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	RegressTree tree = RegressTree();
	tree.push("+");
	tree.getLeft().push("56");
	tree.getRight().push("34");
	tree.getString();
}

class RegressTree {
public:
	string inf{};
	RegressTree* parent=NULL;
	RegressTree* left=NULL;
	RegressTree* right=NULL;

	RegressTree() {

	}

	RegressTree(const RegressTree& rt) = delete;

	string getString() {
		if (left == NULL || right == NULL)
			return "";
		return "(" + left->getString() + inf + right->getString() + ")";
	}
	void push(string el) {
		if (inf)
			inf = el;
	}

	RegressTree& goLeft() {
		if (left == NULL)
			return RegressTree();
		return *left;
	}
	RegressTree& goRight() {
		if (right == NULL)
			return RegressTree();
		return *right;
	}

	RegressTree getLeft() {
		if (left == NULL)
			return RegressTree();
		return *left;
	}
	RegressTree getRight() {
		if (right == NULL)
			return RegressTree();
		return *right;
	}
};