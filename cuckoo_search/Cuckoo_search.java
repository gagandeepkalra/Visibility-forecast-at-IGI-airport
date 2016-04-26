/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cuckoo_search;

import java.util.Random;
import org.neuroph.core.NeuralNetwork;
import org.neuroph.core.data.DataSet;
import org.neuroph.core.data.DataSetRow;

/**
 *
 * @author diwakar
 */
public class Cuckoo_search {

    /**
     * @param args the command line arguments
     */
    final int N_NESTS;
    final double ABANDON_PROBABILITY;
    final int N_GENERATIONS;

    public Cuckoo_search(int N_NESTS, double ABANDON_PROBABILITY, int N_GENERATIONS) {
        this.N_NESTS = N_NESTS;
        this.ABANDON_PROBABILITY = ABANDON_PROBABILITY;
        this.N_GENERATIONS = N_GENERATIONS;
    }

    public SolutionSet solve(Problem problem, Fitness fitness) {
        SolutionSet solutionSet = new SolutionSet(N_NESTS, new Solution(problem.getNoOfWeights(), fitness));

        for (int i = 0; i < N_GENERATIONS; ++i) {
            Solution randSolution = solutionSet.getRandomSolution();
            Solution newSolution = randSolution.randomWalk();

            //get a random solution to replace
            int randIndex = (new Random()).nextInt(solutionSet.size());
            if (fitness.getFitness(newSolution) > fitness.getFitness(solutionSet.get(randIndex))) {
                solutionSet.set(randIndex, newSolution);
            }

            solutionSet.abandonWorstSols(ABANDON_PROBABILITY);
        }
        return solutionSet;
    }
}
