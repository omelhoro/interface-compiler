#include "model/namespace.hpp"
#include "parser/namespaceParser.hpp"
#include "formatter/cpp/cppHeaders.hpp"

using namespace std;
using namespace Api;

int main(int argc, char **argv)
{
    Model::NamespacePtr rootNamespace(new Model::Namespace("::"));
    Parser::NamespaceParser parser(rootNamespace);

    if (argc >= 2)
    {
        for (int n = 1; n < argc; ++n)
        {
            try {
                cout << "*** parsing file " << argv[n] << " ***" << endl;

                parser.parseFile(argv[n]);
            }
            catch (const runtime_error &e)
            {
                cout << "[PARSE ERROR] " << e.what() << "please check your yaml file " << argv[n] << endl;
                return 1;
            }
        }

        try {
            parser.resolveTypesInNamespace(rootNamespace);
            parser.listKnownTypes();
        }
        catch (const runtime_error &e)
        {
            cout << "[RESOLVER ERROR] " << e.what() << endl;
            return 1;
        }

        try {
            Gen::CppHeaders generator(rootNamespace, "");
            generator.generate();
        }
        catch (const runtime_error &e)
        {
            cout << "[GENERATOR ERROR] " << e.what() << endl;
            return 1;
        }

        cout << "done" << endl;
        return 0;
    }
    else
    {
        cout << "gimme some YAML to chew on!" << endl;
        return 1;
    }
}
