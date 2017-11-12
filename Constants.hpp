#ifndef CONSTANTS_H
#define CONSTANTS_H

 
constexpr double R(20);													///<Membrane resistance
constexpr double C(1);													///<Membrane capacity
constexpr double H(0.1);												///<To change the steps into milliseconds unit, 1 Step = 0.0001s = 0.1ms
constexpr double TH_POTENTIAL(20);										///<Threshold potential (membrane potential above which a spike occured)
constexpr double DELAY(15);												///<Signal transmission delay between two neurons
constexpr double REFRACTORY_TIME(2);									///<Duration without neuronal activity after a spike -> 2ms
constexpr double J_EXCIT(0.1);											///<Tension transmitted by excitatory neurons
constexpr double G(3.0);													///<Ratio between inhibitor and excitator tension
const double J_INHIB(-G * J_EXCIT);										///<Tension transmitted by inhibitory neurons
constexpr double E(0.1);												///<Number of connexions on number of neurons ratio
constexpr unsigned int NB_EXCIT(10000);									///<Number of excitatory neurons
constexpr double ETA(2.0);												///<Nu_ext / Nu_threshold

const unsigned int BUFFER_SIZE(DELAY+1);
const unsigned int NB_INHIB(0.25*NB_EXCIT);								///<Number of inhibitory neurons
const unsigned int NB_NEURONS(NB_EXCIT + NB_INHIB);						///<Total number of neurons
const unsigned int EXCIT_CONNEXIONS(E*NB_EXCIT);						///<Number of excitatory connexions
const unsigned int INHIB_CONNEXIONS(E*NB_INHIB);						///<Number of inhibitory connexions
const unsigned int TOTAL_CONNEXIONS(EXCIT_CONNEXIONS+INHIB_CONNEXIONS);	///<Total number of connexions

const double TAU(R*C);													///<membrane Capacity and resistance ratio
const double C1 (exp(-H/TAU));											///<Constant for differential equation resolution
const double C2 (1-C1);													///<Constant for differential equation resolution
const double TH_NU(TH_POTENTIAL/(J_EXCIT * TAU));						///<NuThreshold (as described by the teacher on the forum moodle)
const double EXT_NU(ETA*TH_NU);											///<Frequency factor for the background noise (poisson generator)

#endif
