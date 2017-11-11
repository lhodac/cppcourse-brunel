#include "Network.hpp"
#include "Constants.hpp"

using namespace std;

int main()
{	

	Network network;
		
	/* int currentStep(0);
	int nbSteps(6000);
	double I_ext(0.0);
	network.update(currentStep, nbSteps, I_ext);
	*/
	
	
	//Ask the user wich current and simulation time he wants, not activated because the user is the programmer for now

	cout << endl << endl<< "Enter the number of steps you want for the simulation: (1step represents 0.1ms)" << endl;
	cout << "( To generate the Brunel's graphs you need : " << endl << "A: 6000 steps // B,C and D: 12'000 steps) " << endl;
	double nbSteps(0.0);
	cin >> nbSteps;
	if (nbSteps < 0.0){cout << "ERROR: steps number has to be a positive value.";}
	
	cout << "Enter a External Current: (0.0 for Brunel's graph)" << endl;
	double I_ext(0.0);
	cin >> I_ext;

	cout << "Wich brunel graph's numbers list do you want to generate ? 0 = None , 1 = graph A, 2 = graph B, 3 = graph C, 4 = graph D" << endl;
	int graph;
	cin >> graph;
	
	if (graph == 0) { 
		network.update(0, nbSteps, I_ext);}
	else {
		cout << "The lists of numbers will be generate, to see the plot you have to upload 'plotX.txt' on Jupyter and use the python code provided on my gitHub, " << endl;
		cout << "or create a graph on Numbers with 'plotNumbersX.txt'" << endl;
		network.update(0, nbSteps, I_ext, graph);}


	
	return 0;
	 
	 
}

