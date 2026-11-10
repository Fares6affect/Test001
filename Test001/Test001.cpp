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
	cout<<calculator.getFormula(*gm.getFunction().get());
}