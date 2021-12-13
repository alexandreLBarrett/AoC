// Create templater that creates all the files for day X

#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <numeric>
#include <iostream>
#include <regex>

using namespace std;

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
    ifstream file("AoC_template/code_template");

    string templateFileContent{
        istreambuf_iterator<char>(file), 
        istreambuf_iterator<char>(), 
    };

    file.close();

    vector<string> args(argv + 1, argv + argc);
    auto outPath = GetArgValue(args, "-output_path");
    auto day = GetArgValue(args, "-day");
    auto year = GetArgValue(args, "-year");

    templateFileContent = regex_replace(templateFileContent, regex("__YEAR__"), year.data());
    templateFileContent = regex_replace(templateFileContent, regex("__DAY__"), day.data());

    auto basePath = concat({outPath.data(), "\\", year.data(), "\\"});
    auto sourceFile = concat({basePath.c_str(), day.data(), ".cpp"});
    auto dataFile = concat({basePath.c_str(), day.data(), "-data"});
    auto dataTestFile = concat({basePath.c_str(), day.data(), "-data-test"});

    createFileWithContent(sourceFile, templateFileContent);
    createFile(dataFile);
    createFile(dataTestFile);

    cout << "Created template files for puzzle " << day.data() << endl;
}