#include <iostream>
#include <string>

#define NUMBER_OF_SCENARIOS 8

/**
 * @brief Extracts the scenario number from command-line arguments.
 * 
 * This function searches the command-line arguments for a parameter of the form
 * "--scenario=<number>" and converts the number to an integer. It performs
 * basic validation to ensure the scenario number is within the allowed range.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 * @return int The extracted scenario number if valid; returns 0 on error
 *             (invalid format, out of range, or below 1). Prints error messages
 *             to std::cout in case of invalid input.
 * 
 * @note The valid range for scenario numbers is 1 to NUMBER_OF_SCENARIOS.
 */
int extract_scenario_number(int argc, char* argv[])
{
   std::string input_number;

    for (int i = 1; i < argc; ++i) 
    {
        std::string arg = argv[i];
        std::string prefix = "--scenario=";
        if (arg.substr(0, prefix.size()) == prefix) 
        {
            input_number = arg.substr(prefix.size());
        }
    }

    int scenario_number = 0;
    try
    {
        scenario_number = std::stoi(input_number);
    } 
    catch (const std::invalid_argument& e)
    {
        std::cout << "Invalid number!\n";
        return 0;
    } 
    catch (const std::out_of_range& e)
    {
        std::cout << "Number out of range!\n";
        return 0;
    }

    if (scenario_number < 1)
    {
        std::cout << "Scenarios must be over the value of 0" << std::endl;
        return 0;
    }
    if (scenario_number > NUMBER_OF_SCENARIOS)
    {
        std::cout << "Scenarios number can't be above " << NUMBER_OF_SCENARIOS << std::endl;
    }

    return scenario_number;
}


int main(int argc, char* argv[]) 
{
    int scenario_number = extract_scenario_number(argc, argv);

    switch (scenario_number)
    {
    case 0:
        return 0;
    case 1:
        std::cout << "Hello world with scenario " << scenario_number << std::endl;
        break;
    
    default:
        std::cout << "Hello world with scenario " << scenario_number << std::endl;
        break;
    }

    return 0;
}
