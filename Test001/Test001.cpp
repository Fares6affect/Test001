#include <iostream>
#include "Calculator.h"
#include "GenMain.h"
using namespace std;

int main()
{
	Calculator calculator;
	GenMain gm;
	cout<<calculator.getFormula(*gm.getFunction().get());
}