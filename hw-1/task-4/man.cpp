#include <cstdlib>
#include <cstring>
#include <iostream>

int** MatrixConstruct(char type, unsigned long long h, unsigned long long w,
                      void* args) {
  int** matrix = new int*[h + 1]();
  matrix[0] = (int*)h;
  for (int i = 1; i <= h; ++i) {
    matrix[i] = new int[w + 1];
    matrix[i][0] = w;
    for (int j = 1; j <= w; ++j) {
      switch (type) {
        case '0':
          matrix[i][j] = 0;
          break;
        case 'n':
          matrix[i][j] = *(int*)args;
          break;
        case 'r':
          matrix[i][j] = ((int*)args)[j];
          break;
        case 'c':
          matrix[i][j] = ((int*)args)[i];
          break;
        case 'm':
          matrix[i][j] = ((int*)args)[(i - 1) * w + (j - 1) + 1];
          break;
      }
    }
  }
  return matrix;
}

int CountNumbers(char* s) {
  int res = 0;
  bool flagNumber = false;
  while (*s != '\0') {
    if (!flagNumber && *s != ' ') {
      flagNumber = true;
    } else if (flagNumber && *s == ' ') {
      ++res;
      flagNumber = false;
    }
    ++s;
  }
  if (flagNumber) {
    ++res;
  }
  return res;
}

void* StrRes(const char* s) {
  int s_len = strlen(s);
  char type = 's';
  void* r = std::malloc(sizeof(char) * (2 + s_len) + sizeof(int));

  ((char*)r)[0] = type;
  memcpy(((char*)r) + 1, &s_len, 4);
  memcpy(((char*)r) + 5, s, s_len);
  ((char*)r)[5 + s_len] = '\0';
  return r;
}

void* MRes(int*** matrices, int i) {
  char type = 'm';
  void* r = std::malloc(sizeof(char) + sizeof(int) + sizeof(int***));
  ((char*)r)[0] = type;
  memcpy(((char*)r) + 1, &i, 4);
  memcpy(((char*)r) + 5, &matrices, 8);
  return r;
}

int ToInt(char* begin, char* end) {
  int res = 0;
  int sign = 1;
  if (begin < end && begin[0] == '-') {
    ++begin;
    sign = -1;
  }
  while (begin < end) {
    res = res * 10 + (*begin - '0');
    ++begin;
  }
  return sign * res;
}

int LenToSpace(char* s) {
  int res = 0;
  while (s[res] != ' ') {
    if (s[res] == '\0') {
      return -1;
    }
    res++;
  }
  return res;
}

void extend(int***& arr) {
  unsigned long long old_len = (unsigned long long)arr[0];
  unsigned long long new_len = old_len * 2;
  int*** new_arr = new int**[new_len];
  new_arr[0] = (int**)new_len;
  for (unsigned long long i = 1; i < old_len; ++i) {
    new_arr[i] = arr[i];
  }
  for (unsigned long long i = old_len; i < new_len; ++i) {
    new_arr[i] = nullptr;
  }
  std::swap(arr, new_arr);
  delete[] new_arr;
}

