#include "Components/StandardParser.hpp"
#include "Components/Cpp/CppHeadersFormatter.hpp"
#include "Components/JavaScript/JSHeaderFormatter.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	using namespace Everbase::InterfaceCompiler;
    using namespace std;

	ConfigProvider cprov;

    try
    {
        std::ifstream input(argc > 1 ? argv[1] : "../yaml/everbase.yaml");
        input.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

        Components::StandardParser parser;
        Model::RootRef root = parser.execute(cprov, input);

        try
        {
            std::ofstream output("test.cpp", std::ios_base::trunc);
            output.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

            Components::CppHeadersFormatter format;
            format.execute(cprov, root, output);
        }
        catch (const ios_base::failure &e)
        {
            cout << "error opening output file (" << e.what() << ")" << endl;
        }

        try
        {
            std::ofstream output("test.js", std::ios_base::trunc);
            output.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

            Components::JSHeaderFormatter format;
            format.execute(cprov, root, output);
        }
        catch (const ios_base::failure &e)
        {
            cout << "error opening output file (" << e.what() << ")" << endl;
        }
    }
    catch (const ios_base::failure &e)
    {
        cout << "[ERROR] could not read from file (" << e.what() << ")" << endl;
    }
    catch (const std::runtime_error &e)
    {
        cout << "[ERROR] " << e.what() << " - please check your yaml file" << endl;
        return 1;
    }

	return 0;
}
