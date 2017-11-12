#ifndef MANYNETWORK_H
#define MANYNETWORK_H
#include "Neuron.hpp"

typedef std::vector<std::array<int, TOTAL_CONNEXIONS>> Matrice;


/*! 
 * @class Network
 * @brief Represents a Network linking many Neurons between each other  
*/
class Network{
	
private:
	std::array<Neuron*, NB_EXCIT+ NB_INHIB> neurons_;	///< Static table of pointers on all the neurons that the network will make interacting
	Matrice neuronsRelations_;							///< Vector of array of neurons index representing wich one will send current to wich one
	std::array<int,12000> NbSpikesPerStep;					///< Dynamic table representing how many spikes occured in the network in one simulation step
	
public:
	
	/*!
	 * @brief Getter for the neurons relation matrix
	 * @return The neurons relation matrix
	 */
	Matrice getNeuronsRelation() const;
	
	/*!
	 * @brief Getter for the neurons static table
	 * @return The neurons static table
	 */
	std::array<Neuron*, NB_EXCIT+ NB_INHIB> getNeurons() const;
	
	/*!
	 * @brief Function that calls neuron.update on each neuron and neuron.receive on wich target of a spiking neuron
	 * @param Current step of the simulation, number of step we want to reach to end the simulation, External current
	 */
	void update(int currentStep, int nbSteps, double I_ext);
	
	/*!
	 * @brief Overload of update function to allow the generation of the files containing the number list needed to draw Brunel's graphs
	 * @param Current step of the simulation, number of step we want to reach to end the simulation, External current, number of the graph we want to generate
	 */
	void update(int currentStep,int nbSteps, double I_ext, int graph);

	
	/*!
	 * @brief Function that fill randomly the table of relations between neurons with their index
	 */
	void fillNeuronsRelations();
	
	/*!
	 * @brief Function that fill the neurons table by creating New Neurons excitatory or inhibitory
	 */
	void CreateNeurons();
	
	
	/*!
	 * @brief As we create a lot of "New Neurons" we have to free the memory and empty the neurons table at the destruction of the Netword
	 */
	void reset();
	
	/*!
	 * @brief Network constructor, it initialise the table of "number spikes per step" and call the fonctions to create neurones and initialise the relation table
	 */
	Network();
	
	/*!
	 * @brief Network destructor, it calls the reset function to free the pointers memory and empty the neuron table
	 */
	~Network();
};
#endif

