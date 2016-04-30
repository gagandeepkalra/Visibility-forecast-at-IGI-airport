/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cuckoo_search;

import org.neuroph.core.NeuralNetwork;
import org.neuroph.core.data.DataSet;
import org.neuroph.core.data.DataSetRow;

/**
 *
 * @author diwakar
 */
public class Fitness {

    NeuralNetwork<?> mlp;
    DataSet testSet;

    public Fitness(NeuralNetwork<?> mlp, DataSet testSet) {
        this.mlp = mlp;
        this.testSet = testSet;
    }

    public double getFitness(Solution s) {
        double error = 0;
        mlp.setWeights(s.weights);
        for (DataSetRow row : testSet.getRows()) {
            mlp.setInput(row.getInput());
            mlp.calculate();

            double[] in = row.getInput();
            double[] out = mlp.getOutput();
            for (int i = 1; i < out.length; ++i) {
                error += out[i] - in[i];
            }
        }
        return error;
    }
}
