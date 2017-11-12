#include "Network.hpp"


using namespace std;

Matrice Network:: getNeuronsRelation() const
{
	return neuronsRelations_;
}
std::array<Neuron*, NB_EXCIT+ NB_INHIB> Network:: getNeurons() const
{
	return neurons_;
}

void Network::reset()											
{
	if (!neurons_.empty()){
			for(auto& neuron: neurons_){
				delete neuron;
				neuron = nullptr;
			}
	}
	for(auto& ligne : neuronsRelations_) {
		for (auto& numero: ligne){
			numero=0;
			}
		neuronsRelations_.clear();
		}
}

/* Normal update function, it is call when the use doesn't want to generate graphs
 * It calls neuron.update one every neurons without a poisson factor argument so it uses the values in Constant.hpp
 * for EXT_NU, ETA and G
 */
void Network::update(int currentStep,int nbSteps, double I_ext)
{
	
	bool spike(false);
	
	assert(currentStep < nbSteps);
	while(currentStep < nbSteps)
		{
				
		for (int i(0);i <neurons_.size();++i){
			if (neurons_[i]!=nullptr){
			spike = (neurons_[i]->update(currentStep, I_ext, EXT_NU));

				if (spike){
					++NbSpikesPerStep[currentStep];

														
					for(int j(0); j< neuronsRelations_[i].size(); ++j){
							Type type;
							if (neurons_[i]->getType() == Excitatory){

								neurons_[neuronsRelations_[i][j]]->receive(J_EXCIT);
								}
							else {neurons_[neuronsRelations_[i][j]]->receive(J_INHIB);
								}
						}
					}
			}
		}
		++currentStep;
		}
}


/* Update overload: if the user enters 1,2,3 or 4 in the simulation when asked which graph he wants to generate
 * this version of update is called. Depending on the number of the graph, the parameters are changed to generate
 * the right list of number
 */
void Network::update(int currentStep,int nbSteps, double I_ext, int graph)
{	
	int minBoundary;													//From which step we want to record the data in our file
	int maxBoundary;													//Until which step we want to record the data in our file
	string graphName;													//Name of the file in which we write the data for the Python file
	string graphNameNumbers;											//Name of the file in which we write the data for the Numbers file
	double eta;															//factor of TH_NU --> EXT_NU = ETA * TH_NU (for poisson distribution, background noise)
	double g;															//Ratio between excitatory and inhibitory tension (J_INHIB = -g*J_EXCIT)
	
	switch(graph){
		case 1 : 
			graphName= "plotA.txt";
			graphNameNumbers= "plotNumbersA.txt";
			minBoundary=5000;
			maxBoundary= 6000;
			eta= 2.0;
			g= 3.0;
			break;
		case 2 : 
			graphName= "plotB.txt";
			graphNameNumbers= "plotNumbersB.txt";
			minBoundary=10000;
			maxBoundary= 12000;
			eta= 4.0;
			g= 6.0;

			break;
		case 3 : 
			graphName= "plotC.txt";
			graphNameNumbers= "plotNumbersC.txt";
			minBoundary=10000;
			maxBoundary= 12000;
			eta= 2.0;
			g= 5.0;
			break;
		case 4 : 
			graphName= "plotD.txt";
			graphNameNumbers= "plotNumbersD.txt";
			minBoundary=10000;
			maxBoundary= 12000;
			eta= 0.9;
			g= 4.5;

			break;
		}
			
			
			
			
	
	ofstream sortie;
	ofstream sortieNumbers;
	sortie.open(graphName);
	sortieNumbers.open(graphNameNumbers);

	
	bool spike(false);
	
	assert(currentStep < nbSteps);
	while(currentStep < nbSteps)
		{
		//NbSpikesPerStep.push_back(0);
				
		for (int i(0);i <neurons_.size();++i){
			if (neurons_[i]!=nullptr){
				spike = (neurons_[i]->update(currentStep, I_ext, eta*TH_NU));

				if (spike){
					
					NbSpikesPerStep[currentStep]+=1;
					
					if (currentStep> minBoundary and currentStep<maxBoundary ){	sortie << currentStep << '\t' << i << '\n';}
		
					for(int j(0); j< neuronsRelations_[i].size(); ++j){

							if (neurons_[i]->getType() == Excitatory){
								neurons_[neuronsRelations_[i][j]]->receive(J_EXCIT);
								}
							else {neurons_[neuronsRelations_[i][j]]->receive(-g*J_EXCIT); //receive inhibitory tension = -g * excitatory tension
								}

						}
					}

			}
		}
		if (currentStep> minBoundary and currentStep<maxBoundary){
		sortieNumbers << NbSpikesPerStep[currentStep] << endl;}
		++currentStep;
		}
		
	
	sortie.close();
	sortieNumbers.close();

}




void Network::fillNeuronsRelations()
{
	std::random_device random;
	std::mt19937 gen(random());
		
	std::uniform_int_distribution<> Excit_Distrib (0,(NB_EXCIT-1));
	std::uniform_int_distribution<> Inhib_Distrib (NB_EXCIT, (NB_EXCIT + NB_INHIB)-1);
	
	array<int, TOTAL_CONNEXIONS> ligne;
	
	for (int i(0); i<NB_NEURONS ; ++i){
		neuronsRelations_.push_back(ligne);
	}
	
	
	for(auto& liste: neuronsRelations_) {

			
		for (int k(0); k< EXCIT_CONNEXIONS; ++k){						
			assert(k < EXCIT_CONNEXIONS	);
			
			int Ex_distribution(Excit_Distrib(gen));
			liste[k] = Ex_distribution;  								
			}
		
		for (int l(EXCIT_CONNEXIONS); l< (TOTAL_CONNEXIONS); ++l){	
			assert(l < TOTAL_CONNEXIONS);
			
			int In_distribution(Inhib_Distrib(gen));
			liste[l] = In_distribution;
			}
			
		/*for (auto cible: liste){										//Uncomment if you want to print the neurons relations matrix
			cout << cible << "   ";												
			}
		cout << endl;*/
		
		}
		
}

void Network:: CreateNeurons()
{
	for(size_t i(0); i< neurons_.size();++i){
		assert(i < neurons_.size());
		if (i<	NB_EXCIT) { neurons_[i]= new Neuron(1,Excitatory);}
		else {neurons_[i] = new Neuron(1,Inhibitory);}
		}
}

Network::Network()
{
	for (auto& n: NbSpikesPerStep){
		n=0;
	}
	
	
	CreateNeurons();
	fillNeuronsRelations();
}

Network::~Network()
{
	
	reset();

}

