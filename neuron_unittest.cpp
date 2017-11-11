#include "Network.hpp"
#include "Neuron.hpp"
#include "gtest/include/gtest/gtest.h"
#include <iostream>
#include <cmath>
using namespace std;


/* Test n°1 : test the neuron's membrane potential after one step (one update)
 * the value should be equal to the equation indicated
 * EXPECT_EQ checks if the two values are equal
 * The external current is 1.00 in this test
 */

TEST (NeuronTest, membranePotential_) 
{
	Neuron neuron(0);
		neuron.update(0, 1.0);
	EXPECT_EQ(20.0*(1.0-std::exp(-0.1/20.0)),neuron.getMembranePotential());
}

/* Test n°2 : test if the numbers of spikes correctly increases and if the potential goes down to zero after each spike 
 * (924 and 1850 are the steps were the spikes occurs so the spike is written on the spike table at steps 925 and 1851)
 * EXPECT_EQ checks if the two values are equal
 * The external current is 1.01 in this test so the neuron will reach the threshold potential (20)
 */
TEST (NeuronTest, nbSpikes_)
{	
	Neuron neuron(0);
	///< We update 924 times the neuron and check if there were no spike until then
	for (int i(0); i<924 ; ++i){
		neuron.update(i,1.01);
	}
	EXPECT_EQ(0,neuron.getNbSpikes());
	
	///< We update it one more time and check if the number of spike is 1 and the neuron is refractory (Potential=0.0)
	neuron.update(0,1.01);
	EXPECT_EQ(1,neuron.getNbSpikes());
	EXPECT_EQ(0, neuron.getMembranePotential());
	
	for (int i(925); i< 1868; ++i){
		neuron.update(i,1.01);
		}
	///< We update it until 1850 steps and check if there was one single spike until then
	EXPECT_EQ(1,neuron.getNbSpikes());
	///< We update it one more time and check if the number of spike is 2 and the neuron is refractory (Potential=0.0)
	neuron.update(0,1.01);
	EXPECT_EQ(2,neuron.getNbSpikes());
	EXPECT_EQ(0, neuron.getMembranePotential());

}

/* Test n°3: Test if the time of a spike is correctly put in the SpikeTime table
 * (924 is the number of step were the first spikes occurs, on the first line of the table it shoul be written 924
 * EXPECT_EQ checks if the two values are equal
 * The external current is 1.01 in this test so the neuron will reach the threshold potential (20)
 * */
TEST (NeuronTest, spikeTimes_)
{
	
	Neuron neuron(0);
	///< We update 1000 times the neuron so we know that at least one spike occured
	for (int i(0); i<1000 ; ++i){
		neuron.update(i,1.01);
	}

	int spikeTime(neuron.getTimeSpikes()[0]);
	
	EXPECT_EQ(924, spikeTime);
}

/* Test n°4: Test if with a 1.00 currentthere are no spikes because the threshold potential should never be reached
 * and if we continue updating after that with a 0.0 current, the membrane potential should tend to zero
 * EXPECT_EQ checks if the two values are equal
 * EXPECT_GT(val1, val2) checks if val1>val2
 * EXPECT_NEAR(val1,val2, abs error) checks if the difference between val1 and val2 doesn't exceed the given absolute error
 * */ 
TEST (NeuronTest, Current_)
{
	Neuron neuron(0);
	
	///< We update 10'000 times (large amount of steps) the neuron to check if there is some spikes 
	for (int i(0); i<10000 ; ++i){
		neuron.update(i,1.00);
	}
	EXPECT_EQ(0, neuron.getNbSpikes());
	
	///<Expect 0.001, Absolute value of 19.999-membranePotential>
	///<The difference between 19.999 the neuron's membrane potential should be lower than 0.001, it will never reach 20*/
	EXPECT_GT(1E-3, std:: fabs(19.999 - neuron.getMembranePotential())); 
	
	///< We update if 10'000 more but with a 0.0 current and chekc if the current tend to zero
	for (int i(0); i<10000; ++i){
		neuron.update(i,0.0);
	}
	EXPECT_NEAR(0, neuron.getMembranePotential(), 1E-3);

}

