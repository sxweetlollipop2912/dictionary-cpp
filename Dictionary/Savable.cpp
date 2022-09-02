#include "Savable.h"

SavableString::SavableString(SavableString::Super string) {
    (Super &) (*this) = std::move(string);
}