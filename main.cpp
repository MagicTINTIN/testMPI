#include <iostream>
#include <string>
#include <cmath>
#include <mpi.h>
#include <iomanip>
#include <fstream>
#include <limits> 
#include "imageprocess.h"
#include "includes/nlohmann/json.hpp"

using json = nlohmann::json;

std::string getName(std::string const &name, int const &val, int const &max)
{
	int nb0max = std::floor(std::log10(std::max(1, max)));
	int nb0val = std::floor(std::log10(std::max(1, val)));
	std::string output = name;
	for (size_t i = 0; i < (nb0max - nb0val); i++)
	{
		output += "0";
	}
	output += std::to_string(val) + ".jpg";
	return output;
}

long averageMultipleImages(int *imageSet, int const &nbImages, int const &processNb, int const &totalSize, std::string const &prefix)
{
	rgb values(0, 0, 0);
	int imagesProcessed(0);
	for (size_t i = 0; i < nbImages; i++)
	{
		if (imageSet[i] != -1)
		{
			std::string filename = getName(prefix, imageSet[i], totalSize);
			values += averageColorImg(filename);
			imagesProcessed++;
		}
	}

	if (imagesProcessed > 0)
		values /= imagesProcessed;
	else
		return -1;

	return combineColors(values);
}

// long averagePi(int *imageSet, int const &nbImages, int const &processNb, int const &totalSize, std::string const &prefix)
// {

// 	// for (size_t i = 0; i < nbImages; i++)
// 	// {
// 	// 	if (imageSet[i] != -1)
// 	// 	{
// 	// 		std::string filename = getName(prefix, imageSet[i], totalSize);
// 	// 		values += averageColorImg(filename);
// 	// 		imagesProcessed++;
// 	// 	}
// 	// }

// 	if (imagesProcessed > 0)
// 		values /= imagesProcessed;
// 	else
// 		return -1;

// 	return combineColors(values);
// }

int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		std::cout << "\nPlease enter json file as argument" << std::endl
				  << "For instance : build/MPIFarm P170B328_ServieresV_L3_small.json" << std::endl;
		return 0;
	}
	else if (argc > 3)
	{
		std::cout << "\nPlease enter only one json file as argument and optionnaly --benchmark as second argument" << std::endl
				  << "For instance : build/MPIFarm P170B328_ServieresV_L3_small.json" << std::endl;
		return 0;
	}

	int *imageSet = nullptr;
	int elementsPerProcess(0);
	int elementsNotProcessed(0);
	int totalElements(0), fakeElements(0), totalWithFakeElements(0);

	std::ifstream f(argv[1]);
	json dataSetJSON = json::parse(f);
	f.close();

	bool benchmark = false;
	if (argc == 3 && std::string(argv[2]) == "--benchmark")
		benchmark = true;

	MPI::Init();
	int rank = MPI::COMM_WORLD.Get_rank();
	int totalProcesses = MPI::COMM_WORLD.Get_size();

	double startTime, stopTime;

	totalElements = dataSetJSON["sequenceNumbers"].size();
	elementsPerProcess = totalElements / totalProcesses;
	elementsNotProcessed = totalElements % totalProcesses;
	totalWithFakeElements = (elementsNotProcessed > 0) ? totalProcesses * (elementsPerProcess + 1) : totalProcesses * elementsPerProcess;
	if (elementsNotProcessed > 0)
		elementsPerProcess++;
	fakeElements = totalWithFakeElements - totalElements;

	std::ofstream exportFlux("/home/user/Documents/filefromMPIFarm", std::ios::app);
	exportFlux << MPI::Wtime() << ": Process " << rank << " spawned here." << std::endl;
	exportFlux.close();

	if (rank == 0)
	{
		imageSet = new int[totalWithFakeElements]{-1};
		int elementNbOfJSON(0);
		for (size_t pnb = 0; pnb < totalProcesses; pnb++)
		{
			for (size_t eop = 0; eop < elementsPerProcess; eop++)
			{
				if (eop == elementsPerProcess - 1 && pnb < fakeElements)
					imageSet[pnb * elementsPerProcess + eop] = -1;
				else
					imageSet[pnb * elementsPerProcess + eop] = dataSetJSON["sequenceNumbers"][elementNbOfJSON++];
			}
		}

		/* Uncomment to visualise the distribution among processes
		 *
		 * for (size_t i = 0; i < totalWithFakeElements; i++)
		 * {
		 * 	if (i % elementsPerProcess == 0)
		 * 		std::cout << "| ";
		 * 	std::cout << imageSet[i] << " ";
		 * }
		 * std::cout << "|";
		 * std::cout << std::endl;
		 */

		std::cout << "\nGetting average color of image sequence: " << dataSetJSON["sequenceNumbers"][0] << "-" << dataSetJSON["sequenceNumbers"][totalElements - 1] << std::endl;
		startTime = MPI::Wtime();
	}

	int partialImageSet[elementsPerProcess];
	MPI::COMM_WORLD.Scatter(imageSet, elementsPerProcess, MPI::INT, partialImageSet, elementsPerProcess, MPI::INT, 0);
	// long average = averageMultipleImages(partialImageSet, elementsPerProcess, rank, dataSetJSON["maxSize"].get<int>(), dataSetJSON["sequenceName"].get<std::string>());
	double average = calculateAveragePi(totalProcesses);

	double *averagesPiParts = nullptr;
	if (rank == 0)
	{
		delete[] imageSet;
		averagesPiParts = new double[totalProcesses];
	}

	MPI::COMM_WORLD.Gather(&average, 1, MPI::DOUBLE, averagesPiParts, 1, MPI::DOUBLE, 0);
	MPI::Finalize();

	if (rank == 0)
	{
		double values = 0;
		stopTime = MPI::Wtime();
		int receivedProcesses(0);
		for (size_t i = 0; i < totalProcesses; i++)
		{
			if (averagesPiParts[i] != -1)
			{
				double val = averagesPiParts[i];
				std::cout << "Process n°" << i << " : pi:" << std::fixed << std::setprecision(10) << val * totalProcesses << std::endl;

				values += val;
				// receivedProcesses++;
			}
		}
		// if (receivedProcesses > 0)
		// 	values /= receivedProcesses;
		// long averageHex = combineColors(values);
		std::cout << "Global average pi value: " << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << values << std::endl;

		std::cout << "Time elapsed: " << stopTime - startTime << "s" << std::endl;

		if (benchmark)
		{
			std::ofstream exportFlux("timeSession");
			exportFlux << stopTime - startTime;
			exportFlux.close();
		}
	}

	delete[] averagesPiParts;
	return 0;
}