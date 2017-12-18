/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2017, Regents of the University of California.
 *
 * This file is part of ndncert, a certificate management system based on NDN.
 *
 * ndncert is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * ndncert is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received copies of the GNU General Public License along with
 * ndncert, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndncert authors and contributors.
 */

#ifndef NDNCERT_CA_CONFIG_HPP
#define NDNCERT_CA_CONFIG_HPP

#include "certificate-request.hpp"
#include "client-config.hpp"
#include <ndn-cxx/security/v2/certificate.hpp>

namespace ndn {
namespace ndncert {

/**
 * @brief The function should be able to convert a probe info string to an identity name
 *
 * The function should throw exceptions when there is an unexpected input.
 */
using ProbeHandler = function<std::string/*identity name*/ (const std::string&/*requester input*/)>;

/**
 * @brief The function should recommend a CA plus an identity name from the given list
 *        based on LIST additional info
 *
 * The function should throw exceptions when there is an unexpected input.
 */
using RecommendCaHandler = function<std::tuple<Name/*CA name*/, std::string/*identity*/>
                                    (const std::string&/*requester input*/,
                                     const std::list<Name>&/*related CA list*/)>;

/**
 * @brief The function would be invoked whenever the certificate request gets update
 */
using RequestUpdateCallback = function<void (const CertificateRequest&/*the latest request info*/)>;

class CaItem
{
public:
  // basic info
  Name m_caName;

  // related CAs
  std::list<Name> m_relatedCaList;

  // essential config
  time::seconds m_freshnessPeriod;
  time::days m_validityPeriod;
  std::list<std::string> m_supportedChallenges;

  // optional parameters
  std::string m_probe;
  std::string m_targetedList;
  std::string m_caInfo;

  // callbacks
  ProbeHandler m_probeHandler;
  RecommendCaHandler m_recommendCaHandler;
  RequestUpdateCallback m_requestUpdateCallback;
};

/**
 * @brief Represents a CA configuration instance
 *
 * For CA configuration format, please refer to:
 *   https://github.com/named-data/ndncert/wiki/Ca-Configuration-Sample
 *
 * @note Changes made to CaConfig won't be written back to the config
 */
class CaConfig
{
public:
  /**
   * @brief Error that can be thrown from CaConfig
   */
  class Error : public std::runtime_error
  {
  public:
    using std::runtime_error::runtime_error;
  };

public:
  void
  load(const std::string& fileName);

private:
  void
  parse(const JsonSection& configJson);

  std::list<std::string>
  parseChallengeList(const JsonSection& configSection);

  std::list<Name>
  parseRelatedCaList(const JsonSection& section);

public:
  std::list<CaItem> m_caItems;
};

} // namespace ndncert
} // namespace ndn

#endif // NDNCERT_CA_CONFIG_HPP
