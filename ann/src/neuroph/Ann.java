package neuroph;

import org.neuroph.core.NeuralNetwork;
import org.neuroph.core.data.DataSet;
import org.neuroph.core.data.DataSetRow;

public class Ann {
	public static void main(String[] args) {
		boolean enableHeader = false;
		if (Integer.parseInt(args[2]) == 1)
			enableHeader = true;

		NeuralNetwork<?> mlp = NeuralNetwork.createFromFile(args[0]);
		DataSet testSet = DataSet.createFromFile(args[1], mlp.getInputsCount(), mlp.getOutputsCount(), ",",
				enableHeader);

		if (enableHeader) {
			// print header as given in testSet
			for (String h : testSet.getColumnNames())
				System.out.print(h + ",");

			// print headers for network outputs
			String[] header = testSet.getColumnNames();
			int i = testSet.getInputSize();
			System.out.print("Network_" + header[i++]);

			for (int j = 1; j < testSet.getOutputSize(); ++j, ++i)
				System.out.print(",Network_" + header[i]);
			System.out.println("");
		}

		for (DataSetRow row : testSet.getRows()) {
			for (double in : row.getInput())
				System.out.print(in + ",");
			for (double out : row.getDesiredOutput())
				System.out.print(out + ",");

			mlp.setInput(row.getInput());
			mlp.calculate();

			double[] out = mlp.getOutput();
			System.out.printf("%.6f", out[0]);
			for (int i = 1; i < out.length; ++i)
				System.out.printf(",%.6f", out[1]);
			System.out.println("");
		}
	}
}
