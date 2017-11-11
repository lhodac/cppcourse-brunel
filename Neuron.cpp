#include "Neuron.hpp"

using namespace std;


double Neuron:: getMembranePotential() const
{
	return membranePotential_;
}
int Neuron:: getNbSpikes() const
{
	return nbSpikes_;
}
vector<double> Neuron:: getTimeSpikes() const
{
	return spikeTimes_;
}
Type Neuron:: getType() const
{
	return type_;	
}

bool Neuron:: isRefractory() const
{
	return refractory_;
}

void Neuron:: setMembranePotential(double potential)
{
	membranePotential_= potential;
}

void Neuron:: setIext(double I)
{
	I_ext_= I;
}

void Neuron:: setRefractory(bool r)
{
	refractory_= r;
}
																	
void Neuron::addTimeSpike(double time)									//To add one time at the spikeTimes table
{
	spikeTimes_.push_back(time);
}

/*To update the simulation at each steps, it returns a boolean because we need to know if there is a spike or not.
 * poissonFactor which is by default EXT_NU (cf. Constants.hpp) can be change at the update call depending on wich brunel's graph
 * we want to generate (we have EXT_NU = ETA * TH_NU and eta changes from one to another graph
 * */																		
bool Neuron::update(int currentStep, double I_ext, double poissonFactor)
{
	updateBuffer();
	

	bool isSpiking(false);

	if (membranePotential_>= TH_POTENTIAL){
		isSpiking = true;
		addTimeSpike(currentStep);
		++nbSpikes_;
		setRefractory(true);
		setMembranePotential(0.0);
		}
		
	if (!isRefractory()){
		if (backgroundNoise_){
			static std::random_device random;
			static std::mt19937 gen(random());
			static std::poisson_distribution<> distrib(poissonFactor*H);
			setMembranePotential(solveDifferentialEquation(I_ext) + buffer_[0] + distrib(gen)* J_EXCIT);
			}
		else {
			setMembranePotential(solveDifferentialEquation(I_ext) +buffer_[0]);
		}
	}
	
	if(isRefractory()){
		membranePotential_=0.0;
		--refractoryTime_;
		if (refractoryTime_ <= 0.0) { 
				refractoryTime_= REFRACTORY_TIME;
				setRefractory(false);}
		}
	buffer_[0]=0.0;		
	//cout << currentStep << '\t' << membranePotential_ << endl;
	return isSpiking;	
}
	

double Neuron:: solveDifferentialEquation(double I_ext) const
{
	return C1 * membranePotential_ + I_ext * R * C2;
}


double Neuron:: updateBuffer()
{
	for (size_t i (1); i < buffer_.size() ; ++i){
		buffer_[i-1] = buffer_[i];
		}
	
	buffer_[buffer_.size()-1] = 0.0;

	return buffer_[0];
}

void Neuron:: receive(double J)
{	

		assert(DELAY < buffer_.size());
		buffer_[DELAY]+= J;

}

Neuron:: Neuron(bool backgroundNoise,Type type)
: membranePotential_(0.0), nbSpikes_(0), refractory_(false), refractoryTime_(REFRACTORY_TIME),backgroundNoise_(backgroundNoise),type_(type)
{
	for(auto& b : buffer_){
		b= 0.0;
		}
}

Neuron:: ~Neuron()
{
}





