/**
 * Copyright Soramitsu Co., Ltd. 2018 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_TRANSPORT_BUILDER_HPP
#define IROHA_TRANSPORT_BUILDER_HPP

#include "common/result.hpp"
#include "utils/polymorphic_wrapper.hpp"

namespace shared_model {
  namespace proto {

    template <typename T, typename SV>
    class TransportBuilder {
     public:
      TransportBuilder(typename T::TransportType transport,
                       const SV &validator = SV())
          : transport_(transport), stateless_validator_(validator) {}

      iroha::expected::Result<T, std::string> build() {
        auto answer = stateless_validator_.validate(
            detail::makePolymorphic<T>(transport_));
        if (answer.hasErrors()) {
          throw std::invalid_argument(answer.reason());
        }
        return iroha::expected::makeValue(T(std::move(transport_)));
      }

     private:
      typename T::TransportType transport_;
      SV stateless_validator_;
    };

  }  // namespace proto
}  // namespace shared_model
#endif  // IROHA_TRANSPORT_BUILDER_HPP