char* ToString(const void* data) {
  if (data == nullptr) {
    char* empty_string = new char[1];
    empty_string[0] = '\0';
    return empty_string;
  }
  char type = ((char*)data)[0];
  char* res{};
  switch (type) {
    case 's': {
      int len;
      memcpy(&len, ((char*)data) + 1, 4);
      res = new char[len + 1];
      memcpy(res, ((char*)data) + 5, len);
      res[len] = '\0';
    } break;
    case 'm': {
      int ind;
      int*** matrices;
      memcpy(&ind, ((char*)data) + 1, 4);
      memcpy(&matrices, ((char*)data) + 5, 8);
      int** matrix = matrices[ind];
      int h = (unsigned long long)matrix[0];
      int w = matrix[1][0];
      int len = h * w;
      for (int i = 1; i <= h; ++i) {
        for (int j = 1; j <= w; ++j) {
          int tmp = matrix[i][j];
          if (tmp == 0) {
            len += 1;
            continue;
          }
          if (tmp < 0) {
            len += 1;
            tmp *= -1;
          }
          while (tmp > 0) {
            tmp /= 10;
            len += 1;
          }
        }
      }
      res = new char[len];
      int resp = 0;
      for (int i = 1; i <= h; ++i) {
        for (int j = 1; j <= w; ++j) {
          int cur_num = matrix[i][j];
          if (cur_num == 0) {
            res[resp++] = '0';
            res[resp++] = ' ';
            continue;
          }
          if (cur_num < 0) {
            res[resp++] = '-';
            cur_num *= -1;
          }
          while (cur_num > 0) {
            res[resp++] = '0' + cur_num % 10;
            cur_num /= 10;
          }
          res[resp++] = ' ';
        }
        res[resp - 1] = '\n';
      }
      res[resp - 1] = '\0';
    } break;
    default: {
      const char* cres = "ToStringError";
      int cres_len = std::strlen(cres);
      res = new char[cres_len + 1];
      memcpy(res, cres, cres_len);
      res[cres_len] = '\0';
    }
  }
  return res;
}

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

void* help(int***& matrices, char* args) {
  const char* help_text =
      "Программа для работы с матрицами\n\
  Список команд:\n\
  help - выводит этот текст\n\
  set i h w [options] - создает матрицу размера h на w и сохраняет по индексу i, если индекс свободен (индекс начинается с 1). Возможные options:\n\
  пустой - заполнена нулями\n\
  -n x - заполнена числом x\n\
  -r w чисел через пробел - заполняет все строки введенной строкой из w чисел\n\
  -c h чисел через пробел - заполняет все столбцы введенной строкой из h чисел\n\
  -m w*h чисел через пробел - заполняет всю матрицу - первую строку первыми w числами, вторую - следующими и т.д.\n\
  get i - выводит матрицу по индексу i или сообщает, что индекс свободен\n\
  clear i - удаляет матрицу с индекса i\n\
  list - перечисляет занятые индексы\n\
  transpose i - транспонирует матрицу по индексу i\n\
  det i - считает определитель матрицы по индексу i\n\
  multiply a b i - перемножает матрицы с индексов a и b, результат записывает по индексу i\n";
  return StrRes(help_text);
}

void* set(int***& matrices, char* args) {
  int ind;
  int h;
  int w;
  int i = LenToSpace(args);
  if (i == -1) i = std::strlen(args);
  ind = ToInt(args, args + i);
  args += i;
  if (args[0] == ' ') args += 1;

  i = LenToSpace(args);
  if (i == -1) i = std::strlen(args);
  h = ToInt(args, args + i);
  args += i;
  if (args[0] == ' ') args += 1;

  i = LenToSpace(args);
  if (i == -1) i = std::strlen(args);
  w = ToInt(args, args + i);
  args += i;

  while ((unsigned long long)matrices[0] <= (unsigned long long)ind) {
    extend(matrices);
  }
  if (matrices[ind] != nullptr) {
    return StrRes("index is not empty");
  }
  char type = '0';
  if (args[0] != '\0' && args[1] != '\0' && args[2] != '\0') {
    type = args[2];
  }
  switch (type) {
    case '0':
      matrices[ind] = MatrixConstruct('0', h, w, nullptr);
      break;
    case 'n': {
      char* p = args + 4;
      int nextlen = LenToSpace(p);
      if (nextlen == -1) nextlen = std::strlen(p);
      int arg_number = ToInt(p, p + nextlen);
      matrices[ind] = MatrixConstruct('n', h, w, (void*)&arg_number);
    } break;
    case 'r':
    case 'c':
    case 'm': {
      char* p = args + 4;
      int argslen = CountNumbers(p);
      int* numbers = new int[argslen + 1];
      numbers[0] = argslen;
      for (int k = 1; k <= argslen; ++k) {
        int nextlen = LenToSpace(p);
        if (nextlen == -1) nextlen = std::strlen(p);
        numbers[k] = ToInt(p, p + nextlen);
        p += nextlen;
        if (p[0] == ' ') p += 1;
      }
      matrices[ind] = MatrixConstruct(type, h, w, (void*)numbers);
      delete[] numbers;
    } break;
  }
  return MRes(matrices, ind);
}

