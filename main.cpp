// Determines which tests should run
#define RUN_CPU_TESTS true
#define RUN_STRING_TESTS false


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
void printSeparator();
vector<string> splitString(string);
vector<string> splitString(string, string);
#pragma endregion


int main()
{
    clock_t initialTicks, duration;
    string str1 = "This is a string separated by spaces.";
    string str2 = "This/string/is/separated/by/slashes.";
    string delimiter = "/";
    vector<string> values;
    
    /*-----------------------------------------------------------------------------------------------------------*/
    printSeparator();

#if RUN_CPU_TESTS
    // Time how long it takes to run the function smaller numbers are better
    initialTicks = clock();
    cout << "Cores found: " << alternate_get_number_of_cores() << endl;
    duration = clock() - initialTicks;

    // clock() returns a number of cpu ticks, if we divide ticks by CLOCKS_PER_SEC we get a time in seconds
    cout << "Time to run alternate_get_number_of_cores(): " << ((double)duration / CLOCKS_PER_SEC) << " seconds\n";    
    
    /*-----------------------------------------------------------------------------------------------------------*/
    printSeparator();

    // Get starting number of ticks
    initialTicks = clock();
    cout << "Cores found: " << get_number_of_cores() << endl;
    duration = clock() - initialTicks;
    
    cout << "Time to run get_number_of_cores(): " << ((double)duration / CLOCKS_PER_SEC) << " seconds\n";

    /*-----------------------------------------------------------------------------------------------------------*/
    printSeparator();

    initialTicks = clock();
    cout << "Cores found: " << myGetNumberOfCores() << endl;
    duration = clock() - initialTicks;
    
    cout << "Time to run myGetNumberOfCores(): " << ((double)duration / CLOCKS_PER_SEC) << " seconds\n";

    /*-----------------------------------------------------------------------------------------------------------*/
    printSeparator();

    initialTicks = clock();
    cout << "Cores found: " << alternateGetNumberOfCores() << endl;
    duration = clock() - initialTicks;

    cout << "Time to run alternateGetNumberOfCores(): " << ((double)duration / CLOCKS_PER_SEC) << " seconds\n";

    /*-----------------------------------------------------------------------------------------------------------*/
    printSeparator();
#endif

#if RUN_STRING_TESTS
    initialTicks = clock();
    values = splitString(str1);
    duration = clock() - initialTicks;

    for(string str : values)
        cout << str << " ";
    cout << "\n";

    cout << "Old splitString(): " << ((double)duration / CLOCKS_PER_SEC) << " seconds\n";
    /*-----------------------------------------------------------------------------------------------------------*/
    printSeparator();

    initialTicks = clock();
    values = splitString(str1, " ");
    duration = clock() - initialTicks;

    for(string str : values)
        cout << str << " ";
    cout << "\n";

    cout << "New splitString(): " << ((double)duration / CLOCKS_PER_SEC) << " seconds\n";
    /*-----------------------------------------------------------------------------------------------------------*/
    printSeparator();

    initialTicks = clock();
    values = splitString(str2);
    duration = clock() - initialTicks;

    for(string str : values)
        cout << str << " ";
    cout << "\n";

    cout << "Old splitString(): " << ((double)duration / CLOCKS_PER_SEC) << " seconds\n";
    /*-----------------------------------------------------------------------------------------------------------*/
    printSeparator();

    initialTicks = clock();
    values = splitString(str2, delimiter);
    duration = clock() - initialTicks;

    for(string str : values)
        cout << str << " ";
    cout << "\n";

    cout << "New splitString(): " << ((double)duration / CLOCKS_PER_SEC) << " seconds\n";
    /*-----------------------------------------------------------------------------------------------------------*/
    printSeparator();
#endif

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

/*
 * Print a bunch of dashes to the screen to help separate output into readable blocks
 */
void printSeparator()
{
    cout << "--------------------------------------------------------------\n";
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

/*
 * Splits a given string on spaces
 *
 * @param line String to be split
 *
 * @param delimiter What to split the string on, if no value passed, split on space
 *
 * @return Vector of strings
 */
vector<string> splitString(string line, string delimiter)
{
    size_t position = 0;
    string token;
    vector<string> values;

    while ((position = line.find(delimiter)) != string::npos)
    {
        token = line.substr(0, position);
        values.push_back(token);
        line.erase(0, position + delimiter.length());
    }

    if(line.size() > 0)
        values.push_back(line);

    return values;
}
#pragma endregion