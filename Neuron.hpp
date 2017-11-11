#ifndef MANYNEURON_H
#define MANYNEURON_H

#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <cmath>
#include <cassert>
#include <random>
#include <string>
#include "Constants.hpp"


enum Type {Excitatory, Inhibitory};										///< Neuron Type: Excitatory = 0 ou Inhibitory =1

/*! 
 * @class Neuron
 * @brief Represents one neuron with all it's attributes and functions  
*/
 
class Neuron {
	
private:
	
	double membranePotential_;											///< Membrane Potential
	double I_ext_;														///< External current
	int nbSpikes_;														///< How many time the neuron's potential went higher that the threshold potential
	bool refractory_;													///< Boolean that tells us is the neuron is in a refractory state or not
	double refractoryTime_;												///< Duration in wich the neuron is refractory and does not update it's potential
	std::vector<double> spikeTimes_;									///< Dynamic table were are stored the spike times for one neuron
	std::array<double,BUFFER_SIZE> buffer_;								///< Static table representing the buffer which apply a delay between the sending and the receiving of a spike
	bool backgroundNoise_;												///< Do we want to include the background noises into the simulation
	Type type_;															///< Excitatory or Inhibitory (cf enum)


	

public:
	/*!
	 * @brief Membrane potential getter
	 * @return Membrane potential
	 */
	double getMembranePotential() const;
	
	
	/*!
	 * @brief Number of spikes getter
	 * @return Number of spikes
	 */
	int getNbSpikes() const;
	
	
	/*!
	 * @brief Access to the times when the spikes occured
	 * @return Table of times when the neuron was spiking
	 */
	std::vector<double> getTimeSpikes() const;
	
	
	
	/*!
	 * @brief Access to the type of the neuron
	 * @return (Enum) Type Excitatory or Inhibitory
	 */
	Type getType() const;
	
	/*!
	 * @brief Allows to know wheater the neuron is refractory or not
	 * @return bool true if it's refractory or false if it's not
	 */
	bool isRefractory() const;

	/*!
	 * @brief Setter to change the membrane potential attribute
	 * @param New membrane potential
	 */
	void setMembranePotential(double potential);
	
	/*!
	 * @brief Setter to change the external current attribute
	 * @param New external current
	 */
	void setIext(double I);
	
	
	/*!
	 * @brief Setter to change the state of the neuron
	 * @param New refractory state, boolean true for refractory
	 */
	void setRefractory(bool r);


	/*!
	 * @brief Add a time to the spike times table when a new spike occurs
	 * @param The time when the spike occured
	 */
	void addTimeSpike(double time);



	/*!
	 * @brief  Fonction that makes the membrane potential evolve
	 * @param	Current step of the simulation, The external current, the number for the poisson distribution (EXT_NU by default)
	 * @return true ift the neuron is spiking and false if it's not
	 */
	bool update(int currentStep, double I_ext, double poissonFactor = EXT_NU);
	
	
	/*!
	 * @brief Calculates the new membrane potential with the equation seen in course
	 * @param	External current (because it is present on the equation)
	 * @return the value of the new membrane potential
	 */
	double solveDifferentialEquation(double I_ext) const;
	
	
	/*!
	 * @brief Makes the buffer rotate to return a spike after a specifique delay 
	 * @return The first case of the buffer table, it represents the time when the delay is over
	 */
	double updateBuffer();
	
	
	/*!
	 * @brief Fill "DELAY" case of the buffer with J_EXCIT if the neuron is excitatory or J_INHIB if it's not 
	 * @param Current send by a neuron, excitatory or inhibitory current depending on which neuron send it
	 */
	void receive(double J);

	
	/*!
	 * @brief Neuron Constructor
	 * @param Boolean backgrounfNoise (Wheter we want to include or not the background noise, yes by default), The type of the neuron: enum Excitatory (by default) or Inhibitory
	 */
	Neuron(bool backgroundNoise = true, Type type = Excitatory);
	
	
	/*!
	 * @brief Neuron Destructor
	 */
	~Neuron();
	
	
	};




#endif

