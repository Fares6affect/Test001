#include <iostream>
#include "Calculator.h"
#include "GeneratorFunction.h"
#include "GenMain.h"

using namespace std;

int main()
{
	GeneratorFunction gf;
	Calculator calculator;
	GenMain gm;
	unique_ptr<Node> answer = gm.getFunction();
	cout << calculator.getOptimizeFormula(*answer) << endl << endl;
}