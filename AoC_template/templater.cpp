// Create templater that creates all the files for day X

#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <numeric>
#include <iostream>

using namespace std;

const char* MAIN_CPP_TEMPLATE = 
"#include \"../helpers.h\"\n"
"#include \"../common.h\"\n"
"\n"
"using namespace std;\n"
"\n"
"int main() {\n"
"\n"
"}\n";


string_view GetArgValue(const vector<string>& args, string name) {
    for (const auto& arg : args) {
        auto pos = arg.find('=');

        auto key = string_view(begin(arg), begin(arg) + pos);
        if (key == name)
            return string_view(begin(arg) + pos + 1, end(arg));
    }

    return "";
}

string concat(initializer_list<const char*> values) {
    return accumulate(begin(values), end(values), string());
}

void createFile(const string& filename) {
    ofstream ofs(filename);
    ofs.close();
}

void createFileWithContent(const string& filename, const string& content) {
    ofstream ofs(filename);
    ofs << content;
    ofs.close();
}

int main(int argc, char** argv) {
    vector<string> args(argv + 1, argv + argc);
    auto outPath = GetArgValue(args, "-output_path");
    auto day = GetArgValue(args, "-day");
    auto year = GetArgValue(args, "-year");

    auto basePath = concat({outPath.data(), "\\", year.data(), "\\"});
    auto sourceFile = concat({basePath.c_str(), day.data(), ".cpp"});
    auto dataFile = concat({basePath.c_str(), day.data(), "-data"});
    auto dataTestFile = concat({basePath.c_str(), day.data(), "-data-test"});

    createFileWithContent(sourceFile, MAIN_CPP_TEMPLATE);
    createFile(dataFile);
    createFile(dataTestFile);

    cout << "Created template files for puzzle " << day.data() << endl;
}