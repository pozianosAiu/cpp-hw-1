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

int len(const char* s) {
  for (int i = 0;; ++i) {
    if (s[i] == '\0') {
      return i;
    }
  }
}

bool clear(char (&table)[10][5]) {
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 5; ++j) {
      table[i][j] = ' ';
    }
  }
  return true;
}

bool put(char (&table)[10][5]) {
  int x, y;
  char c;
  std::cin >> x >> y >> c;
  if (x > 9 || y > 4) {
    return false;
  }
  table[x][y] = c;
  return true;
}

bool text(char (&table)[10][5]) {
  int x, y;
  char c[10];
  std::cin >> x >> y >> c;
  if (x + len(c) - 1 > 9 || y > 4) {
    return false;
  }
  for (int i = 0; i < len(c); ++i) {
    table[x + i][y] = c[i];
  }
  return true;
}

bool print(char (&table)[10][5]) {
  for (int j = 0; j < 5; ++j) {
    for (int i = 0; i < 10; ++i) {
      std::cout << table[i][j];
    }
    std::cout << '\n';
  }
  return true;
}

int main() {
  char table[10][5]{};
  const char* cmd_table_names[]{"clear", "put", "text", "print"};
  bool (*cmd_table_pointers[])(char (&)[10][5]){clear, put, text, print};

  while (true) {
    char cmd[20]{};
    std::cin >> cmd;
    if (equal("quit", cmd)) {
      return 0;
    }
    for (unsigned int i = 0; i < std::size(cmd_table_names); ++i) {
      if (!equal(cmd_table_names[i], cmd)) {
        continue;
      }
      if (!cmd_table_pointers[i](table)) {
        std::cout << "error" << '\n';
      }
      break;
    }
  }
}