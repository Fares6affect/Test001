#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include "Calculator.h"
#include "Generator.h"
#include "GenMain.h"
using namespace std;

int main()
{
	Calculator calculator;
	int depth; cin >> depth;
	int n; cin >> n;
	Generator generator(depth, n);
	for (int i = 0; i < 10; i++) {
		unique_ptr<Node> node = generator.generate();
		cout << calculator.getFormula(*node) << endl;
	}
}