/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cuckoo_search;

import org.neuroph.core.NeuralNetwork;
import org.neuroph.core.data.DataSet;

/**
 *
 * @author diwakar
 */
public class MainClass {

    public static void main(String[] args) {
        boolean enableHeader = false;
        if (Integer.parseInt(args[2]) == 1) {
            enableHeader = true;
        }

        NeuralNetwork<?> mlp = NeuralNetwork.createFromFile(args[0]);
        DataSet testSet = DataSet.createFromFile(args[1], mlp.getInputsCount(), mlp.getOutputsCount(), ",",
                enableHeader);

        final int N_NESTS = 15;
        final double ABANDON_PROBABILITY = 0.25;
        final int N_GENERATIONS = 100000;

        int noOfWeights = mlp.getWeights().length;
        Fitness fitness = new Fitness(mlp, testSet);

        Cuckoo_search cs = new Cuckoo_search(N_NESTS, ABANDON_PROBABILITY, N_GENERATIONS);
        Problem problem = new Problem(noOfWeights);
        SolutionSet ansSet = cs.solve(problem, fitness);
    }
}
