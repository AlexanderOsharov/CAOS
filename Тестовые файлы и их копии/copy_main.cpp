#include <iostream>
#include <locale>

#include "bst_container.hpp"

void set_russian_locale() {
  // std::setlocale(LC_ALL, "ru_RU.UTF-8"); // Linux/macOS
  std::setlocale(LC_ALL, "Russian");  // Windows
}

int main() {
  set_russian_locale();

  BST<int> tree;
  for (int x : {50, 30, 70, 20, 40, 60, 80}) {
    tree.insert(x);
  }

  std::cout << "Прямой обход:\n";
  for (const auto& val : tree) {
    std::cout << val << ' ';
  }
  std::cout << "\n";

  std::cout << "Обратный обход:\n";
  for (auto it = rbegin(tree); it != rend(tree); ++it) {
    std::cout << *it << ' ';
  }
  std::cout << "\n";

  auto it = begin(tree);
  ++it;
  --it;
  std::cout << "Двунаправленность: *it = " << *it << "\n";

  return 0;
}
