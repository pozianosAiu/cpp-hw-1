#include <iostream>

bool equal(const char* a, const char* b) {
  for (int i = 0;; ++i) {
    if (a[i] != b[i]) {
      return false;
    }
    if (a[i] == '\0') {
      return true;
    }
  }
}

void deposit(int& balance) {
  int k;
  std::cin >> k;
  if (k < 0) {
    std::cout << "error" << '\n';
    return;
  }
  balance += k;
}

void withdraw(int& balance) {
  int k;
  std::cin >> k;
  if (k < 0 || balance < k) {
    std::cout << "error" << '\n';
    return;
  }
  balance -= k;
}

void balancefunc(int& balance) { std::cout << balance << '\n'; }

void reset(int& balance) { balance = 0; }

void multiply(int& balance) {
  int k;
  std::cin >> k;
  if (k < 0) {
    std::cout << "error" << '\n';
    return;
  }
  balance *= k;
}

int main() {
  int balance{0};
  const char* cmd_table_names[]{"deposit", "withdraw", "balance", "reset",
                                "multiply"};
  void (*cmd_table_pointers[])(int&){deposit, withdraw, balancefunc, reset,
                                     multiply};

  while (true) {
    char cmd[20]{};
    std::cin >> cmd;
    if (equal("stop", cmd)) {
      return 0;
    }
    bool nothing_called = true;
    for (unsigned int i = 0; i < std::size(cmd_table_names); ++i) {
      if (!equal(cmd_table_names[i], cmd)) {
        continue;
      }
      nothing_called = false;
      cmd_table_pointers[i](balance);
      break;
    }
    if (nothing_called) {
      std::cout << "error" << '\n';
    }
  }
}