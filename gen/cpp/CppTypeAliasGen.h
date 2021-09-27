#ifndef GEN_CPP_CPPTYPEALIASGEN_H_
#define GEN_CPP_CPPTYPEALIASGEN_H_

#include "ast/Contract.h"

#include <sstream>

void writeContractTypeAliases(std::stringstream &ss, const Contract& c);

#endif /* GEN_CPP_CPPTYPEALIASGEN_H_ */
