/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cuckoo_search;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

/**
 *
 * @author diwakar
 */
public class SolutionSet {

    ArrayList<Solution> solutions;

    public SolutionSet(int noOfSolutions, Solution solution) {
        solutions = new ArrayList<>();
        int noOfWeights = solution.noOfWeights();
        Fitness fitness = solution.fitness();
        for (int i = 0; i < noOfSolutions; ++i) {
            solutions.add(new Solution(noOfWeights, fitness));
        }
    }

    public Solution getRandomSolution() {
        Random rand = new Random();
        return this.solutions.get(rand.nextInt(solutions.size()));
    }

    public int size() {
        return solutions.size();
    }

    public Solution get(int i) {
        return solutions.get(i);
    }

    public void set(int i, Solution s) {
        solutions.set(i, s);
    }

    public int weightsInSolution() {
        return solutions.get(0).noOfWeights();
    }

    public void abandonWorstSols(double abandonRatio) {
        Collections.sort(solutions);
        int abandonCount = (int) (abandonRatio * solutions.size());
        int wInSols = weightsInSolution();
        Fitness fitness = solutions.get(0).fitness();
        for (int j = 0, i = solutions.size(); j < abandonCount; ++j, --i) {
            solutions.set(i, new Solution(wInSols, fitness));
        }
    }
}
