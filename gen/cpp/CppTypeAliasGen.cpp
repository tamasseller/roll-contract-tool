#include "CppTypeAliasGen.h"

#include "CppCommon.h"

#include <algorithm>
#include <list>

struct TypeAliasGenerator
{
	static inline std::string alias(const std::string& pName, const std::string &eName, const int n) {
		return indent(n) + "using " + eName + " = " + pName + "::" + eName + "<rpc::Many>;";
	}

	static inline void handleSessionItem(std::vector<std::string> &r, const std::string pName, const Contract::Session::ForwardCall &f, const int n)
	{
		r.push_back(alias(pName, sessionForwardCallSignatureTypeName(f.name), n));
	}

	static inline void handleSessionItem(std::vector<std::string> &r, const std::string pName, const Contract::Session::CallBack & cb, const int n)
	{
		r.push_back(alias(pName, sessionCallbackSignatureTypeName(cb.name), n));
	}

	static inline void handleSessionItem(std::vector<std::string> &r, const std::string pName, const Contract::Session::Ctor & c, const int n)
	{
		r.push_back(alias(pName, sessionAcceptSignatureTypeName(c.name), n));
		r.push_back(alias(pName, sessionCreateSignatureTypeName(c.name), n));
	}

	static inline void handleItem(std::vector<std::string> &r, const std::string pName, const Contract::Session &s, const int n)
	{
		std::vector<std::string> result;

		const auto sName = pName + "::" + sessionNamespaceName(s.name);

		result.push_back(alias(sName, sessionCallExportTypeName(s.name), n + 1));
		result.push_back(alias(sName, sessionCallbackExportTypeName(s.name), n + 1));

		for(const auto& it: s.items) {
			std::visit([n, &result, &sName](const auto& i){ return handleSessionItem(result, sName, i, n + 1); }, it.second);
		}

		std::stringstream ss;
		writeBlock(ss, "struct " + sessionNamespaceName(s.name), result, n);
		ss << ";";
		r.push_back(ss.str());
	}

	static inline void handleItem(std::vector<std::string> &r, const std::string pName, const Contract::Alias &a, const int n) {
		r.push_back(alias(pName, userTypeName(a.name), n));
	}

	static inline void handleItem(std::vector<std::string> &r, const std::string pName, const Contract::Function &f, const int n) {
		r.push_back(alias(pName, (f.returnType) ? functionSignatureTypeName(f.name) : actionSignatureTypeName(f.name), n));
	}
};

void writeContractTypeAliases(std::stringstream &ss, const Contract& c)
{
	const std::string pName = contractParametricBlockNameRef(c.name);

	std::vector<std::string> result;
	for(const auto& i: c.items)
	{
		std::visit([&result, &pName](const auto& i){ TypeAliasGenerator::handleItem(result, pName, i, 1); }, i.second);
	}

	writeTopLevelBlock(ss, "struct " + contractTypeBlockNameRef(c.name), result);
}
