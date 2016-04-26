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

    private ArrayList<Double> weights;
    private Fitness fitness;

    public Solution(int noOfWeights, Fitness fitness) {
        weights = new ArrayList<>(noOfWeights);
        this.fitness = fitness;
        Random rand = new Random();
        for (int i = 0; i < weights.size(); ++i) {
            this.weights.set(i, rand.nextDouble());
        }
    }

    public Solution(Solution s) {
        this.weights = new ArrayList<>(s.weights);
        this.fitness = s.fitness;
    }

    public Solution randomWalk() {
        Solution result = new Solution(this);

        return result;
    }

    public int noOfWeights() {
        return weights.size();
    }

    @Override
    public int compareTo(Solution o) {
        return (int) (fitness.getFitness(this) - fitness.getFitness(o));
    }

    public Fitness fitness() {
        return fitness;
    }
}
