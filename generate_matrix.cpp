#include <iostream> // FOR CONSOLE INPUT AND OUTPUT
#include <fstream>  // FOR FILE STREAM OPERATION
#include <cstdlib>  // FOR RAND(), SRAND()
#include <ctime>    // FOR TIME() TO SEED THE RAND

// DEFINE SIZE WEIGHT AND EDGE

// DEFINE MATRIX SIZE 1000*1000
#define SIZE 1000

// DEFINE MAXIMUM POSSIBLE WEIGHT FOR AN EDGE (1 TO 100)
#define MAX_WEIGHT 100

// DEFINE A CONSTANT TO REPRESENT NO EDGE BETWEEN NODES
#define NO_EDGE 9999

int main()
{

    // OPEN FILE FOR WRITING
    // CREATE AN OUTPUT FILE STREAM OBJECT TO WRITE
    std::ofstream fout("input.csv");

    // CHECK IF FILE OPEN SUCCESSFULLY
    if (!fout.is_open())
    {
        std::cerr << "Failed to open file. \n";
        return 1;
    }

    // Initialize the random number generator with current system time
    srand(time(0));

    // LOOP THROUGH EACH ROW (i) OF THE MATRIX
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            int weight;

            // Diagonal REPRESENTS SELF LOOP SO THAT MUST BE 0
            if (i == j)
            {
                weight = 0;
            }
            // graph is undirected and symmetric
            // skip lower triangle for (j < i) do not want duplicate edges
            else if (j < i)
            {
                weight = NO_EDGE;  // Lower triangle: no edge (infinity)
            }
            else
            {
                // Randomly decide whether there's an edge (50% chance)
                if (rand() % 2 == 0)
                {
                    weight = NO_EDGE; // No edge (infinity)
                }
                else
                {
                    weight = rand() % MAX_WEIGHT + 1; // Edge with weight 1–100
                }
            }
            // WRITE WEIGHT TO THE CSV FILE
            fout << weight;

            // Add comma except for last column in the row
            if (j != SIZE - 1)
            {
                fout << ",";
            }
        }

        // Move to next line after each row
        fout << "\n";
    }

    // Close the output file after writing is done
    fout.close();

    // Notify user of successful matrix generation
    std::cout << "Matrix generated to input.csv\n";

    return 0; // Program ended successfully
}
