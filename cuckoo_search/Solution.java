/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cuckoo_search;

import java.util.ArrayList;
import java.util.Random;

/**
 *
 * @author diwakar
 */
public class Solution implements Comparable<Solution> {

    double[] weights;
    private Fitness fitness;

    public Solution(int noOfWeights, Fitness fitness) {
        weights = new double[noOfWeights];
        this.fitness = fitness;
        Random rand = new Random();
        for (int i = 0; i < weights.length; ++i) {
            this.weights[i] = rand.nextDouble();
        }
    }

    public Solution(Solution s) {
        this.weights = new double[s.weights.length];
        for (int i = 0; i < this.weights.length; ++i) {
            this.weights[i] = s.weights[i];
        }
        this.fitness = s.fitness;
    }

    public Solution randomWalk() {
        Solution result = new Solution(this);

        return result;
    }

    public int noOfWeights() {
        return weights.length;
    }

    @Override
    public int compareTo(Solution o) {
        return (int) (fitness.getFitness(this) - fitness.getFitness(o));
    }

    public Fitness fitness() {
        return fitness;
    }
}
