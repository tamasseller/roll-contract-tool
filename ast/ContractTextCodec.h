#ifndef RPC_TOOL_ASTRANSMODEL_H_
#define RPC_TOOL_ASTRANSMODEL_H_

#include "Contract.h"
#include <iosfwd>
#include <string>

std::vector<Contract> deserializeText(std::istream &input);
std::string serializeText(const std::vector<Contract>& ast);

#endif /* RPC_TOOL_ASTRANSMODEL_H_ */