void* get(int***& matrices, char* args) {
  int i = LenToSpace(args);
  if (i == -1) i = std::strlen(args);
  int ind = ToInt(args, args + i);
  if (ind <= 0 || (unsigned long long)ind >= (unsigned long long)matrices[0]) {
    return StrRes("index is empty");
  }
  if (matrices[ind] == nullptr) {
    return StrRes("index is empty");
  }
  return MRes(matrices, ind);
}

void* clear(int***& matrices, char* args) {
  int i = LenToSpace(args);
  if (i == -1) i = std::strlen(args);
  int ind = ToInt(args, args + i);
  if (ind <= 0 || (unsigned long long)ind >= (unsigned long long)matrices[0]) {
    return StrRes("index is empty");
  }
  if (matrices[ind] == nullptr) {
    return StrRes("index is empty");
  }
  unsigned long long h = (unsigned long long)matrices[ind][0];
  for (unsigned long long k = 1; k <= h; ++k) {
    delete[] matrices[ind][k];
  }
  delete[] matrices[ind];
  matrices[ind] = nullptr;
  return nullptr;
}

void* list(int***& matrices, char* args) {
  unsigned long long arrlen = (unsigned long long)matrices[0];

  int len = 0;
  for (unsigned long long i = 1; i < arrlen; ++i) {
    if (matrices[i] == nullptr) {
      continue;
    }
    int tmp = i;
    while (tmp > 0) {
      tmp /= 10;
      len += 1;
    }
    len += 1;
  }
  if (len == 0) {
    return StrRes("all indexes are empty");
  }
  char* res = new char[len + 1];
  int resp = 0;
  for (unsigned long long i = 1; i < arrlen; ++i) {
    if (matrices[i] == nullptr) {
      continue;
    }
    int tmp = i;
    int digits[20];
    int nd = 0;
    while (tmp > 0) {
      digits[nd++] = tmp % 10;
      tmp /= 10;
    }
    for (int d = nd - 1; d >= 0; --d) {
      res[resp++] = '0' + digits[d];
    }
    res[resp++] = ' ';
  }
  res[resp - 1] = '\0';
  void* r = StrRes(res);
  delete[] res;
  return r;
}

