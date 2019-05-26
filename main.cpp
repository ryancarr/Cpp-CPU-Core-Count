#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#pragma region Using statements
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::istream_iterator;
using std::string;
using std::thread;
using std::vector;
#pragma endregion

#pragma region Function Declarations
int alternateGetNumberOfCores();
int alternate_get_number_of_cores();
int get_number_of_cores();
void getStream(string, ifstream&);
int myGetNumberOfCores();
void printSpacer();
vector<string> splitString(string);
#pragma endregion


int main()
{
    clock_t timerStart, timerStop;
    
    /*-----------------------------------------------------------------------------------------------------------*/
    printSpacer();

    // Time how long it takes to run the function smaller numbers are better
    timerStart = clock();
    cout << "Cores found: " << alternate_get_number_of_cores() << endl;
    timerStop = clock() - timerStart;

    cout << "Time to run alternate_get_number_of_cores(): " << ((double)timerStop / CLOCKS_PER_SEC) << " seconds\n";    
    
    /*-----------------------------------------------------------------------------------------------------------*/
    printSpacer();

    timerStart = clock();
    cout << "Cores found: " << get_number_of_cores() << endl;
    timerStop = clock() - timerStart;
    
    cout << "Time to run get_number_of_cores(): " << ((double)timerStop / CLOCKS_PER_SEC) << " seconds\n";

    /*-----------------------------------------------------------------------------------------------------------*/
    printSpacer();

    timerStart = clock();
    cout << "Cores found: " << myGetNumberOfCores() << endl;
    timerStop = clock() - timerStart;
    
    cout << "Time to run myGetNumberOfCores(): " << ((double)timerStop / CLOCKS_PER_SEC) << " seconds\n";

    /*-----------------------------------------------------------------------------------------------------------*/
    printSpacer();

    timerStart = clock();
    cout << "Cores found: " << alternateGetNumberOfCores() << endl;
    timerStop = clock() - timerStart;

    cout << "Time to run alternateGetNumberOfCores(): " << ((double)timerStop / CLOCKS_PER_SEC) << " seconds\n";

    /*-----------------------------------------------------------------------------------------------------------*/
    printSpacer();

    return 0;
}

#pragma region Function Definitions
/*
 * Use thread builtin class to get number of CPU cores
 * 
 * @return Int representing the number of CPU cores
 */
int alternateGetNumberOfCores()
{
    return thread::hardware_concurrency();
}

/*
 * Alternate solution to get number of CPU cores
 * 
 * @return Int representing the number of CPU cores
 */
int alternate_get_number_of_cores()
{
    // Get the number of host cpu cores
    string line;
    string name = "cpu cores";
    int cores = 0;
    
    ifstream stream;
    getStream("/proc/cpuinfo", stream);

    while (getline(stream, line))
    {
        if (line.compare(0, name.size(),name) == 0 && cores == 0)
        {
            vector<string> values = splitString(line);
            cores = stoi(values[3]);

            name = "flags";
            continue;
        }

        if (line.compare(0, name.size(),name) == 0)
        {
            vector<string> flags = splitString(line);
            
            for(string flag : flags)
                if(flag == "ht")
                {
                    stream.close();
                    return (cores * 2);
                }
        }
    }
    return 0;
}

/*
 * Provided solution to get number of CPU cores
 * 
 * @return Int representing the number of CPU cores
 */
int get_number_of_cores()
{
    // Get the number of host cpu cores
    string line;
    string name = "cpu cores";
    
    ifstream stream;
    getStream("/proc/cpuinfo", stream);

    while (getline(stream, line))
    {
        if (line.compare(0, name.size(),name) == 0)
        {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);

            stream.close();
            return stoi(values[3]);
        }
    }
    return 0;
}

/*
 * Opens an input file stream and performs basic error checking
 * 
 * @param path Absolute path to file to open
 * 
 * @param stream If successful a reference to the stream is created
 */
void getStream(string path, ifstream& stream)
{
    stream.open (path, ifstream::in);
    if (!stream && !stream.is_open())
    {
        stream.close();
        cout << "Unable to open stream: " << path;
    }
}

/*
 * Get the number of cores your CPU has
 * 
 * @return Int representing the number of CPU cores
 */
int myGetNumberOfCores()
{
    string searchTerm = "processor";

    string line;
    ifstream inputStream;
    vector<string> values;

    getStream("/proc/cpuinfo", inputStream);

    while(getline(inputStream, line))
    {
        if(line.compare(0, searchTerm.size(), searchTerm) == 0)
            values = splitString(line);
    }

    inputStream.close();

    return (stoi(values[2]) + 1);
}

void printSpacer()
{
    cout << "------------------------------------------------------------\n";
}

/*
 * Splits a given string on spaces
 * 
 * @param line String to be split
 * 
 * @return Vector of strings
 */
vector<string> splitString(string line)
{
    // Use sstream to slice the string and place in a vector
    istringstream buffer(line);
    istream_iterator<string> begin(buffer), end;

    // Build vector of strings
    vector<string> values(begin, end);
    
    return values;
}
#pragma endregion