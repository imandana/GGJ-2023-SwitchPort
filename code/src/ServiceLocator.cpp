#include "ServiceLocator.h"

std::unordered_map<std::type_index, std::shared_ptr<void>> ServiceLocator::services;