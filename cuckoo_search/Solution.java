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

    public Solution randomWalk() {//we have to evolve the best solution in a generation(not any random).
        Solution result = new Solution(this);
        int n = result.noOfWeights();
        double beta = 1.5, sigma=0.0;
        sigma=(Gamma.gamma(1+beta)*Math.sin(Math.PI*beta/2.0)/(Gamma.gamma((1+beta)/2.0)*beta*Math.pow(2.0,(beta-1)/2.0) ))^(1.0/beta);
        
        Random rand = new Random();
        double u = new double[n], v=new double[n], step=new double[n];
        
        for(int i=0; i<n; ++i)
        {
            u[i]=rand.nextDouble()*sigma;
            v[i]=rand.nextDouble();
            step[i]=u[i]/Math.pow(Math.abs(v[i]), 1.0/beta);
            weights[i]=weights[i]+step[i]*rand.nextDouble();
        }
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