/* Test n°5: Test if neuron2 receive the second spike of the neuron1 after a time = DELAY = 15
* EXPECT_EQ checks if the two values are equal
* */
TEST (TwoNeuronTest, NbPostSpike_)
{
	Neuron neuron1(0);
	Neuron neuron2(0);
	
	///< We update 1850+DELAY times and if there is a spike, neuron2 receive the current. Neuron1's potential should be equal to zero after the spike
	for (auto i=0;i<1868+ DELAY; ++i){
		if (neuron1.update(1,1.01)){
			neuron2.receive(J_EXCIT);
			EXPECT_EQ(0.0,neuron1.getMembranePotential());
			}
		///< We Update the neuron 2 one time, so it will spike
		neuron2.update(0,1.00);
		}
	EXPECT_EQ(0,neuron2.getNbSpikes());
	///< We update it one more time so the spike occured and is count, we check it's membrane potential too (refractory , equal to zero)
	neuron2.update(1,1.00);
	EXPECT_EQ(0.0, neuron2.getMembranePotential());
	EXPECT_EQ(1, neuron2.getNbSpikes());
	
}

/* Test n°6: Test the dimensions of the to tables in Network class : neurons_ and neuronsRelation_
 * neurons_ dimensions should be : NB_NEURONS = 12500
 * neuronsRelation_ dimensions sould be : NB_NEURONS = 12500 (vertically) X TOTAL_CONNEXIONS = 12500 (horizontally)
 * EXPECT_EQ checks if the two values are equal
 */
TEST (NetworkTest, TablesDimensions_)
{
	///<Creates a network, creates the 12500 neurons and fill the neuronsRelations_ matrix
	Network net;
	
	EXPECT_EQ(net.getNeuronsRelation().size(), NB_NEURONS);
	for (int i(0); i< TOTAL_CONNEXIONS; ++i){
		EXPECT_EQ(net.getNeuronsRelation()[i].size(), TOTAL_CONNEXIONS);
		}
	
	EXPECT_EQ(net.getNeurons().size(), NB_NEURONS);
}

/*Test n°7: Test if the Relation matrix is well filled
 * In the X first columns should only be written excitatory neurons index (n<10'000) with X= EXCIT_CONNEXIONS
 * In the Y last columns should only be written inhibitory neurons index (n>10'000) with Y= INHIB_CONNEXIONS
 * EXPECT_GT(val1, val2) checks if val1>val2
 */
TEST (NetworkTest, RelationMatrix_)
{
	Network net;
	
	for (int j(0); j< EXCIT_CONNEXIONS; ++j){
		EXPECT_GT(10000, net.getNeuronsRelation()[0][j]);
		EXPECT_GT(10000, net.getNeuronsRelation()[NB_NEURONS-1][j]);
			}
	for (int j(EXCIT_CONNEXIONS); j< INHIB_CONNEXIONS; ++j){
			EXPECT_GT(net.getNeuronsRelation()[0][j], 10000);
			EXPECT_GT(net.getNeuronsRelation()[NB_NEURONS-1][j],10000);
			}
	
}

/*Test n°8: Allows to test the entire relation Matrix but it is commented because it take a large amount of time
 * going on all square of the matrix. I made the test n°7 to make a shorter test but it is less sure than this one.
 * (uncomment it by deleting the comments marks to try it)*/

/*TEST (NetworkTest, RelationMatrixLongVersion_)
{
	Network net;
	
	for (int i(0); i<NB_NEURONS; ++i){
		for (int j(0); j< EXCIT_CONNEXIONS; ++j){
			EXPECT_GT(10000, net.getNeuronsRelation()[i][j]);
			}
		for (int j(EXCIT_CONNEXIONS); j< INHIB_CONNEXIONS; ++j){
			EXPECT_GT(net.getNeuronsRelation()[i][j], 10000);
			}
		}
}*/

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	
	return RUN_ALL_TESTS();
}