void* transpose(int***& matrices, char* args) {
  int i = LenToSpace(args);
  if (i == -1) i = std::strlen(args);
  int ind = ToInt(args, args + i);
  if (ind <= 0 || (unsigned long long)ind >= (unsigned long long)matrices[0]) {
    return StrRes("index is empty");
  }
  if (matrices[ind] == nullptr) {
    return StrRes("index is empty");
  }
  int** matrix = matrices[ind];
  unsigned long long h = (unsigned long long)matrix[0];
  unsigned long long w = matrix[1][0];

  int len = 0;
  int tmp = ind;
  while (tmp > 0) {
    len += 1;
    tmp /= 10;
  }
  len += 1;
  tmp = (int)w;
  while (tmp > 0) {
    len += 1;
    tmp /= 10;
  }
  len += 1;
  tmp = (int)h;
  while (tmp > 0) {
    len += 1;
    tmp /= 10;
  }
  len += 1;
  len += 3;
  for (unsigned long long j = 1; j <= w; ++j) {
    for (unsigned long long ii = 1; ii <= h; ++ii) {
      int cur = matrix[ii][j];
      if (cur == 0) {
        len += 1;
      } else {
        if (cur < 0) {
          len += 1;
          cur *= -1;
        }
        while (cur > 0) {
          len += 1;
          cur /= 10;
        }
      }
      len += 1;
    }
  }

  char* res = new char[len + 1];
  int resp = 0;
  int digits[20];
  int nd;

  tmp = ind;
  nd = 0;
  while (tmp > 0) {
    digits[nd++] = tmp % 10;
    tmp /= 10;
  }
  for (int d = nd - 1; d >= 0; --d) res[resp++] = '0' + digits[d];
  res[resp++] = ' ';

  tmp = (int)w;
  nd = 0;
  while (tmp > 0) {
    digits[nd++] = tmp % 10;
    tmp /= 10;
  }
  for (int d = nd - 1; d >= 0; --d) res[resp++] = '0' + digits[d];
  res[resp++] = ' ';

  tmp = (int)h;
  nd = 0;
  while (tmp > 0) {
    digits[nd++] = tmp % 10;
    tmp /= 10;
  }
  for (int d = nd - 1; d >= 0; --d) res[resp++] = '0' + digits[d];
  res[resp++] = ' ';

  res[resp++] = '-';
  res[resp++] = 'm';
  res[resp++] = ' ';

  for (unsigned long long j = 1; j <= w; ++j) {
    for (unsigned long long ii = 1; ii <= h; ++ii) {
      int cur = matrix[ii][j];
      if (cur == 0) {
        res[resp++] = '0';
      } else {
        if (cur < 0) {
          res[resp++] = '-';
          cur *= -1;
        }
        nd = 0;
        while (cur > 0) {
          digits[nd++] = cur % 10;
          cur /= 10;
        }
        for (int d = nd - 1; d >= 0; --d) res[resp++] = '0' + digits[d];
      }
      res[resp++] = ' ';
    }
  }
  res[resp - 1] = '\0';

  clear(matrices, args);
  void* r = set(matrices, res);
  delete[] res;
  return r;
}

void* det(int***& matrices, char* args) {
  int i = LenToSpace(args);
  if (i == -1) i = std::strlen(args);
  int ind = ToInt(args, args + i);
  if (ind <= 0 || (unsigned long long)ind >= (unsigned long long)matrices[0]) {
    return StrRes("index is empty");
  }
  if (matrices[ind] == nullptr) {
    return StrRes("index is empty");
  }
  int** matrix = matrices[ind];
  unsigned long long h = (unsigned long long)matrix[0];
  unsigned long long w = matrix[1][0];
  if (h != w) {
    return StrRes("matrix is not square");
  }
  int** tmp = new int*[h + 1];
  tmp[0] = (int*)h;
  for (int i = 1; i <= h; ++i) {
    tmp[i] = new int[h + 1];
    tmp[i][0] = h;
    for (int j = 1; j <= h; ++j) {
      tmp[i][j] = matrix[i][j];
    }
  }
  long long det_val = 0;
  int sign = 1;
  long long prev = 1;
  bool nonsingular = true;
  for (int k = 1; k < h; ++k) {
    if (tmp[k][k] == 0) {
      int pivot = -1;
      for (int i = k + 1; i <= h; ++i) {
        if (tmp[i][k] != 0) {
          pivot = i;
          break;
        }
      }
      if (pivot == -1) {
        nonsingular = false;
        break;
      }
      int* tmp_row = tmp[k];
      tmp[k] = tmp[pivot];
      tmp[pivot] = tmp_row;
      sign = -sign;
    }
    for (int i = k + 1; i <= h; ++i) {
      for (int j = k + 1; j <= h; ++j) {
        tmp[i][j] = (tmp[i][j] * tmp[k][k] - tmp[i][k] * tmp[k][j]) / prev;
      }
    }
    prev = tmp[k][k];
  }
  if (!nonsingular) {
    det_val = 0;
  } else if (h == 1) {
    det_val = tmp[1][1];
  } else {
    det_val = (long long)sign * tmp[h][h];
  }
  for (int i = 1; i <= h; ++i) {
    delete[] tmp[i];
  }
  delete[] tmp;
  int len = 0;
  long long v = det_val;
  if (v < 0) {
    v = -v;
  }
  if (v == 0) {
    len = 1;
  } else {
    while (v > 0) {
      len += 1;
      v /= 10;
    }
  }
  if (det_val < 0) {
    len += 1;
  }
  char* res = new char[len + 1];
  res[len] = '\0';
  v = det_val;
  int rp = len;
  if (v < 0) {
    res[0] = '-';
    v = -v;
  }
  if (v == 0) {
    res[--rp] = '0';
  } else {
    while (v > 0) {
      res[--rp] = '0' + v % 10;
      v /= 10;
    }
  }
  void* r = StrRes(res);
  delete[] res;
  return r;
}

