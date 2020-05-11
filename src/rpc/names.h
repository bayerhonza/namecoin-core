// Copyright (c) 2014-2020 Daniel Kraft
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_RPC_NAMES_H
#define BITCOIN_RPC_NAMES_H

#include <script/script.h>

#include <names/encoding.h>
#include <rpc/util.h>

#include <string>
#include <vector>

class CNameData;
class COutPoint;
class CScript;
class UniValue;

UniValue getNameInfo (const UniValue& options,
                      const valtype& name, const valtype& value,
                      const COutPoint& outp, const CScript& addr);
UniValue getNameInfo (const UniValue& options,
                      const valtype& name, const CNameData& data);
void addExpirationInfo (int height, UniValue& data);

#ifdef ENABLE_WALLET
class CWallet;
void addOwnershipInfo (const CScript& addr,
                       const CWallet* pwallet,
                       UniValue& data);
#endif

/**
 * Decodes a name given through the RPC interface and throws a
 * JSONRPCError if it is invalid for the requested encoding.  The encoding
 * is extracted from the options object if it is there with the "nameEncoding"
 * key, or else the configured default name encoding it used.
 */
valtype DecodeNameFromRPCOrThrow (const UniValue& val, const UniValue& opt);

/**
 * Decodes a value given through the RPC interface and throws an error if it
 * is invalid.  This is the same as DecodeNameFromRPCOrThrow, except that it
 * extracts the "valueEncoding" from the options and uses the default encoding
 * for values instead of names.
 */
valtype DecodeValueFromRPCOrThrow (const UniValue& val, const UniValue& opt);

void pushTimestampOfDataTx(CNameData, bool, UniValue&, std::__cxx11::string);

/**
 * Builder class for the RPC results for methods that return information about
 * names (like name_show, name_scan, name_pending or name_list).  Since the
 * exact fields contained depend on the case, this class
 * provides a simple and fluent interface to build the right help text for
 * each case.
 */
class NameInfoHelp
{

private:

  /** Result fields that have already been added.  */
  std::vector<RPCResult> fields;

public:

  explicit NameInfoHelp ();

  NameInfoHelp& withExpiration ();

  /**
   * Adds a new field for the result.
   */
  NameInfoHelp&
  withField (const RPCResult& field)
  {
    fields.push_back (field);
    return *this;
  }

  /**
   * Constructs the final RPCResult for all fields added.
   */
  RPCResult
  finish ()
  {
    return RPCResult (RPCResult::Type::OBJ, "", "", fields);
  }

};

/**
 * Builder class for the help of the "options" argument for name RPCs.
 */
class NameOptionsHelp
{

private:

  /** Inner RPCArgs for RPCHelpMan.  */
  std::vector<RPCArg> innerArgs;

public:

  NameOptionsHelp ();

  /**
   * Adds the options for write-type RPCs (e.g. name_update).
   */
  NameOptionsHelp& withWriteOptions ();

  NameOptionsHelp& withNameEncoding ();
  NameOptionsHelp& withValueEncoding ();

  /**
   * Variant of withField that also adds the innerArgs field correctly.
   */
  NameOptionsHelp& withArg (const std::string& name, RPCArg::Type type,
                            const std::string& doc);

  /**
   * Adds a new inner argument with a default value.
   */
  NameOptionsHelp& withArg (const std::string& name, RPCArg::Type type,
                            const std::string& defaultValue,
                            const std::string& doc);

  /**
   * Constructs the RPCArg object for the options argument described by this
   * builder instance.
   */
  RPCArg buildRpcArg () const;

};

#endif // BITCOIN_RPC_NAMES_H
