// lameduck.cpp : Defines the entry point for the application.
//

#include <fstream>
#include <stack>
#include <fmt/ostream.h>

void compile() {
  std::ifstream in("hello.bf");
  std::ofstream out("tmp.asm");

  auto prolog = R"(
  global _start
  extern getchar
  extern putchar
  extern exit
  section .text
_start:
  sub rsp, 4000
  mov eax, 0
  mov ecx, 4000
  mov rdi, rsp
  rep stosb
  mov r12, rsp
  sub rsp, 64
)";

  out << prolog;

  //00000000000000100000000000000000
  //              ^r12
  // .++-[..[.]+++]++
  std::stack<int> labels;
  auto current_label = 0;

#ifdef _WIN32
  auto arg_reg = "cl";
#else
  auto arg_reg = "dil";
#endif

  char c;
  while (in >> c) {
    switch (c) {
    case '>': out << "  add r12, 1\n"; break;
    case '<': out << "  sub r12, 1\n"; break;
    case '+': out << "  add byte [r12], 1\n"; break;
    case '-': out << "  sub byte [r12], 1\n"; break;
    case ',': out
      << "  call getchar\n"
      << "  mov byte [r12], al\n";
      break;

    case '.': fmt::print(out,
      "  mov {}, [r12]\n"
      "  call putchar\n",
      arg_reg);
      break;
      
    case '[': fmt::print(out,
      "label{0}:\n"
      "  cmp byte [r12], 0\n"
      "  jz label{0}end\n",
      current_label);
      labels.push(current_label);
      current_label++;
      break;

    case ']': fmt::print(out,
      "  jmp label{0}\n"
      "label{0}end:\n",
      labels.top());
      labels.pop();
      break;

    default: break;
    }
  }

  auto epilog = R"(
  add rsp, 4064
  mov eax, 0
  call exit
)";

  out << epilog;
}

void assemble() {
#ifdef _WIN32
  std::system("nasm -fwin64 tmp.asm -o tmp.obj");
#else
  std::system("nasm -felf64 tmp.asm -o tmp.o");
#endif
}

void link() {
#ifdef _WIN32
  auto linker = std::getenv("LINK64");
  auto ucrt = std::getenv("UCRT64");
  auto cmd = fmt::format(R"(""{}" /SUBSYSTEM:CONSOLE /ENTRY:_start tmp.obj /OUT:hello.exe "{}"")",
    linker, ucrt);
  std::system(cmd.c_str());
#else
  std::system("ld -lc tmp.o -o hello -I /lib64/ld-linux-x86-64.so.2");
#endif
}

int main() {
  compile();
  assemble();
  link();
}
