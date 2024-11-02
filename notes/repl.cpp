#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

class CppREPL {
private:
  std::string temp_dir;
  std::string temp_file;
  std::vector<std::string> previous_declarations;

  std::string create_full_program(const std::string &input) {
    std::stringstream ss;
    ss << "#include <iostream>\n"
       << "#include <cmath>\n"
       << "#include <complex>\n"
       << "#include <vector>\n"
       << "#include <string>\n\n"
       << "using namespace std;\n\n"
       << "int main() {\n";

    // Add previous declarations
    for (const auto &decl : previous_declarations) {
      ss << "    " << decl << ";\n";
    }

    // Add new input
    ss << "    auto result = " << input << ";\n"
       << "    cout << result << endl;\n"
       << "    return 0;\n"
       << "}\n";

    return ss.str();
  }

  bool compile_and_run(const std::string &code) {
    // Write code to temporary file
    std::ofstream temp(temp_file);
    temp << code;
    temp.close();

    // Compile
    std::string compile_cmd =
        "g++ -std=c++17 " + temp_file + " -o " + temp_dir + "/prog";
    if (system(compile_cmd.c_str()) != 0) {
      std::cerr << "Compilation failed!\n";
      return false;
    }

    // Run
    system((temp_dir + "/prog").c_str());
    return true;
  }

public:
  CppREPL() {
    // Create temporary directory
    temp_dir = std::filesystem::temp_directory_path().string() + "/cpp_repl";
    std::filesystem::create_directory(temp_dir);
    temp_file = temp_dir + "/temp.cpp";
  }

  ~CppREPL() {
    // Cleanup
    std::filesystem::remove_all(temp_dir);
  }

  void run() {
    std::string input;
    std::cout << "C++ REPL (type 'exit' to quit)\n";

    while (true) {
      std::cout << ">>> ";
      std::getline(std::cin, input);

      if (input == "exit") {
        break;
      }

      // Check if this is a declaration (ends with semicolon)
      if (input.back() == ';') {
        previous_declarations.push_back(input.substr(0, input.length() - 1));
        continue;
      }

      std::string program = create_full_program(input);
      compile_and_run(program);
    }
  }
};

int main() {
  CppREPL repl;
  repl.run();
  return 0;
}
