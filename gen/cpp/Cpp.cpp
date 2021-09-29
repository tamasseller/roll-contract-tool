#include "Cpp.h"
#include "CppSymGen.h"
#include "CppCommon.h"
#include "CppParamTypeGen.h"
#include "CppTypeAliasGen.h"
#include "CppClientProxy.h"
#include "CppServerProxy.h"
#include "CppSessionProxy.h"
#include "CppStructSerdes.h"

const CodeGenCpp CodeGenCpp::instance;

static inline std::string allcapsEscape(const std::string &str)
{
	std::string ret(str.size(), '\0');

	for(auto i = 0u; i < str.size(); i++)
	{
		ret[i] = std::isalnum(str[i]) ? std::toupper(str[i]) : '_';
	}

	return ret;
}

std::string CodeGenCpp::generate(const std::vector<Contract>& cs, const std::string& name, bool doClient, bool doService) const
{
	std::stringstream ss;

	const auto guardMacroName = "_" + allcapsEscape(name) + "_";

	ss << "#ifndef " << guardMacroName << std::endl;
	ss << "#define " << guardMacroName << std::endl << std::endl;

	ss << "#include \"RpcCall.h\"" << std::endl;

	if(doClient)
	{
		ss << "#include \"RpcClient.h\"" << std::endl;
	}

	ss << "#include \"RpcStruct.h\"" << std::endl;
	ss << "#include \"RpcSymbol.h\"" << std::endl;
	ss << "#include \"RpcSession.h\"" << std::endl;

	if(doService)
	{
		ss << "#include \"RpcService.h\"" << std::endl;
	}

	ss << "#include \"RpcTypeInfo.h\"" << std::endl << std::endl;
	ss << "#include \"RpcCollection.h\"" << std::endl;

	for(const auto& c: cs)
	{
		std::vector<std::string> members = {
			indent(1) + "class Parametric;",
			indent(1) + "class Types;",
			indent(1) + "class Symbols;",
		};

		if(doClient)
		{
			members.push_back(indent(1) + "template<class> class ClientProxy;");
		}

		if(doService)
		{
			members.push_back(indent(1) + "template<class, class> class ServerProxy;");
		}

		writeTopLevelBlock(ss, "struct " + contractRootBlockName(c.name), std::move(members));

		writeParametricContractTypes(ss, c);
		writeStructTypeInfo(ss, c);
		writeContractTypeAliases(ss, c);
		writeContractSymbols(ss, c);

		if(doClient)
		{
			writeSessionProxies(ss, c, ClientSessionProxyFilterFactory{});
			writeClientProxy(ss, c);
		}

		if(doService)
		{
			writeSessionProxies(ss, c, ServerSessionProxyFilterFactory{});
			writeServerProxy(ss, c);
		}
	}

	ss << std::endl << "#endif /* " << guardMacroName << " */" << std::endl;
	return ss.str();
}
