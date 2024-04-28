#pragma once
#include <iostream>
#include <memory>

class GenPass {
  std::string pass;
  const std::string charset = "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789";
  public:
  std::string gen_pass(int length){
    this->pass.reserve(length);
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < length; ++i) {
      this->pass += this->charset[
        rand() % this->charset.length()
      ];
    }
    return pass;
  }
};