void* multiply(int***& matrices, char* args) {
  int i = LenToSpace(args);
  if (i == -1) i = std::strlen(args);
  int a = ToInt(args, args + i);
  args += i;
  if (args[0] == ' ') args += 1;

  i = LenToSpace(args);
  if (i == -1) i = std::strlen(args);
  int b = ToInt(args, args + i);
  args += i;
  if (args[0] == ' ') args += 1;

  i = LenToSpace(args);
  if (i == -1) i = std::strlen(args);
  int ind = ToInt(args, args + i);

  while ((unsigned long long)matrices[0] <= (unsigned long long)ind) {
    extend(matrices);
  }
  if (a <= 0 || (unsigned long long)a >= (unsigned long long)matrices[0] ||
      matrices[a] == nullptr) {
    return StrRes("index a is empty");
  }
  if (b <= 0 || (unsigned long long)b >= (unsigned long long)matrices[0] ||
      matrices[b] == nullptr) {
    return StrRes("index b is empty");
  }
  if (matrices[ind] != nullptr) {
    return StrRes("index is not empty");
  }
  unsigned long long h1 = (unsigned long long)matrices[a][0];
  unsigned long long w1 = matrices[a][1][0];
  unsigned long long h2 = (unsigned long long)matrices[b][0];
  unsigned long long w2 = matrices[b][1][0];
  if (w1 != h2) {
    return StrRes("dimensions mismatch");
  }
  int** result = new int*[h1 + 1]();
  result[0] = (int*)h1;
  for (unsigned long long r = 1; r <= h1; ++r) {
    result[r] = new int[w2 + 1];
    result[r][0] = w2;
    for (unsigned long long c = 1; c <= w2; ++c) {
      int sum = 0;
      for (unsigned long long k = 1; k <= w1; ++k) {
        sum += matrices[a][r][k] * matrices[b][k][c];
      }
      result[r][c] = sum;
    }
  }
  matrices[ind] = result;
  return MRes(matrices, ind);
}

int main() {
  // программа работает с матрицами, обрабатывая запросы пользователя
  // int*** используется для хранения массива матриц
  // void* используется в конструкторе матрицы - мы можем задать матрицу разными
  // способами, а писать несколько функций под эти задачи - дублировать код.
  // Также используется в выводе результатов функции
  int*** matrices = new int**[2]();
  matrices[0] = (int**)1;  // в первой ячейке храним длину
  const char* cmd_table_names[]{"help", "set",       "get", "clear",
                                "list", "transpose", "det", "multiply"};
  void* (*cmd_table_pointers[])(int***&, char*){help, set,       get, clear,
                                                list, transpose, det, multiply};

  while (true) {
    char cmd[20]{};
    std::cin >> cmd;
    if (equal("exit", cmd)) {
      return 0;
    }
    char args[1024]{};
    std::cin.getline(args, 1024);
    bool nothing_called = true;
    for (unsigned int i = 0; i < std::size(cmd_table_names); ++i) {
      if (!equal(cmd_table_names[i], cmd)) {
        continue;
      }
      nothing_called = false;
      void* res = cmd_table_pointers[i](matrices, args + 1);
      char* res_str = ToString(res);
      std::cout << res_str;
      delete[] res_str;
      std::free(res);
      break;
    }
    if (nothing_called) {
      std::cout << "unknown command, type help for command list" << '\n';
    }
  }
}